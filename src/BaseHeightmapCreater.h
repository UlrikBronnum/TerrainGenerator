#pragma once

#include "ofMain.h"
#include "Gui.h"
#include "ImageProcessing.h"

class BaseHeightmapCreater
{
public:
	BaseHeightmapCreater();
	~BaseHeightmapCreater();

	void setup(int pow2PlusSize);
	void draw(int x, int y);

	void mouseMoved(ofMouseEventArgs & args);
	void mouseDragged(ofMouseEventArgs & args);
	void mousePressed(ofMouseEventArgs & args);
	void mouseReleased(ofMouseEventArgs & args);


	void generateMap();

	int getSubDivisions() { return subDivitions; }
	ofPixels* getMap() { return &map; }
	
	GuiMenu* getMenu() { return &button; }

protected:

	void	editMap(int x, int y);
	void	setNeighborhoodValue(ofPixels &heightmap, int row, int colum, unsigned char value);
	float	getNeighborhoodMean(ofPixels &heightmap, int row, int column);
	void	recalculateMap();

	GuiMenu		menu;
	GuiMenu		button;
	ofPixels	map;
	ofPixels	base;
	ofTexture	textureEdit;
	ofTexture	textureMap;

	int			pow2Plus1;

	int			subDivitions;

	int			positionX;
	int			positionY;

	int			drawSize;

	const short sizes[6] = { 1025, 513, 257, 129, 65, 33 };

};
