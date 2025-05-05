#pragma once
class CSaveLoad
{
public:
    static const wchar_t* GetContentPath() { return g_szContent; }
    static std::wstring GetRelativePath(const std::wstring& _FullPath);
    
    //static void SaveDiscordURL(CLevel* _Level, const std::wstring& _strLevelPath);
    //static void LoadDiscordURL(CLayer* _Layer, FILE* _File);

private:
    static wchar_t g_szContent[255];
};

