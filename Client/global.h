#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <Windows.h>
#include <typeinfo>
#include <iostream>
#include <vector>
#include <regex>
#include <thread>
#include <future>
#include <atomic>

using std::wstring;
using std::string;

#include <wrl.h>
using namespace Microsoft::WRL;

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
using namespace std::experimental;
using namespace std::experimental::filesystem;
using std::experimental::filesystem::path;

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")