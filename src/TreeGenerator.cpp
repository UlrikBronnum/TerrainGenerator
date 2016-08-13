#include "TreeGenerator.h"



TreeGenerator::TreeGenerator()
{
	menu.setup(256, 1, "", 0, 0, false);
	menu.addIntSlider(256, 32, "Mesh Resolution", 3, 100, 5);
	menu.addFloatSlider(256, 32, "Mesh Height", 50, 500, 50);
	menu.addFloatSlider(256, 32, "Mesh Radius", 3, 25, 5);
	menu.addIntSlider(256, 32, "Max Splits", 1, 100, 50);
	menu.addIntSlider(256, 32, "When To Split", 1, 10, 3);
	menu.addIntSlider(256, 32, "Number of Splits", 1, 7, 3);
	menu.addFloatSlider(256, 32, "Stem Spread", 0.0f, 0.5f, 0.33f);
	menu.addFloatSlider(256, 32, "Branch Spread", 0.0f, 1.0f, 0.33f);
	menu.addFloatSlider(256, 32, "Stem To Branch Thikness", 0.1f, 1.0f, 0.75f);
	menu.addButton(256, 32, "Generate");

	menu.getButton("Generate")->setState(true);

	stemToBranchThikness = 0.75f;
	maxSplits = 50;
	whenToSplit = 3;
	meshResolution = 5;
	numberOfSplits = 3;
	stemSpread = 0.33f;
	branchSpread = 0.33f;
}


TreeGenerator::~TreeGenerator()
{

}
bool TreeGenerator::update(GraphicMesh &mesh)
{
	stemToBranchThikness = menu.getFloatSlider("Stem To Branch Thikness")->getValue();
	maxSplits = menu.getIntSlider("Max Splits")->getValue();
	whenToSplit = menu.getIntSlider("When To Split")->getValue();
	meshResolution = menu.getIntSlider("Mesh Resolution")->getValue();
	meshHeight = menu.getFloatSlider("Mesh Height")->getValue();
	meshRadius = menu.getFloatSlider("Mesh Radius")->getValue();
	numberOfSplits = menu.getIntSlider("Number of Splits")->getValue();
	branchSpread = menu.getFloatSlider("Branch Spread")->getValue();
	stemSpread = menu.getFloatSlider("Stem Spread")->getValue();

	if (menu.getButton("Generate")->getState())
	{
		generate(mesh);
		menu.getButton("Generate")->setState(false);
		return true;
	}
	return false;
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
	ofSetColor(255, 255, 255);
	menu.draw(0,0);
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
			mesh.addNormal(ofVec3f(0, 0, 1));
			mesh.addTangent(ofVec3f(0, 0, 1));

			float num = (float)k / ( (float)treeMesh[i].vertices.size() - 1.0f);
			mesh.addColor(ofFloatColor(0, 0, treeMesh[i].texPos / (float)numOfSections));
			mesh.addTexCoord(ofVec2f(  num, treeMesh[i].texPos) );
		}
	}
	
	for (int i = 1; i < treeMesh.size(); i++)
	{	
		int parent = treeMesh[i].perantIndex;
		if(parent != -1)
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

	// Slider meshResolution
	int pointsPerCircumfrence = meshResolution;


	float angle = 360.0f / (float)(pointsPerCircumfrence);
	float radius = meshRadius;
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
		
		treeMesh.push_back(treeStruct);
	}
}

