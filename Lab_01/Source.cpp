#include <Windows.h>
#include <tchar.h>
#include <cstdlib>


//-- Prototypes -------------------
LRESULT CALLBACK SimWndProc(HWND, UINT, WPARAM, LPARAM);


//-- Global Variables ------------ 
int g_rectRight = 0;
int g_rectBottom = 0;
bool g_lmousedown = false;
int g_rectLeft = 0;
int g_rectTop = 0;


//  Стартовая функция 
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	MSG msg;
	HWND hWnd;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = TEXT("VMainWindowClass");
	wc.lpfnWndProc = SimWndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	HBRUSH hbr;


	int r = rand();
	int g = rand();
	int b = rand();

	hbr = CreateSolidBrush(RGB(r, g, b));
	wc.hbrBackground = hbr;

	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Ошибка регистрации класса окна!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	DWORD Stl;
	Stl = WS_OVERLAPPEDWINDOW^WS_MINIMIZEBOX;

	hWnd = CreateWindowEx(NULL, wc.lpszClassName,
			TEXT("Володько Виталий Иванович"),
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
		MessageBox(NULL, TEXT("Окно не создано!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
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

// Оконная процедура
LRESULT CALLBACK SimWndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{

	HDC hDC;
	RECT Rect;//до этого писалось в никуда, теперь будет писаться в эту структуру.
	RECT tRect;


	switch (msg)
	{
	case WM_LBUTTONDOWN:
	{
		//GetClientRect(hWnd, &Rect);//передаем указатель на нее

		HDC hDc;
		hDc = GetDC(hWnd);
		DWORD x, y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		g_rectLeft = x;
		g_rectTop = y;

		InvalidateRect(hWnd, &Rect, TRUE);//и тут
		//Rectangle(hDc, 50, 50, a, b);

		g_lmousedown = true;

	}
		return 0;

	case WM_MOUSEMOVE:
	{		
		if (g_lmousedown == true)
		{
			GetClientRect(hWnd, &Rect);//передаем указатель на нее

			HDC hDc;
			hDc = GetDC(hWnd);
			DWORD x, y;
			x = LOWORD(lParam);
			y = HIWORD(lParam);


			//MessageBox(hWnd, TEXT(""), TEXT(""), MB_OK);
			g_rectRight = x;
			g_rectBottom = y;


			LPTSTR lptext = TEXT("Тестовое сообщение");

			tRect = Rect;
			tRect.left = x;
			tRect.top = y;
			tRect.right = tRect.left+150;
			tRect.bottom= tRect.top+20;

			InvalidateRect(hWnd, &Rect, TRUE);//и тут

			//Rectangle(hDc, g_rectLeft, g_rectTop, g_rectRight, g_rectBottom);		
			Ellipse(hDc, g_rectLeft, g_rectTop, g_rectRight, g_rectBottom);
			//DrawText(hDc, lptext, lstrlen(lptext), &tRect, DT_CENTER);
		}
	}
	return 0;

	case WM_LBUTTONUP:
	{
		g_lmousedown = false;

		//GetClientRect(hWnd, &Rect);//передаем указатель на нее

		//HDC hDc;
		//hDc = GetDC(hWnd);

		//InvalidateRect(hWnd, &Rect, TRUE);//и тут

		//Rectangle(hDc, 50, 50, Rect.right - x, Rect.bottom - y);

	}
	return 0;

	case WM_PAINT:    // Вывод при обновлении окна
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps); // Получение контекста для
									 // обновления окна
		GetClientRect(hWnd, &Rect);//передаем указатель на нее


		LPTSTR lptext = TEXT("Прямоугольная область текста");

		DrawText(hDC, lptext,lstrlen(lptext),&Rect, DT_CENTER);

		//Rectangle(hDC, g_rectLeft, g_rectTop, g_rectRight, g_rectBottom);
		Ellipse(hDC, g_rectLeft, g_rectTop, g_rectRight, g_rectBottom);
		EndPaint(hWnd, &ps); // Завершение обновления окна
	}
	return 0;

	case WM_CLOSE:  // Перед разрушением
		
		if(MessageBox(hWnd, TEXT("Вы действиетльно хотите выйти?"), TEXT("Выход"), MB_OKCANCEL | MB_ICONWARNING) == 1) // если нажата OK
		{
			DestroyWindow(hWnd); // Посылка WM_QUIT приложению
		}

		break;

	case WM_DESTROY:  // Завершение работы приложения
		PostQuitMessage(0); // Посылка WM_QUIT приложению
		break;

	default: // Вызов "Обработчика по умолчанию"
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return FALSE;// Для ветвей с "break"
}