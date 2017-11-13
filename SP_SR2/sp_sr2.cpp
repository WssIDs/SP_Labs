#include <Windows.h>
#include <tchar.h>

//-- Prototypes -------------------
LRESULT CALLBACK Pr2_WndProc(HWND, UINT, WPARAM, LPARAM);


//-- Global Variables ------------ 
LPTSTR g_lpszClassName = TEXT("sp_pr2_class");
LPTSTR g_lpszAplicationTitle = TEXT("�����������: �������� �������, 60331-1");
LPTSTR g_lpszDestroyTitle = TEXT("������� 1");
LPTSTR g_lpszDestroyMessage = TEXT("������ ����� �������� � ����������� ��������� WM_DESTROY. ��������� ��������� �� Windows � ����� � ����������� ����.");



typedef struct
{
	TCHAR colorName[26];
	COLORREF color;
} ColorItem;

ColorItem Items[] =
{
	{ TEXT("�����"), RGB(0, 0, 255) },
	{ TEXT("�������"), RGB(255, 0, 0) },
	{ TEXT("�������"), RGB(0, 255, 0) },
	{ TEXT("������"), RGB(0, 0, 0) }
};




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
	static HWND hListBox;

#define IDC_LIST	30100 

	HDC hDC;

	switch (msg)
	{
	case WM_CREATE:
	{
		MessageBox(hWnd, TEXT("����������� ��������� WM_CREATE"), g_lpszDestroyTitle, MB_OK | MB_ICONEXCLAMATION);

		hListBox = CreateWindowEx(WS_EX_PALETTEWINDOW,TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 10, 10, 200, 150, hWnd, (HMENU)IDC_LIST, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		for (int i = 0; i < ARRAYSIZE(Items); i++)
		{
			int pos = (int)SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)Items[i].colorName);
			SendMessage(hListBox, LB_SETITEMDATA, pos, (LPARAM)i);
		}

		SetFocus(hListBox);

		// �������������� ������
		InvalidateRect(hListBox, NULL, TRUE);

	}
	return 0;

	case WM_PAINT:    // ����� ��� ���������� ����
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps); // ��������� ��������� ��� ���������� ����

		EndPaint(hWnd, &ps); // ���������� ���������� ����
	}
	return 0;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			case IDC_LIST:
			{
				switch (HIWORD(wParam))
				{
					case LBN_SELCHANGE:
					{
						HDC hdc = GetDC(hWnd);//�������� ��������
						RECT rc;
						GetClientRect(hWnd, &rc);

						RECT rcFill = rc;
						rcFill.top = rcFill.bottom - 30;
						rcFill.right = rcFill.left + 300;
						FillRect(hdc, &rcFill, CreateSolidBrush(RGB(255, 0, 0)));

						int lbItem = (int)SendMessage(hListBox, LB_GETCURSEL, 0, 0);
						int i = (int)SendMessage(hListBox, LB_GETITEMDATA, lbItem, 0);

						SetTextColor(hdc, Items[i].color);
						TextOut(hdc, 10, rc.bottom-20,Items[i].colorName, lstrlen(Items[i].colorName));
						ReleaseDC(hWnd, hdc);
					}
					break;
				}
			}
			break;
		}
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