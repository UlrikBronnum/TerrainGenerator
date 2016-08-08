#pragma once

#include "ofMain.h"
#include "ofGraphics.h "

#include "ImageProcessing.h"
#include "GraphicMesh.h"
#include "Gui.h"
#include "TextureContainer.h"
#include "MainApp.h"
#include "ImageFilter.h"
#include "MoveCam.h"

#include "WaterErosion.h"
#include "BaseHeightmapCreater.h"
#include "TreeGenerator.h"

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void waterErode();
		void waterErodeClass();

		void generateTerrain();

		MoveCam		camera;
		GraphicMesh mesh;
		GraphicMesh sphere;
		ofShader	shader;
		ofShader	afterEffectShader;
		
		ofFbo		treeFbo;
		ofShader	treeShader;
		GraphicMesh treeMesh;
		ofImage		treeDiffuse;
		ofImage		treeNormal;


		ofFbo		depthFbo;
		ofShader	depthShader;


		ofImage		diffuse1;
		ofImage		normal1;
		ofImage		diffuse2;
		ofImage		normal2;
		ofImage		diffuse3;
		ofImage		normal3;
		ofImage		diffuse4;
		ofImage		normal4;

		GuiMenu		lMenu;

		ofFbo		fbo;
		ofFbo		afterEffectFbo;

		ofTexture	waterFlowChart;
		ofTexture	heightChart;

		ofVec4f		lightPosition;
		int			dims;
		ofPixels	terrain;


		WaterErosion	eroder;

		BaseHeightmapCreater bhc;


		TreeGenerator tree;
};
