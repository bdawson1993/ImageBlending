#include "stdafx.h"
#include "MeanBlending.h"

//Defualt constructor loading all images
MeanBlending::MeanBlending() : Blending::Blending()
{
	image = new Image(Blending::GetImage(0).w, Blending::GetImage(0).h, Image::Rgb(0));
}

//Constutor passing in already loaded images
MeanBlending::MeanBlending(vector<Image> &imgaes) : Blending::Blending(imgaes)
{
	image = new Image(Blending::GetImage(0).w, Blending::GetImage(0).h, Image::Rgb(0));
}

MeanBlending::~MeanBlending()
{
	delete image;
}

//Calculate Mean
void MeanBlending::Blend()
{
	Blending::log.StartTimer();
	Image::Rgb newPixel = Image::Rgb();

	//loop through each pixel in each image
	for (int pixel = 0; pixel < (image->h * image->w); pixel++)
	{
		for (int files = 0; files < GetImages().size(); files++)
		{
			newPixel += ReadPixel(files, pixel);
		}

		//divide each value by 13 and set rgb to 0 for next iterations
		image->pixels[pixel].r = newPixel.r / 13;
		image->pixels[pixel].g = newPixel.g / 13;
		image->pixels[pixel].b = newPixel.b / 13;

		newPixel = Image::Rgb(0);
	}

	Blending::log.EndTimer("Mean Blending");

}

//Peform image blending using HA
void MeanBlending::AsyncBlend()
{
	Blending::log.StartTimer();
	const int size = Blending::GetImage(0).w * Blending::GetImage(0).h;

	//create array views - dirty find alternative
	//first 6 images
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

	//perform proccessing 
	parallel_for_each(computedImage.extent, [=](index<1> pixel) restrict(amp)
	{
		computedImage[pixel].r += img1[pixel].r + img2[pixel].r + img3[pixel].r + img4[pixel].r +
			img5[pixel].r + img6[pixel].r + img7[pixel].r + img8[pixel].r + img9[pixel].r + img10[pixel].r + img11[pixel].r +
			img12[pixel].r + img13[pixel].r;

		computedImage[pixel].g += img1[pixel].g + img2[pixel].g + img3[pixel].g + img4[pixel].g +
			img5[pixel].g + img6[pixel].g + img7[pixel].g + img8[pixel].g + img9[pixel].g + img10[pixel].g + img11[pixel].g +
			img12[pixel].g + img13[pixel].g;

		computedImage[pixel].b += img1[pixel].b + img2[pixel].b + img3[pixel].b + img4[pixel].b +
			img5[pixel].b + img6[pixel].b + img7[pixel].b + img8[pixel].b + img9[pixel].b + img10[pixel].b + img11[pixel].b +
			img12[pixel].b + img13[pixel].b;

		computedImage[pixel].r = computedImage[pixel].r / 13;
		computedImage[pixel].g = computedImage[pixel].g / 13;
		computedImage[pixel].b = computedImage[pixel].b / 13;
	});
	

	//build new image
	for (int pixel = 0; pixel < (image->w * image->h); pixel++)
	{
		image->pixels[pixel] = computedImage[pixel];
	}
	Blending::log.EndTimer("Async Mean Blending");
	

}


Image MeanBlending::GetImage()
{
	return *MeanBlending::image;
}


