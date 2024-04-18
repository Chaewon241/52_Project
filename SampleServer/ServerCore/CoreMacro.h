#pragma once


// 인위적으로 크래쉬내기
// complie 단계에서 잡아버리지 않게 하기 위해 매크로 사용
//

// CRASH(입력)
#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

// __analysis_assume(crash != nullptr); -> crash는 nullptr이 아니라고 먼저 해주기
// crash가 nullptr이면 미리 컴파일러가 잡아줄 수 있기 때문.
// 

#define ASSERT_CRASH(expr)			\
{									\
	if (!(expr))					\
	{								\
		CRASH("ASSERT_CRASH");		\
		__analysis_assume(expr);	\
	}								\
}