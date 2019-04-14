#include "stdafx.h"
#include "MedianBlending.h"

//Defualt constructor loading all images
MedianBlending::MedianBlending() : Blending::Blending()
{
	MedianBlending::image = new Image(Blending::GetImage(0).w, Blending::GetImage(0).h, Image::Rgb(0));
}

//Constutor passing in already loaded images
MedianBlending::MedianBlending(vector<Image> &images) : Blending::Blending(images)
{
	MedianBlending::image = new Image(Blending::GetImage(0).w, Blending::GetImage(0).h, Image::Rgb(0));
}

MedianBlending::~MedianBlending()
{
	delete image;
}

//Blend the images
void MedianBlending::Blend()
{
	Blending::log.StartTimer();

	vector<float> redPixels;
	vector<float> bluePixels;
	vector<float> greenPixels;

	for (int pixel = 0; pixel < (image->h * image->w); pixel++)
	{
		for (int files = 0; files < GetImages().size(); files++)
		{
			//build pixel data
			redPixels.push_back(ReadPixel(files, pixel).r);
			greenPixels.push_back(ReadPixel(files, pixel).g);
			bluePixels.push_back(ReadPixel(files, pixel).b);
		}

		//sort the pixels
		sort(redPixels.begin(), redPixels.end());
		sort(greenPixels.begin(), greenPixels.end());
		sort(bluePixels.begin(), bluePixels.end());

		//calcualte the average
		image->pixels[pixel].r = CalculateMedian(redPixels);
		image->pixels[pixel].b = CalculateMedian(bluePixels);
		image->pixels[pixel].g = CalculateMedian(greenPixels);

		//clear for next iteration
		redPixels.clear();
		greenPixels.clear();
		bluePixels.clear();
	}

	Blending::log.EndTimer("Median Blending");
}

Image MedianBlending::GetImage()
{
	return *MedianBlending::image;
}

//Calculte median from a list of floats
float MedianBlending::CalculateMedian(vector<float> pixeldata)
{
	int size = GetImages().size();
	if ((size % 2) == 0)
	{
		int left = size / 2 - 1;
		int right = size / 2 + 1;

		float medianPixel;
		medianPixel = pixeldata[left];
		medianPixel += pixeldata[right];

		medianPixel /= 2;
		return medianPixel;
	}

	else
	{
		int middle = floor(size / 2);
		return pixeldata[middle];
	}
}

//AMP function for sorting numbers
array_view<float,1> Sort(float dataToSort[]) restrict(amp)
{
	for (int x = 0; x != 13; x++)
	{
		for (int y = 0; y != 13; y++)
		{
			if (dataToSort[x] < dataToSort[y])
			{
				float Temp = dataToSort[x];
				dataToSort[x] = dataToSort[y];
				dataToSort[y] = Temp;
			}
			else
			{
				float Temp = dataToSort[y];
				dataToSort[y] = dataToSort[x];
				dataToSort[x] = Temp;
			}
		}

	}

	array_view<float, 1> sortedData(13, dataToSort);
	return sortedData;
}

//AMP function for computing median from a list of numbers
float ComputeMedian(array_view<float,1> pixeldata) restrict(amp)
{
	int size = 13;
	if ((size % 2) == 0)
	{
		int left = size / 2 - 1;
		int right = size / 2 + 1;

		float medianPixel;
		medianPixel = pixeldata[left];
		medianPixel += pixeldata[right];

		medianPixel /= 2;
		return medianPixel;
	}

	else
	{
		int middle = concurrency::precise_math::floorf(size / 2);
		return pixeldata[middle];
	}
	
}

