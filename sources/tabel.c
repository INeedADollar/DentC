#include "tabel.h"
#include "console.h"

//Creaza un tableLabel.
tableLabel* createTableLabel(c_char* text, uint width) {
	NULL_CHECK(text)

	tableLabel* label = malloc(sizeof(tableLabel));
	NULL_CHECK(label)

	label->labelText = malloc(strlen(text) + 1);
	NULL_CHECK_EX(label->labelText, free(label); return NULL;)
	
	strcpy(label->labelText, text);
	label->width = width;

	return label;
}

//Creaza un tableLabelsArray.
tableLabelsArray createTableLabelsArray() {
	tableLabelsArray arr;
	arr.labelsArray = NULL;
	arr.arraySize = 0;
	
	return arr;
}

//Adauga un tableLabel in tableLabelsArray.
int addToTableLabelsArray(tableLabelsArray* arr, tableLabel* label) {
	NULL_CHECK(arr)
	NULL_CHECK(label)

	NULL_CHECK_EX(arr->labelsArray,
		arr->labelsArray = malloc(sizeof(tableLabel*));
		NULL_CHECK(arr->labelsArray)

		arr->labelsArray[0] = label;
		arr->arraySize++;
		return 1;
	)

	tableLabel** testLabel = realloc(arr->labelsArray, (arr->arraySize + 1) * sizeof(tableLabel*));
	NULL_CHECK(testLabel)

	arr->labelsArray = testLabel;
	arr->labelsArray[arr->arraySize] = label;
	arr->arraySize++;

	return 1;
}

//Salveaza array-ul ce a fost dat functiei startTabel.
static tableLabelsArray currentArr;

//Verifica daca un label e prea lung, iar daca da elimina ce e in plus si adauga ... la final.
int checkLabelLength(tableLabel** label) {
	NULL_CHECK(label)
	NULL_CHECK(*label)
	NULL_CHECK((*label)->labelText)

	int labelLen = strlen((*label)->labelText);
	if (labelLen > (*label)->width) {
		uint labelWidth = (*label)->width;
		(*label)->labelText[labelWidth - 3] = '.';
		(*label)->labelText[labelWidth - 2] = '.';
		(*label)->labelText[labelWidth - 1] = '.';
		(*label)->labelText[labelWidth] = 0;
	}

	return 1;
}

//Incepe afisarea unui tabel.
int startTabel(tableLabelsArray arr) {
	NULL_CHECK(arr.labelsArray)

	SetConsoleOutputCP(65001);
	currentArr = arr;
	COORD initCursorPos = getConsoleCursorPosition();
	COORD cursorPos = initCursorPos;
	HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	NULL_CHECK(stdOut)

	WriteConsoleW(stdOut, L"╔", 1, NULL, NULL);
	FAIL_CHECK(setConsoleCursorPosition(cursorPos.X, cursorPos.Y + 1))
	WriteConsoleW(stdOut, L"║", 1, NULL, NULL);
	int labelStart = 0, labelLen = 0;

	for (int i = 0; i < arr.arraySize; i++) {
		FAIL_CHECK(setConsoleCursorPosition(cursorPos.X + 1, cursorPos.Y + 1))
		checkLabelLength(&arr.labelsArray[i]);
		labelLen = strlen(arr.labelsArray[i]->labelText);
		labelStart = (arr.labelsArray[i]->width - labelLen) / 2;

		FAIL_CHECK(setConsoleCursorPosition(cursorPos.X + labelStart + 1, cursorPos.Y + 1))
		WriteConsoleA(stdOut, arr.labelsArray[i]->labelText, labelLen, NULL, NULL);

		FAIL_CHECK(setConsoleCursorPosition(cursorPos.X + 1, cursorPos.Y))
		for (int j = 0; j < arr.labelsArray[i]->width; j++)
			WriteConsoleW(stdOut, L"═", 1, NULL, NULL);

		if (i == arr.arraySize - 1) {
			cursorPos = getConsoleCursorPosition();
			WriteConsoleW(stdOut, L"╗", 1, NULL, NULL);

			FAIL_CHECK(setConsoleCursorPosition(cursorPos.X, cursorPos.Y + 1))
			WriteConsoleW(stdOut, L"║", 1, NULL, NULL);
		}
		else {
			cursorPos = getConsoleCursorPosition();
			WriteConsoleW(stdOut, L"╦", 1, NULL, NULL);
			FAIL_CHECK(setConsoleCursorPosition(cursorPos.X, cursorPos.Y + 1))
			WriteConsoleW(stdOut, L"║", 1, NULL, NULL);
		}
	}

	FAIL_CHECK(setConsoleCursorPosition(initCursorPos.X, cursorPos.Y + 2))
	return 1;
}

//Adauga o linie in tabel.
int addRow(tableLabelsArray arr) {
	NULL_CHECK(arr.labelsArray)
	NULL_CHECK(currentArr.labelsArray)

	COORD initCursorPos = getConsoleCursorPosition();
	COORD cursorPos = initCursorPos;
	HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	NULL_CHECK(stdOut)

	WriteConsoleW(stdOut, L"╠", 1, NULL, NULL);
	FAIL_CHECK(setConsoleCursorPosition(cursorPos.X, cursorPos.Y + 1))
	WriteConsoleW(stdOut, L"║", 1, NULL, NULL);
	int labelStart = 0, labelLen = 0;

	for (int i = 0; i < arr.arraySize; i++) {
		checkLabelLength(&arr.labelsArray[i]);
		WriteConsoleA(stdOut, arr.labelsArray[i]->labelText, strlen(arr.labelsArray[i]->labelText), NULL, NULL);

		FAIL_CHECK(setConsoleCursorPosition(cursorPos.X + 1, cursorPos.Y))
		for (int j = 0; j < arr.labelsArray[i]->width; j++)
			WriteConsoleW(stdOut, L"═", 1, NULL, NULL);

		if (i == arr.arraySize - 1) {
			cursorPos = getConsoleCursorPosition();
			WriteConsoleW(stdOut, L"╣", 1, NULL, NULL);

			FAIL_CHECK(setConsoleCursorPosition(cursorPos.X, cursorPos.Y + 1))
			WriteConsoleW(stdOut, L"║", 1, NULL, NULL);
		}
		else {
			cursorPos = getConsoleCursorPosition();
			WriteConsoleW(stdOut, L"╬", 1, NULL, NULL);
			FAIL_CHECK(setConsoleCursorPosition(cursorPos.X, cursorPos.Y + 1))
			WriteConsoleW(stdOut, L"║", 1, NULL, NULL);
		}

		free(arr.labelsArray[i]);
	}

	free(arr.labelsArray);
	FAIL_CHECK(setConsoleCursorPosition(initCursorPos.X, cursorPos.Y + 2))
	return 1;
}

//Incheie afisarea unui tabel.
int endTable() {
	NULL_CHECK(currentArr.labelsArray)

	HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	NULL_CHECK(stdOut)

	WriteConsoleW(stdOut, L"╚", 1, NULL, NULL);

	for (int i = 0; i < currentArr.arraySize; i++) {
		for (int j = 0; j < currentArr.labelsArray[i]->width; j++)
			WriteConsoleW(stdOut, L"═", 1, NULL, NULL);

		if(i == currentArr.arraySize - 1)
			WriteConsoleW(stdOut, L"╝", 1, NULL, NULL);
		else
			WriteConsoleW(stdOut, L"╩", 1, NULL, NULL);

		free(currentArr.labelsArray[i]);
	}

	free(currentArr.labelsArray);
	printf("\n");
	currentArr = createTableLabelsArray();
	return 1;
}