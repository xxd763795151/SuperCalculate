// SuperCalculate.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SuperCalculate.h"
#include "ExpCalc.h"

#define MAX_LOADSTRING 100
#define WNDX 400
#define WNDY 300
#define BEGIN_LEFT 10
#define BEGIN_TOP  20
#define BTN_NUM 20
#define SHOWX WNDX-2*BEGIN_LEFT
#define SHOWY 80
#define GAP 4
#define BTNX 50
#define BTNY 30
#define LEN 100
#define SHOW_KEY_DIS 20

typedef struct tagBtn
{
	TCHAR szText[4];
	unsigned id;
}BTN_S;

BTN_S g_stBtns[] = {
	{ L"7",7 },
	{ L"8", 8 },
	{ L"9", 9 },
	{ L"+", 11 },

	{ L"4", 4 },
	{ L"5", 5 },
	{ L"6", 6 },
	{ L"-", 12 },

	{ L"1", 1 },
	{ L"2", 2 },
	{ L"3", 3 },
	{ L"*", 13 },

	{ L"0", 0 },
	{ L".", 10 },
	{ L"/", 14 },

	{ L"(", 15 },
	{ L")", 16 },
	{ L"<-", 17 },
	{ L"C", 18 },
	
	{ L"=", 19 },
};
// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
HWND g_hWnd;
HDC g_hDc;
HDC g_hDcBuf;
HBITMAP g_hBmp;
HWND g_hBtnWnd[BTN_NUM];
TCHAR g_szExpress[LEN];
TCHAR g_szRst[LEN] = { 0 };
TCHAR g_szEqu[2] = { 0 };
int g_len = 0;
CExpCalc g_ExpCalc;
// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
VOID				DrawCalc();
VOID				CreateButton();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	//LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	wcscpy_s(szTitle, L"表达式计算器");
	LoadString(hInstance, IDC_SUPERCALCULATE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SUPERCALCULATE));

	// 主消息循环: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SUPERCALCULATE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+2);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SUPERCALCULATE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
	   (GetSystemMetrics(SM_CXSCREEN) - WNDX) / 2, (GetSystemMetrics(SM_CYSCREEN) - WNDY) / 2,
	   WNDX, WNDY, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   g_hWnd = hWnd;
   g_hDc = GetDC(hWnd);
   CreateButton();
   //CreateSolidBrush(RGB(0x5C,0xAC,0xEE));//
   /*g_hDcBuf = CreateCompatibleDC(g_hDc);
   g_hBmp = CreateCompatibleBitmap(g_hDcBuf, WNDX, WNDY);
   SelectObject(g_hDcBuf, g_hBmp);*/

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect = { BEGIN_LEFT, BEGIN_TOP, SHOWX, SHOWY };
	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		if (wmId < BTN_NUM)
		{
			if (g_stBtns[wmId].id < 17)
			{
				g_szExpress[g_len] = g_stBtns[wmId].szText[0];
				++g_len;
				InvalidateRect(hWnd,&rect,TRUE);
				break;
			}
			if (g_stBtns[wmId].id == 17)
			{
				if (g_len)
				{
					g_szExpress[wcslen(g_szExpress)] = 0;
					--g_len;
					InvalidateRect(hWnd, &rect, TRUE);
				}
				break;
			}
			if (g_stBtns[wmId].id == 18)
			{
				ZeroMemory(g_szExpress, LEN);
				ZeroMemory(g_szRst, LEN);
				ZeroMemory(g_szEqu, 2);
				g_len = 0;
				InvalidateRect(hWnd, &rect, TRUE);
				break;
			}
			if (g_stBtns[wmId].id == 19)
			{
				wchar_t szTmp[LEN] = { 0 };
				wcscpy(szTmp, g_szExpress);
				g_ExpCalc.AddFlag(szTmp);
				g_ExpCalc.Postfix(szTmp);
				if (!g_ExpCalc.Calculate(szTmp, g_szRst))
					wcscpy(g_szRst, L"孩子,你的表达式输入有误,请再检查一遍...");
				wcscpy(g_szEqu, L"=");
				InvalidateRect(hWnd, &rect, TRUE);
				break;
			}
		}
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  在此添加任意绘图代码...
		DrawCalc();
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		/*DeleteObject(g_hBmp);
		DeleteObject(g_hDcBuf);*/
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
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