void TreeGenerator::genPoints()
{
	points.clear();
	branches.clear();


	BranchingNode mainStemNode;
	mainStemNode.startPoint = ofVec3f(0, 0, 0);
	mainStemNode.pointDir = ofVec3f(0, 0, 1);
	mainStemNode.branchRadius = 0.5f;
	mainStemNode.parent = -1;
	mainStemNode.index = 0;
	mainStemNode.branchPos = 0;

	Branch b;
	b.branchPoints = genBranches(mainStemNode, 0, maxSplits, meshHeight, true);
	for (int h = 0; h < b.branchPoints.size(); h++)
	{
		b.branchPoints[h].index = points.size();
		points.push_back(b.branchPoints[h]);
	}
	
	int basePoints = b.branchPoints.size();

	int step = ((float)maxSplits / 15.0f < 1) ? 1 : maxSplits / 15;
	for (int i = whenToSplit; i < basePoints; i+= step)
	{
		for (int j = 0; j < numberOfSplits; j++)
		{
			Branch b1;
			b1.branchPoints = genBranches(b.branchPoints[i], j, basePoints * (1.0 - ((float)i / (float)basePoints)), (meshHeight * 0.75f) * (1.0 - ((float)i / (float)basePoints)) * stemToBranchThikness, false);
			for (int h = 0; h < b1.branchPoints.size(); h++)
			{
				b1.branchPoints[h].index = points.size();
				if (h != 0)
					b1.branchPoints[h].parent = points.size() - 1;

				points.push_back(b1.branchPoints[h]);
			}
			branches.push_back(b1);
		}
	}
	for (int g = 0; g < branches.size(); g ++)
	{
		int basePointsSize = branches[g].branchPoints.size();

		int branchStep = ((float)basePointsSize / 15.0f < 1) ? 1 : basePointsSize / 15;
		for (int i = 1; i < basePointsSize; i += branchStep)
		{
			for (int j = 0; j < numberOfSplits; j++)
			{
				Branch b1;
				b1.branchPoints = genBranches(branches[g].branchPoints[i], j, basePointsSize * (1.0 - ((float)i / (float)basePoints)), (meshHeight * 0.25f)  * (1.0 - ((float)i / (float)basePoints)) * stemToBranchThikness, false);
				for (int h = 0; h < b1.branchPoints.size(); h++)
				{
					b1.branchPoints[h].index = points.size();
					if (h != 0)
						b1.branchPoints[h].parent = points.size() - 1;

					points.push_back(b1.branchPoints[h]);
				}
			}
		}
	}

}


