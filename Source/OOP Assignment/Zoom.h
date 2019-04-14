#pragma once
#include "Image.h"
#include "Logger.h"

class  Zoom : public Image
{
public:
	Zoom();
	~Zoom();
	void Scale();
	void Scale(int value);
	Image GetImage();

private:
	Image *image;
	Logger log; 
	


};
