#pragma once
#include <windows.h>

class TimeSystem
{
public:
	TimeSystem();

	void Start();
	void End();

	double GetElaspedTime();

private:
	LARGE_INTEGER Frequency;
	LARGE_INTEGER BeginTime;
	LARGE_INTEGER Endtime;
};

