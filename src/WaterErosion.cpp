#include "WaterErosion.h"
#include "ImageProcessing.h"


WaterErosion::WaterErosion()
{
	width = 0;
	height = 0;

	waterToAdd = 0.03f;

	menu.setup(256, 1, "", 0, 0, false);
	menu.addFloatSlider(256, 32, "water", 0.01f, 1.0f, 1.0f);
	menu.addFloatSlider(256, 32, "max saturation", 0.01f, 0.5f, 0.1f);
	menu.addFloatSlider(256, 32, "Add Soil", 0.001f, 0.5f, 0.05f);
	menu.addFloatSlider(256, 32, "Remove Soil", 0.001f, 0.5f, 0.05f);
}


WaterErosion::~WaterErosion()
{
}

void WaterErosion::update()
{
}

void WaterErosion::loadHeightMap(ofPixels& heights) 
{
	width = heights.getWidth();
	height = heights.getHeight();

	//heightMap.allocate(width, height, OF_IMAGE_GRAYSCALE);
	//heightMap.setFromPixels(heights.getPixels(), width, height, OF_IMAGE_GRAYSCALE);

	heightMap = heights;

	flowMap.allocate(width, height, OF_IMAGE_COLOR_ALPHA);

}
void WaterErosion::runErosion() 
{
	waterToAdd = menu.getFloatSlider("water")->getValue();
	maxSaturation = menu.getFloatSlider("max saturation")->getValue();
	addSoil = menu.getFloatSlider("Add Soil")->getValue();
	removeSoil = menu.getFloatSlider("Remove Soil")->getValue();

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++){
			for (int c = 0; c < 3; c++)
			{
				flowMap.getPixels()[(y * width + x) * 4 + c] = 0;
			}
			flowMap.getPixels()[(y * width + x) * 4 + 3] = 255;
		}
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			waterFlowErosion(x, y, ofVec2f(ofRandom(-1.5, 1.5), ofRandom(-1.5, 1.5)) , waterToAdd);
		}
	}

	Kernel k;
	k.setSize(3);
	k.gaussianKernel(1.0f);

	ofPixels terrain;
	terrain.allocate(width, height, OF_IMAGE_GRAYSCALE);
	GreyProcessing::greyBlur(heightMap.getPixels(), terrain.getPixels(), width, height, k, CLAMP);

	heightMap.setFromPixels(terrain.getPixels(), width, height, OF_IMAGE_GRAYSCALE);
	
}
void WaterErosion::waterFlowErosion(int x, int y, ofVec2f directionVector, float water, float sediments, float lastSlope)
{
	if ((x >= 0 && x < width) && (y >= 0 && y < height))
	{
		int aValue = flowMap.getPixels()[(y * width + x) * 4 + 3];
		int bValue = flowMap.getPixels()[(y * width + x) * 4 + 2];
		int gValue = flowMap.getPixels()[(y * width + x) * 4 + 1];
		int rValue = flowMap.getPixels()[(y * width + x) * 4 + 0];

		int	hValue = heightMap.getPixels()[y * width + x];

		ofVec2f newDir;

		int	slope	= water ; 
		int step	= 0;

		int	radius	= 1;


		for (int fy = -radius; fy <= radius; fy++) {
			for (int fx = -radius; fx <= radius; fx++)
			{
				if (fy != 0 || fx != 0)
				{
					int nY = y + fy;
					int nX = x + fx;

					if ((nX >= 0 && nX < width) && (nY >= 0 && nY < height))
					{
						
						int localCell = heightMap.getPixels()[nY * width + nX];
						int localSlope = localCell - hValue;

						if (localSlope < slope)
						{
							ofVec2f localDir = ofVec2f(fx, fy);
							float theDot = directionVector.dot(localDir);
							
							if (lastSlope > -2) {
								if (theDot > -0.1 && theDot < 1 && aValue != 0)
								{
									slope = localSlope;
									newDir = ofVec2f(fx, fy);
								}
							}
							else {
								if (theDot >= 0 && aValue != 0)
								{
									slope = localSlope;
									newDir = ofVec2f(fx, fy);
									step = (localSlope > 0) ? localSlope : 0;
								}
							}
							
						}
					}
					else {
						return;
					}
				}
			}
		}

		

		if (newDir != ofVec2f(0, 0))
		{
			flowMap.getPixels()[(y * width + x) * 4 + 3] = 0;

			int tmpSlope = slope;
			if (water > 0) 
			{
				int step = slope;

				if (slope >= 0)
				{
					if (rValue > 0)
					{
						water += rValue;
						flowMap.getPixels()[(y * width + x) * 4 + 0] = 0;
					}
					water = water - slope;
					slope = -water;
				}

				float saturation = sediments / water;
				float force = water + abs(slope * 0.66 + lastSlope * 0.34 );

				if (slope <= lastSlope && saturation < water/8.0f)
				{
					float carryingCap = (water * maxSaturation - saturation) / (water * maxSaturation);
					float amount = (carryingCap > 0) ? (carryingCap * abs(force) ) * removeSoil: 0;

					if (amount < 0) {
						amount = 0;
					}
					else if (amount > 3) {
						amount = 3;		
					}
	
					heightMap.getPixels()[y * width + x] = (hValue - amount > 0) ? hValue - amount : 0;

					sediments += amount/3;
					water += waterToAdd;

					flowMap.getPixels()[(y * width + x) * 4 + 1] = (gValue++ > 255) ? 255 : gValue++;
				}
				
				if (step >= lastSlope)
				{
					float amount = sediments * addSoil ;
					if (amount < 0) 
					{
						amount = 0;
					}

					heightMap.getPixels()[y * width + x] = (hValue + amount <= 255) ? hValue + amount : 255;

					sediments -= amount;

					flowMap.getPixels()[(y * width + x) * 4 + 1] = (gValue++ > 255) ? 255 : gValue++;
				}

				waterFlowErosion(x + newDir.x, y + newDir.y, newDir, water, sediments, slope);

				flowMap.getPixels()[(y * width + x) * 4 + 2] = (bValue + water > 255) ? 255 : bValue + water;

				flowMap.getPixels()[(y * width + x) * 4 + 3] = 255;
			}
		}
		else 
		{
			for (int fy = -radius; fy <= radius; fy++) {
				for (int fx = -radius; fx <= radius; fx++)
				{
					int nY = y + fy;
					int nX = x + fx;

					if ((nX >= 0 && nX < width) && (nY >= 0 && nY < height))
					{
						int hV = heightMap.getPixels()[nY * width + nX] ;
						int addHeight = (sediments / 9.0f > 4) ? 4: sediments / 9.0f;

						flowMap.getPixels()[(y * width + x) * 4 + 0] = (water/9.0f > 255) ? 255 : water/9.0f;
						heightMap.getPixels()[nY * width + nX] = (hV + addHeight <= 255) ? hV + addHeight : 255;
					}
				}
			}
		}
	}
}
/*
void WaterErosion::waterFlowErosion(int x, int y, ofVec2f directionVector, float averageSlope, float water, float speed)
{
	if ((x >= 0 && x < width) && (y >= 0 && y < height))
	{
		
		int		cell = heightMap.getPixels()[y * width + x];

		ofVec2f newDir;

		int	slope = (water > 5) ? 5 : water; //1;
		
		if (directionVector != ofVec2f(0, 0))
			directionVector.normalize();
		else
			return;
		
		
		int		radius = 1;
		for (int fy = -radius; fy <= radius; fy++) {
			for (int fx = -radius; fx <= radius; fx++)
			{
				if (fy != 0 || fx != 0)
				{
					int nY = y + fy;
					int nX = x + fx;

					if ((nX >= 0 && nX < width) && (nY >= 0 && nY < height))
					{
						ofVec2f localDir = ofVec2f(fx, fy);
						localDir.normalize();
						float theDot = directionVector.dot(localDir);

						
						if (averageSlope >= -8) {
							if (theDot > -0.1 && theDot < 1)
							{
								int localCell = heightMap.getPixels()[nY * width + nX];
								int localSlope = localCell - cell;

								if (localSlope < slope)
								{
									slope = localSlope;
									newDir = ofVec2f(fx, fy);
								}
								
							}
						}
						else {
							if (theDot >= 0)
							{
								int localCell = heightMap.getPixels()[nY * width + nX];
								int localSlope = localCell - cell;

								if (localSlope < slope)
								{
									slope = localSlope;
									newDir = ofVec2f(fx, fy);
								}
							}
						}
					}
					else {
						return;
					}
				}
			}
		}

		if (newDir != ofVec2f(0, 0))
		{
			int hValue = heightMap.getPixels()[y * width + x];

			averageSlope = (averageSlope * 0.75 + slope * 0.25);

			float value = -1.0f*averageSlope;

			speed = speed * 0.75 + value * 0.25;

			water += waterToAdd;
			
			if (slope > 0 )
				if (water - abs(slope) > 0)
					water -= abs(slope);
				else
					water = 0.0f;
			
			if (speed > 0)
			{
				int bValue = flowMap.getPixels()[(y * width + x) * 3 + 2];
				int gValue = flowMap.getPixels()[(y * width + x) * 3 + 1];
				int rValue = flowMap.getPixels()[(y * width + x) * 3 + 0];

				float force = abs(speed * water * (averageSlope / slope) * 9.82);

				if (removeSoil)
				{
					if (averageSlope > slope && force > addSoil)//0.1)
					{
						float amount = (water * force > 1) ? 1 : water * force;

						heightMap.getPixels()[y * width + x] = (hValue - amount > 0) ? hValue - amount : 0;
						flowMap.getPixels()[(y * width + x) * 3 + 1] = 255;
					}
					else if (force > 0.01)
					{
						float amount = (water * force > 1) ? 1: water * force;

						heightMap.getPixels()[y * width + x] = (hValue - amount > 0) ? hValue - amount : 0;
						flowMap.getPixels()[(y * width + x) * 3 + 1] = 255;
					}
				}

				if (averageSlope < slope || force < addSoil)
				{
					float amount = (water  / speed > 1) ? 1 : water  / speed;
					heightMap.getPixels()[y * width + x] = (hValue + amount <= 255) ? hValue + amount : 255;
					flowMap.getPixels()[(y * width + x) * 3 + 0] = 255;
				}

				waterFlowErosion(x + newDir.x, y + newDir.y, newDir, averageSlope, water, speed);
				
				if(water/waterToAdd > 64)
				flowMap.getPixels()[(y * width + x) * 3 + 2] = 255;

			}
			
		}
	}
}
*/
/*
void WaterErosion::waterFlowErosion(int x, int y, ofVec2f directionVector, float averageSlope, float water, float speed)
{
	if ((x >= 0 && x < width) && (y >= 0 && y < height))
	{

		int		cell = heightMap.getPixels()[y * width + x];

		ofVec2f newDir;

		int	slope = 1;

		if (directionVector != ofVec2f(0, 0))
			directionVector.normalize();
		else
			return;


		int		radius = 1;
		for (int fy = -radius; fy <= radius; fy++) {
			for (int fx = -radius; fx <= radius; fx++)
			{
				if (fy != 0 || fx != 0)
				{
					int nY = y + fy;
					int nX = x + fx;

					if ((nX >= 0 && nX < width) && (nY >= 0 && nY < height))
					{
						ofVec2f localDir = ofVec2f(fx, fy);
						localDir.normalize();
						float theDot = directionVector.dot(localDir);


						if (averageSlope >= -2) {
							if (theDot > 0 && theDot < 1)
							{
								int localCell = heightMap.getPixels()[nY * width + nX];
								int localSlope = localCell - cell;

								if (localSlope < slope)
								{
									slope = localSlope;
									newDir = ofVec2f(fx, fy);
								}
							}
						}
						else {
							if (theDot >= 0)
							{
								int localCell = heightMap.getPixels()[nY * width + nX];
								int localSlope = localCell - cell;

								if (localSlope < slope)
								{
									slope = localSlope;
									newDir = ofVec2f(fx, fy);
								}
							}
						}
					}
					else {
						return;
					}
				}
			}
		}

		if (newDir != ofVec2f(0, 0))
		{
			int hValue = heightMap.getPixels()[y * width + x];

			averageSlope = (averageSlope * 0.75 + slope * 0.25);

			float value = -1.0f*averageSlope;

			speed = speed * 0.75 + value * 0.25;

			water += waterToAdd;

			if (slope > 0 && water - slope > 0)
				if (water - slope > 0)
					water -= slope;
				else
					water = 0.0f;

			if (speed > 0)
			{
				int bValue = flowMap.getPixels()[(y * width + x) * 3 + 2];
				int gValue = flowMap.getPixels()[(y * width + x) * 3 + 1];
				int rValue = flowMap.getPixels()[(y * width + x) * 3 + 0];

				float amount = (water / speed > 5) ? 5 : water / speed;

				if (slope <= averageSlope)
				{
					heightMap.getPixels()[y * width + x] = (hValue - amount > 0) ? hValue - amount : 0;
					flowMap.getPixels()[(y * width + x) * 3 + 0] = (rValue - amount > 0) ? rValue - amount : 0;
				}
				if (speed < addSoil)
				{
					heightMap.getPixels()[y * width + x] = (hValue + amount <= 255) ? hValue + amount : 255;
					flowMap.getPixels()[(y * width + x) * 3 + 0] = 255;
				}


				flowMap.getPixels()[(y * width + x) * 3 + 2] = (bValue + water * 32 <= 255) ? bValue + water * 32 : 255;
				flowMap.getPixels()[(y * width + x) * 3 + 1] = (gValue + speed <= 255) ? gValue + speed : 255;


				waterFlowErosion(x + newDir.x, y + newDir.y, newDir, averageSlope, water, speed);
			}
			else
			{
				heightMap.getPixels()[y * width + x] = (hValue + 1 <= 255) ? hValue + 1 : 255;
				//flowMap.getPixels()[(y * width + x) * 3 + 0] = 255;
			}
		}
	}
}
*/
