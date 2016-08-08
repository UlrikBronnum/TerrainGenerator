#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::waterErode() {
	
	ofPixels terrain2;
	terrain2.allocate(dims, dims, OF_IMAGE_COLOR);

	for (int y = 0; y < dims; y++) {
		for (int x = 0; x < dims; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				terrain2.getPixels()[(y * dims + x) * 3 + c] = 0;
			}
		}
	}

	for (int y = 0; y < dims; y++) {
		for (int x = 0; x < dims; x++)
		{
			ImageProcessing::waterFlow(terrain.getPixels(), terrain2.getPixels(), dims, dims, x, y, ofVec2f(ofRandom(-1.5, 1.5), ofRandom(-1.5, 1.5)));
		}
	}
	waterFlowChart.loadData(terrain2);


	float min = lMenu.getIntSlider("Min")->getValue();
	float max = lMenu.getIntSlider("Max")->getValue();
	

	GreyProcessing::greyStretch(terrain.getPixels(), terrain.getPixels(), terrain.getWidth(), terrain.getHeight(), min, max);
	
	Kernel k;
	k.setSize(11);
	k.gaussianKernel(1.2f);

	ofPixels terrain3;
	terrain3.allocate(dims, dims, OF_IMAGE_GRAYSCALE);
	GreyProcessing::greyBlur(terrain.getPixels(), terrain3.getPixels(), terrain.getWidth(), terrain.getHeight(), k, CLAMP);
	
	for (int y = 0; y < dims; y++) {
		for (int x = 0; x < dims; x++)
		{
			terrain.getPixels()[y * dims + x] = terrain3.getPixels()[y * dims + x];
		}
	}
	
	int height = lMenu.getIntSlider("Height")->getValue();
	mesh.createMeshFromImage(terrain, 50.0f, height);
	mesh.loadAttributes(shader);
	heightChart.loadData(terrain);
}
//--------------------------------------
void ofApp::waterErodeClass() 
{
	eroder.runErosion();

	int height = lMenu.getIntSlider("Height")->getValue();
	mesh.createMeshFromImage(*eroder.getHeightMap(), 50.0f, height);
	mesh.loadAttributes(shader);
	heightChart.loadData(*eroder.getHeightMap());

	waterFlowChart.loadData(*eroder.getFlowMap());

}
//--------------------------------------------------------------
void ofApp::generateTerrain() 
{
	/*
	ofPixels terrain2;
	terrain2.setFromExternalPixels(bhc.getMap()->getPixels(), bhc.getMap()->getWidth(), bhc.getMap()->getHeight(), OF_IMAGE_GRAYSCALE);
	
	float	roughness = lMenu.getFloatSlider("Roughness")->getValue();
	int		startSize = lMenu.getIntSlider("Start size")->getValue();
	GreyProcessing::greyDiamondSquare(terrain2.getPixels(), terrain.getWidth(), terrain.getWidth()/ startSize, roughness);

	Kernel k;
	k.setSize(3);
	k.gaussianKernel(1.0f);
	GreyProcessing::greyBlur(terrain2.getPixels(), terrain.getPixels(), terrain.getWidth(), terrain.getHeight(), k, CLAMP);
	*/
	bhc.generateMap();

	heightChart.loadData(*bhc.getMap());
	terrain.setFromPixels(bhc.getMap()->getPixels(), bhc.getMap()->getWidth(), bhc.getMap()->getWidth(), OF_IMAGE_GRAYSCALE);

	int height = lMenu.getIntSlider("Height")->getValue();
	mesh.createMeshFromImage(terrain, 50.0f, height);
	mesh.loadAttributes(shader);
}
//--------------------------------------------------------------
void ofApp::setup()
{
	dims = 513;



	bhc.setup(dims);


	ofDisableArbTex();
	//glEnable(GL_DEPTH_TEST);
	lightPosition = ofVec4f(0, -1, -0.9, 0);

	lMenu.setup(256, 16, "Image Options", 0, 0, false);
	lMenu.addIntSlider(256, 32, "tiling", 1, 512, 32);

	lMenu.addIntSlider(256, 32, "Min", 0, 255, 10);
	lMenu.addIntSlider(256, 32, "Max", 0, 255, 255);

	lMenu.addFloatSlider(256, 32, "Roughness", 0, 5, 1);
	lMenu.addIntSlider(256, 32, "Start size", 1, 12, 1);
	lMenu.addIntSlider(256, 32, "Height", 100, 10000, 3000);

	//lMenu.addElement((sharedGuiPtr)eroder.getMenu());

	ofFbo::Settings settings;
	settings.width = 1024;
	settings.height = 768;
	settings.useDepth = true;
	settings.useStencil = false;
	settings.depthStencilAsTexture = true;
	fbo.allocate(settings);
	treeFbo.allocate(settings);

	settings.width = 1024;
	settings.height = 768;
	settings.useDepth = false;
	settings.useStencil = false;
	settings.depthStencilAsTexture = false;
	afterEffectFbo.allocate(settings);

	settings.width = 1024;
	settings.height = 768;
	settings.useDepth = false;
	settings.useStencil = false;
	settings.depthStencilAsTexture = false;
	depthFbo.allocate(settings);
	depthShader.load("res/shaders/depthVertex.vert", "res/shaders/depthFragment.frag");
	
	
	
	terrain.allocate(dims, dims, OF_IMAGE_GRAYSCALE);
	
	waterFlowChart.allocate(dims,dims,GL_RGBA);

	//shader.load("res/shaders/generateSplatmap.vert", "res/shaders/generateSplatmap.frag");
	shader.load("res/shaders/terrainNormal.vert", "res/shaders/terrainNormal.frag");
	afterEffectShader.load("res/shaders/defaultVertex.vert", "res/shaders/afterEfectFragment.frag");
	
	

	//generateTerrain();

	eroder.loadHeightMap(terrain);


	treeShader.load("res/shaders/meshNormal.vert", "res/shaders/meshNormal.frag");
	tree.generate(treeMesh);
	treeMesh.loadAttributes(treeShader);
	
	treeDiffuse.loadImage("res/textures/barkD.jpg");
	treeDiffuse.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	treeDiffuse.getTexture().generateMipmap();
	treeDiffuse.getTexture().setTextureMinMagFilter(GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR);
	treeNormal.loadImage("res/textures/barkN.png");
	treeNormal.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	treeNormal.getTexture().generateMipmap();
	treeNormal.getTexture().setTextureMinMagFilter(GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR);

	camera.lookAt(tree.getStart());
	camera.setCamPosition(tree.getStart() + ofVec3f(0, 0, 10));
	
	//camera.setCamPosition(mesh.getCenter() + ofVec3f(0, 0, 3000));
	//camera.lookAt(mesh.getCenter());

	diffuse1.loadImage("res/textures/sandDiffuse.jpg");
	diffuse1.resize(512, 512);
	diffuse1.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	diffuse1.getTexture().generateMipmap();
	diffuse1.getTexture().setTextureMinMagFilter(GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR);
	normal1.loadImage("res/textures/sandNormal.png");
	normal1.resize(512, 512);
	normal1.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	normal1.getTexture().generateMipmap();
	normal1.getTexture().setTextureMinMagFilter(GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR);

	diffuse2.loadImage("res/textures/grassDiffuse.jpg");
	diffuse2.resize(512, 512);
	diffuse2.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	diffuse2.getTexture().generateMipmap();
	diffuse2.getTexture().setTextureMinMagFilter(GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR);
	normal2.loadImage("res/textures/grassNormal.png");
	normal2.resize(512, 512);
	normal2.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	normal2.getTexture().generateMipmap();
	normal2.getTexture().setTextureMinMagFilter(GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR);

	diffuse3.loadImage("res/textures/gravelDiffuse.jpg");
	diffuse3.resize(512, 512);
	diffuse3.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	diffuse3.getTexture().generateMipmap();
	diffuse3.getTexture().setTextureMinMagFilter(GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR);
	normal3.loadImage("res/textures/gravelNormal.png");
	normal3.resize(512, 512);
	normal3.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	normal3.getTexture().generateMipmap();
	normal3.getTexture().setTextureMinMagFilter(GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR);

	diffuse4.loadImage("res/textures/cliffDiffuse.jpg");
	diffuse4.resize(512, 512);
	diffuse4.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	diffuse4.getTexture().generateMipmap();
	diffuse4.getTexture().setTextureMinMagFilter(GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR);
	normal4.loadImage("res/textures/cliffNormal.png");
	normal4.resize(512, 512);
	normal4.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	normal4.getTexture().generateMipmap();
	normal4.getTexture().setTextureMinMagFilter(GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR);

	
}

