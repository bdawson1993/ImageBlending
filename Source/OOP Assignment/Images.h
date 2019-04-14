#pragma once

#include <vector>
#include "Image.h"
#include "Logger.h"

using namespace std;

class  Images
{
public:
	Images();
	Images(vector<Image> &images);
	void LoadImages();
	void WriteImage(string fileName, Image image);
	Image::Rgb ReadPixel(int pictureNum, int pixelLoc);
	Image GetImage(int index);
	vector<Image> &GetImages();
	Logger log;
	
private:
	vector<Image> *images = new vector<Image>(13);
	

};