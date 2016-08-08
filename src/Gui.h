#pragma once

#include "ofMain.h"
#include "GraphicMesh.h"

class Gui
{
public:
	Gui();
	~Gui();

	void setup(	int eWidth, int eHeight, string eName, 
				ofVec2f texCoord1 = ofVec2f(0.0f, 0.0f), ofVec2f texCoord2 = ofVec2f(0.0f, 1.0f), 
				ofVec2f texCoord3 = ofVec2f(1.0f, 0.0f), ofVec2f texCoord4 = ofVec2f(1.0f, 1.0f));


	void drawMesh(ofVec3f color = ofVec3f(1.0f));
	virtual void draw(int x, int y){}

	void resize(int w, int h);

	bool inside(int x, int y, int w, int h);

	void setPosition(int x, int y);

	virtual int getWidth() { return width; }
	virtual int getHeight() { return height; }

	string getName() { return name; }
	void setName(string nName) { name = nName; }

	string getLabel() { return label; }
	void setLabel(string nLabel) { label = nLabel; }

	GraphicMesh		mesh;

protected:

	string			name;
	string			label;
	int				margin;

	bool			mouseInside;

	int				posX;
	int				posY;

	int				width;
	int				height;

	ofShader		shader;

	ofTexture*		diffuse;
	ofTexture*		normalmap;

};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------


class GuiElement :
public Gui
{
public:
	GuiElement();
	~GuiElement();


	void enableEvents();
	void disableEvents();

	// Mouse interactions
	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args) { return false; };
	virtual bool mouseDragged(ofMouseEventArgs & args) { return false; }
	virtual bool mouseReleased(ofMouseEventArgs & args) { return false; };

	// Key interactions
	virtual bool keyPressed(ofKeyEventArgs & args) { return false; };
	virtual bool keyReleased(ofKeyEventArgs & args) { return false; };

	bool getState() { return state; }
	void setState(bool initState) { state = initState; }
	


	bool getIsMouseEnabled() { return mouseEnable; }
	bool getIsKeyEnabled() { return keyEnable; }


protected:

	bool            state;
	bool			isIndependent;

	bool            mouseEnable;
	bool            keyEnable;

	
	// Mouse activate events
	void registerMouseEvent();
	void unregisterMouseEvent();

	// Key activate events
	void registerKeyEvent();
	void unregisterKeyEvent();


};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

enum AlignmentTypeVertical {
	ABOVE = 3,
	TOP = 0,
	vCENTER = 1,
	BUTTOM = 2,
	BELOW = 4
};
enum AlignmentTypeHorizontal {
	LEFT = 0,
	hCENTER = 1,
	RIGHT = 2
};
struct TextAlignment {
	AlignmentTypeVertical v;
	AlignmentTypeHorizontal h;
};

class GuiText
{
public:
	GuiText();
	virtual ~GuiText();

	// Setup font variables
	virtual void setupFont(string fontName, int fontSize, bool localFolder = true);
	virtual void setupFont(ofTrueTypeFont*  nFont);

	// Draw a string
	void draw(string text, int x, int y, ofColor color);
	void drawText(string text, int x, int y, ofColor color);

	// Get width of a given string
	float getTextWidth(string text);
	// Get height of a given string
	float getTextHeight(string text);
	// Set line height
	void setLineHeight(float h);
	// Get line height
	float getLineHeight();
	// Set space width
	void setSpaceWidth(float w);
	// Get space width
	float getSpaceWidth();
	//
	string getFontReference() { return fontLocation; }
	int getFontReferenceSize() { return fontSize; }
	//
	ofRectangle getBoundingBox(string text);

	// Fit text to text field size
	string cutTextWidth(string inputStr, int width);
	// Fit text to text field size
	string adjustTextWidth(string inputStr, int width);
	string adjustTextWidth(string inputStr, int width, int& stringHeight);
	// Fit text to text field size
	int findTextWidth(string inputStr, int width);

	ofTrueTypeFont* getFont() { return font; }



protected:

	// AlignTextToParent
	int setAlignedX(AlignmentTypeHorizontal hori, int width, string text);
	int setAlignedY(AlignmentTypeVertical vert, int height, string text);

	string              fontLocation;
	int                 fontSize;

	ofTrueTypeFont*     font;
	bool                fontIsLocal;


private:
};


//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
template<typename Type>
class Slider :
public GuiElement, public GuiText
{
public:
	Slider();
	virtual ~Slider();

	// Draw object
	virtual void draw(int x, int y);

	// Mouse interactions
	bool mouseDragged(ofMouseEventArgs & args);
	bool mouseReleased(ofMouseEventArgs & args);

	Type    getValue() { return value; }
	void    setValue(Type  minValue, Type  maxVAlue, Type  startValue) ;


protected:

	// Get slider percentage
	float       getPercentage();

	bool        dragInit;

	Type        value;
	Type        max;
	Type        min;

	float       sliderPosition;
	

private:
};

typedef Slider<float> FloatSlider;
typedef Slider<int> IntSlider;

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
class Button :
public GuiElement, public GuiText
{
public:
	Button();
	virtual ~Button();

	// Draw object
	virtual void draw(int x, int y);

	// Mouse interactions
	bool mousePressed(ofMouseEventArgs & args);

	bool  getValue() { return value; }
	void  setValue(bool nValue) { value = nValue; state = nValue;}

	int getLabelHeight();

protected:

	bool                value;
	TextAlignment		align;
	
	int                 stringHeight;

private:
};


//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
typedef shared_ptr<GuiElement>	sharedGuiPtr;
typedef shared_ptr<Button>		ButtonPtr;
typedef shared_ptr<IntSlider>	IntSliderPtr;
typedef shared_ptr<FloatSlider>	FloatSliderPtr;


class GuiMenu :
	public GuiElement, public GuiText
{
public:
	GuiMenu();
	virtual ~GuiMenu();

	void setup(int eWidth, int eHeight, string eName, int placementX, int placementY, bool menuIsSwitch = true,
		ofVec2f texCoord1 = ofVec2f(0.0f, 0.0f), ofVec2f texCoord2 = ofVec2f(0.0f, 1.0f),
		ofVec2f texCoord3 = ofVec2f(1.0f, 0.0f), ofVec2f texCoord4 = ofVec2f(1.0f, 1.0f));

	void draw();
	void draw(int x, int y) override;

	// Mouse interactions
	bool mouseMoved(ofMouseEventArgs & args) override;
	bool mousePressed(ofMouseEventArgs & args) override;
	bool mouseDragged(ofMouseEventArgs & args) override;
	bool mouseReleased(ofMouseEventArgs & args) override;

	void setActiveElementIndex(int elementIndex = -1);

	int getActiveElementIndex() { return activeElement; }
	string getActiveElementName() { return (activeElement != -1) ? elements[activeElement]->getName() : ""; }

	sharedGuiPtr getElement(int index) { if (index >= 0 && index < elements.size()) return elements[index]; }

	void addElement(sharedGuiPtr nElement);
	void addFloatSlider(int eWidth, int eHeight, string eName, float minValue, float maxVAlue, float startValue);
	void addIntSlider(int eWidth, int eHeight, string eName, int minValue, int maxVAlue, int startValue);
	void addButton(int eWidth, int eHeight, string eName);

	void deleteElement(int index);

	int getElementIndex(string name);

	int getNumElements() { return elements.size(); }

	ButtonPtr getButton(string name);
	IntSliderPtr getIntSlider(string name);
	FloatSliderPtr getFloatSlider(string name);

	int getWidth() { return menuWidth; }
	int getHeight() { return menuHeight; }

	void setIsActive(bool activate) { isActive = activate; }

protected:

	vector<sharedGuiPtr> elements;

	Gui					backGround;

	int					menuWidth;
	int					menuHeight;

	bool				isSwitch;
	int					activeElement;

	bool				dragInit;
	ofVec2f				dragCorrection;

	bool				isActive;
};