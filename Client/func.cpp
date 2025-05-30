#include "pch.h"
#include "func.h"


string ToString(const wstring& _str)
{
    return string(_str.begin(), _str.end());
}

wstring ToWString(const string& _str)
{
    return wstring(_str.begin(), _str.end());
}

void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[])
{
    int nLen = (int)strlen(pstrSrc) + 1;
    mbstowcs(pwstrDest, pstrSrc, nLen);
}

// 한글 변환 (wstring to utf8)
string WStringToUtf8(const std::wstring& wstr)
{
    if (wstr.empty()) return {};

    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(sizeNeeded - 1, 0); // -1 to exclude null terminator
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], sizeNeeded, nullptr, nullptr);
    return result;
}

string EscapeJsonString(const std::string& input)
{
    std::string output;
    for (char c : input)
    {
        switch (c)
        {
        case '\"': output += "\\\""; break;
        case '\\': output += "\\\\"; break;
        case '\b': output += "\\b";  break;
        case '\f': output += "\\f";  break;
        case '\n': output += "\\n";  break;
        case '\r': output += "\\r";  break;
        case '\t': output += "\\t";  break;
        default:
            // UTF-8은 그냥 추가 (JSON에서 유효한 UTF-8로 가정)
            output += c;
        }
    }
    return output;
}

void SaveWString(const wstring& _str, FILE* _File)
{
    size_t len = _str.length();
    fwrite(&len, sizeof(size_t), 1, _File);
    fwrite(_str.c_str(), sizeof(wchar_t), _str.length(), _File);
}

void LoadWString(wstring& _str, FILE* _File)
{
    size_t len = 0;
    wchar_t szBuff[256] = {};
    fread(&len, sizeof(size_t), 1, _File);
    fread(szBuff, sizeof(wchar_t), len, _File);
    _str = szBuff;
}
