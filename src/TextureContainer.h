#pragma once
#include "ofMain.h"
#include "ImageProcessing.h"
//#include "GraphicMesh.h"
//#include "Gui.h"

typedef shared_ptr<ofTexture>	sharedTexPtr;

typedef std::map<string, sharedTexPtr> textureMap;
typedef std::pair<string, sharedTexPtr> texturePair;
typedef std::map<string, sharedTexPtr>::iterator it;


class TextureContainer
{
public:
	TextureContainer();
	~TextureContainer();

	string			loadImageFile(string path);

	void			addTexture(ofPixels& pixels, string name);
	void			addTexture(sharedTexPtr texture, string name);

	sharedTexPtr	getTexture(string name);
	bool			readToPixels(ofPixels& pixels, string name);

	int				getNumTextures() { return numTextures; }

	void			removeTexture(string name);

private:

	textureMap	textures;
	int			numTextures;


};

