#include "console.h"

//Returneaza identificatorul ferestrei consolei.
HWND getConsoleHwnd(void)
{
#define MY_BUFSIZE 1024
	HWND hwndFound;
	char pszNewWindowTitle[MY_BUFSIZE];

	char pszOldWindowTitle[MY_BUFSIZE];

	GetConsoleTitleA(pszOldWindowTitle, MY_BUFSIZE);

	wsprintfA(pszNewWindowTitle, "%d/%d",
		GetTickCount64(),
		GetCurrentProcessId());

	SetConsoleTitleA(pszNewWindowTitle);

	Sleep(40);

	hwndFound = FindWindowA(NULL, pszNewWindowTitle);

	SetConsoleTitleA(pszOldWindowTitle);

	return(hwndFound);
}

//Returneaza cate linii si coloane are bufferul ferestrei consolei.
COORD getConsoleSize() {
	HANDLE stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if (stdOutput != INVALID_HANDLE_VALUE) {
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(stdOutput, &info);
		return info.dwSize;
	}
	else
		return (COORD) { 0 };
}

//Centreaza pe ecran fereastra consolei.
void moveConsoleWindowToCenter() {
	HWND consoleHWND = getConsoleHwnd();
	HMONITOR consoleMonitor = MonitorFromWindow(consoleHWND, MONITOR_DEFAULTTONEAREST);
	MONITORINFO info;
	info.cbSize = sizeof(MONITORINFO);

	GetMonitorInfo(consoleMonitor, &info);

	RECT consoleRect;
	GetWindowRect(consoleHWND, &consoleRect);

	int xConsole = (info.rcMonitor.right - info.rcMonitor.left - (consoleRect.right - consoleRect.left)) / 2;
	int yConsole = (info.rcMonitor.bottom - info.rcMonitor.top - (consoleRect.bottom - consoleRect.top)) / 2;

	SetWindowPos(consoleHWND, 0, xConsole, yConsole, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

//Seteaza cate linii si coloane sa aiba bufferul ferestrei consolei.
int setConsoleBufferSize(COORD size) {
	HANDLE stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if (stdOutput != INVALID_HANDLE_VALUE) {
		SMALL_RECT const minimal_window = { 0, 0, 1, 1 };
		int succes = SetConsoleWindowInfo(stdOutput, TRUE, &minimal_window);
		if (!succes)
			return 0;

		succes = SetConsoleScreenBufferSize(stdOutput, size);
		if (!succes)
			return 0;

		SMALL_RECT const window = { 0, 0, size.X - 1, size.Y - 1 };
		succes = SetConsoleWindowInfo(stdOutput, TRUE, &window);
		if (!succes)
			return 0;

		return 1;
	}

	return 0;
}

//Modifica tabla de 16 culori a consolei pentru a putea afisa textul cu alte culori.
void modifyConsoleColorTable() {
	HANDLE stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if (stdOutput != INVALID_HANDLE_VALUE) {
		CONSOLE_SCREEN_BUFFER_INFOEX info;
		info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		GetConsoleScreenBufferInfoEx(stdOutput, &info);
		
		info.ColorTable[6] = RGB(243, 255, 100);
		info.ColorTable[8] = RGB(185, 74, 74);
		SetConsoleScreenBufferInfoEx(stdOutput, &info);
	}
}

//Face cursorul consolei vizibil.
void showConsoleCursor() {
	HANDLE stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if (stdOutput != INVALID_HANDLE_VALUE) {
		CONSOLE_CURSOR_INFO info;
		info.dwSize = 10;
		info.bVisible = TRUE;
		SetConsoleCursorInfo(stdOutput, &info);
	}
}

//Ascunde cursorul consolei.
void hideConsoleCursor() {
	HANDLE stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if (stdOutput != INVALID_HANDLE_VALUE) {
		CONSOLE_CURSOR_INFO info;
		info.dwSize = 100;
		info.bVisible = FALSE;
		SetConsoleCursorInfo(stdOutput, &info);
	}
}

//Initializeaza setarile ferestrei consolei.
int initConsole() {
	HWND consoleWindow = getConsoleHwnd();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));

	setConsoleTextFont(L"Consolas");
	SetConsoleTitleA("DentC v1.0");

	modifyConsoleColorTable();
	
	if (!setConsoleBufferSize((COORD){ 114, 29 }))
			return 0;

	SetConsoleCP(850);
	moveConsoleWindowToCenter();
	hideConsoleCursor();

	return 1;
}

//Seteaza fontul cu care va afisat textul in consola.
void setConsoleTextFont(const wchar_t fontName[]) {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 22; 
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, fontName);
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

//Seteaza culoarea cu care se va afisa textul in consola.
void setConsoleTextColor(int color) {
	HANDLE stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if (stdOutput != INVALID_HANDLE_VALUE)
		SetConsoleTextAttribute(stdOutput, color);
}

