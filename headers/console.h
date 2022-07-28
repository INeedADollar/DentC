#pragma once
#pragma warning(disable : 4996)

#include <Windows.h>
#include <stdio.h>

#include "defines.h"

//Returneaza identificatorul ferestrei consolei.
HWND getConsoleHwnd(void);

//Returneaza cate linii si coloane are bufferul ferestrei consolei.
COORD getConsoleSize();

//Initializeaza setarile ferestrei consolei.
int initConsole();

//Seteaza fontul cu care va afisat textul in consola.
void setConsoleTextFont(const wchar_t fontName[]);

//Returneaza coordonatele cursorului consolei.
COORD getConsoleCursorPosition();

//Seteaza culoarea cu care se va afisa textul in consola.
void setConsoleTextColor(int color);

//Seteaza coordonatele cursorului consolei.
int setConsoleCursorPosition(int x, int y);

//Face cursorul consolei vizibil.
void showConsoleCursor();

//Ascunde cursorul consolei.
void hideConsoleCursor();

//Sterge de pe ecran un anumit numar de linii.
int clearLines(uint iX, uint iY, uint fX, uint fY, uint numberOfLines);

//Citeste n caractere din consola.
int getNCharacters(int n, char* buffer);

//Citeste n caractere din consola si output-ul il mascheaza cu *.
int getPassword(int n, char* buffer);