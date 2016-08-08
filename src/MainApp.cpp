#include "MainApp.h"



MainApp::MainApp()
{
	layers.setup(256, 16, "Layers", 0, 0, true);

	displayOptions.setup(256, 16, "Image Options", 0, 0, true);
	displayOptions.addFloatSlider(256, 32, "Image Size", 0.0f, 5.0f, 1.0f);
	displayOptions.addButton(256, 32, "Save");
	displayOptions.addButton(256, 32, "Delete");

	filterSelector.setup(256, 16, "Filters", 0, 0, true);
	filterSelector.addButton(256, 32, "Invert");
	filterSelector.addButton(256, 32, "BrightnessContrast");
	filterSelector.addButton(256, 32, "Gamma");
	filterSelector.addButton(256, 32, "Stretch");
	filterSelector.addButton(256, 32, "Threshold");
	filterSelector.addButton(256, 32, "Blur");
	filterSelector.addButton(256, 32, "Edge");
	filterSelector.addButton(256, 32, "Normalmap");
	filterSelector.addButton(256, 32, "Heightmap");
	filterSelector.addButton(256, 32, "Normal2Heightmap");
	filterSelector.addButton(256, 32, "Morph");
	filterSelector.addButton(256, 32, "HSB Adjust");

	activeLayer = -1;
	normalPos = -1;
}


MainApp::~MainApp()
{
	
}

void MainApp::setup()
{
	
}
void MainApp::update()
{
	if (baseImage){
		int addNewLayer = filterSelector.getActiveElementIndex();

		if (addNewLayer != -1)
		{
			addLayer(addNewLayer);
		}

		int newActive = layers.getActiveElementIndex();

		if (newActive != activeLayer && newActive != -1 && activeLayer != -1)
		{
			filterContainer[activeLayer]->setIsActive(false);
			filterContainer[newActive]->setIsActive(true);
		}

		activeLayer = newActive;

		if (activeLayer > 0) 
		{
			filterContainer[activeLayer]->update(*filterContainer[activeLayer-1]);
		}
		else {
			if (activeLayer == 0) {
				filterContainer[activeLayer]->update(*baseImage.get());
			}
		}
		if (displayOptions.getElement(1)->getState())
		{
			ofPixels p;
			filterContainer[activeLayer]->getFbo()->readToPixels(p);
			ofSaveImage(p, "saves/test.png");
			displayOptions.getElement(1)->setState(false);
		}else
		if (displayOptions.getElement(2)->getState()) 
		{
			deleteLayer();
			displayOptions.getElement(2)->setState(false);
		}

	}
}
void MainApp::draw()
{
	float size = displayOptions.getFloatSlider("Image Size")->getValue();
	
	ofPushMatrix();
	//ofRotateZ(90);
	if (activeLayer != -1) 
	{
		
		ofSetColor(255);
		int px = ofGetWindowWidth() / 2 - (baseImage->getWidth() * size) / 2;
		int py = ofGetWindowHeight() / 2 - (baseImage->getHeight() * size) / 2;
		filterContainer[activeLayer]->drawFbo(px, py, size);
		filterContainer[activeLayer]->drawController(0, displayOptions.getHeight());
	}
	else {
		if (baseImage) {
			ofSetColor(255);
			int px = ofGetWindowWidth() / 2 - (baseImage->getWidth() * size) / 2;
			int py = ofGetWindowHeight() / 2 - (baseImage->getHeight() * size) / 2;
			baseImage->draw(px, py, baseImage->getWidth() * size, baseImage->getHeight() * size);
		}
	}
	ofPopMatrix();

	if (baseImage) 
	{
		ofSetColor(255);
		displayOptions.draw(0, 0);
		ofSetColor(255);
		filterSelector.draw(ofGetWindowWidth() - filterSelector.getWidth(), 0);
		ofSetColor(255);
		layers.draw(ofGetWindowWidth() - layers.getWidth(), filterSelector.getHeight());
	}
}
void MainApp::addLayer(int newLayer)
{
	switch (newLayer) {
	case 0:
		filterContainer.push_back(new Invert(256, 16));
		break;
	case 1:
		filterContainer.push_back( new BrightnessContrast(256, 16));
		break;
	case 2:
		filterContainer.push_back( new Gamma(256, 16));
		break;
	case 3:
		filterContainer.push_back( new Stretch(256, 16));
		break;
	case 4:
		filterContainer.push_back(new Threshold(256, 16));
		break;
	case 5:
		filterContainer.push_back( new Blur(256, 16));
		break;
	case 6:
		filterContainer.push_back(new Edge(256, 16));
		break;
	case 7:
		filterContainer.push_back(new Normalmap(256, 16));
		normalPos = filterContainer.size()-1;
		break;
	case 8:
		filterContainer.push_back(new Heightmap(256, 16));
		break;
	case 9:
		filterContainer.push_back(new N2H(256, 16));
		break;
	case 10:
		filterContainer.push_back(new Morph(256, 16));
		break;
	case 11:
		filterContainer.push_back(new Hsb(256, 16));
		break;
	default:
		break;
	}
	layers.addButton(256, 32, "layer" + ofToString(layers.getNumElements()));
	filterSelector.setActiveElementIndex(-1);
	layers.setActiveElementIndex(activeLayer+1);
}
void MainApp::deleteLayer()
{
	if (activeLayer != -1 && activeLayer < filterContainer.size())
	{
		filterContainer.erase(filterContainer.begin() + activeLayer);
		layers.deleteElement(activeLayer);
		activeLayer--;
		layers.setActiveElementIndex(activeLayer);

		if(activeLayer >= 0)
			filterContainer[activeLayer]->setIsActive(true);
	}
}
void MainApp::resetLayer()
{

}