#include "pch.h"
#include "CSaveLoad.h"


wchar_t CSaveLoad::g_szContent[255] = {};

//wstring CSaveLoad::GetRelativePath(const wstring& _FullPath)
//{
//    wstring strContentPath = GetContentPath();
//
//    // �Էµ� ��ο�, content ��ΰ� ������ ����
//    if (-1 == _FullPath.find(strContentPath, 0))
//        return nullptr;
//
//    // Content ��� ������ ���ڿ� �������� �߶� ��ȯ
//    wstring strRelativePath = _FullPath.substr(strContentPath.length(), _FullPath.length());
//
//    return strRelativePath;
//}