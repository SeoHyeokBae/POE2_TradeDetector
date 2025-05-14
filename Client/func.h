#pragma once

string ToString(const wstring& _str);
wstring ToWString(const string& _str);
void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[]);
string WStringToUtf8(const std::wstring& wstr);
string EscapeJsonString(const std::string& input);

void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _File);



