#include "XThread.h"
#include <thread>
#include <iostream>
#include <event2/event.h>
#include "XTask.h"
#include <unistd.h>

using namespace std;

static void NotifyCB(evutil_socket_t fd, short which, void *arg)
{
	auto *t = (XThread *)arg;
	t->Notify(fd, which);
}
void XThread::Notify(evutil_socket_t fd, short which)
{
	char buf[2] = { 0 };
	int re =static_cast<int>(read(fd, buf, 1));
	if (re <= 0)
    {
	    return;
    }
	cout << id << " thread " << buf << endl;
	XTask *task = nullptr;
	tasks_mutex.lock();
	if (tasks.empty())
	{
		tasks_mutex.unlock();
		return;
	}
	task = tasks.front();
	tasks.pop_front();
	tasks_mutex.unlock();
	task->Init();
}

void XThread::AddTask(XTask *t)
{
	if (!t)
    {
	    return;
    }
	t->base = this->base;
	tasks_mutex.lock();
	tasks.push_back(t);
	tasks_mutex.unlock();
}
void XThread::Activate() const
{

	int re =static_cast<int>(write(this->notify_send_fd, "c", 1));
	if (re <= 0)
	{
		cerr << "XThread::Activate() failed!" << endl;
	}
}

void XThread::Start()
{
	Setup();
	thread th(&XThread::Main,this);

	th.detach();
}

bool XThread::Setup()
{
    int fds[2];
	if (pipe(fds))
	{
		cerr << "pipe failed!" << endl;
		return false;
	}
	notify_send_fd = fds[1];

	event_config *ev_conf = event_config_new();
	event_config_set_flag(ev_conf, EVENT_BASE_FLAG_NOLOCK);
	this->base = event_base_new_with_config(ev_conf);
	event_config_free(ev_conf);
	if (!base)
	{
		cerr << "event_base_new_with_config failed in thread!" << endl;
		return false;
	}

	event *ev = event_new(base, fds[0], EV_READ | EV_PERSIST, NotifyCB, this);
	event_add(ev, nullptr);

	return true;
}
void XThread::Main()
{
	cout << id << " XThread::Main() begin" << endl;
	event_base_dispatch(base);
	event_base_free(base);

	cout << id << " XThread::Main() end" << endl;
}
