#pragma once
#include "XFtpTask.h"
#include <map>
class XFtpServerCMD :public XFtpTask
{
public:
	bool Init() override;
	void Read(struct bufferevent *bev) override;
	void Event(struct bufferevent *bev, short what) override;
	void Reg(const std::string&, XFtpTask *call);

	XFtpServerCMD()=default;
	~XFtpServerCMD();
private:
	std::map<std::string, XFtpTask*> calls;
	std::map<XFtpTask*, int> calls_del;
};