//Returneaza coordonatele cursorului consolei.
COORD getConsoleCursorPosition()
{
	HANDLE stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if (stdOutput != INVALID_HANDLE_VALUE) {
		CONSOLE_SCREEN_BUFFER_INFO cbsi;
		if (GetConsoleScreenBufferInfo(stdOutput, &cbsi)) 
			return cbsi.dwCursorPosition;
		else 
			return (COORD){ -1, -1 };
	}

	return (COORD) { -1, -1 };
}

//Seteaza coordonatele cursorului consolei.
int setConsoleCursorPosition(int x, int y)
{
	HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (stdOut == INVALID_HANDLE_VALUE)
		return 0;

	COORD coord = { (SHORT)x, (SHORT)y };
	int result = SetConsoleCursorPosition(stdOut, coord);
	return result;
}

//Sterge de pe ecran un anumit numar de linii.
int clearLines(uint iX, uint iY, uint fX, uint fY, uint numberOfLines) {
	COORD consoleSize = getConsoleSize();
	FAIL_CHECK(setConsoleCursorPosition(iX, iY))
	HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD length = consoleSize.X * numberOfLines;
	fflush(stdout);
	
	DWORD written;
	char* emptyCharacters = malloc(length + 1);
	NULL_CHECK(emptyCharacters);
	memset(emptyCharacters, ' ', length);
	WriteConsoleA(stdOut, emptyCharacters, length, NULL, NULL);
	free(emptyCharacters);

	FAIL_CHECK(setConsoleCursorPosition(fX, fY))
	return 1;
}

//Citeste n caractere din consola si returneaza doar cand este apasata tasta enter.
int readNCharacters(int n, char* buffer, int password) {
	if (!n || !buffer)
		return 0;

	HANDLE stdIn = GetStdHandle(STD_INPUT_HANDLE);
	if (stdIn == INVALID_HANDLE_VALUE)
		return 0;

	DWORD cc;
	INPUT_RECORD iRec;
	int c, index = -1, currentPos = 0;

	for (;;) {
		Sleep(0.5);
		ReadConsoleInput(stdIn, &iRec, 1, &cc);
		if (iRec.EventType == KEY_EVENT && iRec.Event.KeyEvent.bKeyDown) {
			KEY_EVENT_RECORD keyEvent = iRec.Event.KeyEvent;
			if (keyEvent.wVirtualKeyCode == VK_ESCAPE)
				return -1;

			if (keyEvent.wVirtualKeyCode == VK_RETURN) {
				fwrite("\n", 1, 1, stdout);
				buffer[index + 1] = 0;
				return 1;
			}

			if (keyEvent.wVirtualKeyCode == VK_BACK) {
				if (currentPos != 0) {
					COORD cursorPos = getConsoleCursorPosition();
					fwrite("\b", 1, 1, stdout);

					currentPos--;
					index--;


					for (int i = currentPos; i <= index; i++) {
						buffer[i] = buffer[i + 1];

						if(!password)
							fwrite(&buffer[i], 1, 1, stdout);
						else
							fwrite("*", 1, 1, stdout);
					}

					buffer[index + 1] = 0;

					fwrite(" \b", 1, 1, stdout);
					FAIL_CHECK(setConsoleCursorPosition(cursorPos.X - 1, cursorPos.Y))
				}

				continue;
			}

			if (keyEvent.wVirtualKeyCode == VK_LEFT) {
				if (currentPos != 0) {
					COORD cursorPos = getConsoleCursorPosition();
					FAIL_CHECK(setConsoleCursorPosition(cursorPos.X - 1, cursorPos.Y))
					currentPos--;
				}

				continue;
			}

			if (keyEvent.wVirtualKeyCode == VK_RIGHT) {
				if (index >= 0 && currentPos <= index) {
					COORD cursorPos = getConsoleCursorPosition();
					FAIL_CHECK(setConsoleCursorPosition(cursorPos.X + 1, cursorPos.Y))
					currentPos++;
				}

				continue;
			}

			if (keyEvent.uChar.AsciiChar != 0 && keyEvent.wVirtualKeyCode != VK_TAB) {
				if (index < n - 1 && currentPos == index + 1) {
					if (!password)
						fwrite(&keyEvent.uChar.AsciiChar, 1, 1, stdout);
					else
						fwrite("*", 1, 1, stdout);

					buffer[index + 1] = keyEvent.uChar.AsciiChar;
					currentPos++;
					index++;
				}

				if (currentPos < index + 1) {
					if (!password)
						fwrite(&keyEvent.uChar.AsciiChar, 1, 1, stdout);
					else
						fwrite("*", 1, 1, stdout);

					currentPos++;
					buffer[currentPos - 1] = keyEvent.uChar.AsciiChar;
				}
			}
		}
	}

	return 1;
}

//Citeste n caractere din consola.
int getNCharacters(int n, char* buffer) {
	return readNCharacters(n, buffer, 0);
}

//Citeste n caractere din consola si output-ul il mascheaza cu *.
int getPassword(int n, char* buffer) {
	return readNCharacters(n, buffer, 1);
}