#include "pch.h"
#include "TextDetectorApplication.h"
#include "func.h"
#include <curl/curl.h>
#include <leptonica/allheaders.h>

HWND TextDetectorApplication::hWnd = NULL;
HWND TextDetectorApplication::hEditLog = NULL;
LogFuncPtr TextDetectorApplication::LogFunc = nullptr;
LogTimeFuncPtr TextDetectorApplication::LogTimeFunc = nullptr;

tesseract::TessBaseAPI* TextDetectorApplication::tessAPI = nullptr;
std::wstring TextDetectorApplication::wLastDetectedTime = L"";
bool TextDetectorApplication::bFirstLogDone = false;
bool TextDetectorApplication::bClearLogDone = false;
bool TextDetectorApplication::bAlreadySent = false;

std::wstring TextDetectorApplication::webhookurl= L"";


void TextDetectorApplication::Initialize(HWND Input_hWnd, HWND Input_hEditLog, LogFuncPtr func, LogTimeFuncPtr timefunc)
{
    hWnd = Input_hWnd;
    hEditLog = Input_hEditLog;
    LogFunc = func;
    LogTimeFunc = timefunc;

    // Tesseract API 초기화
    // 실패시 1반환 -> 메세지 출력
    tessAPI = new tesseract::TessBaseAPI();
    if (tessAPI->Init("C:/Program Files/Tesseract-OCR/tessdata", "kor+eng"))
    {
        MessageBox(nullptr, L"Tesseract 초기화 실패", L"Tesseract 초기화 실패", MB_OK);
        return;
    }
}

cv::Mat HBitmapToMat(HBITMAP hBitmap)
{
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    BITMAPINFOHEADER bi;
    ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmp.bmWidth/2;
    bi.biHeight = -bmp.bmHeight; // OpenCV는 top-down
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;

    cv::Mat mat(bmp.bmHeight, bmp.bmWidth/2, CV_8UC3);
    HDC hDC = GetDC(NULL);
    GetDIBits(hDC, hBitmap, 0, bmp.bmHeight, mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    ReleaseDC(NULL, hDC);

    return mat;
}

void TextDetectorApplication::Run()
{
    LogMessage();

    static DWORD dwLastCaptureTime = 0;
    DWORD dwNow = GetTickCount64();

    if (dwLastCaptureTime == 0) dwLastCaptureTime = dwNow;

    if (dwNow - dwLastCaptureTime >= 5000) // 5초마다 캡처
    {
        bAlreadySent = false;
        dwLastCaptureTime = dwNow;
        HBITMAP CaptureBMP = CaptureScreenToBitmap(); // 캡처 실행
        
        // cv::Mat 변환
        cv::Mat Image = HBitmapToMat(CaptureBMP);

        DoScreenOCR(Image);

        DeleteObject(CaptureBMP);
    }
}

HBITMAP TextDetectorApplication::CaptureScreenToBitmap()
{
    // 전체 화면 크기 가져오기
    int screenX = GetSystemMetrics(SM_CXSCREEN);
    int screenY = GetSystemMetrics(SM_CYSCREEN);

    // 디바이스 컨텍스트 생성
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

    // 비트맵 생성
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, screenX, screenY);
    SelectObject(hMemoryDC, hBitmap);

    // 비트맵에 화면 내용 복사
    BitBlt(hMemoryDC, 0, 0, screenX, screenY, hScreenDC, 0, 0, SRCCOPY);

    // 정리
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);

    return hBitmap;
}

