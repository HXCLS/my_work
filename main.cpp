#include "TcpClient.h"



int main()
{
	system("CHCP 65001");
	WSADATA wsaDate;
	if (WSAStartup(MAKEWORD(2, 2), &wsaDate) != 0)return 0;
	int sendNum = 0;


	std::vector<TcpClient*>clients;
	for (int i = 0;i < 1000;++i)
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
		for (;begin != end;)
		{
			TcpClient* client = *begin;
			client->Update(0);
			for (int i = 0;i < 10;++i)
			{
				client->SendLogin();
				sendNum++;
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
			std::cout << sendNum << std::endl;
			Sleep(s);
		}
		if (times.GetMillisSecond() > doTime + 1000)
		{
			doTime += 1000;
			std::cout << TcpClient::_recvCount << std::endl;
			std::cout << "recv[" << TcpClient::_recvCount / (doTime / 1000) << "/s]" << std::endl;
		}

	}

	return 0;

}


/*TcpClient client;
	if (client.Connect("127.0.0.1", 7890))
	{
		std::cout << "连接成功" << std::endl;
	}
	else
	{
		std::cout << "连接失败" << std::endl;
		return -1;
	}

	while (client.Update(0))
	{
		client.SendLogin();
		Sleep(100);
	}*/
