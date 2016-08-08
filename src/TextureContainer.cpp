#include "TextureContainer.h"



TextureContainer::TextureContainer()
{
	ofDisableArbTex();
}


TextureContainer::~TextureContainer()
{
	textures.clear();
}

sharedTexPtr TextureContainer::getTexture(string name)
{
	sharedTexPtr p;
	if (textures.find(name) != textures.end())
		return p = textures[name];

	return p;
}
bool TextureContainer::readToPixels(ofPixels& pixels, string name)
{
	ofFbo tmp;
	if (textures.find(name) != textures.end()) 
	{
		tmp.allocate(textures[name]->getWidth(), textures[name]->getHeight());
		tmp.attachTexture(*textures[name], textures[name].get()->getTextureData().glInternalFormat,0);
		tmp.readToPixels(pixels);
		return true;
	}
	return false;
}
string TextureContainer::loadImageFile(string path)
{
	string		filename = path;
	ofPixels	loadPixels;

	size_t last = filename.find_last_of(".");
	string fileType = filename.substr(last + 1);

	if( fileType == "jpg" ||
		fileType == "JPG" || 
		fileType == "png" || 
		fileType == "tiff" || 
		fileType == "tga"  
	)
	if ( ofLoadImage(loadPixels, filename) )
	{
		ofTexture* newTexture = new ofTexture();
		if (loadPixels.getNumChannels() == 3) {
			ofPixels output;
			output.allocate(loadPixels.getWidth() , loadPixels.getHeight(),OF_IMAGE_COLOR_ALPHA);
			ImageProcessing::rgbaFromRgbGrey(loadPixels, nullptr, output, loadPixels.getWidth(), loadPixels.getHeight());
			newTexture->loadData(output);
		}
		else {
			newTexture->loadData(loadPixels);
		}
		
		

		size_t found = filename.find_last_of("\\");
		string textureName = filename.substr(found + 1);

		textures.insert(texturePair(textureName, (sharedTexPtr)newTexture));
		numTextures++;

		return textureName;
	}
	return "";
}
void TextureContainer::addTexture(ofPixels& pixels, string name)
{
	if (textures.find(name) == textures.end()) {
		ofTexture* newTexture = new ofTexture();
		textures.insert(texturePair(name, (sharedTexPtr)newTexture));
	}
	textures[name]->loadData(pixels);
}
void TextureContainer::addTexture(sharedTexPtr texture, string name)
{
	if (textures.find(name) != textures.end()) {
		textures.insert(texturePair(name, texture));
	}
}
void TextureContainer::removeTexture(string name) 
{
	if (textures.find(name) != textures.end()) {
		textures.erase(name);
	}
}