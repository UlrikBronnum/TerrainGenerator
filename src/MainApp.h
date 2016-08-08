#pragma once


#include "ofMain.h"
#include "ImageProcessing.h"
#include "Gui.h"
#include "ImageFilter.h"

class MainApp
{
public:
	MainApp();
	~MainApp();

	void setup();
	void update();
	void draw();

	void addLayer(int newLayer);
	void deleteLayer();
	void resetLayer();

	void setBaseImage(sharedTexPtr tex) { if(!baseImage) baseImage = tex; }

	void readToPixels(ofPixels& pixels);

	ImageFilter* getLayer() { if (filterContainer.size() != 0) return filterContainer[filterContainer.size()-1]; else return nullptr; }

private:

	vector<ImageFilter*>	filterContainer;
	sharedTexPtr			baseImage;
	ofTexture*				normalImage;


	GuiMenu					layers;
	GuiMenu					filterSelector;

	GuiMenu					displayOptions;

	int						activeLayer;

	int						normalPos;
};

