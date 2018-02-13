#include "sp_pr2-5.h"

//  Стартовая функция 
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hWnd;

	if (!Register(hInstance)) return FALSE;

	hWnd = Create(hInstance, nCmdShow);

	if (hWnd == NULL) return FALSE;

	HACCEL hAccel;
	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hWnd, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

// Оконная процедура
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_COMMAND, km_OnCommand);				// Обработка меню
		HANDLE_MSG(hWnd, WM_MENUSELECT, km_OnMenuSelect);		// При смене элемента меню
		HANDLE_MSG(hWnd, WM_CLOSE, km_OnClose);					// Перед разрушением
		HANDLE_MSG(hWnd, WM_CREATE, km_OnCreate);				// Вывод при обновлении окна
		HANDLE_MSG(hWnd, WM_PAINT, km_OnPaint);					// Вывод при обновлении окна
		HANDLE_MSG(hWnd, WM_DESTROY, km_OnDestroy);				// Завершение работы приложения
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


/* Регистрация класса окна */
BOOL Register(HINSTANCE hInst)
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = g_lpszClassName;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInst, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Ошибка регистрации класса окна!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

/* Создание окна */
HWND Create(HINSTANCE hInstance, int nCmdShow)
{
	DWORD Stl;
	Stl = WS_OVERLAPPEDWINDOW;

	g_lpszMainMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

	HWND hWnd = CreateWindowEx(NULL, g_lpszClassName,
		g_lpszAplicationTitle,
		Stl,
		200,
		200,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		g_lpszMainMenu,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL, TEXT("Окно не создано!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return NULL;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

/* ---------- Обработчики сообщений -------------------------------- */

// WM_CREATE
BOOL km_OnCreate(HWND hWnd, LPCREATESTRUCT lpszCreateStruct)
{

	return TRUE;
}

// WM_ONPAINT
void km_OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps); // Получение контекста для обновления окна

	EndPaint(hWnd, &ps); // Завершение обновления окна
}

// WM_ONCLOSE
void km_OnClose(HWND hWnd)
{
	DestroyWindow(hWnd);
}

// WM_MENUSELECT
void km_OnMenuSelect(HWND hWnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
}

// WM_DESTROY
void km_OnDestroy(HWND hWnd)
{
	PostQuitMessage(0); // Посылка WM_QUIT приложению
}

#include "stdio.h"

// WM_COMMAND
void km_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	TCHAR buff[200];
	wsprintf(buff, TEXT("%d"), id);

	switch (id)
	{
		case IDM_FILE_EXIT:
		{
			DestroyWindow(hWnd);
		}
		break;
		case IDM_MAP_CREATEFILE:
		{
			OPENFILENAME ofn;
			TCHAR szFile[160];
			TCHAR Buffer[MAX_BYTES] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = _T("All\0*.*\0Text\0*.TXT\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&ofn) == TRUE)
			{
				TCHAR buf[MAX_LOADSTRING] = { 0 };
				wsprintf(buf, TEXT("Создан объект \"файл\" %s"), ofn.lpstrFile);
				MessageBox(hWnd, buf, TEXT("Info"), MB_OK);
				if (!CopyFile(ofn.lpstrFile, TEXT(FILENAME), FALSE))
				{
					MessageBox(NULL, TEXT("New file could not be created."), TEXT("FileRev"), MB_OK);
					break;
				}
				else
				{
					hFile = CreateFile(
						TEXT(FILENAME),
						GENERIC_WRITE | GENERIC_READ,
						0,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL
					);
				}
				if (hFile == INVALID_HANDLE_VALUE)
				{
					MessageBox(NULL, TEXT("File could not be opened."), TEXT("FileRev"), MB_OK);
					break;
				}
				else
				{
					SendMessage(hWnd, WM_PAINT, NULL, NULL);
					g_lpszMainMenu = GetMenu(hWnd);
					EnableMenuItem(g_lpszMainMenu, IDM_MAP_CREATEMAPFILE, MF_ENABLED);
					EnableMenuItem(g_lpszMainMenu, IDM_MAP_CREATEFILE, MF_GRAYED);
					dwFileSize = GetFileSize(hFile, NULL);
				}
			}
		}
		break;
		case IDM_MAP_CREATEMAPFILE:
		{
			hFileMap = CreateFileMapping(
				hFile,
				NULL,
				PAGE_READWRITE,
				0,
				dwFileSize + sizeof(WCHAR),
				NULL
			);
			if (hFileMap == NULL)
			{
				MessageBox(NULL, TEXT("File map could not be opened."), TEXT("FileRev"), MB_OK);
				CloseHandle(hFile);
				return;
			}
			else
			{
				MessageBox(hWnd, TEXT("Создан объект \"проецирумый файл\""), TEXT("Информация"), MB_OK);
				SendMessage(hWnd, WM_PAINT, NULL, NULL);
				EnableMenuItem(g_lpszMainMenu, IDM_MAP_VIEWMAP, MF_ENABLED);
				EnableMenuItem(g_lpszMainMenu, IDM_MAP_CREATEMAPFILE, MF_GRAYED);
			}
		}
		break;
		case IDM_MAP_VIEWMAP:
		{
			lpvFile = MapViewOfFile(
				hFileMap,
				FILE_MAP_WRITE,
				0,
				0,
				0
			);

			if (lpvFile == NULL)
			{
				MessageBox(NULL, TEXT("Could not map view of file."), TEXT("FileRev"), MB_OK);
				CloseHandle(hFileMap);
				CloseHandle(hFile);
				return;
			}
			else
			{
				int countOfWords = 1;
				int lenght = 0;
				lpchANSI = (LPSTR)lpvFile;
				lpchANSI[dwFileSize] = 0;
				lpchANSI = strchr(lpchANSI, (int)' ');


				for (int i = 0; i < strlen(lpchANSI); i++)
				{
					if (lpchANSI[i] == ' ')
					{
						countOfWords++;
					}
					if (lpchANSI[i] == ',' || lpchANSI[i] == '.' || lpchANSI[i] == ':')
					{
						continue;
					}
					else
					{
						lenght++;
					}
				}

				WCHAR avcountword[250];
				swprintf(avcountword, L"\r\n Averange count of words: %d", lenght / countOfWords);

				TCHAR tString[250];
				int len = wcstombs(tString, avcountword, wcslen(avcountword));
				tString[len] = '\0';
				lstrcat(lpchANSI, tString);
				dwFileSize += strlen(tString);
				lpchANSI[dwFileSize] = 0;

				MessageBox(NULL, TEXT("Отображено на адресное пространство"), TEXT("Info"), MB_OK);
				EnableMenuItem(g_lpszMainMenu, IDM_MAP_VIEWUNMAP, MF_ENABLED);
				EnableMenuItem(g_lpszMainMenu, IDM_MAP_VIEWMAP, MF_GRAYED);
			}
		}
		break;
		case IDM_MAP_VIEWUNMAP:
		{
			UnmapViewOfFile(lpvFile);
			MessageBox(hWnd, TEXT("Закрытие отображения"), TEXT("Info"), MB_OK);
			EnableMenuItem(g_lpszMainMenu, IDM_MAP_CLOSEHANDLES, MF_ENABLED);
			EnableMenuItem(g_lpszMainMenu, IDM_MAP_VIEWUNMAP, MF_GRAYED);
		}
		break;
		case IDM_MAP_CLOSEHANDLES:
		{
			CloseHandle(hFileMap);
			SetFilePointer(hFile, dwFileSize, NULL, FILE_BEGIN);
			SetEndOfFile(hFile);
			CloseHandle(hFile);
			MessageBox(hWnd, TEXT("Закрытие дескрипторов объектов ядра"), TEXT("Info"), MB_OK);
			EnableMenuItem(g_lpszMainMenu, IDM_MAP_NOTEPAD, MF_ENABLED);
			EnableMenuItem(g_lpszMainMenu, IDM_MAP_CLOSEHANDLES, MF_GRAYED);
		}
		break;
		case IDM_MAP_NOTEPAD:
		{
			si.cb = sizeof(si);
			si.wShowWindow = SW_SHOW;
			si.dwFlags = STARTF_USESHOWWINDOW;
			LPTSTR cmdLine;
			cmdLine = (LPTSTR)calloc(lstrlen(TEXT("notepad.exe ")), sizeof(WCHAR));
			lstrcpy((LPTSTR)cmdLine, TEXT("notepad.exe "));
			lstrcat((LPTSTR)cmdLine, TEXT(FILENAME));
			if (CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
			{
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
			}

		}
		break;
		default:
		{
			MessageBox(hWnd, TEXT("Команда не реализована"), buff, MB_OK);
		}
		break;
	}
}
