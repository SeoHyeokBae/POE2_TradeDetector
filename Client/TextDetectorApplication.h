#pragma once

#include "framework.h"
#include <windows.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>


typedef void(*LogFuncPtr)(HWND hEdit, const std::wstring&);
typedef std::wstring(*LogTimeFuncPtr)();

class TextDetectorApplication
{
public:
	static void Initialize(HWND Input_hWnd, HWND Input_hEditLog, LogFuncPtr func, LogTimeFuncPtr timefunc);
	static void Run();
	static void LogMessage();
	static void Release();

	// 화면 캡처
	static HBITMAP CaptureScreenToBitmap();
	// 텍스트 추출
	static void DoScreenOCR(cv::Mat image);
	// 디스코드 메세지 전송
	static void SendDiscordMessage(const std::wstring& message);
	// 로그 정리
	static void  CleanLogExceptLatestDetections();
	// Webhook 링크 파일 체크
	static const std::wstring LoadWebhookFromFile();
    // 최근 메세지 도착시간 찾기
    static void GetDeliveryTime(const std::wstring& text, std::wstring& getTime);

    static std::wstring wLastDetectedTime;
    static bool bFirstLogDone;
	static bool bClearLogDone;
	static bool bAlreadySent;

private:
	static HWND hWnd;
	static HWND hEditLog;
	static tesseract::TessBaseAPI* tessAPI;
	// 로그 출력 함수
	static LogFuncPtr LogFunc;
	// 로그 시간 출력 함수
	static LogTimeFuncPtr LogTimeFunc;
};

