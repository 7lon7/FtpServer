#include "XFtpFactory.h"
#include "XFtpServerCMD.h"
#include "XFtpUSER.h"
#include "XFtpLIST.h"
#include "XFtpPORT.h"
#include "XFtpRETR.h"
#include "XFtpSTOR.h"
XTask *XFtpFactory::CreateTask()
{
	auto *x = new XFtpServerCMD();

	x->Reg("USER", new XFtpUSER());
	auto * list = new XFtpLIST();
	x->Reg("PWD", list);
	x->Reg("LIST", list);
	x->Reg("CWD", list);
	x->Reg("CDUP", list);
	x->Reg("PORT", new XFtpPORT());
	x->Reg("RETR", new XFtpRETR());
	x->Reg("STOR", new XFtpSTOR());
	
	return x;
}
