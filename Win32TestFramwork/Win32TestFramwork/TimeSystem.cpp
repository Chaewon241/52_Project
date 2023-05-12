#include "WinApp.h"
#include "TimeSystem.h"
#include <cmath>

// 여기 바꿈
// GetTickCount는 더 많이 돌지만 정확도는 낮음
// QueryPerformanceCounter는 덜 돌지만 정확도가 높음 그래서 이거 쓰자

namespace time
{
	// 엄청엄청 큰 integer(구조체 형태)
	// 초기화
	LARGE_INTEGER previousTime = { 0 };
	LARGE_INTEGER currentTime = { 0 };
	LARGE_INTEGER frequency = { 0 };

	float deltaTime = 0;

	void InitTime()
	{
		// 한번 가져오고
		QueryPerformanceFrequency(&frequency);
		// 업데이트 할 때마다 querycount 값을 가져옴
		QueryPerformanceCounter(&previousTime);
	}

	void UpdateTime()
	{
		previousTime = currentTime;
		QueryPerformanceCounter(&currentTime);

		// 업데이트했을 때 한 프레임 안에서 저장된 델타를 가져옴
		deltaTime = (float)(currentTime.QuadPart - previousTime.QuadPart) / (float)frequency.QuadPart * 1000.0f; //ms
	}

	const float GetFrameRate()
	{
		if (deltaTime == 0) return 0;

		return ceil(((1000.0f / deltaTime) * 1000) / 1000);
	}

	const float GetDeltaTime() { return deltaTime; }
}