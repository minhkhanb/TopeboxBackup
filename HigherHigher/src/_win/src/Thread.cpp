#include "pch.h"
#include "../../game/GameCore/CGame.h"
#include "Thread.h"
// Pure virtual destructor: function body required
Runnable::~Runnable(){};
Thread::Thread(std::auto_ptr<Runnable> r) : runnable(r) {
	if(!runnable.get())
		printError(L"Thread(std::auto_ptr<Runnable> r) failed at ",
				__FILE__, __LINE__);
	hThread = 
		(HANDLE)_beginthreadex(NULL,0,Thread::startThreadRunnable,
				(LPVOID)this, CREATE_SUSPENDED, &wThreadID);
	if(!hThread)
		printError(L"_beginthreadex failed at ",__FILE__, __LINE__);
}

Thread::Thread() : runnable(NULL) {
	hThread = 
		(HANDLE)_beginthreadex(NULL,0,Thread::startThread,
				(LPVOID)this, CREATE_SUSPENDED, &wThreadID);
	if(!hThread)
		printError(L"_beginthreadex failed at ",__FILE__, __LINE__);
}

unsigned WINAPI Thread::startThreadRunnable(LPVOID pVoid) {
	Thread* runnableThread = static_cast<Thread*>(pVoid);
	runnableThread->result = runnableThread->runnable->run();
	runnableThread->setCompleted();
	return reinterpret_cast<unsigned>(runnableThread->result);
}

unsigned WINAPI Thread::startThread(LPVOID pVoid) {
	Thread* aThread = static_cast<Thread*>(pVoid);
	aThread->result = aThread->run();
	aThread->setCompleted();
	return reinterpret_cast<unsigned>(aThread->result);
}

Thread::~Thread() {
	if(wThreadID != GetCurrentThreadId()) {
		DWORD rc = CloseHandle(hThread);
		if(!rc) printError(L"CloseHandle failed at ",__FILE__, __LINE__);
	}
}

void Thread::start() {
	assert(hThread);
	DWORD rc = ResumeThread(hThread);
	// thread created is in suspended state, 
	// so this starts it running
	if(!rc) printError
			(L"ResumeThread failed at ",__FILE__, __LINE__);
}

void* Thread::join() {
	// A thread calling T.join() waits until thread T completes.
	return result;
}

void Thread::setCompleted() {
	// Notify any threads that are waiting in join()
}

void Thread::printError(LPTSTR lpszFunction, LPSTR fileName, int lineNumber)
{
	TCHAR szBuf[256];
	LPSTR lpErrorBuf;
	DWORD errorCode=GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER||
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		(LPTSTR)&lpErrorBuf,
		0,
		NULL);
	wsprintf(szBuf,L"%s failed at line %d in %s with error %d: %s", 
		     lpszFunction, lineNumber, fileName, errorCode, lpErrorBuf);
	DWORD numWritten; 
	WriteFile(GetStdHandle(STD_ERROR_HANDLE),
		szBuf,
		strlen(reinterpret_cast <const char *> (szBuf)),
		&numWritten,
		FALSE);
	LocalFree(lpErrorBuf);
	exit(errorCode);
}


 void* GameServerRunnable::run()
{
	std::cout << "Thread " << myID << " is running" << std::endl;
	//CGame::_this->ServerStart();
	//CGame::_this->ServerRun();
#if ADD_ADVERT
	CGame::_this->_advert.ServerStart();
#endif
	return reinterpret_cast<void*>(myID);
}
 void* GameServerThread::run()
{
		std::cout << "Thread " << myID << " is running" << std::endl;
		//CGame::_this->ServerStart();
		//CGame::_this->ServerRun();
		return reinterpret_cast<void*>(myID);
}