void TextDetectorApplication::SendDiscordMessage(const std::wstring& message)
{
    CURL* curl = curl_easy_init();
    if (!curl) return;

    // text채널 설정에서 웹훅 url
    //const std::string webhook_url = "https://discord.com/api/webhooks/1367564424298233999/F-I5tGFqM8suQ9m64yNicJao8PGDK_Q2STvR79QRK00IL78yY3S-Lbaa9RF8nDA_CWeF";
    const std::string webhook_url = ToString(webhookurl);
    const char* url = webhook_url.c_str();

    std::string utf8_message = WStringToUtf8(message);
    std::string json_data = "{\"content\":\"" + utf8_message + "\"}";

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");


    struct curl_httppost* post = nullptr;
    struct curl_httppost* last = nullptr;

    // JSON 메시지 추가
    curl_formadd(&post, &last,
        CURLFORM_COPYNAME, "payload_json",
        CURLFORM_COPYCONTENTS, json_data.c_str(),
        CURLFORM_END);

    // 이미지 파일 첨부 (multipart name="file" 필드로 사용됨)
    curl_formadd(&post, &last,
        CURLFORM_COPYNAME, "file",
        CURLFORM_FILE, "screenshot.png",
        CURLFORM_FILENAME, "screenshot.png",
        CURLFORM_CONTENTTYPE, "image/png",
        CURLFORM_END);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);

    // 메세지 전송 오류출력
    CURLcode res = curl_easy_perform(curl);
    const char* errMsg = curl_easy_strerror(res);
    if (res != CURLE_OK)
    {
        //wchar_t buffer[256];
        //swprintf_s(buffer, 256, L"Discord 메세지 전송 실패\n오류코드: %d", static_cast<int>(res));
        //MessageBox(nullptr, buffer, L"Discord 메세지 전송 실패", MB_OK);
        wchar_t wideMsg[512];
        swprintf_s(wideMsg, 512, L"Discord 메세지 전송 실패\n오류코드: %d\n설명: %S", res, errMsg);
        MessageBox(nullptr, wideMsg, L"Discord 메세지 전송 실패", MB_OK);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

void TextDetectorApplication::DoScreenOCR(cv::Mat image)
{
    tessAPI->SetImage(image.data, image.cols, image.rows, 3, image.step);

    char* outText = tessAPI->GetUTF8Text();

    int utf8Len = MultiByteToWideChar(CP_UTF8, 0, outText, -1, nullptr, 0);
    std::wstring wText(utf8Len, 0);
    MultiByteToWideChar(CP_UTF8, 0, outText, -1, &wText[0], utf8Len);

    if (!bAlreadySent && wText.find(L"구매") != std::wstring::npos)
    {
        // 최근 메세지 시간 받아오기
        std::wstring latestTime;
        GetDeliveryTime(wText, latestTime);

        if (!latestTime.empty() && latestTime != wLastDetectedTime)
        {
            // 최근 메세지 시간 저장
            wLastDetectedTime = latestTime;

            if (LogFunc && LogTimeFunc)
            {
                LogFunc(hEditLog, LogTimeFunc() + L" 감지됨: 구매자 발견!\r\n");
            }

            // 캡처이미지 저장
            cv::imwrite("screenshot.png", image);
            // 디스코드 메세지 전송
            SendDiscordMessage(L"감지됨: 구매하고 싶습니다!");
            bAlreadySent = true;
        }
    }
}

void TextDetectorApplication::LogMessage()
{
    static DWORD dwLastLogTime = 0;
    static DWORD dwLastLogClearTime = 0;

    DWORD dwNow = GetTickCount64();

    if (!bFirstLogDone)
    {
        if (LogFunc) LogFunc(hEditLog, LogTimeFunc() + L" 실시간 감지중 ...\r\n");
        bFirstLogDone = true;
        dwLastLogTime = dwNow;
        dwLastLogClearTime = dwNow;
    }
    else
    {
        if (dwNow - dwLastLogTime >= 1200000) // 20분 = 1200000ms
        {
            if (LogFunc) LogFunc(hEditLog, LogTimeFunc() + L" 실시간 감지중 ...\r\n");
            dwLastLogTime = dwNow;
        }

        if (dwNow - dwLastLogClearTime >= 10800000) // 3시간 뒤 로그 정리
        {
            dwLastLogClearTime = dwNow;
            CleanLogExceptLatestDetections();
        }
    }
}

void TextDetectorApplication::CleanLogExceptLatestDetections()
{
    if (!hEditLog) return;

    const int bufferSize = 32768;
    wchar_t buffer[bufferSize] = { 0 };
    GetWindowText(hEditLog, buffer, bufferSize);

    std::wstring allText = buffer;
    std::vector<std::wstring> lines;
    size_t pos = 0;
    while ((pos = allText.find(L"\r\n")) != std::wstring::npos)
    {
        lines.push_back(allText.substr(0, pos));
        allText.erase(0, pos + 2);
    }
    if (!allText.empty())
        lines.push_back(allText); // 마지막 줄

    // 마지막 "감지 시작", 마지막 "실시간 감지중"을 찾기
    std::wstring lastStartLine, lastFoundLine, lastDetectingLine;
    for (auto it = lines.rbegin(); it != lines.rend(); ++it)
    {
        if (lastStartLine.empty() && it->find(L"감지 시작") != std::wstring::npos)
            lastStartLine = *it;

        if (lastFoundLine.empty() && it->find(L"감지됨") != std::wstring::npos)
            lastFoundLine = *it;

        if (lastDetectingLine.empty() && it->find(L"실시간 감지중") != std::wstring::npos)
            lastDetectingLine = *it;

        if (!lastStartLine.empty() && !lastDetectingLine.empty())
            break;
    }

    // 새로운 로그 구성
    std::wstring newLog;
    if (!lastStartLine.empty())
        newLog += lastStartLine + L"\r\n";
    if (!lastFoundLine.empty())
        newLog += lastFoundLine + L"\r\n";
    if (!lastDetectingLine.empty())
        newLog += lastDetectingLine + L"\r\n";

    // 로그 업데이트
    SetWindowText(hEditLog, newLog.c_str());
}

const std::wstring TextDetectorApplication::LoadWebhookFromFile()
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, L"webhookurl.txt", L"rt");

    if (!pFile) { fclose(pFile); return L"";}

    wstring URL;
    wchar_t szBuff[1024] = {};
    if (fgetws(szBuff, sizeof(szBuff) / sizeof(wchar_t), pFile))
    {
        size_t len = wcslen(szBuff);
        if (len > 0 && szBuff[len - 1] == L'\n') 
        {
            szBuff[len - 1] = L'\0';
        }
        URL = szBuff;
    }
    else 
    { 
        URL = L""; 
    }

    fclose(pFile);

    return URL;
}

