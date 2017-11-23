#include <iostream>
#include <memory>
#include <cassert>
#include <windows.h>
#include <process.h>
#if !_THREAD_H
#define _THREAD_H 1
class Runnable {
public:
	virtual void* run() = 0;
	virtual ~Runnable() = 0;
};



class Thread {
public:
	Thread(std::auto_ptr<Runnable> run);
	Thread();
	virtual ~Thread();
	void start();
	void* join();
private:
	HANDLE hThread;
	unsigned wThreadID;
	// runnable object will be deleted automatically
	std::auto_ptr<Runnable> runnable;
	Thread(const Thread&);
	const Thread& operator=(const Thread&);
	// called when run() completes
	void setCompleted();
	// stores return value from run()
	void* result;
	virtual void* run() {return 0;}
	static unsigned WINAPI startThreadRunnable(LPVOID pVoid);
	static unsigned WINAPI startThread(LPVOID pVoid);
	void printError(LPTSTR lpszFunction, LPSTR fileName, int lineNumber);
};


class GameServerRunnable: public Runnable {
public:
	GameServerRunnable(int ID) : myID(ID) {}
	virtual void* run();
private:
	int myID;
};

class GameServerThread: public Thread {
public:
	GameServerThread(int ID) : myID(ID) {}
	virtual void* run();
private:
	int myID;
};
#endif