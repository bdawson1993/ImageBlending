#include "stdafx.h"
#include "SigmaClippedMean.h"

//Defualt constructor loading all images
SigmaClippedMean::SigmaClippedMean() : Blending::Blending()
{
	SigmaClippedMean::image = new Image(Blending::GetImage(0).w, Blending::GetImage(0).h, Image::Rgb(0));
}

//release all class pointers
SigmaClippedMean::~SigmaClippedMean()
{
	delete image;
}

//Peform Sigma Clipped Mean
void SigmaClippedMean::Blend()
{
	Image *standardDeviation = new Image(Blending::GetImage(0).w, Blending::GetImage(0).h, Image::Rgb(0));
	Image *mean = new Image(Blending::GetImage(0).w, Blending::GetImage(0).h, Image::Rgb(0));
	Image *median = new Image(Blending::GetImage(0).w, Blending::GetImage(0).h, Image::Rgb(0));
	Blending::log.StartTimer();

	//get the mean
	MeanBlending *meanBlending = new MeanBlending(Blending::GetImages());
	meanBlending->log.SetLogShow(false);
	meanBlending->Blend();
	*mean = meanBlending->GetImage();
	delete meanBlending;
	
	//get the median
	MedianBlending *medianBlending = new MedianBlending(Blending::GetImages());
	medianBlending->log.SetLogShow(false);
	medianBlending->Blend();
	*median = medianBlending->GetImage();
	delete medianBlending;
	
	//calaculte the standard deviation and remove points in stack
	standardDeviation = &CalculateStandardDeviation(Blending::GetImages(), *mean);
	Image::Rgb sum;
	for (int pixel = 0; pixel < (Blending::GetImage(0).w * Blending::GetImage(0).h); pixel++)
	{
		for (int images = 0; images < Blending::GetImages().size(); images++)
		{
			sum = median->pixels[pixel];
			sum += standardDeviation->pixels[pixel];

			if (Blending::GetImage(images).pixels[pixel] != sum)
			{
				Blending::GetImage(images).pixels[pixel] = Image::Rgb(0, 0, 0);
			}
		}
	}

	MeanBlending *meanBlend = new MeanBlending(GetImages());
	meanBlend->log.SetLogShow(false);
	meanBlending->Blend();
	SigmaClippedMean::image = &meanBlending->GetImage();

	Blending::log.EndTimer("Sigma Clipped");

}

//Subtract mean and sqaure the result
Image::Rgb SubtractMean(array_view<Image::Rgb, 1> img, index<1> pixel, array_view<Image::Rgb, 1> mean) restrict(amp)
{
	img[pixel].r -= mean[pixel].r;
	img[pixel].g -= mean[pixel].g;
	img[pixel].b -= mean[pixel].b;

	img[pixel].r = concurrency::fast_math::pow(img[pixel].r, 2);
	img[pixel].g = concurrency::fast_math::pow(img[pixel].g, 2);
	img[pixel].b = concurrency::fast_math::pow(img[pixel].b, 2);

	return img[pixel];
}

//AMP function for removing pixels from images
Image::Rgb RemovePoints(array_view<Image::Rgb, 1> img, index<1> pixel, array_view<Image::Rgb, 1> median, array_view<Image::Rgb> stdDeviation) restrict(amp)
{
	Image::Rgb newPixel = Image::Rgb();
	newPixel.r += median[pixel].r + stdDeviation[pixel].r;
	newPixel.g += median[pixel].r + stdDeviation[pixel].g;
	newPixel.b += median[pixel].r + stdDeviation[pixel].b;


	if ((img[pixel].r != newPixel.r) && (img[pixel].g != newPixel.g) && (img[pixel].b != newPixel.b))
	{
		img[pixel].r = NULL;
		img[pixel].g = NULL;
		img[pixel].b = NULL;
	}

	return img[pixel];
}

