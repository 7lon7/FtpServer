#include "XFtpSTOR.h"
#include <iostream>
#include <event2/bufferevent.h>
#include <event2/event.h>
using namespace std;

void XFtpSTOR::Parse(std::string type, std::string msg)
{
	int pos =static_cast<int>(msg.rfind(' ') + 1);
	string filename = msg.substr(pos, msg.size() - pos - 2);
	string path = cmdTask->rootDir;
	path += cmdTask->curDir;
	path += filename;
	fp = fopen(path.c_str(), "wb");
	if (fp)
	{
		ConnectPORT();

		ResCMD("125 File OK\r\n");
		bufferevent_trigger(bev, EV_READ, 0);
	}
	else
	{
		ResCMD("450 file open failed!\r\n");
	}
}
void XFtpSTOR::Read(struct bufferevent *bev)
{
	if (!fp)
    {
	    return;
    }
	for (;;)
	{
		int len =static_cast<int>(bufferevent_read(bev, buf, sizeof(buf)));
		if (len <= 0)
        {
		    return;
        }
		int size =static_cast<int>(fwrite(buf, 1, len, fp));
		cout << "<"<<len<<":"<<size << ">" << flush;
	}
}
void XFtpSTOR::Event(struct bufferevent *bev, short what)
{
	if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
	{
		cout << "XFtpSTOR BEV_EVENT_EOF | BEV_EVENT_ERROR |BEV_EVENT_TIMEOUT" << endl;
		Close();
		ResCMD("226 Transfer complete\r\n");
	}
	else if (what&BEV_EVENT_CONNECTED)
	{
		cout << "XFtpSTOR BEV_EVENT_CONNECTED" << endl;
	}
}