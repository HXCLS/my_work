#include "TcpClient.h"

int main()
{
	system("CHCP 65001");
	const int nClients = 1000;
	WSADATA wsaDate;
	if (WSAStartup(MAKEWORD(2, 2), &wsaDate) != 0)return 0;

	std::vector<TcpClient*>clients;
	for (int i = 0;i < nClients;++i)
	{
		TcpClient* client = new TcpClient();
		if (!client->Connect("127.0.0.1", 7890))
		{
			std::cout << "连接失败" << std::endl;
		}
		else
		{
			clients.push_back(client);
		}
	}

	X::Timestamp timestamp;
	X::Timestamp times;
	int doTime = 0;
	times.Update();
	while (true)
	{
		timestamp.Update();
		auto begin = clients.begin();
		auto end = clients.end();
		int msgNum = 200;
		for (;begin != end;)
		{
			TcpClient* client = *begin;
			client->Update(0);
			for (int i = 0;i < msgNum;++i)
			{
				if (i >= msgNum / 2)
				{
					client->SendLogin();
				}
				else
				{
					client->SendLogin2();
				}
			}
			if (!client->OnSend())
			{
				client->Close();
				begin = clients.erase(begin);
				end = clients.end();
				delete client;
				continue;
			}
			++begin;
		}

		//定时每秒发送
		int s = 1000 - timestamp.GetMillisSecond();
		if (s > 0)
		{
			Sleep(s);

		}
		if (times.GetMillisSecond() > doTime + 1000)
		{
			doTime += 1000;
			std::cout << TcpClient::_recvCount << std::endl;
			std::cout << "recv[" << TcpClient::_recvCount / (doTime / 1000) << "/s]"
				<< "  login ok" << "[" << TcpClient::_loginok << "]" << "  "
				<< "login false" << "[" << TcpClient::_loginfalse << "]" << std::endl;
			TcpClient::_loginfalse = 0;
			TcpClient::_loginok = 0;
		}
	}

	return 0;

}
