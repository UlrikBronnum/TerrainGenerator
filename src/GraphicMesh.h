#pragma once

#include "ofMain.h"

class GraphicMesh 
{
public:
	GraphicMesh();
	~GraphicMesh();

	void draw();
	void drawWireframe();
	void drawNormals();


	void createMesh(ofMesh& m);
	void createQuad(int w, int h, 
					ofVec2f texCoord1 = ofVec2f(1.0f,1.0f) , ofVec2f texCoord2 = ofVec2f(0.0f, 1.0f), 
					ofVec2f texCoord3 = ofVec2f(1.0f, 0.0f), ofVec2f texCoord4 = ofVec2f(0.0f, 0.0f));

	void createMeshFromImage(ofPixels& heightmap, float multiplier, float maxHeight);
	void setMeshHeightsFromImage(ofPixels& heightmap, float multiplier, float maxHeight);

	void resizeQuad(int w, int h);

	float getGridHeight(int x, int y);
	
	ofVec3f getCenter() { return mesh.getCentroid(); }


	void loadAttributes(ofShader& shader);

	void calcNormals();
	void calcTangents();

	void smoothNormals(float angle){ mesh.smoothNormals(angle); }
	void addVertex(ofVec3f vert) { mesh.addVertex(vert); }
	void addNormal(ofVec3f norm) { mesh.addNormal(norm); }
	void addTangent(ofVec3f tan) { tangents.push_back(tan); }
	void addIndex(int index) { mesh.addIndex(index); }
	void addTexCoord(ofVec2f coord) { mesh.addTexCoord(coord); }
	void addColor(ofColor color) { mesh.addColor(color); }

	void clear() { mesh.clear(); }

	void save();

protected:

	ofVboMesh		mesh;
	
	vector<ofVec3f> tangents;
	
	int width;
	int height;
};

