﻿// BaseWindow.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "BaseWindow.h"
#include "Bitmap/TtBitmap.h"
#include "Bitmap/TtApplication.h"
#include <string>

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND                hWindow = nullptr;
BOOL                GAppRun = TRUE;
HDC                 hDC = nullptr;
TtBitmap            BackBuffer;
TtBitmap            Bmp01;

void TickOnIdle(HWND hwnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BASEWINDOW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    TtApplication::Get()->Initialize();

    Bmp01.LoadBmp(hDC, TtApplication::Get()->GetAbsPath("/test.bmp").c_str());
    Bmp01.FlushPixels();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BASEWINDOW));

    MSG msg;

    // 主消息循环:
    while (GAppRun)//GetMessage(&msg, nullptr, 0, 0))
    {
        auto hasMsg = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        if (hasMsg)
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
                TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
        else
        {
            TickOnIdle(hWindow);
        }
    }
    
    ::ReleaseDC(hWindow, hDC);
    return (int) msg.wParam;
}

void TickOnIdle(HWND hWnd)
{
    /*std::string text = "我是Titan，是Johnson的儿子";
    RECT rc{};
    rc.left = 0;
    rc.top = 0;
    rc.right = 500;
    rc.bottom = 100;
    ::DrawTextA(hDC, text.c_str(), (int)text.length(), &rc, DT_CENTER);*/

    int x = BackBuffer.GetWidth();
    int y = BackBuffer.GetHeight();
    
    BackBuffer.Clear(TtRGBA(0, 0, 0, 0));
    for (int i = 0; i < x; i++)
    {
        BackBuffer.SetPixel(i, y / 2, TtRGBA(0xff, 0, 0, 0xff));
    }
	for (int i = 0; i < y; i++)
	{
		BackBuffer.SetPixel(x / 2, i, TtRGBA(0xff, 0, 0, 0xff));
	}
    BackBuffer.FlushPixels();
    BackBuffer.DrawToDC(hDC, 0, 0, BackBuffer.GetWidth(), BackBuffer.GetHeight(), 0, 0);
    Bmp01.DrawToDC(hDC, 0, 0, Bmp01.GetWidth(), Bmp01.GetHeight(), 0, 0);
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BASEWINDOW));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BASEWINDOW);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWindow = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWindow)
   {
      return FALSE;
   }

   hDC = ::GetDC(hWindow);

   ShowWindow(hWindow, nCmdShow);
   UpdateWindow(hWindow);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
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
        }
        break;
    case WM_SIZE:
        {
		    switch (wParam)
		    {
		        case SIZE_MINIMIZED:
			        break;
		        case SIZE_RESTORED:
                    break;
		        case SIZE_MAXIMIZED:
			        break;
		    }
            int width = (int)LOWORD(lParam);
            int height = (int)HIWORD(lParam);

            BackBuffer.SetSize(hDC, width, height);
        }
        break;
    //case WM_PAINT:
    //    {
    //        PAINTSTRUCT ps;
    //        HDC hdc = BeginPaint(hWnd, &ps);
    //        // TODO: 在此处添加使用 hdc 的任何绘图代码...
    //        EndPaint(hWnd, &ps);
    //    }
    //    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        GAppRun = FALSE;
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
