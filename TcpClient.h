#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_


#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include<iostream>
#include<vector>
#include"NetType.h"
#define NET_BUFF_LEN (4096)
#include "../HighServer/Timestamp.hpp"


class TcpClient
{
private:
	SOCKET _sock;
	SOCKADDR_IN _addr;
	char _recvBuff[NET_BUFF_LEN];
	int _recvLen;
	char _sendBuff[NET_BUFF_LEN];
	int _sendLen;
public:
	static int _loginok;
	static int _loginfalse;
	static int _recvCount;

	TcpClient();
	bool Connect(const char* ip, int port);
	bool Update(int t);
	bool Send(const char* msg, int len);
	void RemoveRecvBuff(int len);
	void RemoveSendBuff(int len);
	bool OnSend();
	void Close();
public:
	int OnNetMsg(const char* msg, int len);
	void SendLogin();
	void SendLogin2();
};



#endif//_TCPCLIENT_H_
