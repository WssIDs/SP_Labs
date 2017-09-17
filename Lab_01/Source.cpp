#include <Windows.h>
#include <tchar.h>

//-- Prototypes -------------------
LRESULT CALLBACK SimWndProc(HWND, UINT, WPARAM, LPARAM);


//-- Global Variables ------------ 


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
	hbr = CreateSolidBrush(RGB(255, 0, 0));
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

	switch (msg)
	{

	case WM_PAINT:    // Вывод при обновлении окна
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps); // Получение контекста для обновления окна
		EndPaint(hWnd, &ps); // Завершение обновления окна
	}
	return 0;

	case WM_CLOSE:  // Перед разрушением
	{

		int iRetValue1, iRetValue2;
		LPCTSTR lpszMesBoxTitle = TEXT("Вариант 1");
		LPCTSTR lpszMesBoxText = TEXT("The message box contains three push buttons: Abort, Retry, and Ignore.");
		LPTSTR lpszResponce;

		do {
			iRetValue1 = MessageBox(hWnd, lpszMesBoxText, lpszMesBoxTitle, MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION);
			
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
	

	case WM_DESTROY:  // Завершение работы приложения
		PostQuitMessage(0); // Посылка WM_QUIT приложению
		break;

	default: // Вызов "Обработчика по умолчанию"
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return FALSE;// Для ветвей с "break"
}