//perform blending using HA
void SigmaClippedMean::AsyncBlend()
{
	/*accelerator acc = accelerator();
	acc.set_default_cpu_access_type(access_type_read_write);
	accelerator_view view = acc.default_view;*/


	Blending::log.StartTimer();
	const int size = Blending::GetImage(0).w * Blending::GetImage(0).h;

	Image::Rgb *mean = new Image::Rgb[size];
	Image::Rgb *median = new Image::Rgb[size];

	//mean blending
	MeanBlending *meanBlending = new MeanBlending(Blending::GetImages());
	meanBlending->log.SetLogShow(false);
	meanBlending->AsyncBlend();
	mean = meanBlending->GetImage().pixels;
	delete meanBlending;
	
	//median blending
	MedianBlending *medianBlending = new MedianBlending(Blending::GetImages());
	medianBlending->log.SetLogShow(false);
	medianBlending->AsyncBlend();
	median = medianBlending->GetImage().pixels;
	delete medianBlending;
	amp_uninitialize();
	
	int count = 0;




#pragma region Create Compute Arrays
	array_view<Image::Rgb, 1> img1(size, Blending::GetImage(0).pixels);
	array_view<Image::Rgb, 1> img2(size, Blending::GetImage(1).pixels);
	array_view<Image::Rgb, 1> img3(size, Blending::GetImage(2).pixels);
	array_view<Image::Rgb, 1> img4(size, Blending::GetImage(3).pixels);
	array_view<Image::Rgb, 1> img5(size, Blending::GetImage(4).pixels);
	array_view<Image::Rgb, 1> img6(size, Blending::GetImage(5).pixels);
	array_view<Image::Rgb, 1> img7(size, Blending::GetImage(6).pixels);
	array_view<Image::Rgb, 1> img8(size, Blending::GetImage(7).pixels);
	array_view<Image::Rgb, 1> img9(size, Blending::GetImage(8).pixels);
	array_view<Image::Rgb, 1> img10(size, Blending::GetImage(9).pixels);
	array_view<Image::Rgb, 1> img11(size, Blending::GetImage(10).pixels);
	array_view<Image::Rgb, 1> img12(size, Blending::GetImage(11).pixels);
	array_view<Image::Rgb, 1> img13(size, Blending::GetImage(12).pixels);
	
	array_view<Image::Rgb, 1> meanImg(size, mean);
	array_view<Image::Rgb, 1> medianImg(size, median);
	array_view<Image::Rgb, 1> computedImage(size);

#pragma endregion

	//Subtract mean from all images
	parallel_for_each(computedImage.extent, [=](index<1> pixel) restrict(amp)
	{
		img1[pixel] = SubtractMean(img1, pixel, meanImg);
		img2[pixel] = SubtractMean(img2, pixel, meanImg);
		img3[pixel] = SubtractMean(img3, pixel, meanImg);
		img4[pixel] = SubtractMean(img4, pixel, meanImg);
		img5[pixel] = SubtractMean(img5, pixel, meanImg);
		img6[pixel] = SubtractMean(img6, pixel, meanImg);
		img7[pixel] = SubtractMean(img7, pixel, meanImg);
		img8[pixel] = SubtractMean(img8, pixel, meanImg);
		img9[pixel] = SubtractMean(img9, pixel, meanImg);
		img10[pixel] = SubtractMean(img10, pixel, meanImg);
		img11[pixel] = SubtractMean(img11, pixel, meanImg);
		img12[pixel] = SubtractMean(img12, pixel, meanImg);
		img13[pixel] = SubtractMean(img13, pixel, meanImg);
	});

#pragma region Sync Data Back
	img1.synchronize();
	img2.synchronize();
	img3.synchronize();
	img4.synchronize();
	img5.synchronize();
	img6.synchronize();
	img7.synchronize();
	img8.synchronize();
	img9.synchronize();
	img10.synchronize();
	img11.synchronize();
	img12.synchronize();
	img13.synchronize();
#pragma endregion

	//calculate the variance
	MeanBlending *variance = new MeanBlending(Blending::GetImages());
	variance->log.SetLogShow(false);
	variance->AsyncBlend();
	


	array_view<Image::Rgb, 1> standardDeviation(size, variance->GetImage().pixels);
	

	//sqaure the variance
	parallel_for_each(computedImage.extent, [=](index<1> pixel) restrict(amp)
	{
		standardDeviation[pixel].r = concurrency::fast_math::sqrt(standardDeviation[pixel].r);
		standardDeviation[pixel].b = concurrency::fast_math::sqrt(standardDeviation[pixel].g);
		standardDeviation[pixel].g = concurrency::fast_math::sqrt(standardDeviation[pixel].b);

		img1[pixel] = RemovePoints(img1, pixel, medianImg, standardDeviation);
		img2[pixel] = RemovePoints(img2, pixel, medianImg, standardDeviation);
		img3[pixel] = RemovePoints(img3, pixel, medianImg, standardDeviation);
		img4[pixel] = RemovePoints(img4, pixel, medianImg, standardDeviation);
		img5[pixel] = RemovePoints(img5, pixel, medianImg, standardDeviation);
		img6[pixel] = RemovePoints(img6, pixel, medianImg, standardDeviation);
		img7[pixel] = RemovePoints(img7, pixel, medianImg, standardDeviation);
		img8[pixel] = RemovePoints(img8, pixel, medianImg, standardDeviation);
		img9[pixel] = RemovePoints(img9, pixel, medianImg, standardDeviation);
		img10[pixel] = RemovePoints(img10, pixel, medianImg, standardDeviation);
		img11[pixel] = RemovePoints(img11, pixel, medianImg, standardDeviation);
		img12[pixel] = RemovePoints(img12, pixel, medianImg, standardDeviation);
		img13[pixel] = RemovePoints(img13, pixel, medianImg, standardDeviation);
		
	});

#pragma region Sync Data Back
	img1.synchronize();
	img2.synchronize();
	img3.synchronize();
	img4.synchronize();
	img5.synchronize();
	img6.synchronize();
	img7.synchronize();
	img8.synchronize();
	img9.synchronize();
	img10.synchronize();
	img11.synchronize();
	img12.synchronize();
	img13.synchronize();
#pragma endregion
	
	meanBlending = new MeanBlending(GetImages());
	meanBlending->AsyncBlend();
	*image = meanBlending->GetImage();
	delete meanBlending;
	delete variance;
	amp_uninitialize();

	log.EndTimer("Sigma Clipped Mean");
}

