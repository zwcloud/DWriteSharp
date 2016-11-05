#pragma region debug functions
#include <list>
#include <windows.h>
#include <iostream>
#include <fstream>


void _DebugAssert(bool expression, const char *Text, ...);
void _DebugAssert(bool expression, const wchar_t *Text, ...);
void _DebugPrintf(const char *Text, ...);
void _DebugPrintf(const wchar_t *Text, ...);

#define PRINT_TO_FILE 0

#if PRINT_TO_FILE
extern std::ofstream out;
#endif

#ifdef _DEBUG
#define DebugAssert(condition,text,...) if(!(condition)){_DebugAssert((condition), (text), __VA_ARGS__); DebugBreak();}
#define DebugPrintf(text,...) _DebugPrintf((text), __VA_ARGS__)
#else
#define DebugAssert(condition,text,...) 
#define DebugPrintf(text,...) 
#endif

#pragma region debug functions
inline void _DebugAssert(bool expression, const char *Text, ...)
{
	if (expression)
	{
		return;
	}
	char CaptionText[12];
	char ErrorText[2048];
	va_list valist;

	strcpy_s(CaptionText, 11, "Error");

	// Build variable text buffer
	va_start(valist, Text);
	vsprintf_s(ErrorText, 1900, Text, valist);
	va_end(valist);

	// Display the message box
	MessageBoxA(NULL, ErrorText, CaptionText, MB_OK | MB_ICONEXCLAMATION);

#if PRINT_TO_FILE
	out << ErrorText;
#else
	OutputDebugStringA(ErrorText);
#endif
}

inline void _DebugAssert(bool expression, const wchar_t *Text, ...)
{
	if (expression)
	{
		return;
	}
	wchar_t CaptionText[12];
	wchar_t ErrorText[2048];
	va_list valist;

	wcscpy_s(CaptionText, 11, L"Error");

	// Build variable text buffer
	va_start(valist, Text);
	vswprintf_s(ErrorText, 2000, Text, valist);
	va_end(valist);

	// Display the message box
	MessageBoxW(NULL, ErrorText, CaptionText, MB_OK | MB_ICONEXCLAMATION);
#if PRINT_TO_FILE
	out << ErrorText;
#else
	OutputDebugStringW(ErrorText);
#endif

	DebugBreak();
}

inline void _DebugPrintf(const char *Text, ...)
{
	char _Text[2048];
	va_list valist;

	// Build variable text buffer
	va_start(valist, Text);
	vsprintf_s(_Text, 2000, Text, valist);
	va_end(valist);

#if PRINT_TO_FILE
	out << _Text;
#else
	// write to debug console
	OutputDebugStringA(_Text);
#endif
}

inline void _DebugPrintf(const wchar_t *Text, ...)
{
	wchar_t _Text[2048];
	va_list valist;

	// Build variable text buffer
	va_start(valist, Text);
	vswprintf_s(_Text, 2000, Text, valist);
	va_end(valist);

#if PRINT_TO_FILE
	out << _Text;
#else
	// write to debug console
	OutputDebugStringW(_Text);
#endif
}
#pragma endregion