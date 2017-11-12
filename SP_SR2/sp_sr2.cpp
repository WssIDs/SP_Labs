#include <Windows.h>
#include <tchar.h>

//-- Prototypes -------------------
LRESULT CALLBACK Pr2_WndProc(HWND, UINT, WPARAM, LPARAM);


//-- Global Variables ------------ 
LPTSTR g_lpszClassName = TEXT("sp_pr2_class");
LPTSTR g_lpszAplicationTitle = TEXT("�����������: �������� �������, 60331-1");
LPTSTR g_lpszDestroyTitle = TEXT("������� 1");
LPTSTR g_lpszDestroyMessage = TEXT("������ ����� �������� � ����������� ��������� WM_DESTROY. ��������� ��������� �� Windows � ����� � ����������� ����.");

//  ��������� ������� 
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	MSG msg;
	HWND hWnd;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = g_lpszClassName;
	wc.lpfnWndProc = Pr2_WndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	HBRUSH hbr;
	hbr = CreateSolidBrush(RGB(255, 0, 0));
	wc.hbrBackground = hbr;

	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("������ ����������� ������ ����!"),
			TEXT("������"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	DWORD Style;
	Style = WS_OVERLAPPEDWINDOW ^ WS_MINIMIZEBOX;

	hWnd = CreateWindowEx(NULL, wc.lpszClassName,
		g_lpszAplicationTitle,
		Style,
		200,
		200,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL, TEXT("���� �� �������!"),
			TEXT("������"), MB_OK | MB_ICONERROR);
		return FALSE;
	}


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

// ������� ���������
LRESULT CALLBACK Pr2_WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit;

#define IDC_EDIT		30100 

	HDC hDC;

	switch (msg)
	{
	case WM_CREATE:
	{
		MessageBox(hWnd, TEXT("����������� ��������� WM_CREATE"), g_lpszDestroyTitle, MB_OK | MB_ICONEXCLAMATION);

		RECT rc;
		GetClientRect(hWnd, &rc);

		hEdit = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VSCROLL | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_AUTOHSCROLL, rc.left, rc.top, rc.right, rc.bottom, hWnd, (HMENU)IDC_EDIT, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		SetFocus(hEdit);
	}
	return 0;

	case WM_PAINT:    // ����� ��� ���������� ����
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps); // ��������� ��������� ��� ���������� ����
		RECT rc;
		GetClientRect(hWnd, &rc);

		SetWindowPos(hEdit, HWND_TOP, rc.left, rc.top, rc.right, rc.bottom, NULL);

		EndPaint(hWnd, &ps); // ���������� ���������� ����
	}
	return 0;

	case WM_DESTROY:  // ���������� ������ ����������
	{
		MessageBox(hWnd, g_lpszDestroyMessage, g_lpszDestroyTitle, MB_OK | MB_ICONEXCLAMATION);

		PostQuitMessage(0); // ������� WM_QUIT ����������
	}
	return 0;

	default: // ����� "����������� �� ���������"
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return FALSE;// ��� ������ � "break"
}