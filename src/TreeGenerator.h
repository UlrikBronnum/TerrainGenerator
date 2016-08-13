#pragma once

#include "ofMain.h"
#include "GraphicMesh.h"
#include "Gui.h"


struct BranchingNode 
{
	ofVec3f				  startPoint;
	ofVec3f				  pointDir;
	float 				  branchRadius;
	int					  branchPos;
	int					  index;
	int					  parent;
};
struct Branch 
{
	vector<BranchingNode> branchPoints;
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

	bool update(GraphicMesh &mesh);
	void draw();
	void generate(GraphicMesh &mesh);

	ofVec3f getStart() {return start;}

protected:

	void genMesh(GraphicMesh &mesh);
	void genShape();

	vector<BranchingNode> genBranches(BranchingNode nodes, int index, int numOfIndices, float branchHeight, bool isStem);

	void genPoints();

	ofVec3f getBranchDir();
	ofVec3f getStemDir();

	int numOfSections;

	vector<Branch>				branches;

	vector<BranchingNode>		points;
	vector<treeStructure>		treeMesh;

	ofVec3f start;

	GuiMenu menu;

	float	 stemToBranchThikness;
	int      maxSplits;
	int      whenToSplit;
	int      meshResolution;
	float	 meshHeight;
	float	 meshRadius;
	int		 numberOfSplits;
	float	 branchSpread;
	float	 stemSpread;
};

