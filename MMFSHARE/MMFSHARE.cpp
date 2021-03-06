#include <windows.h>
#include <windowsx.h>

#include "resource.h"

//Глобальные переменные
HANDLE s_hFileMap = NULL;
LPVOID g_lpView;

BOOL  Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	// Связываем значок с диалоговым окном
	HINSTANCE hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
	HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MMFSHARE));//__TEXT("MMFShare"))
	SetClassLong(hwnd, GCL_HICON, (LONG)hIcon);

	// Инициализируем поле ввода тестовыми данными
	HWND hEditWnd = GetDlgItem(hwnd,   // handle of dialog box 
		IDC_DATA // identifier of control 
	);
	Edit_SetText(hEditWnd, __TEXT("Здесь вводите данные."));

	// Создаем в памяти проецируемый файл, содержащий
	// данные, набранные в поле ввода. Файл занимает 4 Кб
	// и называется MMFSharedData.
	s_hFileMap = CreateFileMapping((HANDLE)0xFFFFFFFF,
		NULL, PAGE_READWRITE, 0, 4 * 1024,
		__TEXT("MMFSharedData"));

	if (s_hFileMap != NULL)
	{
		// Создание проецируемого файла завершилось успешно
		// Проецируем оконное представление файла
		// на адресное пространство
		g_lpView = MapViewOfFile(s_hFileMap,
			FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 4 * 1024);
		if ((BYTE *)g_lpView != NULL)
		{
			// Окно спроецировано успешно: поместим содержимое
			// элемента управления EDIT в проецируемый файл
			Edit_GetText(GetDlgItem(hwnd, IDC_DATA),
				(LPTSTR)g_lpView, 4 * 1024);
			Button_Enable(GetDlgItem(hwnd, IDC_CLOSEFILE), TRUE);
			Button_Enable(GetDlgItem(hwnd, IDC_OPENFILE), FALSE);
		}
		else
		{
			CloseHandle(s_hFileMap);
			MessageBox(hwnd, __TEXT("Can't map view of file. "),
				NULL, MB_OK);
		}
	}
	else
	{
		MessageBox(hwnd, __TEXT("Can't create file mapping."), NULL, MB_OK);
	}
	return (TRUE);
}

void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl,
	UINT codeNotify)
{
	switch (id)
	{
	case IDC_READ:
	{
		if ((BYTE *)g_lpView != NULL)
		{
			Edit_SetText(GetDlgItem(hwnd, IDC_DATA), (LPTSTR)g_lpView);
		}
		else
		{
			MessageBox(hwnd, __TEXT("Can't map view."), NULL, MB_OK);
		}
		break;
	}

	case IDC_WRITE:
	{
		if ((BYTE *)g_lpView != NULL)
		{
			Edit_GetText(GetDlgItem(hwnd, IDC_DATA), (LPTSTR)g_lpView, 4 * 1024);
		}
		else
		{
			MessageBox(hwnd, __TEXT("Can't map view."), NULL, MB_OK);
		}
		break;
	}

	case IDC_CLOSEFILE:
		// Прекращаем проецирование окна. 
		UnmapViewOfFile((LPVOID)g_lpView);
		if (CloseHandle(s_hFileMap))
		{
			// Пользователь закрыл файл.  Новый файл создать можно,
			// но закрыть его нельзя.
			Button_Enable(GetDlgItem(hwnd, IDC_WRITE), FALSE);
			Button_Enable(GetDlgItem(hwnd, IDC_READ), FALSE);
			Button_Enable(GetDlgItem(hwnd, IDC_OPENFILE), TRUE);
			Button_Enable(hwndCtl, FALSE);
		}
		break;

	case IDC_OPENFILE:
	{
		// Смотрим: не существует ли уже проецируемый в память файл
		// с именем MMFSharedDafa	
		s_hFileMap = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE,
			FALSE, __TEXT("MMFSharedData"));
		if (s_hFileMap == NULL)
		{
			s_hFileMap = CreateFileMapping((HANDLE)0xFFFFFFFF,
				NULL, PAGE_READWRITE, 0, 4 * 1024,
				__TEXT("MMFSharedData"));
		}
		if (s_hFileMap != NULL)
		{
			// Такой файл существует. Проецируем его оконное
			// представление на адресное пространство процесса.
			g_lpView = MapViewOfFile(s_hFileMap,
				FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
			if ((BYTE *)g_lpView != NULL)
			{
				// Пользователь не может создать сейчас
				// еще один файл
				Button_Enable(hwndCtl, FALSE);
				Button_Enable(GetDlgItem(hwnd, IDC_WRITE), TRUE);
				Button_Enable(GetDlgItem(hwnd, IDC_READ), TRUE);
				// Пользователь может закрыть файл
				Button_Enable(GetDlgItem(hwnd, IDC_CLOSEFILE), TRUE);
			}
			else
			{
				CloseHandle(s_hFileMap);
				MessageBox(hwnd, __TEXT("Can't map view."), NULL, MB_OK);
			}
		}
		else
		{
			MessageBox(hwnd, __TEXT("Can't open mapping."), NULL, MB_OK);
		}
		break;
	}

	case IDCANCEL:
		EndDialog(hwnd, id);
		break;
	}
}

BOOL CALLBACK Dlg_Proc(HWND hDlg, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{

	BOOL fProcessed = TRUE;
	switch (uMsg)
	{
		HANDLE_MSG(hDlg, WM_INITDIALOG, Dlg_OnInitDialog);
		HANDLE_MSG(hDlg, WM_COMMAND, Dlg_OnCommand);
	default:
		fProcessed = FALSE;
		break;
	}
	return(fProcessed);
}

int WINAPI WinMain(HINSTANCE hinstExe,
	HINSTANCE hinstPrev,
	LPSTR lpszCmdLine,
	int nCrndShow)
{
	DialogBox(hinstExe, MAKEINTRESOURCE(IDD_MMFSHARE),
		NULL, (DLGPROC)Dlg_Proc);
	return (0);
}