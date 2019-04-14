#pragma once
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

//Loging utility class

class Logger
{
public:
	Logger();
	void StartTimer();
	void EndTimer(string timerName);
	void SetLogShow(bool log);
	void Log(string fileName, int height, int width, int colorDepth);
	void RemoveOldLog()
	{
		remove("Log.dat");
	}

private:
	clock_t timer;
	bool showLog = true;
	ofstream file;

};
