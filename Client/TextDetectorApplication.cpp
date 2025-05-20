#include "pch.h"
#include "TextDetectorApplication.h"
#include "func.h"
#include <curl/curl.h>
#include <fstream>
#include <sstream>
#include <string>

HWND TextDetectorApplication::hWnd = NULL;
HWND TextDetectorApplication::hEditLog = NULL;
LogFuncPtr TextDetectorApplication::LogFunc = nullptr;
LogTimeFuncPtr TextDetectorApplication::LogTimeFunc = nullptr;

std::wstring TextDetectorApplication::webhookurl= L"";
std::wstring TextDetectorApplication::chatlog_path = L"";
std::wstring TextDetectorApplication::LastLine = L"";

bool TextDetectorApplication::bImageMode = false;

std::atomic<bool> g_bIsTaskRunning = false;
std::future<void> g_futureTask;

void TextDetectorApplication::Initialize(HWND Input_hWnd, HWND Input_hEditLog, LogFuncPtr func, LogTimeFuncPtr timefunc)
{
    hWnd = Input_hWnd;
    hEditLog = Input_hEditLog;
    LogFunc = func;
    LogTimeFunc = timefunc;
}

void TextDetectorApplication::Run()
{
    if (g_bIsTaskRunning.exchange(true))
        return; // 이미 작업 중이면 바로 종료

    g_futureTask = std::async(std::launch::async, [&]()
        {
            // RAII 스타일로 무조건 끝나면 플래그 해제
            struct AutoFlagClear
            {
                std::atomic<bool>& flag;
                AutoFlagClear(std::atomic<bool>& f) : flag(f) {}
                ~AutoFlagClear() { flag.store(false); }
            } clearTaskFlag(g_bIsTaskRunning);

            if (IsChatLogUpdated())
            {
                if (LastLine.find(L"구매하고 싶습니다") != std::wstring::npos
                    && LastLine.find(L"@수신") != std::wstring::npos)
                {
                    ParseTradeMessage(LastLine);
                }
            }
            g_bIsTaskRunning.store(false);
        });
}

void TextDetectorApplication::ParseTradeMessage(const std::wstring& message)
{
    wstring sender, price, item;

    // 1. 가격
    size_t priceStart = message.find(L"안녕하세요, ");
    if (priceStart != std::wstring::npos)
    {
        priceStart += 7; // "안녕하세요, " 다음부터
        size_t priceEnd = message.find(L"(으)로", priceStart);
        if (priceEnd != std::wstring::npos)
        {
            price = message.substr(priceStart, priceEnd - priceStart);
        }
    }

    // 2. 아이템명
    size_t itemStart = message.find(L"리그의 ");
    if (itemStart != std::wstring::npos)
    {
        itemStart += 4; // "리그의 " 다음부터
        size_t itemEnd = message.find(L"(을)를 구매하고 싶습니다", itemStart);
        if (itemEnd != std::wstring::npos)
        {
            item = message.substr(itemStart, itemEnd - itemStart);
        }
    }

    // 3. 발신자
    size_t senderStart = message.find(L"@수신 ");
    if (senderStart != std::wstring::npos)
    {
        senderStart += 4; // "@수신 " 다음부터
        size_t senderEnd = message.find(L":", senderStart);
        if (senderEnd != std::wstring::npos)
        {
            sender = message.substr(senderStart, senderEnd - senderStart);
        }
    }

    // 로그 메시지 문구
    wstring LogMessage = L" | " + price + L" | " + item + L" (@" + sender + L")";

    // 디스코드 메시지 보내기
    if(SendDiscordMessage(price, item, sender))
    {
        LogFunc(hEditLog, LogTimeFunc() + L" 전송됨: " + LogMessage + L"\r\n");
    }
    else
    {
        LogFunc(hEditLog, LogTimeFunc() + L" 전송실패: " + LogMessage + L"\r\n");
    }
}

