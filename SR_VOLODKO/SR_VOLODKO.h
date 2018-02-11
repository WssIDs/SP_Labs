#include <cstring>
#ifdef SRVOLODKO_EXPORTS
#define SRVOLODKO_API __declspec(dllexport)
#else
#define SRVOLODKO_API __declspec(dllimport)
#endif

SRVOLODKO_API int DigitsCount(char const * fileName);
SRVOLODKO_API void UpdateFile(char const * fileName, TCHAR * text);
SRVOLODKO_API void StartNotepad(TCHAR * fileName);