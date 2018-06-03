#include <iostream>
#include <string>

#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/ThreadTarget.h"
#include "Poco/Event.h"
#include "Poco/Timestamp.h"
#include "Poco/Timespan.h"

//////////////////////////////////////////////////////////////////////////
//
// 首先实现一个我自己的线程执行类
class MyRunnable : public Poco::Runnable
{
public:
	MyRunnable() {};
	~MyRunnable() {};

public:
	void SetParam(const char *id, const char *name, const char *url, const char *path) {
		id_ = id;
		name_ = name;
		url_ = url;
		path_ = path;
	}

public:
	virtual void run() {
		Poco::Thread *ptr_thread = Poco::Thread::current();

		while (true)
		{
			if (ptr_thread != NULL)
				std::cout<<"ThreadName : "<<ptr_thread->name().c_str()<<std::endl;

			std::cout<<"id : "<<id_.c_str()<<std::endl;
			std::cout<<"name : "<<name_.c_str()<<std::endl;
			std::cout<<"url : "<<url_.c_str()<<std::endl;
			std::cout<<"path : "<<path_.c_str()<<std::endl;

			ptr_thread->sleep(5000);
		}
		
	}

private:
	std::string id_;
	std::string name_;
	std::string url_;
	std::string path_;
};

int main(int argc, const char *argv[])
{
	// 线程对象
	Poco::Thread thread_01("thread_01");

	// 我们定义的线程执行类
	MyRunnable my_runnable_01;
	my_runnable_01.SetParam("123", "aaa", "http://127.0.0.1/", "E:\\123.txt");

	// 判断线程对象是都正在运行
	if (!thread_01.isRunning())
		std::cout<<"线程未运行"<<std::endl;
	else
		std::cout<<"线程正在运行"<<std::endl;

	thread_01.start(my_runnable_01);
	Poco::Thread::sleep(200);

	if (!thread_01.isRunning())
		std::cout<<"线程未运行"<<std::endl;
	else
		std::cout<<"线程正在运行"<<std::endl;

	try
	{
		thread_01.join(15000);
	}
	catch(Poco::Exception &e)
	{
		std::string msg;
		//e.message(msg);
		std::string disp_str = e.displayText();
		const char *e_classname = e.className();
		int errCode = e.code();

		Sleep(1000);
	}
	

	return 0;
}