#pragma once


#include "ofMain.h"
#include "Gui.h"

class WaterErosion
{
public:
	WaterErosion();
	~WaterErosion();


	void loadHeightMap(ofPixels& height);

	void update();

	void runErosion();

	void waterFlowErosion(int x, int y, ofVec2f directionVector = ofVec2f(0, 0), float water = 0.1f,float sediments = 0.0f, float lastSlope = 0.0f);
	//void waterFlowErosion	(int x, int y, ofVec2f directionVector = ofVec2f(0, 0), float averageSlope = 0.0f, float water = 0.0f, float speed = 1.0f);


	ofPixels*	getHeightMap() { return &heightMap; }
	ofPixels*	getFlowMap() { return &flowMap; }

	GuiMenu*	getMenu() { return &menu; }

protected:


	ofPixels	heightMap;
	ofPixels	flowMap;
		
	int			width;
	int			height;

	float		waterToAdd;
	float		maxSaturation;
	float       addSoil;
	float		removeSoil;
	bool		doErosion;

	GuiMenu		menu;
};

