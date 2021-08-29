// 3s_timer.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include <commctrl.h>
#include "3s_timer.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HWND mhWnd;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY3STIMER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, 0))
    {
        return FALSE;
    }
     
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3STIMER));

    MSG msg;

    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), mhWnd, About);

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {0};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    //wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY3STIMER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY3STIMER);
    wcex.lpszClassName  = szWindowClass;
    //wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   mhWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
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

DWORD fps = 24;
long int timer_step = 0;
long int timer_time = 0;
HWND trackbar;
DWORD timer_size = 1000;
DWORD prev_ms = 0;

void GetInternalData(HWND hDlg, UINT message, UINT_PTR timerID, DWORD milliseconds)
{
    

    if (!prev_ms)
    {
        prev_ms = milliseconds;
        return;
    }

    DWORD elapsed = milliseconds - prev_ms;
    prev_ms = milliseconds;


    if (timer_step == 0)//low stop
    {
        timer_time += elapsed;

        if (timer_time > timer_size)
        {
            timer_time = 0;
            timer_step = 1;
        }
        return;
    }


    if (timer_step == 2)//high stop
    {
        timer_time -= elapsed;

        if (timer_time < 1)
        {
            timer_time = timer_size;
            timer_step = 3;
        }
        return;
    }

    if (timer_step == 1)//go up
    {
        timer_time += elapsed;

        SendMessage(trackbar, PBM_SETPOS,
            timer_time,
            0);

        if (timer_time > timer_size)
        {
            timer_time = timer_size;
            timer_step = 2;
        }
        return;
    }

    if (timer_step == 3)//go down
    {
        timer_time -= elapsed;

        SendMessage(trackbar, PBM_SETPOS,
            timer_time,
            0);

        if (timer_time < 1)
        {
            timer_time = 0;
            timer_step = 0;
        }
        return;
    }


    }

void ResetTimer(HWND hDlg, DWORD time)
{
    KillTimer(hDlg, 1);

    wchar_t caption[100];
    swprintf_s(caption, _T("About %i timer"), time);

    SetWindowText(hDlg, caption);


    trackbar = GetDlgItem(hDlg, IDC_PROGRESS1);

    timer_size = time * 1000;

    SendMessage(trackbar, PBM_SETRANGE,
        (WPARAM)FALSE,
        MAKELPARAM(0, timer_size));

    SetTimer(hDlg,                 // handle to main window 
        1,               // timer identifier 
        1000/fps,                     //25fps
        (TIMERPROC)GetInternalData);
 }

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {

    case WM_INITDIALOG:
    {


        ResetTimer(hDlg, 3);        

        return (INT_PTR)TRUE;
    }


    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            PostQuitMessage(0);
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDC_BUTTON1)
        {
            ResetTimer(hDlg, 1);
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDC_BUTTON2)
        {
            ResetTimer(hDlg, 2);
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDC_BUTTON3)
        {
            ResetTimer(hDlg, 3);
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDC_BUTTON4)
        {
            ResetTimer(hDlg, 4);
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDC_BUTTON5)
        {
            ResetTimer(hDlg, 5);
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDC_BUTTON6)
        {
            ResetTimer(hDlg, 6);
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDC_BUTTON7)
        {
            ResetTimer(hDlg, 7);
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDC_BUTTON8)
        {
            ResetTimer(hDlg, 8);
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDC_BUTTON9)
        {
            ResetTimer(hDlg, 9);
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDC_BUTTON10)
        {
            ResetTimer(hDlg, 10);
            return (INT_PTR)TRUE;
        }

        break;
    }
    return (INT_PTR)FALSE;
}
