#include "XFtpLIST.h"
#include <iostream>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <string>

using namespace std;

void XFtpLIST::Write(struct bufferevent *bev)
{
	ResCMD("226 Transfer complete\r\n");
	Close();

}
void XFtpLIST::Event(struct bufferevent *bev, short what)
{
	if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
	{
		cout << "BEV_EVENT_EOF | BEV_EVENT_ERROR |BEV_EVENT_TIMEOUT" << endl;
		Close();
	}
	else if(what & BEV_EVENT_CONNECTED)
	{
		cout << "XFtpLIST BEV_EVENT_CONNECTED" << endl;
	}
}

void XFtpLIST::Parse(std::string type, std::string msg)
{
	string resmsg;

	if (type == "PWD")
	{
		//257 "/" is current directory.
		resmsg = "257 \"";
		resmsg += cmdTask->curDir;
		resmsg += "\" is current dir.\r\n";

		ResCMD(resmsg);
	}
	else if (type == "LIST")
	{
		//-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n
		ConnectPORT();
		//2 1502 150
		ResCMD("150 Here comes the directory listing.\r\n");
		//string listdata = "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n";
		string listdata = GetListData(cmdTask->rootDir + cmdTask->curDir);
		Send(listdata);
	}
	else if (type == "CWD")
	{
		//CWD test\r\n
		int pos =static_cast<int> (msg.rfind(' ') + 1);

		string path = msg.substr(pos, msg.size() - pos - 2);
		if (path[0] == '/')
		{
			cmdTask->curDir = path;
		}
		else
		{
			if (cmdTask->curDir[cmdTask->curDir.size() - 1] != '/')
            {
			    cmdTask->curDir += "/";
            }
			cmdTask->curDir += path + "/";
		}
		//  /test/
		ResCMD("250 Directory succes chanaged.\r\n");
	}
	else if (type == "CDUP")
	{
		//  /Debug/test_ser.A3C61E95.tlog /Debug   /Debug/
		string path = cmdTask->curDir;
		////  /Debug/test_ser.A3C61E95.tlog /Debug  
		if (path[path.size() - 1] == '/')
		{
			path = path.substr(0, path.size() - 1);
		}
		int pos =static_cast<int>(path.rfind('/'));
		path = path.substr(0, pos);
		cmdTask->curDir = path;
		ResCMD("250 Directory succes chanaged.\r\n");
	}
}
std::string XFtpLIST::GetListData(const std::string& path)
{
	//-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n
	string data;

	string cmd = "ls -l ";
	cmd += path;
	cout << "popen:" << cmd << endl;
	FILE *f = popen(cmd.c_str(), "r");
	if (!f)
    {
	    return data;
    }
	char buffer[1024] = { 0 };
	for (;;)
	{
		int len =static_cast<int>(fread(buffer, 1, sizeof(buffer) - 1, f));
		if (len <= 0)
        {
		    break;
        }
		buffer[len] = '\0';
		data += buffer;
	}
	pclose(f);

	return data;
}