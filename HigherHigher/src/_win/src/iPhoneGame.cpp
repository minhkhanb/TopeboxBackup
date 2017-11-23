// iPhoneGame.cpp : Defines the entry point for the application.
//
#include "pch.h"
#include <windows.h>
#include "Thread.h"
#include "iPhoneGame.h"
#include "../../game/GameCore/CGame.h"
#include "../../game/Lib/Sound/BassLib.h"
#include <time.h>
#include "stdio.h"
//#include "bass.h"

//extern float _sensor_x;
//extern bool _hold_key_left;
//extern bool _hold_key_right;
#if !defined(SIMULATOR_SCREEN_SCALE)
#define SIMULATOR_SCREEN_SCALE 1.0f
#endif

#define MAX_LOADSTRING 100

extern "C" UINT8 *_soundDATA[CORE_GAME_MAX_SOUND];
bool _is_interupted = false;
//for debug
#if USE_LOG
static FILE* s_LogFile = NULL;
#define LOG_FILE	"iPhoneGame_%li.log"
static char s_LogFileName[64];
#endif 

void PrintLog(const char* format, ...)
{
#if USE_LOG

	if (s_LogFile == NULL)
	{
		GX_MEMSET(s_LogFileName, 0, 64);
		SPRINTF( s_LogFileName, LOG_FILE, time(NULL) );
		fopen_s(&s_LogFile, s_LogFileName, "w");
	}
	
	va_list arg_list;
	va_start(arg_list, format);

	if (s_LogFile != NULL)
	{
		vfprintf(s_LogFile, format, arg_list);
		fprintf("\n");
		fflush(s_LogFile);
	}

	va_end(arg_list);
	
#endif
}




// Global Variables:
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

HINSTANCE		hInst			= NULL;	// current instance
HWND			hMainWnd		= NULL; // main window
HDC				hGameWndDC		= NULL;	// current DC of the main window

int				nAppExit;		// application exit flag
BOOL			bPause			= FALSE;
BOOL			bPlayOneFrame	= FALSE;
BOOL			bConPause		= FALSE;

CGame*			g_pGame			= NULL;	
BOOL			LANDSCAPE_ENABLED = false;

//OpenGL var
HGLRC			hRC 			= NULL;		// Permanent Rendering Context
GLfloat			aspectRatio 	= 1.0f;
BOOL			bLockDisplay	= FALSE;	// to be thread safe

//Device
Device dv;
//OpenGL function
int InitGL(GLvoid);
GLvoid CloseGL(GLvoid);
void Refresh3D();

//game control function
void GamePause()
{
	bPause = TRUE;
}

void GameResume()
{
	bPause = FALSE;
	bPlayOneFrame = FALSE;
	bConPause = FALSE;
}

void PauseScreen()
{
	TextOutA( hGameWndDC, 10, 10, "Game Pause! F6 resume.", 22 );	
}

void PCDisplayUpdate(HDC hdc)
{
	if (bLockDisplay)
	{
		return;
	}
	
	bLockDisplay = TRUE;

	SwapBuffers(hGameWndDC);

	bLockDisplay = FALSE;
}


unsigned int GetTime()
{
	return GetTickCount();
}

void MainUpdate()
{
	if (_is_interupted){
		::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		TextOutA(hGameWndDC, 10, 10, "Interupt! F3 againt to resume.", 22);
	}
	if(bPause)
	{
		PauseScreen();
		return;
	}
	if ((!bPause || bPlayOneFrame) && !bConPause)
	{
		//Refresh3D();			
		bPlayOneFrame = FALSE;
		
		g_pGame->Run();
	}

	if (g_pGame->_isExit)
	{
		nAppExit = 1;
	}

	// draw screen to window
	if (!bConPause)
	{
		PCDisplayUpdate(hGameWndDC);
	}
}



// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

	size_t size = wcstombs(NULL, lpCmdLine, 0);
	if (size > 0)
	{
		g_pGame = new CGame();
		g_pGame->Init();

		char * charCmd = new char[size + 1];
		wcstombs(charCmd, lpCmdLine, size + 1);

		g_pGame->ProcessCommandLine(charCmd);
		SAFE_DEL_ARRAY(charCmd);

		return 0;
	}

#if !defined(_DISABLE_CONSOLE)
	AllocConsole();
	freopen("con","w",stdout);
	freopen("con","w",stderr);
