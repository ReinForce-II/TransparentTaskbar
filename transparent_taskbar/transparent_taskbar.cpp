// transparent_taskbar.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "transparent_taskbar.h"

enum AccentState
{
	ACCENT_DISABLED = 4, // Disables TTB for that taskbar
	ACCENT_ENABLE_GRADIENT = 1, // Makes the taskbar a solid color specified by nColor. This mode doesn't care about the alpha channel.
	ACCENT_ENABLE_TRANSPARENTGRADIENT = 2, // Makes the taskbar a tinted transparent overlay. nColor is the tint color, sending nothing results in it interpreted as 0x00000000 (totally transparent, blends in with desktop)
	ACCENT_ENABLE_BLURBEHIND = 3, // Makes the taskbar a tinted blurry overlay. nColor is same as above.
	ACCENT_ENABLE_TINTED = 5, // This is not a real state. We will handle it later.
	ACCENT_NORMAL_GRADIENT = 6, // Another fake value, handles the 
};

enum AccentFlags {
	DrawLeftBorder = 0x20,
	DrawTopBorder = 0x40,
	DrawRightBorder = 0x80,
	DrawBottomBorder = 0x100
};

typedef struct
{
	int nAccentState;
	int nFlags;
	int nColor;
	int nAnimationId;
}ACCENTPOLICY;

typedef struct
{
	int nAttribute;
	PVOID pData;
	ULONG ulDataSize;
}WINCOMPATTRDATA;

void SetWindowBlur(HWND hWnd)
{
	const HINSTANCE hModule = LoadLibrary(TEXT("user32.dll"));
	if (hModule)
	{
		typedef BOOL(WINAPI*pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);
		const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hModule, "SetWindowCompositionAttribute");
		if (SetWindowCompositionAttribute)
		{
			ACCENTPOLICY policy = { ACCENT_ENABLE_TRANSPARENTGRADIENT, 0, 0x60000000, 0 };
			WINCOMPATTRDATA data = { 19, &policy, sizeof(ACCENTPOLICY) };
			SetWindowCompositionAttribute(hWnd, &data);
		}
		FreeLibrary(hModule);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	while (true) {
		HWND taskbar = FindWindow(L"Shell_TrayWnd", NULL);
		HWND startmenu = FindWindow(L"Windows.UI.Core.CoreWindow", L"Start");
		SetWindowBlur(taskbar);
		SetWindowBlur(startmenu);
		Sleep((DWORD)100);
	}
}
