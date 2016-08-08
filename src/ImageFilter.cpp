#include "ImageFilter.h"



ImageFilter::ImageFilter()
{
	ofDisableArbTex();
}


ImageFilter::~ImageFilter()
{
}

void ImageFilter::update(ImageFilter& input) 
{
	hasChanged = true;
	update(*input.getFbo());
}
void ImageFilter::drawController(int x, int y) 
{
	controller.draw(x, y);
}

void ImageFilter::drawFbo(int x, int y, float scale)
{
	ofSetColor(255);
	fbo.draw(x, y, fbo.getWidth()*scale,fbo.getHeight()*scale);
}
void ImageFilter::setIsActive(bool activate)
{
	controller.setIsActive(activate);
}
Invert::Invert(int menuWidth, int menuHeight)
{
	controller.setup(menuWidth, menuHeight, "Invert", 0, 0, false);
	controller.addButton(menuWidth, menuHeight * 2, "Red");
	controller.addButton(menuWidth, menuHeight * 2, "Green");
	controller.addButton(menuWidth, menuHeight * 2, "Blue");
}
Invert::~Invert()
{
}
void Invert::update(ofTexture&  input)
{
	ofVec3f invertChannel(0.0f);

	if (controller.getButton("Red")->getState())
	{
		invertChannel.x = 1.0f;
	}
	if (controller.getButton("Green")->getState())
	{
		invertChannel.y = 1.0f;
	}
	if (controller.getButton("Blue")->getState())
	{
		invertChannel.z = 1.0f;
	}
	GPUProcessing::invert(input,fbo, invertChannel);
}

void Invert::update(ofFbo&  input)
{
	hasChanged = true;
	update(input.getTexture());
}

Hsb::Hsb(int menuWidth, int menuHeight)
{
	controller.setup(menuWidth, menuHeight, "Invert", 0, 0, false);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Hue", -180, 180, 0);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Saturation", -100, 100, 0);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Brightness", -100, 100, 0);
}
Hsb::~Hsb()
{
}
void Hsb::update(ofTexture&  input)
{
	ofFbo tmp;
	tmp.allocate(input.getWidth(), input.getHeight());
	tmp.attachTexture(input, input.getTextureData().glInternalFormat, 0);
	update(tmp);

}
void Hsb::update(ofFbo&  input)
{
	unsigned char* o = new unsigned char[input.getWidth() * input.getHeight() * 4];
	if(updatePixels || !pixels.isAllocated()) {
		input.readToPixels(pixels);
		updatePixels = false;
	}
	int h = controller.getFloatSlider("Hue")->getValue();
	int s = controller.getFloatSlider("Saturation")->getValue();
	int b = controller.getFloatSlider("Brightness")->getValue();

	ColorProcessing::HsbAdjust(pixels.getPixels(), o, input.getWidth(), input.getHeight() , h, s, b);

	fbo.allocate(input.getWidth(), input.getHeight());
	fbo.getTexture().loadData(o, input.getWidth(), input.getHeight(),GL_RGBA);
	delete[] o;
}
void Hsb::update(ImageFilter&  input)
{
	updatePixels = input.getHasChanged();
	update(*input.getFbo());
}

BrightnessContrast::BrightnessContrast(int menuWidth, int menuHeight)
{
	controller.setup(menuWidth, menuHeight, "Brightness/Contrast Control", 0, 0);
	controller.addIntSlider(menuWidth, menuHeight * 2, "Brightness", -255, 255, 0);
	controller.addIntSlider(menuWidth, menuHeight * 2, "Contrast", -255, 255, 0);
}
BrightnessContrast::~BrightnessContrast()
{
}
void BrightnessContrast::update(ofTexture&  input)
{
	int b = controller.getIntSlider("Brightness")->getValue();
	int c = controller.getIntSlider("Contrast")->getValue();
	GPUProcessing::brightCont(input, fbo, b, c);
}
void BrightnessContrast::update(ofFbo&  input)
{
	update(input.getTexture());
}

Gamma::Gamma(int menuWidth, int menuHeight)
{
	controller.setup(menuWidth, menuHeight, "Gamma Control", 0, 0);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Gamma", 0.0f, 7.0f, 1.0f);
}

Gamma::~Gamma()
{
}

void Gamma::update(ofTexture&  input)
{
	float g = controller.getFloatSlider("Gamma")->getValue();
	GPUProcessing::gamma(input, fbo, g);
}
void Gamma::update(ofFbo&  input)
{
	update(input.getTexture());
}

