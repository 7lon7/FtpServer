#include <event2/event.h>
#include <event2/listener.h>
#include <cstring>
#include "XThreadPool.h"
#include <csignal>
#include <iostream>
#include "XFtpFactory.h"
using namespace std;
#define SPORT 21
void listen_cb(struct evconnlistener * e, evutil_socket_t s, struct sockaddr *a, int socklen, void *arg)
{
	cout << "listen_cb" << endl;
	XTask *task = XFtpFactory::Get()->CreateTask();
	task->sock = s;
	XThreadPool::Get()->Dispatch(task);
}
int main()
{

	//忽略管道信号，发送数据给已关闭的socket
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
	    return 1;
    }
	//1 初始化线程池
	XThreadPool::Get()->Init(10);

    std::cout << "test thread pool!\n"; 
	//创建libevent的上下文
	event_base * base = event_base_new();
	if (base)
	{
		cout << "event_base_new success!" << endl;
	}
	//监听端口
	//socket ，bind，listen 绑定事件
	sockaddr_in sin{0};
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SPORT);

	evconnlistener *ev = evconnlistener_new_bind(base,	// libevent的上下文
		listen_cb,					//接收到连接的回调函数
		base,						//回调函数获取的参数 arg
		LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,//地址重用，evconnlistener关闭同时关闭socket
		10,							//连接队列大小，对应listen函数
		(sockaddr*)&sin,							//绑定的地址和端口
		sizeof(sin)
		);
	//事件分发处理
	if(base)
    {
	    event_base_dispatch(base);
    }
	if(ev)
    {
	    evconnlistener_free(ev);
    }
	if(base)
    {
	    event_base_free(base);
    }
	return 0;
}
