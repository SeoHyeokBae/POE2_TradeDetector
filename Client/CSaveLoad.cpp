#include "pch.h"
#include "CSaveLoad.h"


wchar_t CSaveLoad::g_szContent[255] = {};

//wstring CSaveLoad::GetRelativePath(const wstring& _FullPath)
//{
//    wstring strContentPath = GetContentPath();
//
//    // 입력된 경로에, content 경로가 없으면 실패
//    if (-1 == _FullPath.find(strContentPath, 0))
//        return nullptr;
//
//    // Content 경로 끝에서 문자열 끝까지를 잘라서 반환
//    wstring strRelativePath = _FullPath.substr(strContentPath.length(), _FullPath.length());
//
//    return strRelativePath;
//}