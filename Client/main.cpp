// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "framework.h"
#include "Client.h"
#include <ctime>

#include "TextDetectorApplication.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HWND hButtonStart;
HWND hEditLog;
HWND hChatLabel;
HWND hEditChatPath;
HWND hButtonSetChatPath;
HWND hEditWebhook;
HWND hWebhookLabel;
HWND hButtonSetWebhook;
HWND hCheckBoxImage;

std::wstring g_WebhookUrl;
std::wstring g_ChatPath;

bool bIsDetecting = false;
const wchar_t* ORIGINAL_TITLE = L"POE2_TradeDetector";

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK    ChildDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


// 현재 시간 문자열 생성 함수
std::wstring GetCurrentTimestamp() 
{
    time_t now = time(nullptr);
    struct tm localTime;
    localtime_s(&localTime, &now);

    wchar_t buffer[100];
    wcsftime(buffer, sizeof(buffer) / sizeof(wchar_t), L"[%Y-%m-%d %H:%M:%S] ", &localTime);
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
    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDI_ICON1));

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

                Sleep(1);
            }

            //if (FAILED(application.GetExit()))
            //{
            //    return FALSE;
            //}
        }

    }

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

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

   HWND hWnd = CreateWindowW(szWindowClass, ORIGINAL_TITLE, WS_OVERLAPPEDWINDOW,
       startX + 50, startY, 700, 475, nullptr, nullptr, hInstance, nullptr);

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

    switch (message)
    {
    case WM_CREATE:

        // 시작 버튼
        hButtonStart = CreateWindow
        (
            L"BUTTON", L"시작",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            20, 100, 100, 30,
            hWnd, (HMENU)IDC_START_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr
        );

        // 로그 출력 Edit 박스 (멀티라인)
        hEditLog = CreateWindowEx
        (
            WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
            20, 140, 440, 250,
            hWnd, (HMENU)IDC_LOG_EDIT, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr
        );

        hChatLabel = CreateWindow
        (
            L"STATIC", L"채팅 파일 경로",
            WS_CHILD | WS_VISIBLE,
            20, 20, 105, 25, 
            hWnd, nullptr, hInst, nullptr
        );

        // 채팅 경로 텍스트 박스
        hEditChatPath = CreateWindowEx
        (
            WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY | ES_AUTOHSCROLL | WS_BORDER,
            130, 20, 225, 27, hWnd, (HMENU)IDC_WEBHOOK_EDIT, hInst, nullptr
        );

        // 찾아보기 버튼
        hButtonSetChatPath = CreateWindow
        (
            L"BUTTON", L"찾아보기", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            360, 20, 100, 27, hWnd, (HMENU)IDC_CHATPATH_SET_BUTTON, hInst, nullptr
        );

        hWebhookLabel = CreateWindow
        (
            L"STATIC", L"Webhook URL",
            WS_CHILD | WS_VISIBLE,
            20, 60, 95, 25,
            hWnd, nullptr, hInst, nullptr
        );

        // 웹훅 텍스트 박스
        hEditWebhook = CreateWindowEx
        (
            WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY | ES_AUTOHSCROLL | WS_BORDER,
            120, 60, 255, 27, hWnd, (HMENU)IDC_WEBHOOK_EDIT, hInst, nullptr
        );

        // 등록/변경 버튼
        hButtonSetWebhook = CreateWindow
        (
            L"BUTTON", L"등록", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            400, 60, 60, 27, hWnd, (HMENU)IDC_WEBHOOK_SET_BUTTON, hInst, nullptr
        );
        
        // 초기 webhook 로드
        g_WebhookUrl = TextDetectorApplication::LoadWebhookFromFile();
        if (!g_WebhookUrl.empty())
        {
            SetWindowText(hEditWebhook, g_WebhookUrl.c_str());

            if (g_WebhookUrl.find(L"주소를 등록 해주세요") != std::wstring::npos)
            {
                SetWindowText(hButtonSetWebhook, L"등록");
            }
            SetWindowText(hButtonSetWebhook, L"변경");
        }
        else
        {
            TextDetectorApplication::SaveWebhookFromFile(L"Discord Webhook 주소를 등록 해주세요");
            SetWindowText(hEditWebhook, L"Discord Webhook 주소를 등록 해주세요");
            SetWindowText(hButtonSetWebhook, L"등록");
        }

        // 초기 ChatPath 로드
        g_ChatPath = TextDetectorApplication::LoadChatPathFromFile();
        if (!g_ChatPath.empty())
        {
            SetWindowText(hEditChatPath, g_ChatPath.c_str());
        }

        // 커런시 이미지 강조
        hCheckBoxImage = CreateWindowW
        (
            L"BUTTON", L"with Currency Image",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            20, 400, 155, 25,
            hWnd, (HMENU)IDC_IMAGE_CHECKBOX, hInst, nullptr
        );
        break;


    case WM_SIZE:
    {
        int width = LOWORD(lParam);   // 현재 윈도우 너비
        int height = HIWORD(lParam);  // 현재 윈도우 높이

        // 고정 높이들
        int margin = 20;
        int labelHeight = 25;
        int editHeight = 27;
        int buttonHeight = 27;
        int sectionGap = 15;

        // 채팅 경로 라벨, 에디트, 버튼
        int chatLabelWidth = 105;
        int chatEditWidth = width - 3 * margin - chatLabelWidth - 100; // 100: 버튼 너비
        MoveWindow(hChatLabel, margin, margin, chatLabelWidth, labelHeight, TRUE);
        MoveWindow(hEditChatPath, margin + chatLabelWidth + 5, margin, chatEditWidth, editHeight, TRUE);
        MoveWindow(hButtonSetChatPath, width - margin - 100, margin, 100, buttonHeight, TRUE);

        // 웹훅 라벨, 에디트, 버튼
        int webhookLabelTop = margin + editHeight + sectionGap;
        MoveWindow(hWebhookLabel, margin, webhookLabelTop, chatLabelWidth, labelHeight, TRUE);
        MoveWindow(hEditWebhook, margin + chatLabelWidth + 5, webhookLabelTop, chatEditWidth + 40, editHeight, TRUE);
        MoveWindow(hButtonSetWebhook, width - margin - 60, webhookLabelTop, 60, buttonHeight, TRUE);

        // 시작 버튼
        int startButtonTop = webhookLabelTop + editHeight + sectionGap;
        MoveWindow(hButtonStart, margin, startButtonTop, 100, 30, TRUE);


        // 로그 에디트 박스
        int logEditTop = startButtonTop + 30 + sectionGap;
        int logEditHeight = height - logEditTop - margin * 2;
        if (logEditHeight < 0) logEditHeight = 0;
        MoveWindow(hEditLog, margin, logEditTop, width - 2 * margin, logEditHeight, TRUE);

        // 체크 박스
        MoveWindow(hCheckBoxImage, width - margin - 155, logEditTop + logEditHeight + 5 , 155, 25, TRUE);

        break;
    }
    // 최소 윈도우 크기 설정 (예: 너비 520, 높이 480)
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* pMinMax = (MINMAXINFO*)lParam;

        pMinMax->ptMinTrackSize.x = 280;
        pMinMax->ptMinTrackSize.y = 300;

        return 0; // 꼭 return 0 해주기
    }

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDC_START_BUTTON:
                g_ChatPath = TextDetectorApplication::LoadChatPathFromFile();
                if (g_ChatPath.empty())
                {
                    MessageBox(nullptr,  L"채팅 파일 경로를 설정해주세요.", L"파일 경로 조회 실패", MB_OK);
                    break;
                }

                bIsDetecting = true;
                TextDetectorApplication::IsChatLogUpdated();
                AppendLog(hEditLog, GetCurrentTimestamp() + L" 모니터링 시작!\r\n");
                // 윈도우 제목 변경 (작업표시줄에 보이는 텍스트)
                SetWindowText(hWnd, L"메시지 감지중");

                // 시작 버튼 비활성화
                EnableWindow(hButtonStart, FALSE);

                // 중지 버튼 생성
                hButtonStop = CreateWindow(
                    L"BUTTON", L"중지",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    140, 104, 100, 30,
                    hWnd, (HMENU)IDC_STOP_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr
                );
                break;

            case IDC_STOP_BUTTON:
                bIsDetecting = false;
                TextDetectorApplication::StopRun();
                AppendLog(hEditLog, GetCurrentTimestamp() + L" ========== 모니터링 중단 ==========\r\n");

                // 윈도우 제목 원래대로 복원
                SetWindowText(hWnd, ORIGINAL_TITLE);

                // 시작 버튼 다시 활성화
                EnableWindow(hButtonStart, TRUE);

                // 중지 버튼 제거
                DestroyWindow(hButtonStop);
                hButtonStop = nullptr;
                break;

            case IDC_WEBHOOK_SET_BUTTON:
            {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_WEBHOOK_DIALOG), hWnd, ChildDialogProc);
                break;
            }

            case IDC_CHATPATH_SET_BUTTON:
            {
                OPENFILENAME ofn;
                wchar_t szFile[MAX_PATH] = { 0 };

                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
                ofn.lpstrTitle = L"채팅 로그 텍스트 파일 선택";

                // 기존 작업 디렉토리 저장
                wchar_t currentDir[MAX_PATH];
                GetCurrentDirectory(MAX_PATH, currentDir);

                if (GetOpenFileName(&ofn))
                {
                    // 선택된 경로를 Edit 박스에 표시
                    SetWindowText(hEditChatPath, szFile);

                    // 작업 디렉토리 복원
                    SetCurrentDirectory(currentDir);

                    TextDetectorApplication::SaveChatPathFromFile(szFile);
                }
                break;
            }

            case IDC_IMAGE_CHECKBOX:
            {
                HWND hCheckBox = GetDlgItem(hWnd, IDC_IMAGE_CHECKBOX);
                BOOL bChecked = SendMessage(hCheckBox, BM_GETCHECK, 0, 0) == BST_CHECKED;

                if (bChecked)
                {
                    TextDetectorApplication::bImageMode = true;
                }
                else
                {
                    TextDetectorApplication::bImageMode = false;
                }
                break;
            }
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

// Webhook 등록/변경 창 프로시저
INT_PTR CALLBACK ChildDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        // 기존 Webhook 주소가 있으면 EditBox에 표시
        if (!g_WebhookUrl.empty())
        {
            SetDlgItemText(hDlg, IDC_EDITWEBHOOK_EDIT, g_WebhookUrl.c_str());
        }
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_CHILD_OK:
        {
            wchar_t buffer[512];
            GetDlgItemText(hDlg, IDC_EDITWEBHOOK_EDIT, buffer, 512);
            g_WebhookUrl = buffer;

            // 메인 창의 EditBox와 버튼 텍스트 업데이트
            TextDetectorApplication::SaveWebhookFromFile(g_WebhookUrl);
            SetWindowText(hEditWebhook, g_WebhookUrl.c_str());
            SetWindowText(hButtonSetWebhook, L"변경");

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        case IDCANCEL:  // ESC나 닫기 버튼 눌렀을 때
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;

    case WM_CLOSE:
        EndDialog(hDlg, IDCANCEL);
        return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}