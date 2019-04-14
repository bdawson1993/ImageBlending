#pragma once

#include "Blending.h"
#include <amp.h>
#include <amp_math.h>

using namespace std;
using namespace concurrency;

class  MedianBlending : public Blending
{
public:
	MedianBlending();
	MedianBlending(vector<Image> &images);
	~MedianBlending();
	void Blend();
	void AsyncBlend();
	Image GetImage();

private:
	Image *image;
	float CalculateMedian(vector<float> pixeldata);

	
	
};

