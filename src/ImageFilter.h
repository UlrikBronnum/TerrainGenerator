#pragma once

#include "ofMain.h"

#include "ImageProcessing.h"
#include "TextureContainer.h"
#include "Gui.h"

class ImageFilter
{
public:
	ImageFilter();
	~ImageFilter();

	virtual void update(ofFbo& input) = 0;
	virtual void update(ofTexture& input) = 0;
	virtual void update(ImageFilter& input);

	
	void drawController(int x, int y);
	virtual void drawFbo(int x, int y, float scale);

	ofFbo* getFbo() { return &fbo; }
	ofTexture* getTexture() { return &fbo.getTexture(); }
	
	bool getHasChanged() 
	{ 
		bool value = hasChanged;
		hasChanged = false;
		return value;
	}

	virtual void setIsActive(bool activate);

protected:

	ofFbo			fbo;

	string			textureTarget;
	GuiMenu			controller;

	bool			hasChanged;
};
class Hsb:
public ImageFilter
{
public:
	Hsb(int menuWidth, int menuHeight);
	~Hsb();

	void update(ofTexture& input) override;
	void update(ofFbo& input) override;
	void update(ImageFilter& input) override;

protected:

	ofPixels pixels;
	bool	 updatePixels;
};

class Invert :
public ImageFilter
{
public:
	Invert(int menuWidth, int menuHeight);
	~Invert();

	void update(ofTexture& input) override;
	void update(ofFbo& input) override;

};

class BrightnessContrast :
public ImageFilter
{
public:
	BrightnessContrast(int menuWidth, int menuHeight);
	~BrightnessContrast();

	void update(ofTexture& input) override;
	void update(ofFbo& input) override;

};

class Gamma :
public ImageFilter
{
public:
	Gamma(int menuWidth, int menuHeight);
	~Gamma();

	void update(ofTexture& input) override;
	void update(ofFbo& input) override;
};

class Stretch :
public ImageFilter
{
public:
	Stretch(int menuWidth, int menuHeight);
	~Stretch();

	void update(ofTexture& input) override;
	void update(ofFbo& input) override;

};

class Threshold :
public ImageFilter
{
public:
	Threshold(int menuWidth, int menuHeight);
	~Threshold();

	void update(ofTexture& input) override;
	void update(ofFbo& finput) override;
};

class Blur :
public ImageFilter
{
public:
	Blur(int menuWidth, int menuHeight);
	~Blur();

	void update(ofTexture& input) override;
	void update(ofFbo& input) override;

	void setIsActive(bool activate) override;

protected:

	GuiMenu	kernelSelector;
	Kernel  kernel;
};

class Edge :
public ImageFilter
{
public:
	Edge(int menuWidth, int menuHeight);
	~Edge();

	void update(ofTexture& input) override;
	void update(ofFbo& input) override;

	void setIsActive(bool activate) override;

protected:

	GuiMenu	kernelSelector;
};

class Normalmap :
	public ImageFilter
{
public:
	Normalmap(int menuWidth, int menuHeight);
	~Normalmap();

	void update(ofTexture& input) override;
	void update(ofFbo& input) override;

	void setIsActive(bool activate) override;

protected:

	GuiMenu	kernelSelector;
};

class Morph :
public ImageFilter
{
public:
	Morph(int menuWidth, int menuHeight);
	~Morph();

	void update(ofTexture& input) override;
	void update(ofFbo& input) override;

	void setIsActive(bool activate) override;
	

protected:

	Kernel	kernel;
	GuiMenu	kernelSelector;
};

class Heightmap :
	public ImageFilter
{
public:
	Heightmap(int menuWidth, int menuHeight);
	~Heightmap();

	void update(ofTexture& input) override;
	void update(ofFbo& input) override;

	void setIsActive(bool activate) override;

protected:

	Kernel	kernel;
	GuiMenu	kernelSelector;
};

class N2H :
public ImageFilter
{
public:
	N2H(int menuWidth, int menuHeight);
	~N2H();

	void update(ofTexture& input) override;
	void update(ofFbo& input) override;

	void setIsActive(bool activate) override;

};

