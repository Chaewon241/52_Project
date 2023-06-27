#pragma once

class TimeManager
{
protected:
	float m_deltaTime;
	float m_prevTime;
	float m_curTime;
	float m_time;

public:
	TimeManager* m_Instance;

public:
	TimeManager();
	~TimeManager();

	void Initialize();
	void Update();
	float GetDeltatime();
};

