#pragma once

#include "framework.h"
#include <windows.h>
#include <string>

typedef void(*LogFuncPtr)(HWND hEdit, const std::wstring&);
typedef std::wstring(*LogTimeFuncPtr)();

class TextDetectorApplication
{
public:
	static void Initialize(HWND Input_hWnd, HWND Input_hEditLog, LogFuncPtr func, LogTimeFuncPtr timefunc);
	static void Run();

	// 디스코드 메세지 전송
	static bool SendDiscordMessage(const std::wstring& price, const std::wstring& item, const std::wstring& sender);
	// Webhook 링크 파일 체크
	static const std::wstring LoadWebhookFromFile();
	static void SaveWebhookFromFile(const std::wstring& url);

    // Webhook 링크 파일 체크
    static const std::wstring LoadChatPathFromFile();
	static void SaveChatPathFromFile(const std::wstring& path);
    // Dir 감지 핸들관리
    static bool IsChatLogUpdated();
    static void StopRun();
    // 채팅 내용 파씽
    static void ParseTradeMessage(const std::wstring& message);

    static bool bImageMode;
private:
	static HWND hWnd;
	static HWND hEditLog;
	// 로그 출력 함수
	static LogFuncPtr LogFunc;
	// 로그 시간 출력 함수
	static LogTimeFuncPtr LogTimeFunc;

    static wstring webhookurl;
    static wstring chatlog_path;
    static wstring LastLine;
};

