#ifndef _NETTYPE_H_
#define _NETTYPE_H_
#include<string.h>

enum class NetEnum
{
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


struct tagNetLoginCToS :tagNetHeader
{
	char username[6];
	char password[6];
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
	char m[10];
	tagNetLoginSToC(const char* msg) :tagNetHeader(sizeof(tagNetLoginSToC), NetEnum::E_LOGINSTOC)
	{
		strcpy(m, msg);
	}
};


#endif//_MESSAGETYPE_H_
