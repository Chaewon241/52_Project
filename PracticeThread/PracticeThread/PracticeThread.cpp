#include <functional> //std::function
#include <iostream>
#include <conio.h>
#include <map>
#include <thread>
#include <concurrent_queue.h>
using namespace Concurrency;

using namespace std;

// 뭘 주문했는가.
enum class CMD_TYPE : unsigned short
{
	NONE = 0,
	E커피 = 1,
	E샌드위치,
	E떡볶이
};

// 주문서 타입
class CmdMsg
{
public:
	CMD_TYPE cmdType;

protected:
	CmdMsg() { cmdType = CMD_TYPE::NONE; }
};

class Cmd커피 : public CmdMsg
{
public:
	Cmd커피(const char* type)
	{
		cmdType = CMD_TYPE::E커피;
		m_type = type;
	}

	string m_type;

private:
	Cmd커피() = delete;
};

class Cmd샌드위치 : public CmdMsg
{
public:
	Cmd샌드위치(int type)
	{
		cmdType = CMD_TYPE::E샌드위치;
		m_type = type;
	}

	int m_type;

private:
	Cmd샌드위치() = delete;
};

class Cmd떡볶이 : public CmdMsg
{
public:
	Cmd떡볶이(int param1, int param2)
	{
		cmdType = CMD_TYPE::E떡볶이;
		m_param1 = param1;
		m_param2 = param2;
	}

	int m_param1;
	int m_param2;

private:
	Cmd떡볶이() = delete;
};


// 처리 = 함수
void Make커피(string param)
{
	cout << param << " 커피 시키신 분?" << endl;
}

void Make샌드위치(int nParam1)
{
	if (nParam1 < 0 || nParam1 >= 2)
	{
		cout << "안팔아" << endl;
		return;
	}

	string 샌드위치종류[2] = { "햄샌드위치", "치킨샌드위치" };

	cout << 샌드위치종류[nParam1] << "시키신 분?" << endl;
}

void Make떡볶이(int nParam1, int nParam2)
{
	if (nParam1 < 0 || nParam1 >= 2 || nParam2 < 0 || nParam2 >= 2)
	{
		cout << "안팔아" << endl;
		return;
	}

	string 떡볶이종류[2] = { "매운떡볶이", "로제떡볶이" };
	string 사이즈[2] = { "보통", "특대" };

	cout << 떡볶이종류[nParam1] << " " << 사이즈[nParam2] << " 시키신 분?" << endl;

}

class CmdDiapatcher
{
public:
	void InitCommand()
	{
		m_cmdMap.emplace(CMD_TYPE::E커피, [](CmdMsg* cmdMsg) {
			Cmd커피* pCmd = reinterpret_cast<Cmd커피*>(cmdMsg);
			Make커피(pCmd->m_type);
			});

		m_cmdMap.emplace(CMD_TYPE::E샌드위치, [](CmdMsg* cmdMsg) {
			Cmd샌드위치* pCmd = reinterpret_cast<Cmd샌드위치*>(cmdMsg);
			Make샌드위치(pCmd->m_type);
			});

		m_cmdMap.emplace(CMD_TYPE::E떡볶이, [](CmdMsg* cmdMsg) {
			Cmd떡볶이* pCmd = reinterpret_cast<Cmd떡볶이*>(cmdMsg);
			Make떡볶이(pCmd->m_param1, pCmd->m_param2);
			});
	}

	void DispatchCommand(CmdMsg* cmdMsg)
	{
		auto cmdFun = FindCommand(cmdMsg);
		cmdFun(cmdMsg);
	}

	//void FindCommand(CmdMsg* cmdMsg)
	std::function<void(CmdMsg*)> FindCommand(CmdMsg* cmdMsg)
	{
		CmdMsg* cmd = reinterpret_cast<CmdMsg*>(cmdMsg);

		auto it = m_cmdMap.find(cmd->cmdType);
		if (it == m_cmdMap.end()) std::terminate();
		return it->second;
	}

private:
	std::map<CMD_TYPE, std::function<void(CmdMsg*)> > m_cmdMap;
};

int main()
{
	cout << "hardware_concurrency : " << std::thread::hardware_concurrency() << endl;

	// 함수 객체 활용
	CmdDiapatcher _cmdDispatcher;
	_cmdDispatcher.InitCommand();


	concurrent_queue<shared_ptr<CmdMsg>> msgQueue;
	bool ck = false;

	auto workFun = ([&_cmdDispatcher, &msgQueue, &ck]
		{
			int count = 0;
			while (count < 10)
			{
				shared_ptr<CmdMsg> Cmd;
				if (msgQueue.try_pop(Cmd))
				{
					_cmdDispatcher.DispatchCommand(Cmd.get());
					++count;
					ck = true;
					// 여기에 current thread id를 출력해보세요.
					auto threadId = std::this_thread::get_id();
					cout << threadId << endl;
				}
			}

			cout << "장사 끝!" << endl;

		} // 다른 스레드!!
	);

	//auto workFun1 = ([&ck]
	//	{
	//		int count = 0;
	//		while (count < 5)
	//		{
	//			if (ck)
	//			{
	//				++count;
	//				ck = false;
	//			}
	//		}
	//		cout << "다섯개 돌파!" << endl;

	//	} // 다른 스레드!!
	//);


	// thread 야 이 일좀 해줘 (worker 함수) -> (worker 객체)
	std::thread workerThread(workFun);

	// 작업 스레드를 2개로 만들어보세요.
	std::thread workerThread1(workFun);

	// 여긴 메인 스레드
	int KeyInfo;

	do
	{
		KeyInfo = _getch();

		if (tolower(KeyInfo) == '1')
		{
			shared_ptr<CmdMsg> cmdMsg(new Cmd커피("아아"));
			msgQueue.push(cmdMsg);
		}
		else if (tolower(KeyInfo) == '2')
		{
			shared_ptr<CmdMsg> cmdMsg(new Cmd샌드위치(0));
			msgQueue.push(cmdMsg);
		}
		else if (tolower(KeyInfo) == '3')
		{
			shared_ptr<CmdMsg> cmdMsg(new Cmd샌드위치(1));
			msgQueue.push(cmdMsg);
		}
		else if (tolower(KeyInfo) == '4')
		{
			shared_ptr<CmdMsg> cmdMsg(new Cmd떡볶이(1, 1));
			msgQueue.push(cmdMsg);
		}

	} while (tolower(KeyInfo) != 'q');

	workerThread.join();
	workerThread1.join();

	system("pause");
}
