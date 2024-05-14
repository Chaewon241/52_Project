#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

/// <summary>
/// Listener와 마찬가지로 IOCP에 등록할 대상
/// </summary>

class Session : public IocpObject
{

	friend class Listener;
	friend class IocpCore;
	friend class Service;

public:
	Session();
	virtual ~Session();

public:
	void				Disconnect(const WCHAR* cause);

	shared_ptr<Service>	GetService() { return _service.lock(); }
	void				SetService(shared_ptr<Service> service) { _service = service; }

public:
	/* 정보 관련 */
	void				SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress			GetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	// 내가 연결되어있는지에 따라서 실질적으로 패킷을 계속 recv 해줄 것인지
	bool				IsConnected() { return _connected; }
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

public:
	/* 인터페이스 구현 */
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* 전송 관련 */
	// register -> 비동기 함수를 걸어준다
	void				RegisterConnect();
	void				RegisterRecv();
	void				RegisterSend();
	// process -> 성공적으로 register되면 호출
	void				ProcessConnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(int32 numOfBytes);
	// register랑 process 둘이 짝지어서 다님

	void				HandleError(int32 errorCode);

protected:
	/* 컨텐츠 코드에서 오버로딩 */
	virtual void		OnConnected() { }
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) { }
	virtual void		OnDisconnected() { }

public:
	// TEMP
	char _recvBuffer[1000];

private:
	// 서비스를 등록하거나 끊을 수 있기 때문에 weak_ptr로
	weak_ptr<Service>	_service;
	SOCKET			_socket = INVALID_SOCKET;
	NetAddress		_netAddress = {};
	// 접속중인지 아닌지
	Atomic<bool>	_connected = false;

private:
	USE_LOCK;

	/* 수신 관련 */

	/* 송신 관련 */

private:
	/* IocpEvent 재사용 */
	RecvEvent			_recvEvent;
};