//--------------------------------------------------------------
void ofApp::update()
{
	camera.update();
	eroder.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	int tiling = lMenu.getIntSlider("tiling")->getValue();

	
/*
	ofFbo slope;
	GPUProcessing::normalmap(heightChart, slope, kirsch_horizontal, kirsch_vertical, 1.4, ofVec3f(1, 0, 0));

	ofPushMatrix();
	fbo.begin();
	ofClear(0, 0, 0, 255);

	shader.begin();
	shader.setUniformTexture("slopeMap", slope.getTexture(), 1);
	shader.setUniformTexture("waterMap", waterFlowChart, 2);

	fbo.draw(0,0);

	shader.end();

	fbo.end();
	ofPopMatrix();
	*/

	
	/*

	fbo.begin();
	camera.begin();

	ofClear(0, 0, 0, 255);
		
		
		ofEnableDepthTest();
		shader.begin();
		
		shader.setUniformTexture("texColor0", diffuse1.getTexture(), 1);
		shader.setUniformTexture("texNormal0", normal1.getTexture(), 2);
		shader.setUniformTexture("texColor1", diffuse2.getTexture(), 3);
		shader.setUniformTexture("texNormal1", normal2.getTexture(), 4);
		shader.setUniformTexture("texColor2", diffuse3.getTexture(), 5);
		shader.setUniformTexture("texNormal2", normal3.getTexture(), 6);
		shader.setUniformTexture("texColor3", diffuse4.getTexture(), 7);
		shader.setUniformTexture("texNormal3", normal4.getTexture(), 8);
		shader.setUniformTexture("texColorWater", waterFlowChart, 9);

		shader.setUniform3f("ambientLight", ofVec3f(0.1f,0.1f,0.3f));
		shader.setUniform4f("lightPos", lightPosition);
		shader.setUniform4f("tiling", ofVec4f(tiling, tiling,0,0));

		shader.setUniform1f("nearClip", camera.getNearClip());
		shader.setUniform1f("farClip", camera.getFarClip());

			mesh.draw();

		shader.end();
		ofDisableDepthTest();
		
		
	camera.end();
	fbo.end();


	
	ofDisableDepthTest();

	
	afterEffectFbo.begin();
	ofClear(0, 0, 0, 255);
	afterEffectShader.begin();
		
		afterEffectShader.setUniform4f("light", lightPosition);
		afterEffectShader.setUniform3f("cameraForward", camera.getLookAtDir());
		afterEffectShader.setUniformTexture("depthTexture", fbo.getDepthTexture(), 1);
		afterEffectShader.setUniformTexture("sceneTexture", fbo.getTexture(), 2);

		fbo.draw(0, 0);

	afterEffectShader.end();
	afterEffectFbo.end();

	ofSetColor(255);
	fbo.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	//afterEffectFbo.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	
	ofSetColor(255,128,128);
	ofDrawBitmapString(ofGetFrameRate(),ofGetWindowWidth() - 64, 16);
	bhc.draw(0, 0);
	eroder.getMenu()->draw(0, bhc.getMenu()->getHeight() + bhc.getMenu()->getWidth());
	waterFlowChart.draw(0, bhc.getMenu()->getHeight() + bhc.getMenu()->getWidth() + eroder.getMenu()->getHeight(), 256, 256);



	ofDisableDepthTest();
	*/
	
	
	

	ofSetBackgroundColor(0);
	treeFbo.begin();
	ofClear(0, 0, 0, 255);
		camera.begin();
	
		ofEnableDepthTest();

		ofPushMatrix();
			treeShader.begin();
			treeShader.setUniformTexture("tex0", treeDiffuse.getTexture(), 1);
			treeShader.setUniformTexture("tex1", treeNormal.getTexture(), 2);
			treeShader.setUniform3f("ambientLight", ofVec3f(0.5f));
			treeShader.setUniform4f("lightPos", lightPosition);
			treeShader.setUniform4f("tiling", ofVec4f(1, 0.5, 0, 0));

			treeMesh.draw();

			treeShader.end();

			treeMesh.drawNormals();
		ofPopMatrix();

		camera.end();
	treeFbo.end();

	//treeFbo.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	ofDisableDepthTest();

	depthFbo.begin();
		depthShader.begin();
		depthShader.setUniformTexture("depthTexture", treeFbo.getDepthTexture(), 1);
		depthShader.setUniform1f("nearClip", camera.getNearClip());
		depthShader.setUniform1f("farClip", camera.getFarClip());

		treeFbo.draw(0, 0);

		depthShader.end();
	depthFbo.end();

	ofSetColor(255, 255, 255);
	depthFbo.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 13) {
		//waterErodeClass();
		tree.generate(treeMesh);
		treeMesh.loadAttributes(treeShader);
		
	}

	if (key == 'e') {
		string fileName = "test.raw";
		ofBuffer buffer((const char*)eroder.getHeightMap()->getPixels(), eroder.getHeightMap()->getWidth() * eroder.getHeightMap()->getHeight());
		ofBufferToFile(fileName, buffer, true);
	}
	if (key == 'g') {
		generateTerrain();
		eroder.loadHeightMap(*bhc.getMap());
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{ 
	
}
