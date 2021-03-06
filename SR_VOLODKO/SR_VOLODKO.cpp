// SR_VOLODKO.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SR_VOLODKO.h"
#include <stdio.h>

SRVOLODKO_API int DigitsCount(const char * fileName)
{
	FILE* fp;
	fopen_s(&fp, fileName, "r");
	
	int countOfWords = 1;
	int lenght = 0;
	char c;

	while ((c = fgetc(fp)) != EOF)//считываем символ из файла data в переменную c до символа EOF.
	{
		if (c == ' ')
		{
			countOfWords++;
		}
		if (c == ',' || c == '.' || c == ':')
		{
			continue;
		}
		else
		{
			lenght++;
		}
	}

	fclose(fp);//проверить код закрытия или удачно закрыты файлы!

	return lenght/countOfWords;
}

SRVOLODKO_API void UpdateFile(char const * fileName, TCHAR * text)
{
	FILE * f;
	fopen_s(&f,fileName, "a");
	char buf[200];
	CharToOem(text, buf);

	fprintf(f, "\n%s", buf);
	fclose(f);
}

SRVOLODKO_API void StartNotepad(TCHAR* fileName)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO sti;
	ZeroMemory(&sti, sizeof(STARTUPINFO));
	sti.cb = sizeof(STARTUPINFO);
	TCHAR cmdParam[260];
	wsprintf(cmdParam, TEXT("Notepad %s"), fileName);
	CreateProcess(NULL, cmdParam,
		NULL, NULL, 0, 0, NULL, NULL, &sti, &pi);
}
