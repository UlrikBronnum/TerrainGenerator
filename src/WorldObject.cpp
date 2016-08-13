#include "WorldObject.h"



WorldObject::WorldObject()
{

}
WorldObject::~WorldObject()
{

}
void WorldObject::setPosition(ofVec3f newPosition)
{
	position = newPosition;
}
ofVec3f WorldObject::getPosition()
{
	return position;
}
void WorldObject::setRotation(ofVec3f newRotation)
{
	rotation = newRotation;
}
ofVec3f WorldObject::getRotation()
{
	return rotation;
}
void WorldObject::setMeshRef(string meshName)
{
	mesh = meshName;
}
string WorldObject::getMeshRef()
{
	return mesh;
}

void WorldObject::setShaderRef(string shaderName)
{
	shader = shaderName;
}
string	WorldObject::getShaderRef()
{
	return shader;
}

void WorldObject::setTextureRef(string textureName)
{
	texture = textureName;
}
string WorldObject::getTextureRef()
{
	return texture;
}