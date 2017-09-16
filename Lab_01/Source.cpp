#include <Windows.h>
#include <tchar.h>

//-- Prototypes -------------------
LRESULT CALLBACK SimWndProc(HWND, UINT, WPARAM, LPARAM);


//-- Global Variables ------------ 
int g_rectRight = 0;
int g_rectBottom = 0;
bool g_lmousedown = false;
int g_rectLeft = 0;
int g_rectTop = 0;


//  ��������� ������� 
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	MSG msg;
	HWND hWnd,hWnd1;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = TEXT("VMainWindowClass");
	wc.lpfnWndProc = SimWndProc;
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

	DWORD Stl;
	Stl = WS_OVERLAPPEDWINDOW^WS_MINIMIZEBOX;

	hWnd = CreateWindowEx(NULL, wc.lpszClassName,
			TEXT("�������� ������� ��������"),
			Stl,
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

	hWnd1 = CreateWindowEx(NULL, wc.lpszClassName,
		TEXT("�������� ������� �������� rfds"),
		Stl,
		200,
		200,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd1)
	{
		MessageBox(NULL, TEXT("���� �� �������!"),
			TEXT("������"), MB_OK | MB_ICONERROR);
		return FALSE;
	}


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	ShowWindow(hWnd1, nCmdShow);
	UpdateWindow(hWnd1);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

// ������� ���������
LRESULT CALLBACK SimWndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{

	HDC hDC;
	RECT Rect;//�� ����� �������� � ������, ������ ����� �������� � ��� ���������.
	RECT tRect;


	switch (msg)
	{
	case WM_LBUTTONDOWN:
	{
		//GetClientRect(hWnd, &Rect);//�������� ��������� �� ���

		HDC hDc;
		hDc = GetDC(hWnd);
		DWORD x, y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		g_rectLeft = x;
		g_rectTop = y;

		InvalidateRect(hWnd, &Rect, TRUE);//� ���
		//Rectangle(hDc, 50, 50, a, b);

		g_lmousedown = true;

	}
		return 0;
	
	case WM_LBUTTONUP:
	{
		g_lmousedown = false;
	}
	return 0;


	case WM_MOUSEMOVE:
	{		
		if (g_lmousedown == true)
		{
			GetClientRect(hWnd, &Rect);//�������� ��������� �� ���

			HDC hDc;
			hDc = GetDC(hWnd);
			DWORD x, y;
			x = LOWORD(lParam);
			y = HIWORD(lParam);


			//MessageBox(hWnd, TEXT(""), TEXT(""), MB_OK);
			g_rectRight = x;
			g_rectBottom = y;


			LPTSTR lptext = TEXT("�������� ���������");

			tRect = Rect;
			tRect.left = x;
			tRect.top = y;
			tRect.right = tRect.left+150;
			tRect.bottom= tRect.top+20;

			InvalidateRect(hWnd, &Rect, TRUE);//� ���

			//Rectangle(hDc, g_rectLeft, g_rectTop, g_rectRight, g_rectBottom);		
			Ellipse(hDc, g_rectLeft, g_rectTop, g_rectRight, g_rectBottom);
			//DrawText(hDc, lptext, lstrlen(lptext), &tRect, DT_CENTER);
		}
	}
	return 0;

	case WM_PAINT:    // ����� ��� ���������� ����
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps); // ��������� ��������� ���
									 // ���������� ����
		GetClientRect(hWnd, &Rect);//�������� ��������� �� ���


		LPTSTR lptext = TEXT("������������� ������� ������");

		DrawText(hDC, lptext,lstrlen(lptext),&Rect, DT_CENTER);

		//Rectangle(hDC, g_rectLeft, g_rectTop, g_rectRight, g_rectBottom);
		Ellipse(hDC, g_rectLeft, g_rectTop, g_rectRight, g_rectBottom);
		EndPaint(hWnd, &ps); // ���������� ���������� ����
	}
	return 0;

	case WM_CLOSE:  // ����� �����������
	{

		int iRetValue1, iRetValue2;
		LPCTSTR lpszMesBoxTitle = TEXT("������� MessageBox");
		LPTSTR lpszResponce;
		do {
			iRetValue1 = MessageBox(hWnd, TEXT("The message box contains three push buttons: Abort, Retry, and Ignore."), lpszMesBoxTitle,
				MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION);
			switch (iRetValue1)
			{
			case IDABORT:
				lpszResponce = TEXT("Pressed ABORT");
				break;
			case IDRETRY:
				lpszResponce = TEXT("Pressed RETRY");
				break;
			case IDIGNORE:
				lpszResponce = TEXT("Pressed IGNORE");
				break;
			default: lpszResponce = TEXT("Answer unknow");
			}
			TCHAR buf[200] = TEXT("");
			lstrcat(buf, lpszResponce);
			lstrcat(buf, TEXT(" Do you continue?"));
			iRetValue2 = MessageBox(hWnd, buf, lpszMesBoxTitle,
				MB_YESNOCANCEL | MB_ICONQUESTION);

		} while (iRetValue2 != IDNO && iRetValue2 != IDCANCEL);


		if (iRetValue2 == IDNO)
		{
			MessageBox(hWnd, "Pressed NO. Application has been terminated", "Close programm",
				MB_OK | MB_ICONQUESTION);
			
			DestroyWindow(hWnd);
		}
	}
	return 0;
	

	case WM_DESTROY:  // ���������� ������ ����������
		PostQuitMessage(0); // ������� WM_QUIT ����������
		break;

	default: // ����� "����������� �� ���������"
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return FALSE;// ��� ������ � "break"
}