Stretch::Stretch(int menuWidth, int menuHeight)
{
	controller.setup(menuWidth, menuHeight, "Stretch Control", 0, 0);
	controller.addIntSlider(menuWidth, menuHeight * 2, "Max", 0, 255, 255);
	controller.addIntSlider(menuWidth, menuHeight * 2, "Min", 0, 255, 0);
}


Stretch::~Stretch()
{
}

void Stretch::update(ofTexture&  input) 
{
	ofFbo tmp;
	tmp.allocate(input.getWidth(), input.getHeight());
	tmp.attachTexture(input, input.getTextureData().glInternalFormat, 0);
	update(tmp);
}
void Stretch::update(ofFbo&  input)
{
	ofPixels pixels;
	input.readToPixels(pixels);
	Histogram hist;
	ofVec3f mins;
	ofVec3f maxs;

	for (int i = 0; i < 3; i++) {
		hist.update(pixels.getPixels(), pixels.getWidth(), pixels.getHeight(), ColorComponent(i), 4);
		mins[i] = hist.getMin();
		maxs[i] = hist.getMax();
	}

	int max = controller.getIntSlider("Max")->getValue();
	int min = controller.getIntSlider("Min")->getValue();

	GPUProcessing::strech(input.getTexture(), fbo, min, max, maxs, mins);
}

Threshold::Threshold(int menuWidth, int menuHeight)
{
	controller.setup(menuWidth, menuHeight, "Threshold Control", 0, 0);
	controller.addIntSlider(menuWidth, menuHeight * 2, "Threshold Red", 0, 255, 0);
	controller.addIntSlider(menuWidth, menuHeight * 2, "Threshold Green", 0, 255, 0);
	controller.addIntSlider(menuWidth, menuHeight * 2, "Threshold Blue", 0, 255, 0);
}
Threshold::~Threshold()
{
}
void Threshold::update(ofTexture& input)
{
	int r = controller.getIntSlider("Threshold Red")->getValue();
	int g = controller.getIntSlider("Threshold Green")->getValue();
	int b = controller.getIntSlider("Threshold Blue")->getValue();

	GPUProcessing::threshold(input, fbo, ofVec3f(r, g, b));
}

void Threshold::update(ofFbo&  input)
{
	update(input.getTexture());
}




Blur::Blur(int menuWidth, int menuHeight)
{
	controller.setup(menuWidth, menuHeight, "Blur", 0, 0);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Sigma", 1.0f, 3.0f, 1.0f);
	controller.addIntSlider(menuWidth, menuHeight * 2, "Radius", 1, 27, 1);

	kernelSelector.setup(menuWidth, menuHeight, "Kernel Menu", 0, 0);
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Gaussian");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Disc");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Box");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Cross");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "HighPass");
	kernelSelector.getButton("Gaussian")->setState(true);

	controller.addElement((sharedGuiPtr)&kernelSelector);
}
Blur::~Blur()
{
}
void Blur::update(ofTexture&  input)
{
	int radius = controller.getIntSlider("Radius")->getValue();
	float sigma = controller.getFloatSlider("Sigma")->getValue();

	kernel.setSize(radius*2+1);
	int kernelChoise = kernelSelector.getActiveElementIndex();

	switch (kernelChoise)
	{
	case 0:
		kernel.gaussianKernel(sigma);
		break;
	case 1:
		kernel.discKernel();
		break;
	case 2:
		kernel.boxKernel();
		break;
	case 3:
		kernel.crossKernel();
		break;
	case 4:
		kernel.highPass();
		break;
	default:
		kernel.gaussianKernel(sigma);
		break;
	}
	GPUProcessing::blur(input, fbo, kernel);
}
void Blur::update(ofFbo&  input)
{
	update(input.getTexture());
}
void Blur::setIsActive(bool activate)
{
	controller.setIsActive(activate);
	kernelSelector.setIsActive(activate);
}
Edge::Edge(int menuWidth, int menuHeight)
{
	controller.setup(menuWidth, menuHeight, "Edge", 0, 0);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Red weight", 0.0f, 1.0f, 0.33f);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Green weight", 0.0f, 1.0f, 0.33f);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Blue weight", 0.0f, 1.0f, 0.33f);
	controller.addIntSlider(menuWidth, menuHeight * 2, "Edge Output", 0, 3, 0);
	controller.addIntSlider(menuWidth, menuHeight * 2, "Edge Threshold", 0, 255, 0);

	kernelSelector.setup(menuWidth, menuHeight, "Kernel Menu", 0, 0);
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Prewitt");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Sobel");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Robinson");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Kirsch");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Scharr");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Normal");
	kernelSelector.setActiveElementIndex(0);

	controller.addElement((sharedGuiPtr)&kernelSelector);
}
Edge::~Edge()
{
}
void Edge::update(ofTexture&  input)
{
	int kernelChoise = kernelSelector.getActiveElementIndex();
	int output = controller.getIntSlider("Edge Output")->getValue();
	int threshold = controller.getIntSlider("Edge Threshold")->getValue();

	float r = controller.getFloatSlider("Red weight")->getValue();
	float g = controller.getFloatSlider("Green weight")->getValue();
	float b = controller.getFloatSlider("Blue weight")->getValue();

	switch (kernelChoise)
	{
	case 0:
		GPUProcessing::edge(input, fbo, prewitt_horizontal, prewitt_vertical, prewitt_diagonal, EdgeOutput(output), threshold, ofVec3f(r,g,b) );
		break;
	case 1:
		GPUProcessing::edge(input, fbo, sobel_horizontal, sobel_vertical, sobel_diagonal, EdgeOutput(output), threshold, ofVec3f(r, g, b));
		break;
	case 2:
		GPUProcessing::edge(input, fbo, robinson_horizontal, robinson_vertical, robinson_diagonal, EdgeOutput(output), threshold, ofVec3f(r, g, b));
		break;
	case 3:
		GPUProcessing::edge(input, fbo, kirsch_horizontal, kirsch_vertical, kirsch_diagonal, EdgeOutput(output), threshold, ofVec3f(r, g, b));
		break;
	case 4:
		GPUProcessing::edge(input, fbo, scharr_horizontal, scharr_vertical, scharr_diagonal, EdgeOutput(output), threshold, ofVec3f(r, g, b));
		break;
	case 5:
		GPUProcessing::edge(input, fbo, normal_horizontal, normal_vertical, normal_diagonal, EdgeOutput(output), threshold, ofVec3f(r, g, b));
		break;
	default:
		break;
	}
}
void Edge::update(ofFbo&  input)
{
	update(input.getTexture());
}
void Edge::setIsActive(bool activate)
{
	controller.setIsActive(activate);
	kernelSelector.setIsActive(activate);
}

