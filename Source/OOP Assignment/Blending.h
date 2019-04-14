#pragma once
#include "Images.h"
#include "Logger.h"


class  Blending : public Images
{

public:
	Blending();
	Blending(vector<Image> &imgs);
	virtual void Blend();
	virtual void AsyncBlend();
	
private:
	

};


