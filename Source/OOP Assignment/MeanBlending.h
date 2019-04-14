#pragma once
#include "Blending.h"
#include <amp.h> 

using namespace std;
using namespace concurrency;

class MeanBlending : public Blending
{
public:
	MeanBlending();
	MeanBlending(vector<Image> &imgaes);
	~MeanBlending();
	void Blend();
	void AsyncBlend();
	Image GetImage();


private:
	Image *image;
};

