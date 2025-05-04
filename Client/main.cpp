// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Client.h"
#include "string"
#include <ctime>

#include "TextDetectorApplication.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
//TextDetectorApplication application;

HWND hButtonStart;
HWND hEditLog;

bool bIsDetecting = false;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


// 현재 시간 문자열 생성 함수
std::wstring GetCurrentTimestamp() 
{
    time_t now = time(nullptr);
    struct tm localTime;
    localtime_s(&localTime, &now);

    wchar_t buffer[100];
    wcsftime(buffer, sizeof(buffer) / sizeof(wchar_t), L"[%Y-%m-%d %H:%M] ", &localTime);
    return std::wstring(buffer);
}

//
void AppendLog(HWND hEdit, const std::wstring& text) 
{
    int length = GetWindowTextLength(hEdit);
    SendMessage(hEdit, EM_SETSEL, length, length);
    SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)text.c_str());
}
//

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            if (bIsDetecting)
            {
                //application.Run();
                TextDetectorApplication::Run();
               
            }

            //if (FAILED(application.GetExit()))
            //{
            //    return FALSE;
            //}
        }

    }

    TextDetectorApplication::Release();
    //application.Release();

    return (int) msg.wParam;
}


//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   int screenX = GetSystemMetrics(SM_CXSCREEN); // 메인 모니터 너비
   int screenY = GetSystemMetrics(SM_CYSCREEN); // 메인 모니터 높이

   int startX = screenX / 2; // 오른쪽 절반 시작 위치
   int startY = screenY / 4;

   HWND hWnd = CreateWindowW(szWindowClass, L"POE2_Message", WS_OVERLAPPEDWINDOW,
       startX + 50, startY, 500, 450, nullptr, nullptr, hInstance, nullptr);

   // 기능함수 초기화
   //application.Initialize(hWnd, hEditLog, AppendLog, GetCurrentTimestamp);
   TextDetectorApplication::Initialize(hWnd, hEditLog, AppendLog, GetCurrentTimestamp);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    static HWND hButtonStop;
    static const wchar_t* ORIGINAL_TITLE = L"POE2_Message";

    switch (message)
    {
    case WM_CREATE:
        // 시작 버튼
        hButtonStart = CreateWindow(
            L"BUTTON", L"시작",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            20, 20, 100, 30,
            hWnd, (HMENU)IDC_START_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr
        );

        // 로그 출력 Edit 박스 (멀티라인)
        hEditLog = CreateWindowEx(
            WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
            20, 60, 440, 280,
            hWnd, (HMENU)IDC_LOG_EDIT, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr
        );
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDC_START_BUTTON:
                bIsDetecting = true;
                AppendLog(hEditLog, GetCurrentTimestamp() + L" 감지 시작\r\n");

                // 윈도우 제목 변경 (작업표시줄에 보이는 텍스트)
                SetWindowText(hWnd, L"메세지 감지중");

                // 시작 버튼 비활성화
                EnableWindow(hButtonStart, FALSE);

                // 중지 버튼 생성
                hButtonStop = CreateWindow(
                    L"BUTTON", L"중지",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    140, 20, 100, 30,
                    hWnd, (HMENU)IDC_STOP_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr
                );
                break;

            case IDC_STOP_BUTTON:
                bIsDetecting = false;
                //application.bFirstLogDone = false;
                TextDetectorApplication::bFirstLogDone = false;
                AppendLog(hEditLog, GetCurrentTimestamp() + L" ========== 감지 중단 ==========\r\n");

                // 윈도우 제목 원래대로 복원
                SetWindowText(hWnd, ORIGINAL_TITLE);

                // 시작 버튼 다시 활성화
                EnableWindow(hButtonStart, TRUE);

                // 중지 버튼 제거
                DestroyWindow(hButtonStop);
                hButtonStop = nullptr;
                break;

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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
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

// 정보 대화 상자의 메시지 처리기입니다.
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