Normalmap::Normalmap(int menuWidth, int menuHeight)
{
	controller.setup(menuWidth, menuHeight, "Normalmap", 0, 0);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Red weight", 0.0f, 1.0f, 0.33f);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Green weight", 0.0f, 1.0f, 0.33f);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Blue weight", 0.0f, 1.0f, 0.33f);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Roughness", -2.0, 2.0, 1.0);

	kernelSelector.setup(menuWidth, menuHeight, "Kernel Menu", 0, 0);
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Prewitt");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Sobel");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Robinson");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Kirsch");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Scharr");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Normal");
	kernelSelector.setActiveElementIndex(0);


	controller.addElement((sharedGuiPtr)&kernelSelector);
}
Normalmap::~Normalmap()
{
}

void Normalmap::update(ofTexture&  input)
{
	int kernelChoise = kernelSelector.getActiveElementIndex();
	
	float roughness = controller.getFloatSlider("Roughness")->getValue();
	float r = controller.getFloatSlider("Red weight")->getValue();
	float g = controller.getFloatSlider("Green weight")->getValue();
	float b = controller.getFloatSlider("Blue weight")->getValue();

	switch (kernelChoise)
	{
	case 0:
		GPUProcessing::normalmap(input, fbo, prewitt_horizontal, prewitt_vertical, roughness, ofVec3f(r, g, b));
		break;
	case 1:
		GPUProcessing::normalmap(input, fbo, sobel_horizontal, sobel_vertical, roughness, ofVec3f(r, g, b));
		break;
	case 2:
		GPUProcessing::normalmap(input, fbo, robinson_horizontal, robinson_vertical, roughness, ofVec3f(r, g, b));
		break;
	case 3:
		GPUProcessing::normalmap(input, fbo, kirsch_horizontal, kirsch_vertical, roughness, ofVec3f(r, g, b));
		break;
	case 4:
		GPUProcessing::normalmap(input, fbo, scharr_horizontal, scharr_vertical, roughness, ofVec3f(r, g, b));
		break;
	case 5:
		GPUProcessing::normalmap(input, fbo, normal_horizontal, normal_vertical, roughness, ofVec3f(r, g, b));
		break;
	default:
		break;
	}
}
void Normalmap::update(ofFbo&  input)
{
	update(input.getTexture());
}
void Normalmap::setIsActive(bool activate)
{
	controller.setIsActive(activate);
	kernelSelector.setIsActive(activate);
}
Morph::Morph(int menuWidth, int menuHeight)
{
	controller.setup(menuWidth, menuHeight, "Morph", 0, 0);
	controller.addButton(menuWidth, menuHeight * 2, "Operation");

	kernelSelector.setup(menuWidth, menuHeight, "Kernel Menu", 0, 0);
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Disc");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Box");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Cross");
	kernelSelector.setActiveElementIndex(0);

	controller.addElement((sharedGuiPtr)&kernelSelector);
}
Morph::~Morph()
{
}
void Morph::update(ofTexture& input)
{
	int kernelChoise = kernelSelector.getActiveElementIndex();
	ButtonPtr open = controller.getButton("Operation");

	int value = 0;
	switch (kernelChoise)
	{
	case 0:
		value = kernel.discKernelBinary();
		break;
	case 1:
		value = kernel.boxKernelBinary();
		break;
	case 2:
		value = kernel.crossKernelBinary();
		break;
	default:
		break;
	}
	if (open->getState()) {
		open->setLabel("Open");
		value = 0;
	}else{
		open->setLabel("Close");
	}
	GPUProcessing::morph(input, fbo, kernel, value);
}