bool TextDetectorApplication::SendDiscordMessage(const std::wstring& price, const std::wstring& item, const std::wstring& sender)
{
    wstring message = L"==========================\n가격　: " + price + L"\n"
                                                     + L"아이템: " + item + L"\n"
                                                     + L"구매자: @" + sender + L"\n"
                     + L"==========================";

    CURL* curl = curl_easy_init();
    if (!curl) return false;

    // text채널 설정에서 웹훅 url
    const std::string webhook_url = ToString(webhookurl);
    const char* url = webhook_url.c_str();

    string utf8_message = WStringToUtf8(message);
    string escaped_message = EscapeJsonString(utf8_message);
    string json_data = "{\"content\":\"" + escaped_message + "\"}";

    struct curl_httppost* post = nullptr;
    struct curl_httppost* last = nullptr;

    // JSON 메시지 추가
    curl_formadd(&post, &last,
        CURLFORM_COPYNAME, "payload_json",
        CURLFORM_COPYCONTENTS, json_data.c_str(),
        CURLFORM_END);

    if (bImageMode)
    {
        // 주요 3종 커런시 
        if (price.find(L"divine") != std::wstring::npos)
        {
            curl_formadd(&post, &last,
                CURLFORM_COPYNAME, "file",
                CURLFORM_FILE, "Orb_Images\\divine.png",
                CURLFORM_FILENAME, "divine.png",
                CURLFORM_CONTENTTYPE, "image/png",
                CURLFORM_END);
        }
        else if (price.find(L"chaos") != std::wstring::npos)
        {
            curl_formadd(&post, &last,
                CURLFORM_COPYNAME, "file",
                CURLFORM_FILE, "Orb_Images\\chaos.png",
                CURLFORM_FILENAME, "chaos.png",
                CURLFORM_CONTENTTYPE, "image/png",
                CURLFORM_END);
        }
        else if (price.find(L"exalted") != std::wstring::npos)
        {
            curl_formadd(&post, &last,
                CURLFORM_COPYNAME, "file",
                CURLFORM_FILE, "Orb_Images\\exalted.png",
                CURLFORM_FILENAME, "exalted.png",
                CURLFORM_CONTENTTYPE, "image/png",
                CURLFORM_END);
        }
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);

    // 메세지 전송 오류출력
    CURLcode res = curl_easy_perform(curl);
    const char* errMsg = curl_easy_strerror(res);
    if (res != CURLE_OK)
    {
        wchar_t wideMsg[512];
        swprintf_s(wideMsg, 512, L"Discord 메세지 전송 실패\n오류코드: %d\n설명: %S", res, errMsg);
        MessageBox(nullptr, wideMsg, L"Discord 메세지 전송 실패", MB_OK);

        return false;
    }

    curl_easy_cleanup(curl);

    return true;
}

const std::wstring TextDetectorApplication::LoadChatPathFromFile()
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, L"path&url.txt", L"rt, ccs=UTF-8");

    if (!pFile) return L"";

    std::wstring chatPath;
    wchar_t szBuff[1024] = {};
    if (fgetws(szBuff, sizeof(szBuff) / sizeof(wchar_t), pFile))
    {
        size_t len = wcslen(szBuff);
        if (len > 0 && szBuff[len - 1] == L'\n')
        {
            szBuff[len - 1] = L'\0';
        }
        chatPath = szBuff;
    }
    else
    {
        chatPath = L"";
    }

    chatlog_path = chatPath;
    fclose(pFile);
    return chatPath;
}

