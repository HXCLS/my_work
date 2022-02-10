#ifndef _NETHEADER_H_
#define _NETHEADER_H_
#include<string.h>

enum class NetEnum
{
	E_PING,
	E_LOGINCTOS,
	E_LOGINSTOC
};

struct tagNetHeader
{
	int len;
	int type;
	tagNetHeader(int len, NetEnum type)
	{
		this->type = (int)type;
		this->len = len;
	}
};
struct tagNetPing :tagNetHeader
{
	tagNetPing() :tagNetHeader(sizeof(tagNetPing), NetEnum::E_PING) {}
};

struct tagNetLoginCToS :tagNetHeader
{
	char username[20];
	char password[20];
	tagNetLoginCToS(const char* un, const char* pwd)
		:tagNetHeader(sizeof(tagNetLoginCToS), NetEnum::E_LOGINCTOS)
	{
		strcpy(username, un);
		strcpy(password, pwd);
	}
};

//#define _WINSOCK_DEPRECATED_NO_WARNINGS 


struct tagNetLoginSToC :tagNetHeader
{
	char _[20];
	tagNetLoginSToC(const char* msg) :tagNetHeader(sizeof(tagNetLoginSToC), NetEnum::E_LOGINSTOC)
	{
		strcpy(_, msg);
	}
};


#endif//_NETHEADER_H
