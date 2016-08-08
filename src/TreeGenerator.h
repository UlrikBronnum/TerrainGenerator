#pragma once

#include "ofMain.h"
#include "GraphicMesh.h"

struct BranchingNode 
{
	vector<BranchingNode> dirPoints;
	ofVec3f				  startPoint;
	float 				  branchRadius;
	int					  branchPos;
	int					  index;
	int					  parent;
};
struct treeStructure 
{
	vector<ofVec3f> vertices;
	vector<int>		indices;

	int				texPos;
	int				perantIndex;
};


class TreeGenerator
{
public:
	TreeGenerator();
	~TreeGenerator();

	void draw();
	void generate(GraphicMesh &mesh);

	ofVec3f getStart() {return start;}

protected:

	void genMesh(GraphicMesh &mesh);
	void genShape();
	void genPoints();

	int numOfSections;

	vector<BranchingNode>		points;
	vector<treeStructure>		treeMesh;


	ofVec3f start;
};

