//main.cpp

#include "stdafx.h"
#include <iostream>
#include "MeanBlending.h"
#include "MedianBlending.h"
#include "SigmaClippedMean.h"
#include "Zoom.h"

using namespace std;

int main()
{
	int useHacc = 0;
	std::cout << "************************************" << std::endl;
	std::cout << "Image Stacker / Image Scaler" << std::endl;
	std::cout << "************************************" << std::endl;

	//prompt user to use hardware accelartion
	cout << "This Program can use Hardware accelartion to speed up calculation: " << endl;
	cout << "Would you like to use Hardware accelartion: " << endl;
	cout << "1 - Yes" << endl;
	cout << "2 - No" << endl;
	cin >> useHacc;
	cout << endl;

	//print Acceltor infor
	if (useHacc == 1)
	{
		concurrency::accelerator a;
		wcout << "Graphics Card: " << a.description << endl;
		wcout << "Memory: " << a.dedicated_memory << endl << endl;
	}

	//prompt user what they want to do
	int input = 0;
	cout << "1 - Mean Blending" << endl;
	cout << "2 - Median Blending" << endl;
	cout << "3 - Stigma Clipped Mean" << endl;
	cout << "4 - Scale" << endl;
	cout << "Please pick an option: " << endl;
	cin >> input;

	Logger log;
	log.RemoveOldLog();

	//mean blending
	if (input == 1)
	{
		MeanBlending *meanBlending = new MeanBlending();
		cout << "Peforming Mean Blending..." << endl;

		if (useHacc == 1)
		{
			meanBlending->AsyncBlend();
		}
		else
		{
			meanBlending->Blend();
		}

		meanBlending->WriteImage("Output", meanBlending->GetImage());
		delete meanBlending;
	}

	//median blending
	if (input == 2)
	{
		MedianBlending *medianBlending = new MedianBlending();
		cout << "Calculting Median" << endl;
		if (useHacc == 1)
		{
			medianBlending->AsyncBlend();
		}
		else
		{
			medianBlending->Blend();
		}

		medianBlending->WriteImage("Output", medianBlending->GetImage());
		delete medianBlending;
	}

	//sigma clipped mean
	if (input == 3)
	{
		SigmaClippedMean *sigmaClippedMean = new SigmaClippedMean();
		cout << "Calculating Sigma Clipped Mean" << endl;


		if (useHacc == 1)
		{
			sigmaClippedMean->AsyncBlend();
			
		}

		else
		{
			sigmaClippedMean->Blend();
		}

		sigmaClippedMean->WriteImage("Output", sigmaClippedMean->GetImage());
		delete sigmaClippedMean;
	}

	//zomm
	if (input == 4)
	{
		Zoom *zoom = new Zoom();
		cout << "Scaling Image " << endl;
		zoom->Scale();
		zoom->writePPM(zoom->GetImage(), "Images/Output.ppm");
		delete zoom;

	}

	system("pause");
	return 0;
}