//Perform Blending using HA
void MedianBlending::AsyncBlend()
{
	
	Blending::log.StartTimer();
	const int size = Blending::GetImage(0).w * Blending::GetImage(0).h;
#pragma region Create Compute Arrays
	array_view<const Image::Rgb, 1> img1(size, Blending::GetImage(0).pixels);
	array_view<const Image::Rgb, 1> img2(size, Blending::GetImage(1).pixels);
	array_view<const Image::Rgb, 1> img3(size, Blending::GetImage(2).pixels);
	array_view<const Image::Rgb, 1> img4(size, Blending::GetImage(3).pixels);
	array_view<const Image::Rgb, 1> img5(size, Blending::GetImage(4).pixels);
	array_view<const Image::Rgb, 1> img6(size, Blending::GetImage(5).pixels);
	array_view<const Image::Rgb, 1> img7(size, Blending::GetImage(6).pixels);
	array_view<const Image::Rgb, 1> img8(size, Blending::GetImage(7).pixels);
	array_view<const Image::Rgb, 1> img9(size, Blending::GetImage(8).pixels);
	array_view<const Image::Rgb, 1> img10(size, Blending::GetImage(9).pixels);
	array_view<const Image::Rgb, 1> img11(size, Blending::GetImage(10).pixels);
	array_view<const Image::Rgb, 1> img12(size, Blending::GetImage(11).pixels);
	array_view<const Image::Rgb, 1> img13(size, Blending::GetImage(12).pixels);
	array_view<Image::Rgb, 1> computedImage(size);
	
#pragma endregion

	//perform proccessing
	parallel_for_each(computedImage.extent, [=](index<1> pixel) restrict(amp)
	{
		float redPixels[13];
		float greenPixels[13];
		float bluePixels[13];

		
#pragma region Red Pixels
		redPixels[0] = img1[pixel].r;
		redPixels[1] = img2[pixel].r;
		redPixels[2] = img3[pixel].r;
		redPixels[3] = img4[pixel].r;
		redPixels[4] = img5[pixel].r;
		redPixels[5] = img6[pixel].r;
		redPixels[6] = img7[pixel].r;
		redPixels[7] = img8[pixel].r;
		redPixels[8] = img9[pixel].r;
		redPixels[9] = img10[pixel].r;
		redPixels[10] = img11[pixel].r;
		redPixels[11] = img12[pixel].r;
		redPixels[12] = img13[pixel].r;
#pragma endregion
	
#pragma region Green Pixels
		greenPixels[0] = img1[pixel].g;
		greenPixels[1] = img2[pixel].g;
		greenPixels[2] = img3[pixel].g;
		greenPixels[3] = img4[pixel].g;
		greenPixels[4] = img5[pixel].g;
		greenPixels[5] = img6[pixel].g;
		greenPixels[6] = img7[pixel].g;
		greenPixels[7] = img8[pixel].g;
		greenPixels[8] = img9[pixel].g;
		greenPixels[9] = img10[pixel].g;
		greenPixels[10] = img11[pixel].g;
		greenPixels[11] = img12[pixel].g;
		greenPixels[12] = img13[pixel].g;
#pragma endregion
	
#pragma region Blue Pixels
		bluePixels[0] = img1[pixel].b;
		bluePixels[1] = img2[pixel].b;
		bluePixels[2] = img3[pixel].b;
		bluePixels[3] = img4[pixel].b;
		bluePixels[4] = img5[pixel].b;
		bluePixels[5] = img6[pixel].b;
		bluePixels[6] = img7[pixel].b;
		bluePixels[7] = img8[pixel].b;
		bluePixels[8] = img9[pixel].b;
		bluePixels[9] = img10[pixel].b;
		bluePixels[10] = img11[pixel].b;
		bluePixels[11] = img12[pixel].b;
		bluePixels[12] = img13[pixel].b;
#pragma endregion

		//sort the arrays
		array_view<float, 1> sortedRed = Sort(redPixels);
		array_view<float, 1> sortedGreen = Sort(greenPixels);
		array_view<float, 1> sortedBlue = Sort(bluePixels);
				
		computedImage[pixel].r = ComputeMedian(sortedRed);
		computedImage[pixel].g = ComputeMedian(sortedGreen);
		computedImage[pixel].b = ComputeMedian(sortedBlue);
	});

	
	//build new image
	for (int pixel = 0; pixel < (image->w * image->h); pixel++)
	{
		image->pixels[pixel] = computedImage[pixel];
	}
	
	Blending::log.EndTimer("Async Median Blending");
}