#endif

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IPHONEGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IPHONEGAME));
	
	//game init
	
	
	
	dv.ScreenSize.W = SIMULATOR_SCREEN_WIDTH * SIMULATOR_SCREEN_SCALE;
	dv.ScreenSize.H = SIMULATOR_SCREEN_HEIGHT * SIMULATOR_SCREEN_SCALE;
	dv.IsSupportFacebook = SIMULATOR_SUPPORTED_FACEBOOK;
	dv.IsSupportAppleGameCenter = SIMULATOR_SUPPORTED_APPLE_GAMECENTER;
	dv.setDeviceAgent("Simulator");
	dv.setDeviceModel("Simulator");
	if (SIMULATOR_SCREEN_WIDTH > SIMULATOR_SCREEN_HEIGHT){
		dv.Device_Orientation = Orientation::Landscape_90;
	}
	else{
		dv.Device_Orientation = Orientation::Portrait;
	}
	g_pGame = new CGame();	
	g_pGame->InitDevice(dv);
	//BassLib
	BassLib* sound_engine = new BassLib();
	sound_engine->InitBass(hMainWnd);
	g_pGame->SetSoundEngine(sound_engine);
	g_pGame->Init();

	nAppExit = 0;

	// Main message loop:
	std::auto_ptr<Runnable> r(new GameServerRunnable(1));
	std::auto_ptr<Thread> thread1(new Thread(r));
	thread1->start();

	while (!nAppExit)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				if (msg.message == WM_QUIT)
				{
					nAppExit = 1;
					break;
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
		else
		{
			MainUpdate();
		}
	}
#if ADD_ADVERT
	ADVERT().ServerStop();
#endif
	g_pGame->_isExit = true;
	Sleep(100);
	SAFE_DEL(g_pGame);
	
	while (bLockDisplay);
	bLockDisplay = TRUE;
	
	CloseGL();
	
	ReleaseDC(hMainWnd, hGameWndDC);
	hGameWndDC = NULL;
	DestroyWindow(hMainWnd);
	hMainWnd = NULL;
	
	if (!UnregisterClass( szWindowClass, hInstance ))
	{
		MessageBox(NULL, L"Could Not Unregister Class.", L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
	}

	InvalidateRect(NULL, NULL, TRUE);

	//free mem sound
	sound_engine->Release();
	delete sound_engine;
	sound_engine = NULL;

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IPHONEGAME));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_IPHONEGAME);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable
	float screen_w = SIMULATOR_SCREEN_WIDTH * SIMULATOR_SCREEN_SCALE;
	float screen_h = SIMULATOR_SCREEN_HEIGHT * SIMULATOR_SCREEN_SCALE;
	hMainWnd = CreateWindowEx(	WS_EX_APPWINDOW| WS_EX_WINDOWEDGE,
								szWindowClass, 
								szTitle, 
								WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
								(GetSystemMetrics(SM_CXSCREEN) - screen_w) / 2,
								(GetSystemMetrics(SM_CYSCREEN) - screen_h) / 2,
								screen_w + GetSystemMetrics(SM_CXFIXEDFRAME) * 2 +
												 GetSystemMetrics(SM_CXBORDER) * 2,
								screen_h + GetSystemMetrics(SM_CYCAPTION) +
												 GetSystemMetrics(SM_CYMENU) +
												 GetSystemMetrics(SM_CYFIXEDFRAME) * 2 +
												 GetSystemMetrics(SM_CXBORDER) * 2,
								NULL, 
								NULL, 
								hInstance, 
								NULL);

	if (!hMainWnd)
	{
		return FALSE;
	}

	//OpenGL
	GLuint		PixelFormat;

	hGameWndDC = GetDC(hMainWnd);
	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		16,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
