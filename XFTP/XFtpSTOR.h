#pragma once
#include "XFtpTask.h"

class XFtpSTOR :public XFtpTask
{
public:
	void Parse(std::string type, std::string msg) override;
	void Read(struct bufferevent *bev) override;
	void Event(struct bufferevent *bev, short what) override;
private:
	FILE *fp = nullptr;
	char buf[1024] = { 0 };
};
