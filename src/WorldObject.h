#pragma once

#include "ofMain.h"
#include "GraphicMesh.h"

class WorldObject 
{

public:

	WorldObject();
	~WorldObject();

	void			draw();

	void			setPosition(ofVec3f newPosition);
	ofVec3f			getPosition();

	void			setRotation(ofVec3f newRotation);
	ofVec3f			getRotation();

	void			setMeshRef(string meshName);
	string			getMeshRef();

	void			setShaderRef(string shaderName);
	string			getShaderRef();

	void			setTextureRef(string textureName);
	string			getTextureRef();


protected:

	ofVec3f		position;
	ofVec3f		rotation;

	string		mesh;
	string		shader;

	string		texture;
	
};

