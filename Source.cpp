#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "shlwapi")

#include <windows.h>
#include <shlwapi.h>

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButton1;
	static HWND hButton2;
	static TCHAR szINIFilePath[MAX_PATH];
	TCHAR szText[256];
	switch (msg)
	{
	case WM_CREATE:
		// INIファイルのパスを作成（exeの隣にする）
		GetModuleFileName(((LPCREATESTRUCT)lParam)->hInstance, szINIFilePath, MAX_PATH);
		PathRemoveExtension(szINIFilePath);
		PathAddExtension(szINIFilePath, TEXT(".ini"));
		// ボタンの作成
		hButton1 = CreateWindow(TEXT("BUTTON"), TEXT("INIファイル書き込み"),
			WS_VISIBLE | WS_CHILD, 10, 10, 256, 32, hWnd, (HMENU)100,
			((LPCREATESTRUCT)lParam)->hInstance, 0);
		hButton2 = CreateWindow(TEXT("BUTTON"), TEXT("INIファイル読み込み"),
			WS_VISIBLE | WS_CHILD, 10, 50, 256, 32, hWnd, (HMENU)101,
			((LPCREATESTRUCT)lParam)->hInstance, 0);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == 100) // INIファイル書き込み
		{
			// 文字列をINIファイルに保存
			WritePrivateProfileString(TEXT("APP"), TEXT("VALUE1"), TEXT("STRING1"), szINIFilePath);
			// 整数値をINIファイルに保存（実際には数値を書き込むAPIは無いので文字列に変換して保存）
			wsprintf(szText, TEXT("%d"), 123);
			WritePrivateProfileString(TEXT("APP"), TEXT("VALUE2"), szText, szINIFilePath);
		}
		else if (LOWORD(wParam) == 101) // INIファイル読み込み
		{
			// INIファイルから文字列を取得
			GetPrivateProfileString(TEXT("APP"), TEXT("VALUE1"), TEXT("DEFAULT"), szText, 256, szINIFilePath);
			MessageBox(hWnd, szText, TEXT("確認"), 0);
			// INIファイルから整数値を取得
			const int nValue = GetPrivateProfileInt(TEXT("APP"), TEXT("VALUE2"), -1, szINIFilePath);
			wsprintf(szText, TEXT("%d"), nValue);
			MessageBox(hWnd, szText, TEXT("確認"), 0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Window"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
