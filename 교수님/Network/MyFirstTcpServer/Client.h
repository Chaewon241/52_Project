#pragma once
#include "AsyncSocket.h"
#include "Types.h"

namespace netfish
{
	class Client : public AsyncSocket
	{
		/*
		* �Ǿ� Ŭ���̾�Ʈ�� 1��1 �����Ǵ� ���� Ŭ���� : ��Ʈ��ũ ���� ���� � ����
		*/
	public:
		Client() = default;
		virtual ~Client() = default;

		void SetSessionId(SessionId sessionId) { m_sessionId = sessionId; }
		SessionId GetSessionId() { return m_sessionId; }

	private:
		// ���� ��ü�� Ŭ���̾�Ʈ(����) ��ü�� 1��1 ������ ����
		// ��Ʈ��ũ ������ ����� �ϴ��� ���� ��ü�� ����־���ϴ� ��찡 ���� �� ����.
		// ������ �ٸ� ��ü�� �����ؼ� ó���� �� �Ϸ� �Ǹ� ���� �ؾ���.
		// ���� ��� �α��� ����
		// ������ �Ǹ� Ŭ���̾�Ʈ ��ü�� ����
		// ���������� ���� ��ü�� ���� �ٷ� ���踦 �ξ������� ���ǿ����� �Ǿ�κ��� �����ڵ带 ���� ������
		// ���� �ð��� ���� �Ŀ� �������� ������ ���������.
		//
		SessionId m_sessionId = 0;
	};
}