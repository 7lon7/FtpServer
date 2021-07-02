#pragma once
#include "XFtpTask.h"
class XFtpUSER : public XFtpTask
{
public:
	void Parse(std::string type, std::string msg) override;
	XFtpUSER()=default;
	~XFtpUSER()=default;
};
