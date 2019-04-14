#include "stdafx.h"
#include "Logger.h"

//logging utility class

Logger::Logger()
{
	
}

//start timer
void Logger::StartTimer()
{
	Logger::timer = clock();
}

//end timer and calculte result
void Logger::EndTimer(string timerName)
{
	Logger::timer = clock() - timer;

	//write time file
	file.open("log.dat", ios::app);
	file << timerName << " Completed in"  << (float)timer / CLOCKS_PER_SEC << " seconds" << endl;
	file.close();

	//print time to the console
	if (Logger::showLog == true)
	{
		cout << timerName << " Completed in " << (float)timer / CLOCKS_PER_SEC << " seconds" << endl;
	}
}


void Logger::SetLogShow(bool log)
{
	Logger::showLog = log;
}


void Logger::Log(string fileName, int height, int width, int colorDepth)
{
	file.open("log.dat", std::ios::app);
	file << "File Name: " << fileName << endl <<
		"Height: " << height << endl <<
		"Width: " << width << endl << 
		"Color Depth: " << colorDepth << endl << endl;
	file.close();
}

