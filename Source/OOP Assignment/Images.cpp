#include "stdafx.h"
#include "Images.h"

//Load images 
Images::Images()
{
	Images::log.StartTimer();
	LoadImages();
	Images::log.EndTimer("Loading Images");
}

//pass already loaded images
Images::Images(vector<Image> &imgs)
{
	images = &imgs;
}



//Load all images into memory
void Images::LoadImages()
{
	Image imgRGb;
	string fileName;

	cout << "Loading Images" << endl;

	for (int i = 1; i < 14; i++)
	{
		fileName = "Images/ImageStacker_set1/IMG_" + to_string(i) + ".ppm";
		images->at(i - 1) = imgRGb.readPPM(fileName);
		this->log.Log(imgRGb.imageName, imgRGb.h, imgRGb.w, imgRGb.colorDepth);;
	}

	cout << "Loaded Images" << endl;
}

//Write image to file
void Images::WriteImage(string fileName, Image image)
{
	image.writePPM(image, "Images/" + fileName + ".ppm");
}

//Read pixel from picture num and loc
Image::Rgb Images::ReadPixel(int pictureNum, int pixelLoc)
{
	return images->at(pictureNum).pixels[pixelLoc];
}

//get image of index
Image Images::GetImage(int index)
{
	//check if image is within vector range
	if (index > images->size())
	{
		cout << "Picture doesn't exist" << endl;
		return Image(); //Blank Image
	}

	return images->at(index);
}

//get all images
vector<Image> &Images::GetImages()
{
	return *images;
}