VOID DrawCalc()
{
	Rectangle(g_hDc, BEGIN_LEFT, BEGIN_TOP, SHOWX , SHOWY);
	RECT rect = { BEGIN_LEFT, BEGIN_TOP, SHOWX, SHOWY/2 +BEGIN_TOP};
	SetBkMode(g_hDc, TRANSPARENT);
	if (g_len)
	{
		g_szExpress[g_len] = 0;
		DrawText(g_hDc, g_szExpress, g_len, &rect, DT_BOTTOM | DT_RIGHT);
	}
	RECT rstRect = { BEGIN_LEFT, BEGIN_TOP+40, SHOWX, SHOWY + BEGIN_TOP };
	if (wcslen(g_szRst))
	{
		DrawText(g_hDc, g_szEqu, wcslen(g_szEqu), &rstRect, DT_BOTTOM | DT_LEFT);
		DrawText(g_hDc, g_szRst, wcslen(g_szRst), &rstRect, DT_BOTTOM | DT_RIGHT);
	}
	//BitBlt(g_hDcBuf, 0, 0, WNDX, WNDY, g_hDc, 0, 0, SRCCOPY);
	//HBRUSH brush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));//
	//RECT rc = { 0, 0, WNDX, WNDY };
	//FillRect(g_hDcBuf, &rc, brush);
	//Rectangle(g_hDcBuf, BEGIN_LEFT, BEGIN_TOP, WNDX - 2 * BEGIN_LEFT, 120);
	//BitBlt(g_hDc, 0, 0, WNDX, WNDY, g_hDcBuf, 0, 0, SRCCOPY);
}

VOID CreateButton()
{
	int curX;
	int curY;
	for (size_t i = 0; i < 12; ++i)
	{
		curX = BEGIN_LEFT + i % 4 * (BTNX + GAP);
		curY = SHOWY + SHOW_KEY_DIS + i / 4 * (BTNY + GAP);
		g_hBtnWnd[i] = CreateWindow(L"Button", g_stBtns[i].szText, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			curX,curY , BTNX, BTNY, g_hWnd, (HMENU)i, hInst, NULL);
	}
	//"0"
	curX = BEGIN_LEFT;
	curY = SHOWY + SHOW_KEY_DIS + 3 * (BTNY + GAP);
	g_hBtnWnd[12] = CreateWindow(L"Button", g_stBtns[12].szText, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		curX, curY, BTNX*2+GAP, BTNY, g_hWnd, (HMENU)12, hInst, NULL);

	//"." / "
	for (size_t i = 13; i < 15; ++i)
	{
		curX = BEGIN_LEFT + (i - 13)* (BTNX + GAP) + 2 * (BTNX + GAP);
		g_hBtnWnd[i] = CreateWindow(L"Button", g_stBtns[i].szText, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			curX, curY, BTNX, BTNY, g_hWnd, (HMENU)i, hInst, NULL);
	}

	//(,),<-,C,
	
	int iWidth = (SHOWX - (BEGIN_LEFT + 4 * BTNX + 4 * GAP) - GAP * 2) / 2;
	for (size_t i = 15; i < 19; ++i)
	{
		curX = BEGIN_LEFT + 4 * (BTNX + GAP) + (i - 15) % 2 * (iWidth + 2*GAP);
		curY = SHOWY + SHOW_KEY_DIS + (i - 15) / 2 * (BTNY + GAP);
		g_hBtnWnd[i] = CreateWindow(L"Button", g_stBtns[i].szText, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			curX, curY, iWidth, BTNY, g_hWnd, (HMENU)i, hInst, NULL);
	}

	curX = BEGIN_LEFT + 4 * BTNX + 4 * GAP;
	curY = SHOWY + SHOW_KEY_DIS + 2 * (BTNY + GAP);
	g_hBtnWnd[19] = CreateWindow(L"Button", L"=", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		curX, curY, SHOWX - curX, 2 * BTNY + 1 * GAP, g_hWnd, (HMENU)19, hInst, NULL);
}