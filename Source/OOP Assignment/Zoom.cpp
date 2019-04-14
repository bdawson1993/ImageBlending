#include "stdafx.h"
#include "Zoom.h"

Zoom::Zoom()
{
	log.StartTimer();
	Zoom::image = new Image(readPPM("Images/Zoom/zIMG_1.ppm"));
	log.EndTimer("Loading Zoom Image");
	log.Log(image->imageName, image->h, image->w, image->colorDepth);
}

Zoom::~Zoom()
{
	delete image;
}

void Zoom::Scale()
{
	Scale(2);
}

void Zoom::Scale(int value)
{
	Logger log;
	log.StartTimer();
	Image *newImage = new Image(Zoom::image->w * value, Zoom::image->h * value, Image::Rgb());

	//fill in the colors by skipping value in image
	int counter = 0;
	int oldPixel = 0;

	for (int pixel = 0; pixel < (newImage->w * newImage->h); pixel++)
	{
		for (int line = 0; line != (value - 1); line++)
		{
			//fill line with pixel above it
			if (((pixel % newImage->w) == 0) && (pixel > 0))
			{
				for (int width = 0; width < newImage->w; width++)
				{
					newImage->pixels[pixel] = newImage->pixels[pixel - newImage->w];
					pixel++;
				}
			}
		}

		newImage->pixels[pixel] = image->pixels[oldPixel];
		for (int gap = 0; gap < value - 1; gap++)
		{
			pixel++;
			newImage->pixels[pixel] = image->pixels[oldPixel];
		}
		oldPixel++;

	}

	log.EndTimer("Image Scaling"); 
	image = new Image(*newImage);
}

Image Zoom::GetImage()
{
	return *Zoom::image;
}
