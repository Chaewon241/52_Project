#ifndef PCH_H
#define PCH_H

#include "framework.h"

// Windows ��� ����
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <d3d11.h>

#include "../Engine/define.h"

constexpr int SND_BUF_SIZE = 1024;
constexpr int RCV_BUF_SIZE = 9604;

#define LOG_CONTENTS(message) \
{\
	std::string file = __FILE__;\
	file = file.substr(file.find_last_of("/\\") + 1);\
	printf("[Contents] FILE : %s, LINE : %d\n[Contents] MESSAGE : %s\n\n", file.c_str(), __LINE__, message);\
}
#endif