void Morph::update(ofFbo&  input)
{
	update(input.getTexture());
}
void Morph::setIsActive(bool activate)
{
	controller.setIsActive(activate);
	kernelSelector.setIsActive(activate);
}
Heightmap::Heightmap(int menuWidth, int menuHeight)
{
	controller.setup(menuWidth, menuHeight, "Heightmap", 0, 0);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Red weight", 0.0f, 1.0f, 0.33f);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Green weight", 0.0f, 1.0f, 0.33f);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Blue weight", 0.0f, 1.0f, 0.33f);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Strength", 0.0f, 5.0f, 0.33f);
	controller.addFloatSlider(menuWidth, menuHeight * 2, "Sigma", 1.0f, 3.0f, 1.0f);
	controller.addIntSlider(menuWidth, menuHeight * 2, "Radius", 1, 12, 1);

	kernelSelector.setup(menuWidth, menuHeight, "Kernel Menu", 0, 0);
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Gaussian");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Disc");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Box");
	kernelSelector.addButton(menuWidth, menuHeight * 2, "Cross");
	kernelSelector.getButton("Gaussian")->setState(true);

	controller.addElement((sharedGuiPtr)&kernelSelector);
}
Heightmap::~Heightmap()
{
}
void Heightmap::update(ofTexture& input)
{
	ofFbo fbo1;
	ofFbo fbo2;
	ofFbo fbo3;

	float strength = controller.getFloatSlider("Strength")->getValue();
	float r = controller.getFloatSlider("Red weight")->getValue();
	float g = controller.getFloatSlider("Green weight")->getValue();
	float b = controller.getFloatSlider("Blue weight")->getValue();

	int radius = controller.getIntSlider("Radius")->getValue();
	float sigma = controller.getFloatSlider("Sigma")->getValue();

	kernel.setSize(radius * 2 + 1);
	int kernelChoise = kernelSelector.getActiveElementIndex();

	switch (kernelChoise)
	{
	case 0:
		kernel.gaussianKernel(sigma);
		break;
	case 1:
		kernel.discKernel();
		break;
	case 2:
		kernel.boxKernel();
		break;
	case 3:
		kernel.crossKernel();
		break;
	default:
		kernel.gaussianKernel(sigma);
		break;
	}

	GPUProcessing::blur(input, fbo1, kernel);
	GPUProcessing::invert(fbo1.getTexture(), fbo2, ofVec3f(1, 1, 1));
	GPUProcessing::heightmap(input, fbo2.getTexture(), fbo, strength, ofVec3f(r, g, b));
}

void Heightmap::update(ofFbo&  input)
{
	update(input.getTexture());
}
void Heightmap::setIsActive(bool activate)
{
	controller.setIsActive(activate);
	kernelSelector.setIsActive(activate);
}


N2H::N2H(int menuWidth, int menuHeight)
{
	controller.setup(menuWidth, menuHeight, "Tmp", 0, 0);
}

N2H::~N2H()
{
}
void N2H::update(ofTexture& input)
{
	GPUProcessing::n2h(input, fbo);
}

void N2H::update(ofFbo&  input)
{
	update(input.getTexture());
}
void N2H::setIsActive(bool activate)
{
	controller.setIsActive(activate);
}
