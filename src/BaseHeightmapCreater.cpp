#include "BaseHeightmapCreater.h"



BaseHeightmapCreater::BaseHeightmapCreater()
{
	
}


BaseHeightmapCreater::~BaseHeightmapCreater()
{
}

void BaseHeightmapCreater::setup(int pow2PlusSize)
{
	pow2Plus1 = pow2PlusSize;
	subDivitions = 3;

	map.allocate(pow2Plus1, pow2Plus1, OF_IMAGE_GRAYSCALE);

	base.allocate(pow2Plus1, pow2Plus1, OF_IMAGE_GRAYSCALE);

	textureEdit.allocate(map);
	textureMap.allocate(map);
	
	drawSize = 756;

	menu.setup(drawSize, 1, "color", 0, 0);
	menu.addIntSlider(drawSize, 32, "color picker", 0, 255, 255);
	menu.addIntSlider(drawSize, 32, "subdivisions", 1, 35, 1);
	menu.disableEvents();

	button.setup(256, 1, "", 0, 0,false);
	button.addButton(256, 32, "Edit map");
	button.addFloatSlider(256, 32, "Roughness", 0, 5, 1);
	button.addIntSlider(256, 32, "Start size", 0, 5, 1);
	button.disableEvents();

	ofAddListener(ofEvents().mouseMoved, this, &BaseHeightmapCreater::mouseMoved);
	ofAddListener(ofEvents().mouseDragged, this, &BaseHeightmapCreater::mouseDragged);
	ofAddListener(ofEvents().mousePressed, this, &BaseHeightmapCreater::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &BaseHeightmapCreater::mouseReleased);
}



void BaseHeightmapCreater::draw(int x, int y)
{
	positionX = ofGetWindowWidth() / 2 - drawSize / 2;
	positionY = ofGetWindowHeight() / 2 - drawSize / 2 + menu.getHeight();

	ofSetColor(255);

	button.draw(x, y);
	textureMap.draw(x, y + button.getHeight(), button.getWidth(), button.getWidth());
	if(button.getButton("Edit map")->getState())
		if (textureEdit.isAllocated()){
			textureEdit.draw(positionX, positionY,  drawSize, drawSize);
			menu.draw(positionX, positionY - menu.getHeight());
		}
}

void BaseHeightmapCreater::generateMap() 
{

	unsigned char* blurPixels = new unsigned char[map.getWidth()*map.getHeight()];
	memcpy(blurPixels, base.getPixels(), sizeof(unsigned char) *  base.getWidth() * base.getHeight() );

	float	roughness = button.getFloatSlider("Roughness")->getValue();
	int		startSize = button.getIntSlider("Start size")->getValue();

	if (sizes[startSize] > map.getWidth()) {
		startSize++;
	}
	GreyProcessing::greyDiamondSquare(blurPixels, map.getWidth(), sizes[startSize], roughness);


	Kernel k;
	k.setSize(3);
	k.gaussianKernel(1.0f);
	GreyProcessing::greyBlur(blurPixels, map.getPixels(), map.getWidth(), map.getHeight(), k, CLAMP);

	textureMap.loadData(map);

	delete[]blurPixels;
}

void BaseHeightmapCreater::editMap(int x, int y)
{
	if (x > positionX  && x < positionX + drawSize &&
		y > positionY  && y < positionY + drawSize)
	{
		float stC = x - positionX;
		float stR = y - positionY;

		float squareSize = (float)drawSize / (float)subDivitions;

		stC /= squareSize;
		stR /= squareSize;

		int value = menu.getIntSlider("color picker")->getValue();

		setNeighborhoodValue(base,(int)stR, (int)stC, value);

		textureEdit.loadData(base);
	}
}
void BaseHeightmapCreater::mouseMoved(ofMouseEventArgs & args) {
	if (button.getButton("Edit map")->getState()) {
		menu.mouseMoved(args);
	}
	button.mouseMoved(args);
}

void BaseHeightmapCreater::mousePressed(ofMouseEventArgs & args) {
	if (button.getButton("Edit map")->getState()) {
		editMap(args.x, args.y);
		menu.mousePressed(args);
	}
	button.mousePressed(args);
}

void BaseHeightmapCreater::mouseDragged(ofMouseEventArgs & args)
{
	int sub = menu.getIntSlider("subdivisions")->getValue();
	if (sub != subDivitions) {
		subDivitions = sub;
		recalculateMap();
		textureEdit.loadData(base);
	}
	
	if (button.getButton("Edit map")->getState()) {
		editMap(args.x, args.y);
		menu.mouseDragged(args);
	}
	button.mouseDragged(args);
}
void BaseHeightmapCreater::mouseReleased(ofMouseEventArgs & args) {
	if (button.getButton("Edit map")->getState()) {
		menu.mouseReleased(args);
	}
	button.mouseReleased(args);
}


void BaseHeightmapCreater::setNeighborhoodValue(ofPixels &heightmap,int row, int column, unsigned char value) {

	float squareSize = (float)pow2Plus1/ (float) subDivitions;

	int startX = squareSize * column;
	int startY = squareSize * row;

	for (int ny = startY; ny < startY + squareSize; ny++) {
		for (int nx = startX; nx < startX + squareSize; nx++)
		{
			heightmap.getPixels()[ny * pow2Plus1 + nx] = value;
		}
	}
}
float BaseHeightmapCreater::getNeighborhoodMean(ofPixels &heightmap, int row, int column) {

	float	mean = 0;
	int		count = 0;

	float	squareSize = (float)pow2Plus1 / (float)subDivitions;

	int startX = squareSize * column;
	int startY = squareSize * row;

	for (int ny = startY; ny < startY + squareSize; ny++) {
		for (int nx = startX; nx < startX + squareSize; nx++)
		{
			mean += heightmap.getPixels()[ny * pow2Plus1 + nx];
			count++;
		}
	}
	return mean / (float)count;
}

void BaseHeightmapCreater::recalculateMap() 
{
	for (int r = 0; r < subDivitions; r++)
	{
		for (int c = 0; c < subDivitions; c++)
		{
			float value = getNeighborhoodMean(base,r, c);
			setNeighborhoodValue(base,r, c, (unsigned char)value);
		}
	}
}