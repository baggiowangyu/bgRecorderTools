#include <iostream>
#include <string>
#include <map>

#include "Poco/ThreadPool.h"
#include "Poco/Runnable.h"
#include "Poco/ThreadTarget.h"
#include "Poco/Event.h"
#include "Poco/Timestamp.h"
#include "Poco/Timespan.h"
#include "Poco/UUID.h"
#include "Poco/UUIDGenerator.h"

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

	void SetGlobalId(const char *global_id) {
		global_id_ = global_id;
	}

	std::string GetGlobalId() {
		return global_id_;
	}

public:
	virtual void run() {
		Poco::Thread *ptr_thread = Poco::Thread::current();

		if (ptr_thread != NULL)
			std::cout<<"ThreadName : "<<ptr_thread->name().c_str()<<std::endl;

		std::string id = id_;
		std::string name = name_;
		std::string url = url_;
		std::string path = path_;

		while (true)
		{
			std::cout<<"id : "<<id_.c_str()<<std::endl;
			std::cout<<"name : "<<name_.c_str()<<std::endl;
			std::cout<<"url : "<<url_.c_str()<<std::endl;
			std::cout<<"path : "<<path_.c_str()<<std::endl;

			std::cout<<"id : "<<id.c_str()<<std::endl;
			std::cout<<"name : "<<name.c_str()<<std::endl;
			std::cout<<"url : "<<url.c_str()<<std::endl;
			std::cout<<"path : "<<path.c_str()<<std::endl;

			// 运行结束，通知管理器，将本线程标记为已完成
			// 等待合适的时机
			Poco::Thread::sleep(1000);
		}
	}

private:
	std::string global_id_;

private:
	std::string id_;
	std::string name_;
	std::string url_;
	std::string path_;
};

//////////////////////////////////////////////////////////////////////////
//
// 线程执行类管理器
class RunnableManager
{
public:
	RunnableManager();
	~RunnableManager();

public:
	MyRunnable* Allocate() {
		MyRunnable *my_runnable = new MyRunnable();

		Poco::UUID gid = global_id_generator_.createOne();
		my_runnable->SetGlobalId(gid.toString().c_str());
	}

	void Free(MyRunnable **runnable) {
		delete *runnable;
		*runnable = NULL;
	}

private:
	//void Install

public:
	Poco::UUIDGenerator global_id_generator_;

private:
	std::map<std::string, MyRunnable *> runable_set_;
};

int main(int argc, const char *argv[])
{
	Poco::ThreadPool threadpool_01("threadpool_01");
	int capacity = threadpool_01.capacity();
	int stack_size = threadpool_01.getStackSize();
	int used = threadpool_01.used();
	int allocated = threadpool_01.allocated();
	int available = threadpool_01.available();

	std::cout<<"当前线程池容量为："<<capacity<<"，栈空间为："<<stack_size<<"，已经是用了："
		<<used<<"个线程，已经为："<<allocated<<"个线程分配了资源，还有："<<available<<"个可用线程。"<<std::endl;

	// 启动5个线程任务
	//RunnableManager manager;

	for (int index = 0; index < 1; ++index)
	{
		char id[64] = {0};
		char name[64] = {0};
		char url[64] = {0};
		char path[64] = {0};
		sprintf_s(id, 64, "id_%02d", index);
		sprintf_s(name, 64, "name_%02d", index);
		sprintf_s(url, 64, "url_%02d", index);
		sprintf_s(path, 64, "path_%02d", index);

		// 从管理器中申请
		//MyRunnable *my_runnable = manager.Allocate();
		//my_runnable->SetParam(id, name, url, path);
		//threadpool_01.start(*my_runnable);

		MyRunnable my_runnable;
		my_runnable.SetParam(id, name, url, path);
		threadpool_01.start(my_runnable);
		Poco::Thread::sleep(10);
	}

	threadpool_01.joinAll();
	
	return 0;
}