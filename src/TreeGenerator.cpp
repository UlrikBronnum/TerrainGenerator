#include "TreeGenerator.h"



TreeGenerator::TreeGenerator()
{
	
}


TreeGenerator::~TreeGenerator()
{

}
void TreeGenerator::draw() 
{
	/*
	ofSetColor(0, 255, 0);
	for (int i = 0; i < points.size() - 1; i++) {
		for (int k = 0; k < points[i].dirPoints.size(); k++) {
			ofDrawLine(	points[i].startPoint.x, points[i].startPoint.y, points[i].startPoint.z,
						points[i].dirPoints[k].startPoint.x, points[i].dirPoints[k].startPoint.y, points[i].dirPoints[k].startPoint.z);
		}
	}
	*/
}
void TreeGenerator::generate(GraphicMesh &mesh) 
{
	genPoints();
	genShape();
	genMesh(mesh);
}

void TreeGenerator::genMesh(GraphicMesh &mesh)
{
	mesh.clear();

	for (int i = 0; i < treeMesh.size(); i++) 
	{
		for (int k = 0; k < treeMesh[i].vertices.size(); k++) {
			mesh.addVertex(treeMesh[i].vertices[k]);
			mesh.addNormal(ofVec3f(0,0,1));
			mesh.addTangent(ofVec3f(0, 0, 1));

			float num = (float)k / ( (float)treeMesh[i].vertices.size() - 1.0f);// ((float)treeMesh[i].vertices.size())
			mesh.addColor(ofFloatColor(0, 0, treeMesh[i].texPos / (float)numOfSections));
			//mesh.addColor(ofFloatColor(1, num / (float)treeMesh[i].vertices.size(), treeMesh[i].texPos / (float)numOfSections));
			mesh.addTexCoord(ofVec2f(  num, treeMesh[i].texPos) );//(float)numOfSections)
		}
	}
	
	for (int i = 1; i < treeMesh.size(); i++)
	{	
		int parent = treeMesh[i].perantIndex;

		for (int k = 0; k < treeMesh[i].indices.size()-1; k++)
		{
			int next =  k + 1;//(k + 1 == treeMesh[i].indices.size()) ? 0 :

			mesh.addIndex(treeMesh[i].indices[k]);
			mesh.addIndex(treeMesh[parent].indices[k]);
			mesh.addIndex(treeMesh[i].indices[next]);

			mesh.addIndex(treeMesh[parent].indices[k]);
			mesh.addIndex(treeMesh[parent].indices[next]);
			mesh.addIndex(treeMesh[i].indices[next]);
		}
	}
	
}
void TreeGenerator::genShape()
{
	treeMesh.clear();


	int pointsPerCircumfrence = 24;
	float angle = 360.0f / (float)(pointsPerCircumfrence);
	float radius = 10.0f;
	int index = 0;
	for (int i = 0; i < points.size(); i++) 
	{
		float pAngle = 0.0f;
		float modRadius = radius * points[i].branchRadius;

		treeStructure treeStruct;
		ofVec3f		startVert;
		for (int k = 0; k < pointsPerCircumfrence; k++) { 
			pAngle = ofDegToRad(angle*k);

			ofVec3f p = points[i].startPoint + ofVec3f(cosf(pAngle), sinf(pAngle), 0) * (ofRandom(0.95,1.05) * modRadius);
			if (k == 0)startVert = p;
			treeStruct.vertices.push_back(p);
			treeStruct.indices.push_back(index);
			treeStruct.perantIndex = points[i].parent;
			treeStruct.texPos = points[i].branchPos;
			index++;
		}
		
		treeStruct.vertices.push_back(startVert);
		treeStruct.indices.push_back(index);
		treeStruct.perantIndex = points[i].parent;
		treeStruct.texPos = points[i].branchPos;
		index++;
		/**/
		treeMesh.push_back(treeStruct);
	}
}
void TreeGenerator::genPoints()
{
	points.clear();

	BranchingNode b;
	b.startPoint = ofVec3f(0, 0, 0);
	b.branchRadius = 1.0f;
	b.index = 0;
	b.branchPos = 0;
	start = b.startPoint;
	points.push_back(b);

	int splits = 25;

	numOfSections = splits;

	for (int i = 1; i < splits; i++)
	{
		int split = 1;
		if (i > 2) {
			split = 2;
		}
		ofVec3f start = points[i - 1].startPoint;
		float	size  = points[i - 1].branchRadius;
		int		node  = points[i - 1].index;

		for (int k = 0; k < split; k++)
		{
			float dist = ofRandom((float)splits / 2, (float)splits) * (1.5 - ((float)i / (float)splits / 2) * 0.5);

			BranchingNode tP;
			float mod = ((float)i / (float)splits);
			tP.startPoint = start + ofVec3f(ofRandom(-mod, mod), ofRandom(-mod, mod), ofRandom(0.05 + mod, 1.5)).normalize() * dist;
			tP.branchRadius = (size - 0.05f) / (float)split;
			tP.index = points.size();
			tP.parent = node;
			tP.branchPos = i;

			points[i - 1].dirPoints.push_back(tP);
			points.push_back(tP);
		}
	}
}
/*
void TreeGenerator::genPoints()
{
points.clear();

BranchingNode b;
b.startPoint = ofVec3f(0, 0, 0);
b.branchRadius = 1.0f;
b.index = 0;
b.branchPos = 0;
start = b.startPoint;
points.push_back(b);

int splits = 30;

numOfSections = splits;

for (int i = 1; i < splits; i++)
{
int split = 1;
if (i > 2) {
split = 2;
}
ofVec3f start = points[i - 1].startPoint;
float	size  = points[i - 1].branchRadius;
int		node  = points[i - 1].index;

for (int k = 0; k < split; k++)
{
float dist = ofRandom((float)splits / 2, (float)splits) * (1.5 - ((float)i / (float)splits / 2) * 0.5);

BranchingNode tP;
float mod = ((float)i / (float)splits);
tP.startPoint = start + ofVec3f(ofRandom(-mod, mod)*5.0, ofRandom(-mod, mod)*5.0, ofRandom(0.05 + mod, 1.5)).normalize() * dist;
tP.branchRadius = (size - 0.05f) / (float)split;
tP.index = points.size();
tP.parent = node;
tP.branchPos = i;

points[i - 1].dirPoints.push_back(tP);
points.push_back(tP);
}
}
}*/