void TextDetectorApplication::SaveWebhookFromFile(const std::wstring& url)
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, L"webhookurl.txt", L"w");
    if (!pFile) 
    { 
        MessageBox(nullptr, L"URL 조회 실패", L"webhookurl.txt 파일 생성 실패", MB_OK);
        fclose(pFile);
        return;
    }
    webhookurl = url;
    fputws(url.c_str(), pFile);
    fclose(pFile);
}

void TextDetectorApplication::GetDeliveryTime(const std::wstring& text, std::wstring& getTime)
{
    if (text.empty())
    {
        MessageBox(nullptr, L"TEXT 추출 실패", L"TEXT 내용 없음", MB_OK);
        return ;
    }

    // 정규식을 이용해 [시:분] 형식의 문자열 찾기
    std::wregex timeRegex(LR"(\[(\d{1,2}):(\d{2})\])");
    std::wsregex_iterator iter(text.begin(), text.end(), timeRegex);
    std::wsregex_iterator end;

    int maxHour = -1, maxMinute = -1;

    for (; iter != end; ++iter)
    {
        int hour = std::stoi((*iter)[1]);
        int minute = std::stoi((*iter)[2]);

        // 시간 비교
        if (hour > maxHour || (hour == maxHour && minute > maxMinute))
        {
            maxHour = hour;
            maxMinute = minute;
            getTime = iter->str(); // [시:분] 전체 문자열
        }
    }
}

void TextDetectorApplication::Release()
{
    tessAPI->End();
}
