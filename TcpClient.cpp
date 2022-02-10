
#include "TcpClient.h"

int TcpClient::_recvCount = 0;

TcpClient::TcpClient()
{
	_sock = INVALID_SOCKET;
	_recvLen = 0;
	_sendLen = 0;
}

bool TcpClient::Connect(const char* ip, int port)
{
	_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == INVALID_SOCKET)
		goto end;
	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = inet_addr(ip);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (SOCKET_ERROR == connect(_sock, (SOCKADDR*)&addr, sizeof(addr)))
		goto end;

	return true;
end:
	if (_sock != INVALID_SOCKET)
	{
		closesocket(_sock);
		_sock = INVALID_SOCKET;
	}
	return false;
}

bool TcpClient::Update(int t)
{
	FD_SET reads;
	FD_ZERO(&reads);
	FD_SET(_sock, &reads);
	struct timeval timeout { 0, 1000 * t };
	int nRet = select(NULL, &reads, nullptr, nullptr, &timeout);
	if (nRet <= 0)return true;

	int nRecv = recv(_sock, _recvBuff + _recvLen, NET_BUFF_LEN - _recvLen, 0);
	//断开连接
	if (nRecv <= 0)return false;
	_recvLen += nRecv;
	while (true)
	{
		if (_recvLen == 0)break;
		int nMsg = OnNetMsg(_recvBuff, _recvLen);
		if (nMsg <= 0)break;
		RemoveRecvBuff(nMsg);

	}
	return true;
}

bool TcpClient::Send(const char* msg, int len)
{
	if (_sendLen + len > NET_BUFF_LEN)return false;
	memcpy(_sendBuff + _sendLen, msg, len);
	_sendLen += len;
	return true;
}

void TcpClient::RemoveRecvBuff(int len)
{
	if (len == _recvLen)
	{
		_recvLen = 0;
	}
	else
	{
		memcpy(_recvBuff, _recvBuff + len, _recvLen - len);
		_recvLen -= len;
	}
}

void TcpClient::RemoveSendBuff(int len)
{
	if (len == _sendLen)
	{
		_sendLen = 0;
	}
	else
	{
		memcpy(_sendBuff, _sendBuff + len, _sendLen - len);
		_sendLen -= len;
	}
}


bool TcpClient::OnSend()
{
	if (_sendLen == 0)return true;
	int nSend = send(_sock, _sendBuff, _sendLen, 0);
	if (nSend <= 0)return false;
	RemoveSendBuff(nSend);
	return true;
}

void TcpClient::Close()
{
	_sock = INVALID_SOCKET;
}

int TcpClient::OnNetMsg(const char* msg, int len)
{
	if (sizeof(tagNetHeader) > len)return 0;
	tagNetHeader* header = (tagNetHeader*)msg;
	if (header->len > len)return 0;

	NetEnum type = (NetEnum)header->type;
	switch (type)
	{
	case NetEnum::E_PING:
		break;
	case NetEnum::E_LOGINCTOS:
		break;
	case NetEnum::E_LOGINSTOC:
	{
		tagNetLoginSToC* login = (tagNetLoginSToC*)msg;

		/*std::cout << "登录返回" << login->_ << std::endl;*/
	}
	break;
	default:
		break;
	}

	_recvCount++;
	return header->len;
}

void TcpClient::SendLogin()
{
	tagNetLoginCToS login("123", "123");
	Send((char*)&login, sizeof(tagNetLoginCToS));
}
