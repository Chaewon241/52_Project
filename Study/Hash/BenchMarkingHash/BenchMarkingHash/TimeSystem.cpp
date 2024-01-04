#include "TimeSystem.h"

#include <iostream>

TimeSystem::TimeSystem()
{
	QueryPerformanceFrequency(&Frequency);
}

void TimeSystem::Start()
{
	QueryPerformanceCounter(&BeginTime);
}

void TimeSystem::End()
{
	QueryPerformanceCounter(&Endtime);
}

double TimeSystem::GetElaspedTime()
{
	__int64 elapsed = Endtime.QuadPart - BeginTime.QuadPart;
	double duringtimeSec = (double)elapsed / (double)Frequency.QuadPart;

	std::cout << std::endl << "수행시간 : " << duringtimeSec * 1000 << "ms" << std::endl;

	return duringtimeSec;
}
