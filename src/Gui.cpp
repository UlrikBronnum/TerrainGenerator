#include "Gui.h"



Gui::Gui()
{
	diffuse = nullptr;
	normalmap = nullptr;

}


Gui::~Gui()
{
}

void Gui::setup(int eWidth, int eHeight, string eName, ofVec2f texCoord1, ofVec2f texCoord2, ofVec2f texCoord3, ofVec2f texCoord4)
{
	width = eWidth;
	height = eHeight;
	name = eName;
	label = eName;
	margin = 5;

	mesh.createQuad(width, height, texCoord1, texCoord2, texCoord3, texCoord4);
	shader.load("res/shaders/defaultVertex.vert", "res/shaders/meshColor.frag");
}

void Gui::drawMesh(ofVec3f color)
{
	shader.begin();
	shader.setUniform3f("ambientLight", color);

	ofPushMatrix();

	ofTranslate(posX + width/2, posY + height/2);
	mesh.draw();

	ofPopMatrix();
	shader.end();
}
void Gui::resize(int w, int h)
{
	mesh.resizeQuad(w, h);
	width = w;
	height = h;
}
bool Gui::inside(int x, int y, int w, int h) {
	
	if (x > posX && x < posX + w &&
		y > posY && y < posY + h) 
	{
		return true;
	}
	return false;
}
void Gui::setPosition(int x, int y)
{
	posX = x;
	posY = y;
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

GuiElement::GuiElement()
{
	mouseEnable = false;
	keyEnable = false;

	isIndependent = true;
}
GuiElement::~GuiElement()
{
	disableEvents();
}
bool GuiElement::mouseMoved(ofMouseEventArgs & args) 
{
	if (inside(args.x, args.y, width, height))
	{
		mouseInside = true;
		return true;
	}
	mouseInside = false;
	return false;
};

void GuiElement::enableEvents()
{
	if(isIndependent)
	{
		if (!mouseEnable)
			registerMouseEvent();

		if (!keyEnable)
			registerKeyEvent();
	}
}
void GuiElement::disableEvents()
{
	if (mouseEnable)
		unregisterMouseEvent();

	if (keyEnable)
		unregisterKeyEvent();
}

// Mouse activate events
void GuiElement::registerMouseEvent()
{
	ofAddListener(ofEvents().mouseMoved, this, &GuiElement::mouseMoved);
	ofAddListener(ofEvents().mousePressed, this, &GuiElement::mousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &GuiElement::mouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &GuiElement::mouseReleased);
	mouseEnable = true;
}
void GuiElement::unregisterMouseEvent()
{
	ofRemoveListener(ofEvents().mouseMoved, this, &GuiElement::mouseMoved);
	ofRemoveListener(ofEvents().mousePressed, this, &GuiElement::mousePressed);
	ofRemoveListener(ofEvents().mouseDragged, this, &GuiElement::mouseDragged);
	ofRemoveListener(ofEvents().mouseReleased, this, &GuiElement::mouseReleased);
	mouseEnable = false;
}

// Key activate events
void GuiElement::registerKeyEvent()
{
	ofAddListener(ofEvents().keyPressed, this, &GuiElement::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &GuiElement::keyReleased);
}
void GuiElement::unregisterKeyEvent()
{
	ofRemoveListener(ofEvents().keyPressed, this, &GuiElement::keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &GuiElement::keyReleased);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

GuiText::GuiText()
{
	font = nullptr;
	fontIsLocal = false;
}

GuiText::~GuiText()
{
	if (fontIsLocal) delete font;
}

// Setup font variables
void GuiText::setupFont(string fontName, int _fontSize, bool localFolder)
{
	fontLocation = fontName;
	fontSize = _fontSize;
	if (font != nullptr && fontIsLocal) delete font;
	fontIsLocal = true;
	font = new ofTrueTypeFont();

	if (!font->loadFont("fonts/" + fontName + ".ttf", fontSize, true, true))
	{
		ofTrueTypeFont* t = font;
		font = nullptr;
		delete t;
	}
}
void GuiText::setupFont(ofTrueTypeFont*  nFont)
{
	font = nFont;
	fontIsLocal = false;
}

void GuiText::drawText(string text, int x, int y, ofColor color)
{
	if (font) {
		ofSetColor(color);
		font->drawString(text, x, y);
	}
	else {
		ofSetColor(color);
		ofDrawBitmapString(text, x, y);
	}
}
// Get width of a given string
float GuiText::getTextWidth(string text)
{
	if (font) {
		return font->stringWidth(text);
	}
	return getBoundingBox(text).width;
}

// Get height of a given string
float GuiText::getTextHeight(string text)
{
	if (font) {
		return font->stringHeight(text);
	}
	return getBoundingBox(text).height;
}

// Set line height
void GuiText::setLineHeight(float h)
{
	if (font) {
		font->setLineHeight(h);
	}
}
// Get line height
float GuiText::getLineHeight()
{
	if (font) {
		return font->getLineHeight();
	}
	return getBoundingBox("X").height;
}

// Set space width
void GuiText::setSpaceWidth(float w)
{
	if (font) {
		font->setSpaceSize(w);
	}
}
// Get letter spacing
float GuiText::getSpaceWidth()
{
	if (font) {
		return font->getLetterSpacing();
	}
	return getBoundingBox(" ").width;
}
// Adjust string length to fit field
string GuiText::cutTextWidth(string inputStr, int width)
{
	string nStr = inputStr;
	int textWidth = getTextWidth(nStr);

	while (textWidth >= width - getTextWidth("X")) {
		nStr = nStr.substr(1);
		textWidth = getTextWidth(nStr);
	}
	return nStr;
}
// Adjust string length to fit field
int GuiText::findTextWidth(string inputStr, int width)
{
	string nStr = inputStr;
	int textWidth = getTextWidth(nStr);

	while (textWidth > width) {
		nStr = nStr.substr(0, nStr.size() - 2);
		textWidth = getTextWidth(nStr);
	}
	return nStr.size();
}
// Adjust string width to fit field
string GuiText::adjustTextWidth(string inputStr, int width)
{
	int length = findTextWidth(inputStr, width);
	int lines = inputStr.size() / (length);
	if ((inputStr.size() - length) % length != 0) {
		length -= 1;
		lines = inputStr.size() / (length);
	}

	string output = inputStr;
	for (int i = 0; i < lines; i++) {
		output.insert((length)* (i + 1), "\n");
	}
	return output;
}
// Adjust string width to fit field
string GuiText::adjustTextWidth(string inputStr, int width, int& stringHeight)
{
	int length = findTextWidth(inputStr, width);
	int lines = inputStr.size() / (length);
	stringHeight = 0;
	if ((inputStr.size() - length) % length != 0) {
		length -= 1;
		lines = inputStr.size() / (length);
	}

	string output = inputStr;
	for (int i = 0; i < lines; i++) {
		output.insert((length)* (i + 1), "\n");
	}
	stringHeight = getLineHeight() * (lines + 1);
	return output;
}
ofRectangle GuiText::getBoundingBox(string text)
{
	if (font) {
		return font->getStringBoundingBox(text, 0, 0);
	}
	else {
		ofBitmapFont f;
		return f.getBoundingBox(text, 0, 0);
	}
}

int GuiText::setAlignedX(AlignmentTypeHorizontal hori, int width, string text)
{
	int x = 0;

	switch (hori) {
	case LEFT:
		x = 0;
		break;
	case hCENTER:
		x = width / 2 - getTextWidth(text) / 2;
		break;
	case RIGHT:
		x = width - getTextWidth(text);
		break;
	default:
		break;
	}
	return x;
}
int GuiText::setAlignedY(AlignmentTypeVertical vert, int height, string text)
{
	int y = 0;
	int h = getLineHeight() / 2;
	switch (vert) {
	case TOP:
		y = h + getLineHeight() / 3;
		break;
	case vCENTER:
		y = height / 2 + getBoundingBox("X").height / 10 * 4;
		break;
	case BUTTOM:
		y = height - h + getLineHeight() / 3;
		break;
	case ABOVE:
		y = -h;
		break;
	case BELOW:
		y = height + h + getLineHeight() / 3;
		break;
	default:
		break;
	}
	return y;
}


//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

template<typename Type>
Slider<Type>::Slider()
{
	dragInit = false;

	value = 0;
	max = 0;
	min = 0;
}
template<typename Type>
Slider<Type>::~Slider()
{

}
// Draw object
template<typename Type>
void Slider<Type>::draw(int x, int y)
{
	ofDisableDepthTest();
	setPosition(x, y);
	ofVec3f color(1.0f);
	if (state) {
		color *= 0.5f;
	}
	else
	if (mouseInside) {
		color *= 0.7f;
	}
	drawMesh(color);

	

	ofSetColor(255);
	int _w = (width - margin * 2);
	int length = _w * getPercentage();
	for (int i = 0; i < length; i++)
	{
		ofColor c = ofColor::red;
		c.setBrightness((c.limit() - c.limit() / 10) * (float(i) / (width - margin * 2)) + c.limit() / 10);
		ofSetColor(c);
		ofLine(x + margin + i, y + height / 4, x + margin + i, y + height / 4 * 3);
	}
	string t = (typeid(value) == typeid(float)) ? ofToString(value, 3) : ofToString(value);
	
	drawText(t, x + setAlignedX(RIGHT, _w, t) + margin, y + setAlignedY(vCENTER,height, t), ofColor::black);
	drawText(label, x + setAlignedX(LEFT, _w, label) + margin, y + setAlignedY(vCENTER, height, label), ofColor::white);
	
	ofEnableDepthTest();
}
// Mouse interactions
template<typename Type>
bool Slider<Type>::mouseDragged(ofMouseEventArgs & args)
{
	if (mouseInside)
	{
		dragInit = true;
	}

	if (dragInit)
	{
		if (args.x > posX &&
			args.x < posX + width)
		{
			sliderPosition = args.x - posX;
			Type diff = max - min;
			value = min + (diff * getPercentage());
		}
	}
	state = dragInit;
	return dragInit;
}
template<typename Type>
bool Slider<Type>::mouseReleased(ofMouseEventArgs & args)
{
	dragInit = false;
	state = dragInit;
	return false;
}

template<typename Type>
void Slider<Type>::setValue(Type  minValue, Type  maxVAlue, Type  startValue)
{
	value = startValue;
	min = minValue;
	max = maxVAlue;

	float diff1 = max - value;
	float diff2 = max - min;

	sliderPosition = (width - margin * 2) * ((diff2 - diff1) / diff2);
}

template<typename Type>
float Slider<Type>::getPercentage()
{
	float pct = sliderPosition / (width - margin * 2);
	if (pct > 0.995f) {
		return 1;
	}
	else if (pct < 0.005f) {
		return 0;
	}
	else {
		return pct;
	}
}

template class Slider<int>;
template class Slider<unsigned int>;
template class Slider<float>;
template class Slider<double>;
template class Slider<signed char>;
template class Slider<unsigned char>;
template class Slider<unsigned short>;
template class Slider<short>;

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

Button::Button()
{
	align.h = hCENTER;
	align.v = vCENTER;
}
Button::~Button()
{
}
// Draw object
void Button::draw(int x, int y)
{
	ofDisableDepthTest();
	setPosition(x, y);

	ofVec3f color(1.0f);

	if (state) {
		color *= 0.5f;
	}else 
	if (mouseInside) {
		color *= 0.7f;
	}

	drawMesh(color);

	string tmp = label;
	if (getTextWidth(tmp) > getWidth())
	{
		if (align.v == BELOW || align.v == ABOVE)
		{
			tmp = adjustTextWidth(tmp, getWidth(), stringHeight);
		}
		else
		{
			tmp = adjustTextWidth(tmp, getWidth());
			stringHeight = 0;
		}
	}
	else
	{
		if (align.v == BELOW || align.v == ABOVE)
		{
			stringHeight = getTextHeight("X");
		}
	}
	//
	this->drawText(tmp, x + setAlignedX(align.h,width, tmp), y + setAlignedY(align.v, height, tmp), ofColor::black);
	ofEnableDepthTest();
}
bool Button::mousePressed(ofMouseEventArgs & args)
{
	if (inside(args.x, args.y, width, height)) {
		state = !state;
		value = state;
		return true;
	}
	return false;
}
int Button::getLabelHeight()
{
	return stringHeight;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
GuiMenu::GuiMenu()
{
	margin = 5;
	backGround.setup(0, margin, "Background");
	enableEvents();
	isActive = true;
}
GuiMenu::~GuiMenu()
{
}

void GuiMenu::setup(int eWidth, int eHeight, string eName, int placementX, int placementY, bool menuIsSwitch,
	ofVec2f texCoord1, ofVec2f texCoord2,
	ofVec2f texCoord3, ofVec2f texCoord4)
{
	width = eWidth;
	height = eHeight;
	name = eName;
	isSwitch = menuIsSwitch;
	activeElement = -1;

	mesh.createQuad(width, height, texCoord1, texCoord2, texCoord3, texCoord4);
	shader.load("res/shaders/defaultVertex.vert", "res/shaders/meshColor.frag");

	setPosition(placementX, placementY);
}

void GuiMenu::draw()
{
	draw(posX, posY);
}
void GuiMenu::draw(int x, int y)
{
	setPosition(x, y);
	ofDisableDepthTest();

	ofVec3f color(1.0f);
	if (state) {
		color *= 0.5f;
	}
	else
		if (mouseInside) {
			color *= 0.7f;
		}
	drawMesh(color);
	drawText(label, x + setAlignedX(LEFT, width, label), y + setAlignedY(vCENTER, height, label), ofColor::black);

	backGround.setPosition(x, y + height);
	backGround.drawMesh(ofVec3f(0.8f));
	
/*
	float startY = y + height + margin;
	float posX = float(x);
	float posY = startY;
	for (int i = 0; i < elements.size(); i++)
	{
		if (posX + elements[i]->getWidth() > width && i > 0) {
			posX = 0.0f;
			posY += elements[i - 1]->getHeight() + margin;
			elements[i]->draw(posX, posY);
		}
		else {
			elements[i]->draw(posX, posY);
			posX += elements[i]->getWidth();
		}
	}
*/
	int newY = y + height + margin;
	for (int i = 0; i < elements.size(); i++)
	{
		if (i != 0) {
			newY += elements[i - 1]->getHeight() + margin;
		}
		elements[i]->draw(x, newY);
	}
	
	ofEnableDepthTest();
}
void GuiMenu::setActiveElementIndex(int elementIndex)
{
	if (elementIndex < elements.size() || elementIndex == -1)
		activeElement = elementIndex;

	for (int i = 0; i < elements.size(); i++) 
	{
		if (i == elementIndex) {
			elements[i]->setState(true);
		}
		else {
			elements[i]->setState(false);
		}
	}
}

void GuiMenu::deleteElement(int index)
{
	if (index >= 0 && index < elements.size()) {
		int nheight = backGround.getHeight() - (elements[index]->getHeight() + margin);
		backGround.resize(width, nheight);
		elements.erase(elements.begin() + index);
	}
}
// Mouse interactions
bool GuiMenu::mouseMoved(ofMouseEventArgs & args)
{
	if(isActive){
		if (inside(args.x, args.y, width, height))
		{
			mouseInside = true;
			return true;
		}
		mouseInside = false;

		for (int i = 0; i < elements.size(); i++) {
			elements[i]->mouseMoved(args);
		}
	}
	return false;
}
bool GuiMenu::mousePressed(ofMouseEventArgs & args) {
	if (isActive) {
		int newState = -1;
		for (int i = 0; i < elements.size(); i++) {
			if (elements[i]->mousePressed(args)) {
				if (i != activeElement) {
					newState = i;
				}
				if (i == activeElement && isSwitch) {
					elements[i]->setState(true);
				}
			}
		}
		if (isSwitch)
			if (newState != -1 && newState != activeElement) {
				if (activeElement != -1)
					elements[activeElement]->setState(false);

				activeElement = newState;
			}
	}
	return false;
}
bool GuiMenu::mouseDragged(ofMouseEventArgs & args)
{
	if (isActive) {
		if (!dragInit)
			if (mouseInside)
			{
				dragCorrection.x = posX - args.x;
				dragCorrection.y = posY - args.y;

				dragInit = true;
			}

		if (dragInit)
		{
			posX = args.x + dragCorrection.x;
			posY = args.y + dragCorrection.y;
		}
		else {
			for (int i = 0; i < elements.size(); i++) {
				if (elements[i]->mouseDragged(args)) {
					return false;
				}
			}
		}

		state = dragInit;
	}
	return dragInit;
}
bool GuiMenu::mouseReleased(ofMouseEventArgs & args)
{
	if (isActive) {
		for (int i = 0; i < elements.size(); i++) {
			if (elements[i]->mouseReleased(args)) {
				return false;
			}
		}
		dragInit = false;
		state = dragInit;
	}
	return false;
}
void GuiMenu::addElement(sharedGuiPtr nElement)
{
	if (nElement->getIsMouseEnabled())
		nElement->disableEvents();
/*
	int nheight = backGround.getHeight() + nElement->getHeight() + margin;
	backGround.resize(width, nheight);
	menuHeight = height + nheight;
*/	
	menuWidth = width;
	
	if (elements.size() != 0) {
		if (elements[elements.size() - 1]->getWidth() + nElement->getWidth() > width) {
			int nheight = backGround.getHeight() + nElement->getHeight() + margin;
			backGround.resize(width, nheight);
			menuHeight = height + nheight;
		}
	}
	else {
		int nheight = backGround.getHeight() + nElement->getHeight() + margin;
		backGround.resize(width, nheight);
		menuHeight = height + nheight;
	}
	
	elements.push_back(nElement);
}
void GuiMenu::addFloatSlider(int eWidth, int eHeight, string eName, float minValue, float maxVAlue, float startValue) {
	FloatSlider* newElement = new FloatSlider();
	newElement->setup(eWidth, eHeight, eName);
	newElement->setValue(minValue, maxVAlue, startValue);
	newElement->setState(false);
	addElement((sharedGuiPtr)newElement);
}
void GuiMenu::addIntSlider(int eWidth, int eHeight, string eName, int minValue, int maxVAlue, int startValue) {
	IntSlider* newElement = new IntSlider();
	newElement->setup(eWidth, eHeight, eName);
	newElement->setValue(minValue, maxVAlue, startValue);
	newElement->setState(false);
	addElement((sharedGuiPtr)newElement);
}
void GuiMenu::addButton(int eWidth, int eHeight, string eName) {
	Button* newButton = new Button();
	newButton->setup(eWidth, eHeight, eName);
	newButton->setState(false);
	addElement((sharedGuiPtr)newButton);
}
int GuiMenu::getElementIndex(string name)
{
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i]->getName() == name) {
			return i;
		}
	}
	return -1;
}
ButtonPtr GuiMenu::getButton(string name)
{
	ButtonPtr ptr;
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i]->getName() == name) {
			ptr = std::dynamic_pointer_cast<Button>(elements[i]);
		}
	}
	return ptr;
}
IntSliderPtr GuiMenu::getIntSlider(string name)
{
	IntSliderPtr ptr;
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i]->getName() == name) {
			ptr = std::dynamic_pointer_cast<IntSlider>(elements[i]);
		}
	}
	return ptr;
}
FloatSliderPtr GuiMenu::getFloatSlider(string name)
{
	FloatSliderPtr ptr;
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i]->getName() == name) {
			ptr = std::dynamic_pointer_cast<FloatSlider>(elements[i]);
		}
	}
	return ptr;
}
