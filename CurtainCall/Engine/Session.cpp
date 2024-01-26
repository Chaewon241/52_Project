#include "pch.h"
#include "Session.h"

#include "Client.h"

#include "../NetworkLibrary/ClientSocket.h"
#include "../NetworkLibrary/MyProtocol.h"

Session::Session()
{
}

Session::~Session()
{
}

void Session::Write(char* pData, int len)
{
}

void Session::Read(char* pData, int len)
{
	/*PacketHeader* pHeader = reinterpret_cast<PacketHeader*>(pData);

	switch (pHeader->id)
	{
	case C2S_READY:
		short ready = static_cast<int>((pData[4] - '0') * 10 + (pData[5] - '0'));
		if (ready == 0)
			m_bReady = false;
		else
			m_bReady = true;
	}*/

	short id = static_cast<int>((pData[2] - '0') * 10 + (pData[3] - '0'));

	// todo 채원: switch case문으로 바꿔주기
	if (id == C2S_READY)
	{
		short ready = static_cast<int>((pData[4] - '0') * 10 + (pData[5] - '0'));
		if(ready == 0)
			m_bReady = false;
		else
			m_bReady = true;
	}
}

static int netUpdateCnt = 0;
static int readUpdateCnt = 0;
void Session::NetUpdate()
{
	while(!m_recvQueue.empty())
	{
		netUpdateCnt++;
		Read(m_recvQueue.front().first, m_recvQueue.front().second);
		char* tmp = m_recvQueue.front().first;
		m_recvQueue.pop();
		delete[] tmp;
	}

	while (!m_sendQueue.empty())
	{
		std::shared_ptr<ClientSocket> soc = m_pPeerClient->GetSocket();
		int nSent = soc->Send(m_sendQueue.front().first, m_sendQueue.front().second);

		short size = static_cast<short>(m_sendQueue.front().first[0] - '0') * 10 + static_cast<short>(m_sendQueue.front().first[1] - '0');

		// todo 채원: 사이즈가 다를 때 해주는거 바꾸기
		if (size != nSent)
			continue;

		if (nSent > 0)
		{
			char* tmp = m_sendQueue.front().first;
			PopSendQueue();
			delete[] tmp;
		}

		// 소켓 버퍼가 가득 차서 전송이 불가능
		else if (nSent == 0)
		{

		}

		// 소켓 에러가 발생함. 우짤까
		else
		{

		}
	}
}

void Session::ReadUpdate()
{
	char* tmp = new char[RCV_BUF_SIZE];
	readUpdateCnt++;
	std::shared_ptr<ClientSocket> soc = m_pPeerClient->GetSocket();
	int nRead = soc->Recv(tmp, RCV_BUF_SIZE);

	if (nRead == -1)
	{
		return;
	}

	m_recvQueue.push({ tmp, nRead });
}

void Session::SetClient(std::shared_ptr<ClientNetworkManager> pClient)
{
	m_sessionId = s_sessionIdCounter++;

	m_pPeerClient = pClient;

	m_pPeerClient->SetSessionId(m_sessionId);
}

void Session::Finalize()
{
	while (!m_recvQueue.empty())
	{
		char* deleteQue = m_recvQueue.front().first;
		m_recvQueue.pop();
		delete[] deleteQue;
	}

	while (!m_sendQueue.empty())
	{
		char* deleteQue = m_sendQueue.front().first;
		m_sendQueue.pop();
		delete[] deleteQue;
	}
	printf("netUpdate : %d\n", netUpdateCnt);
	printf("readUpdate : %d\n", readUpdateCnt);
}