0,											// No Stencil Buffer
0,											// No Auxiliary Buffer
PFD_MAIN_PLANE,								// Main Drawing Layer
0,											// Reserved
0, 0, 0										// Layer Masks Ignored
	};

	PixelFormat = ChoosePixelFormat(hGameWndDC, &pfd);

	SetPixelFormat(hGameWndDC, PixelFormat, &pfd);
	hRC = wglCreateContext(hGameWndDC);
	wglMakeCurrent(hGameWndDC, hRC);
	InitGL();

	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	int x, y;
	//unsigned long time;
	//POINT mp;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;

		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_CHAR:

		return 0;

	case WM_KEYDOWN:

		// Pause ON/OFF	
		if ((wParam & 0xFF) == VK_SPACE)
		{
			bPause = !bPause;
		}
		// Play one frame (works in paused mode)...
		if ((wParam & 0xFF) == VK_TAB)
			bPlayOneFrame = TRUE;

		// ConsolePause ON/OFF	
		if ((wParam & 0xFF) == VK_PAUSE)
			bConPause = !bConPause;

		if ((wParam & 0xFF) == VK_F1)
		{
			LANDSCAPE_ENABLED = !LANDSCAPE_ENABLED;

			int normalWidth = SIMULATOR_SCREEN_WIDTH * SIMULATOR_SCREEN_SCALE;
			int normalHeight = SIMULATOR_SCREEN_HEIGHT * SIMULATOR_SCREEN_SCALE;

			int addedWidth = GetSystemMetrics(SM_CXFIXEDFRAME) * 2 + GetSystemMetrics(SM_CXBORDER) * 2;
			int addedHeight = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CXBORDER) * 2;

			if (LANDSCAPE_ENABLED)
			{
				SetWindowPos(hWnd, NULL, 0, 0, normalHeight + addedWidth, normalWidth + addedHeight, SWP_NOMOVE);
			}
			else
			{
				SetWindowPos(hWnd, NULL, 0, 0, normalWidth + addedWidth, normalHeight + addedHeight, SWP_NOMOVE);
			}
		}
		else if ((wParam & 0xFF) == VK_F2){
			if (dv.Device_Orientation == Orientation::Portrait){
				dv.Device_Orientation = Orientation::Landscape_90;
			}else if (dv.Device_Orientation == Orientation::Landscape_90){
				dv.Device_Orientation = Orientation::Landscape_270;
			}
			else if (dv.Device_Orientation == Orientation::Landscape_270){
				dv.Device_Orientation = Orientation::Portrait;
			}
			if (g_pGame != NULL){
				g_pGame->OnChangeOrientation(dv.Device_Orientation);
			}
		}
		else if ((wParam & 0xFF) == VK_F3){
			if (_is_interupted){
				g_pGame->Resume();
				_is_interupted = false;
			}
			else{
				g_pGame->Pause();
				_is_interupted = true;
			}
		}
		else if ((wParam & 0xFF) == VK_F4){
			if (g_pGame){
				g_pGame->OnBackKeyPress();
			}
		}
		else if ((wParam & 0xFF) == VK_F5)
		{
			GamePause();
		}
		else if ((wParam & 0xFF) == VK_F6)
		{
			GameResume();
		}
			
		if (g_pGame && !bPause && ! _is_interupted){
			char key = (wParam & 0xFF);
			g_pGame->addEvent(EVENT_KEYDOWN, &key, 1);
		}
		break;

	case WM_KEYUP:
		if (g_pGame && !bPause && !_is_interupted)
		{
			char key = (wParam & 0xFF);
			g_pGame->addEvent(EVENT_KEYUP, &key, 1);
			switch (key)
			{
			case 49:
				g_pGame->Key1Press();
				break;
			case 50:
				g_pGame->Key2Press();
				break;
			case 51:
				g_pGame->Key3Press();
				break;
			}
			
		}
		break;

	case WM_SIZE:
		return 0;

	case WM_LBUTTONDOWN:
			
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		//4B - touch system
		g_pGame->_touch_id_gen++;
		g_pGame->NotifyTouchesBegan(g_pGame->_touch_id_gen, x, y, 1);
		//4B - end

		break;

	case WM_LBUTTONUP:

		//4B - touch system
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		g_pGame->NotifyTouchesEnd(g_pGame->_touch_id_gen, x, y, 1);
		//4B - end

		break;

	case WM_MOUSEMOVE:
		//4B - touch system
		if(wParam == MK_LBUTTON)
		{				
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			g_pGame->NotifyTouchesMoved(g_pGame->_touch_id_gen, x, y, 1);
		}
		//4B - end

		{
			int xx = LOWORD(lParam);
			int yy = HIWORD(lParam);
			char txt[20];
			sprintf(txt, "x: %d   y: %d", xx, yy);
			SetWindowTextA(hWnd, txt);
		}
		break;
	case WM_CREATE:			
		// initialize BASS
			
		break;
	case WM_ACTIVATE:

		if (LOWORD(wParam) == WA_INACTIVE)
		{
				
		}

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


//void LinkOES2EXT();

int InitGL(GLvoid)										
{
	//LinkOES2EXT ();
	::glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background
	::glClearDepth(1.0f);									// Depth Buffer Setup
	::glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	::glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	::glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);	// Really (NOT) Nice Perspective Calculations

	return TRUE;										// Initialization Went OK
}	


void Refresh3D()
{
	::glEnable(GL_DEPTH_TEST);
	::glDepthMask(TRUE);		
	::glDepthFunc(GL_LEQUAL);
	::glClearDepth(1.0f);
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
}

GLvoid CloseGL(GLvoid)								
{
	if (hRC)	
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRC);
		hRC=NULL;				
	}
}

