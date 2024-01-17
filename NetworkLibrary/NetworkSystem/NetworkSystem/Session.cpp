#include "Session.h"
#include "ClientSocket.h"

void Session::SetClient(ClientSocket* pClient)
{
	m_sessionId = s_sessionIdCounter++;

	m_pClient = pClient;

	m_pClient->SetSessionId(m_sessionId);
}