/*
void TreeGenerator::genPoints()
{
	float height = 12.0f;

	points.clear();

	vector<BranchingNode> initialBranch;

	BranchingNode mainStemNode;
	mainStemNode.startPoint = ofVec3f(0, 0, 0);
	mainStemNode.pointDir = ofVec3f(0, 0, -1);
	mainStemNode.branchRadius = 1.0f;
	mainStemNode.index = 0;
	mainStemNode.branchPos = 0;
	points.push_back(mainStemNode);
	initialBranch.push_back(mainStemNode);

	float tmpHeight = height;
	float tmpSize = mainStemNode.branchRadius;

	for (int i = 1; i < maxSplits; i++)
	{
		ofVec3f start = points[i - 1].startPoint;
		ofVec3f direction = points[i - 1].pointDir;

		float	size = points[i - 1].branchRadius;
		int		node = points[i - 1].index;
		
		float	sectionHeight = (float)maxSplits / (float)tmpHeight; // ( 1.0f - (float)i / (float)maxSplits ) * ( (float)tmpHeight)  ;
		float	sectionRadius = (float)tmpSize / ((float)maxSplits - 1.0f);

		ofVec3f newDirection = (direction + getStemDir()).normalize();
		float	distance = sectionHeight;

		ofVec3f newStartPoint = start + newDirection * distance;

		BranchingNode stemNode;
		stemNode.startPoint = newStartPoint;
		stemNode.branchRadius = tmpSize - sectionRadius * i;
		stemNode.pointDir = newDirection;
		stemNode.index = points.size();
		stemNode.parent = node;
		stemNode.branchPos = i;

		points.push_back(stemNode);

		//tmpHeight -= tmpHeight * 0.06;
	}
	int pointsArraySize = points.size();

	for (int i = whenToSplit; i < pointsArraySize; i+=2)
	{
		for (int j = 0; j < numberOfSplits; j++)
		{
			vector<BranchingNode> mainBranch = genBranches(points[i], i, pointsArraySize, height, stemToBranchThikness);
			if(ofRandom(0,1)>0.33){
				for (int k = 0; k < mainBranch.size(); k++)
				{
					points.push_back(mainBranch[k]);
				}
				for (int k = 0; k < mainBranch.size(); k++)
				{
					vector<BranchingNode> subBranch = genBranches(mainBranch[k], k, mainBranch.size(), height, stemToBranchThikness);
					for (int h = 0; h < subBranch.size(); h++)
					{
						points.push_back(subBranch[h]);
					}
					for (int g = 0; g < subBranch.size(); g++)
					{
						vector<BranchingNode> sub2Branch = genBranches(subBranch[g], g, subBranch.size(), height, stemToBranchThikness);
						for (int f = 0; f < sub2Branch.size(); f++)
						{
							points.push_back(sub2Branch[f]);
						}
					}
				}
			}
		}
	}
}
*/
vector<BranchingNode> TreeGenerator::genBranches(BranchingNode parentNode, int index, int numOfIndices, float branchHeight, bool isStem)
{
	vector<BranchingNode> branchNodes;

	BranchingNode branchNode;
	branchNode.startPoint = parentNode.startPoint;
	if (isStem) 
	{
		branchNode.branchRadius = parentNode.branchRadius;
	}
	else 
	{
		branchNode.branchRadius = parentNode.branchRadius / 3.0f;// *stemToBranchThikness;
	}

	branchNode.pointDir = parentNode.pointDir;
	branchNode.index = branchNodes.size();
	branchNode.parent = -1;
	branchNode.branchPos = parentNode.branchPos + 1;
	branchNodes.push_back(branchNode);

	ofVec3f start = branchNode.startPoint;
	ofVec3f baseDirection = branchNode.pointDir;
	float	size = branchNode.branchRadius;
	int		node = branchNode.index;
	int		tex = branchNode.branchPos;

	int		selector = ofRandom(0, 12.5) + index;
	float	subdivisions = 12.0f;

	float	angle = 360.0f / subdivisions;
	float	random = ofDegToRad(angle * selector);
	
	ofVec3f direction;
	
	if (isStem) 
	{
		direction = baseDirection;
	}
	else 
	{
		direction = (baseDirection + ofVec3f(cosf(random), sinf(random))).normalize();
	}

	for (int k = 0; k < numOfIndices; k++)
	{
		ofVec3f modDir;

		if (isStem) {
			modDir = getStemDir();
		}
		else {
			modDir = getBranchDir();
			direction *= 2.0f;
		}

		float	sectionHeight = (float)branchHeight / ((float)numOfIndices);
		float	sectionRadius = (float)size / ((float)numOfIndices);

		ofVec3f newDirection = (direction + modDir).normalize();

		ofVec3f newStartPoint = start + newDirection * sectionHeight;

		BranchingNode branchNode;
		branchNode.startPoint = newStartPoint;
		branchNode.branchRadius = size - sectionRadius * (float)(k+1);
		branchNode.pointDir = newDirection;
		branchNode.index = branchNodes.size();
		branchNode.parent = node;
		branchNode.branchPos = tex + (k + 1);

		branchNodes.push_back(branchNode);

		start = branchNode.startPoint;
		direction = (direction + branchNode.pointDir).normalize();
		node = branchNode.index;
	}
	
	return branchNodes;
}
/*
void TreeGenerator::genPoints()
{
	float height = 12.0f;

	points.clear();

	BranchingNode mainStemNode;
	mainStemNode.startPoint = ofVec3f(0, 0, 0);
	mainStemNode.pointDir = ofVec3f(0, 0, -1);
	mainStemNode.branchRadius = 1.0f;
	mainStemNode.index = 0;
	mainStemNode.branchPos = 0;
	points.push_back(mainStemNode);

	for (int i = 1; i < maxSplits; i++)
	{

		float	sectionHeight = (1.0f - (float)i / (float)maxSplits) * ((float)height);

		ofVec3f start = points[i - 1].startPoint;
		ofVec3f direction = points[i - 1].pointDir;

		float	size = points[i - 1].branchRadius;
		int		node = points[i - 1].index;

		ofVec3f newDirection = (direction + getStemDir()).normalize();
		float	distance = sectionHeight;

		ofVec3f newStartPoint = start + newDirection * distance;

		BranchingNode stemNode;
		stemNode.startPoint = newStartPoint;
		stemNode.branchRadius = size * 0.90f;
		stemNode.pointDir = newDirection;
		stemNode.index = points.size();
		stemNode.parent = node;
		stemNode.branchPos = i;

		points[i - 1].dirPoints.push_back(stemNode);
		points.push_back(stemNode);

		height -= height * 0.06;
	}
	int pointsArraySize = points.size();

	for (int i = whenToSplit; i < pointsArraySize; i++)
	{
		for (int j = 0; j < numberOfSplits; j++)
		{
			ofVec3f start = points[i].startPoint;
			ofVec3f baseDirection = points[i].pointDir;
			float	size = points[i].branchRadius;
			int		node = points[i].index;

			height = 12.0f * (1.0f - (float)i / ((float)pointsArraySize));

			float	angle = 360.0f / 4.0f;
			float	random = ofDegToRad(angle * ofRandom(0, 4.5));

			ofVec3f direction = (baseDirection + ofVec3f(cosf(random), sinf(random))).normalize();

			for (int k = 0; k < maxSplits / 4; k++)
			{
				float	sectionHeight = (1.0f - (float)k / ((float)maxSplits / 4)) * ((float)height);

				float	distance = sectionHeight;

				ofVec3f newDirection = (direction * distance + getBranchDir()).normalize();

				ofVec3f newStartPoint = start + newDirection * distance;

				BranchingNode branchNode;
				branchNode.startPoint = newStartPoint;
				branchNode.branchRadius = size * stemToBranchThikness * 0.90f;
				branchNode.pointDir = newDirection;
				branchNode.index = points.size();
				branchNode.parent = node;
				branchNode.branchPos = i + k + 1;

				points.push_back(branchNode);

				start = branchNode.startPoint;
				direction = branchNode.pointDir;
				size = branchNode.branchRadius;
				node = branchNode.index;

				height -= height * 0.12f;
			}
		}
	}
}
*/
/*
void TreeGenerator::genPoints()
{
	points.clear();

	BranchingNode b;
	b.startPoint = ofVec3f(0, 0, 0);
	b.pointDir = ofVec3f(0, 0, 1);
	b.branchRadius = 1.0f;
	b.index = 0;
	b.branchPos = 0;
	start = b.startPoint;
	points.push_back(b);


	//Slider maxSplits
	int splits = maxSplits;

	numOfSections = splits;


	int height = 10;


	for (int i = 1; i < splits; i++)
	{
		int split = 1;
		
		//Slider whenToSplit
		if (i > whenToSplit) {
			//Slider numOfSplit;
			split = numberOfSplits;
		}
		ofVec3f start = points[i - 1].startPoint;
		ofVec3f pDir = points[i - 1].pointDir;

		float	size  = points[i - 1].branchRadius;
		int		node  = points[i - 1].index;

		for (int k = 0; k < split; k++)
		{
			float distMod = ofRandom((float)splits / 2, (float)splits) * (1.5 - ((float)i / (float)splits / 2) * 0.5);

			BranchingNode tP;
			float mod = 1.0 - ((float)i / (float)splits);
			float dist = 1.0 - mod * distMod;

			// stemToBranchThikness
			float stemMod = stemToBranchThikness;

			ofVec3f dir = ofVec3f(0,0,0);

			if (k == 0) 
			{
				dir = start + (pDir + getStemDir()).normalize() * dist;
				tP.startPoint = dir;
				tP.branchRadius = size * stemMod * mod;
				tP.pointDir = dir.normalize();
			}
			else 
			{
				dir = pDir + (getBranchDir());
				tP.startPoint = start + dir * dist * mod;
				tP.branchRadius = size * (1.0-stemMod);
				tP.pointDir = dir.normalize();
			}
			tP.index = points.size();
			tP.parent = node;
			tP.branchPos = i;

			points[i - 1].dirPoints.push_back(tP);
			points.push_back(tP);
		}
	}
}
*/
ofVec3f TreeGenerator::getBranchDir() {
	ofVec2f dir = ofVec2f(cosf(ofRandom(0, TWO_PI)), sinf(ofRandom(0, TWO_PI))) * branchSpread;
	return ofVec3f(dir.x, dir.y, ofRandom(-1,1)).normalize();
}
ofVec3f TreeGenerator::getStemDir() {
	ofVec2f dir = ofVec2f(cosf(ofRandom(0, TWO_PI) ) , sinf(ofRandom(0, TWO_PI))) * stemSpread;
	return ofVec3f(dir.x, dir.y, 1.0).normalize();
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