const std::wstring TextDetectorApplication::LoadWebhookFromFile()
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, L"path&url.txt", L"rt, ccs=UTF-8");
    if (!pFile) return L"";

    wchar_t szBuff[1024] = {};
    std::wstring webhook;

    // 첫 줄은 ChatPath, 두 번째 줄을 읽음
    fgetws(szBuff, sizeof(szBuff) / sizeof(wchar_t), pFile); // Skip line 1

    if (fgetws(szBuff, sizeof(szBuff) / sizeof(wchar_t), pFile))
    {
        size_t len = wcslen(szBuff);
        if (len > 0 && szBuff[len - 1] == L'\n') szBuff[len - 1] = L'\0';
        webhook = szBuff;
    }

    webhookurl = webhook;
    fclose(pFile);
    return webhook;
}

void TextDetectorApplication::SaveWebhookFromFile(const std::wstring& url)
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, L"path&url.txt", L"w, ccs=UTF-8");
    if (!pFile) 
    { 
        MessageBox(nullptr, L"URL 조회 실패", L"path&url.txt 파일 생성 실패", MB_OK);
        fclose(pFile);
        return;
    }

    webhookurl = url;
    wstring chatpath = chatlog_path + L"\n";
    fputws(chatpath.c_str(), pFile);
    fputws(url.c_str(), pFile);
    fclose(pFile);
}

void TextDetectorApplication::SaveChatPathFromFile(const std::wstring& path)
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, L"path&url.txt", L"w, ccs=UTF-8");
    if (!pFile)
    {
        MessageBox(nullptr, L"ChatPath 조회 실패", L"path&url.txt 파일 생성 실패", MB_OK);
        fclose(pFile);
        return;
    }
    chatlog_path = path;
    wstring chatpath = chatlog_path + L"\n";
    fputws(chatpath.c_str(), pFile);
    fputws(webhookurl.c_str(), pFile);
    fclose(pFile);
}

bool TextDetectorApplication::IsChatLogUpdated()
{
    if (chatlog_path == L"")
    {
        MessageBox(nullptr, L"오류", L"디렉터리 경로 없음", MB_OK);
        return false;
    }

    HANDLE hFile = CreateFileW(chatlog_path.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE)
        return false;

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(hFile, &fileSize) || fileSize.QuadPart == 0)
    {
        CloseHandle(hFile);
        return false;
    }

    const DWORD readSize = (DWORD)min(fileSize.QuadPart, 8192);
    LARGE_INTEGER readOffset;
    readOffset.QuadPart = fileSize.QuadPart - readSize;
    SetFilePointerEx(hFile, readOffset, NULL, FILE_BEGIN);

    std::vector<char> buffer(readSize + 1, 0);
    DWORD bytesRead = 0;
    BOOL readResult = ReadFile(hFile, buffer.data(), readSize, &bytesRead, NULL);
    CloseHandle(hFile);

    if (!readResult || bytesRead == 0)
        return false;

    // 2. UTF-8 -> UTF-16 변환할 길이 계산
    int wideCharCount = MultiByteToWideChar(
        CP_UTF8,
        0,
        buffer.data(),
        bytesRead,
        nullptr,
        0);

    if (wideCharCount == 0)
        return false;

    // 3. wstring 생성용 버퍼 할당 후 변환
    std::wstring content(wideCharCount, 0);
    MultiByteToWideChar(
        CP_UTF8,
        0,
        buffer.data(),
        bytesRead,
        &content[0],
        wideCharCount);


    std::wstringstream ss(content);
    std::wstring line, streamlastLine;

    while (std::getline(ss, line))
    {
        if (!line.empty() && (line.find(L"이 플레이어는 자리비움 상태입니다") == std::wstring::npos))
            streamlastLine = line;
    }

    if (streamlastLine.empty())
    {
        return false;
    }

    // 첫 시작 버튼 예외
    if (LastLine.empty() && !streamlastLine.empty())
    {
        LastLine = streamlastLine;
        return false;
    }

    // 변화o
    if (LastLine != streamlastLine)
    {
        LastLine = streamlastLine;
        return true;
    }

    return false;
}

void TextDetectorApplication::StopRun()
{
    LastLine = L"";
    g_bIsTaskRunning.store(false);
}