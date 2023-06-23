#include "pch.h"
#include "TimeManager.h"

TimeManager::TimeManager()
	:m_deltaTime(0.f), m_curTime(0.f), m_prevTime(0.f), m_time(0.f)
{
}

TimeManager::~TimeManager()
{
}

void TimeManager::Update()
{
	m_prevTime = m_curTime;
	m_curTime = GetTickCount64() / 1000.f;
	m_deltaTime = m_curTime - m_prevTime;


}