//Get the Image
Image SigmaClippedMean::GetImage()
{
	return *SigmaClippedMean::image;
}

//Calculate Standard Deviation
Image SigmaClippedMean::CalculateStandardDeviation(vector<Image> imgs, Image mean)
{
	Image *variance = new Image(Blending::GetImage(0));
	
	//subtract the mean and sqaure the result
	for (int pixel = 0; pixel < (Blending::GetImage(0).w * Blending::GetImage(0).h); pixel++)
	{
		for (int image = 0; image < Blending::GetImages().size(); image++)
		{
			imgs[image].pixels[pixel] -= mean.pixels[pixel];
			imgs[image].pixels[pixel] *= imgs[image].pixels[pixel];
		}
	}


	//caculate the mean
	MeanBlending *meanBlend = new MeanBlending(imgs);
	meanBlend->log.SetLogShow(false);
	meanBlend->Blend();
	variance = &meanBlend->GetImage();

	//sqaure the variance
	for (int pixel = 0; pixel < (Blending::GetImage(0).w * Blending::GetImage(0).h); pixel++)
	{
		variance->pixels[pixel].r = sqrt(variance->pixels[pixel].r);
		variance->pixels[pixel].g = sqrt(variance->pixels[pixel].g);
		variance->pixels[pixel].b = sqrt(variance->pixels[pixel].b);

	}


	return *variance;
}
