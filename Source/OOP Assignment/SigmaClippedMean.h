#pragma once
#include "MeanBlending.h"
#include "MedianBlending.h"
#include "Blending.h"
#include <math.h>

class  SigmaClippedMean : public Blending
{
public:
	SigmaClippedMean();
	~SigmaClippedMean();
	void Blend();
	void AsyncBlend();
	Image GetImage();
	Image CalculateStandardDeviation(vector<Image> imgs, Image mean);

private:
	Image *image;
	
};

