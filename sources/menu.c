#include <conio.h>

#include "menu.h"
#include "file.h"
#include "time_f.h"
#include "string_map.h"
#include "tabel.h"

#define ANSI_UNDERLINED_PRE  "\033[4m"
#define ANSI_UNDERLINED_POST "\033[0m"
#define ERROR_CHECK(x) if(!x) {MessageBox(getConsoleHwnd(), L"A intervenit o eroare! Aplicatia se va inchide...", L"Eroare", MB_OK | MB_ICONEXCLAMATION);\
							   exit(0);}

StringMapArray optionsArr;
int showMainMenu();
void showThirdMenu(c_char * operation);

//Afiseaza in consola sirul de caractere `error` cu text colorat in rosu.
void showError(c_char error[]) {
	setConsoleTextColor(4); 
	printf("\n%s", error);
	setConsoleTextColor(7);
}

//Afiseaza in consola un mesaj de avertizare cand optiunea 
//pe care a ales-o utilizatorul nu e valida.
void showWrongInputWarning(c_char* message) {
	showError(message);
	Sleep(2000);

	COORD cursorPos = getConsoleCursorPosition();
	ERROR_CHECK(clearLines(0, cursorPos.Y - 1, 0, cursorPos.Y - 1, 2))
}

//Afiseaza in consola o bara de incarcare.
void showLoadingBar()
{
	system("color 0A");
	char a = 177, b = 219;

	printf("\n\n\n\n");
	printf("\n\n\n\n\t\t\t\t\tSe incarca...\n\n");
	printf("\t\t\t\t\t");

	for (int i = 0; i < 26; i++)
		printf("%c", a);

	printf("\r");
	printf("\t\t\t\t\t");

	for (int i = 0; i < 26; i++) {
		printf("%c", b);
		Sleep(100);
	}

	clearLines(0, 8, 0, 0, 3);
}

//Afiseaza animatia de incarcare cand se verifica disponibilitatea cabinetului.
void showLoadingAnimation() {
	printf("\nSe verifica disponibilitatea cabinetului");
	for (int i = 0; i < 5; i++) {
		Sleep(500);
		printf(".");

		Sleep(500);
		printf(".");

		Sleep(500);
		printf(".");

		Sleep(500);
		ERROR_CHECK(setConsoleCursorPosition(40, 17))

		printf("%*.*s", 3, 3, " ");
		ERROR_CHECK(setConsoleCursorPosition(40, 17))
	}

	ERROR_CHECK(clearLines(0, 17, 0, 17, 1))
}

//Afiseaza in consola titlul aplicatiei sub forma de Ascii Artwork.
void showAppTitle() {
	ERROR_CHECK(setConsoleCursorPosition(0, 0))

	setConsoleTextColor(10);
	char appTitle[6][59] = {" _______   _______ .__   __. .___________.  ______ \n",
							"|       \\ |   ____||  \\ |  | |           | /      |\n",
							"|  .--.  ||  |__   |   \\|  | `---|  |----`|  ,----'\n",
							"|  |  |  ||   __|  |  . `  |     |  |     |  |     \n",
							"|  '--'  ||  |____ |  |\\   |     |  |     |  `----.\n",
							"|_______/ |_______||__| \\__|     |__|      \\______|\n\n\n\n\n\n\n" };

	COORD consoleSize = getConsoleSize();
	short titleStartPos = (consoleSize.X - 51) / 2;

	for (int i = 0; i < 6; i++)
		printf("% *.*s %s", titleStartPos, titleStartPos, " ", appTitle[i]);

	setConsoleTextColor(7);
}

char currentUser[21]; //contine numele utilizatorului curent.

//Afiseaza mesajul de bun venit in functie de momentul zilei 
//in care e accesata aplicatia.
int showMainMenuWelcomeMessageByDateTime() {
	timeInfo* nTime = getLocalDatetime();
	NULL_CHECK_EX(nTime,
		showError("A intervenit o eroare. Programul se va inchide acum...");
		Sleep(3000);
		return 0;
	)

	if (nTime->hour.hour >= 0 && nTime->hour.hour < 11) 
		printf("Buna dimineata, %s! Apasati o cifra in functie de optiunea pe care o doriti:\n", currentUser);
	else if (nTime->hour.hour >= 11 && nTime->hour.hour < 19)
		printf("Buna ziua, %s! Apasati o cifra in functie de optiunea pe care o doriti:\n", currentUser);
	else 
		printf("Buna seara, %s! Apasati o cifra in functie de optiunea pe care o doriti:\n", currentUser);

	free(nTime);

	return 1;
}

//Afiseaza optiunile meniului principal.
void showMainMenuOptions() {
	printf("> 1. Vizualizare oferte disponibile\n");
	printf("> 2. Verificati disponibilitate cabinet\n");
	printf("> 3. Programare pentru o anumita zi\n");
	printf("> 4. Vedeti istoricul programarilor\n");
	printf("> 5. Schimbare detalii cont\n");
	printf("> 6. Iesire din aplicatie\n");
	printf("> 7. Iesire din cont\n");
}

//Afiseaza operatiile posibile pentru serviciul ales de utilizator.
void showOperationsByUserChoice(int option) {
	switch (option) {
	case '1': case 27:
		ERROR_CHECK(showMainMenu())
			break;
	case 'c':
		printf("Alegeti operatia:\n");
		printf("> a. %s\n", findValueByKey(optionsArr, "c.a"));
		printf("> b. %s\n", findValueByKey(optionsArr, "c.b"));
		printf("> c. %s\n", findValueByKey(optionsArr, "c.c"));
		printf("> d. %s\n", findValueByKey(optionsArr, "c.d"));
		printf("> e. %s\n", findValueByKey(optionsArr, "c.e"));
		printf("> f. %s\n", findValueByKey(optionsArr, "c.f"));
		printf("> Meniul principal (ESC)\n");
		break;
	case 'd':
		printf("Alegeti operatia:\n");
		printf("> a. %s\n", findValueByKey(optionsArr, "d.a"));
		printf("> b. %s\n", findValueByKey(optionsArr, "d.b"));
		printf("> c. %s\n", findValueByKey(optionsArr, "d.c"));
		printf("> d. %s\n", findValueByKey(optionsArr, "d.d"));
		printf("> e. %s\n", findValueByKey(optionsArr, "d.e"));
		printf("> f. %s\n", findValueByKey(optionsArr, "d.f"));
		printf("> Meniul principal (ESC)\n");
		break;
	case 'e':
		printf("Alegeti operatia:\n");
		printf("> a. %s\n", findValueByKey(optionsArr, "e.a"));
		printf("> b. %s\n", findValueByKey(optionsArr, "e.b"));
		printf("> c. %s\n", findValueByKey(optionsArr, "e.c"));
		printf("> d. %s\n", findValueByKey(optionsArr, "e.d"));
		printf("> e. %s\n", findValueByKey(optionsArr, "e.e"));
		printf("> f. %s\n", findValueByKey(optionsArr, "e.f"));
		printf("> Meniul principal (ESC)\n");
		break;
	case 'f':
		printf("Alegeti operatia:\n");
		printf("> a. %s\n", findValueByKey(optionsArr, "f.a"));
		printf("> b. %s\n", findValueByKey(optionsArr, "f.b"));
		printf("> c. %s\n", findValueByKey(optionsArr, "f.c"));
		printf("> d. %s\n", findValueByKey(optionsArr, "f.d"));
		printf("> e. %s\n", findValueByKey(optionsArr, "f.e"));
		printf("> f. %s\n", findValueByKey(optionsArr, "f.f"));
		printf("> Meniul principal (ESC)\n");
		break;
	case 'g':
		printf("Alegeti operatia:\n");
		printf("> a. %s\n", findValueByKey(optionsArr, "g.a"));
		printf("> b. %s\n", findValueByKey(optionsArr, "g.b"));
		printf("> c. %s\n", findValueByKey(optionsArr, "g.c"));
		printf("> d. %s\n", findValueByKey(optionsArr, "g.d"));
		printf("> e. %s\n", findValueByKey(optionsArr, "g.e"));
		printf("> f. %s\n", findValueByKey(optionsArr, "g.f"));
		printf("> Meniul principal (ESC)\n");
		break;
	default:
		break;
	}
}

//Afiseaza in coltul din dreapta jos al consolei un text 
//cu numele autorului si versiunea aplicatiei.
void showCreatorInfo() {
	COORD cursorPos = getConsoleCursorPosition();
	COORD consoleSize = getConsoleSize();
	ERROR_CHECK(setConsoleCursorPosition(0, consoleSize.Y - 1))

	setConsoleTextColor(9);
	printf("%*.*s %s", consoleSize.X - 36, consoleSize.X - 36, " ", "Creat de Boatca Emanuel. DentC V1.0");

	ERROR_CHECK(setConsoleCursorPosition(cursorPos.X, cursorPos.Y))
	setConsoleTextColor(7);
}

//Afiseaza datele unei programari.
void showAppointment(c_char* fName, c_char* lName, dateInfo date, hourInfo hour, c_char* operation) {
	appointmentData app;
	app.fName = fName;
	app.lName = lName;
	app.username = currentUser;
	app.dateTime = createTimeInfoDH(date, hour);
	app.operation = operation;

	int result = saveAppointment("programari.pdentc", app);
	setConsoleTextColor(5);
	printf("Programarea cu datele:\n\n");
	setConsoleTextColor(7);

	printf("Nume: %s\n", fName);
	printf("Prenume: %s\n", lName);
	printf("Data: %d/%d/%d\n", date.day, date.month, date.year);

	if(hour.minute == 0)
		printf("Ora: %d:00\n", hour.hour);
	else
		printf("Ora: %d:30\n", hour.hour);
	printf("Operatia: %s\n", operation);

	setConsoleTextColor(5);
	if (result)
		printf("\na fost efectuata cu succes. Multumim!");
	else
		printf("\nnu a fost efectuata cu succes. Va rugam sa incercati din nou!");
	setConsoleTextColor(7);
}

MenuTypes currentMenu = MAIN_MENU; //contine tipul de meniu care este afisat la un moment dat in consola.

//Afiseaza in consola meniul principal.
int showMainMenu() {
	currentMenu = MAIN_MENU;
	ERROR_CHECK(clearLines(0, 7, 0, 12, 20))

	if (!showMainMenuWelcomeMessageByDateTime())
		return 0;

	showMainMenuOptions();
	showCreatorInfo();

	return 1;
}

//Afiseaza un ecran unde utilizatorul isi poate crea un cont.
int showSignUpScreen() {
	ERROR_CHECK(clearLines(0, 12, 0, 12, 15))

	COORD consoleSize = getConsoleSize();
	setConsoleTextColor(8);
	printf("%*.*s*Creare cont*\n\n\n\n\n", (consoleSize.X - 13) / 2, (consoleSize.X - 13) / 2, " ");
	setConsoleTextColor(7);

	printf("Introduceti numele de utilizator: \n");
	printf("Introduceti parola: \n");
	printf("Reintroduceti parola: \n");
	printf("\n> Inapoi (ESC)");

	showConsoleCursor();
	COORD cursorPos = getConsoleCursorPosition();
	ERROR_CHECK(setConsoleCursorPosition(34, cursorPos.Y - 4))

	showConsoleCursor();
	char username[21];
	int result = getNCharacters(20, username);
	ERROR_CHECK(result)

	if (result == -1)
		return -1;

	while (searchForUsername("dentc.data", username)) {
		ERROR_CHECK(setConsoleCursorPosition(0, cursorPos.Y + 2))
		showWrongInputWarning("Nume de utilizator folosit. Introduceti alt nume de utilizator!");

		ERROR_CHECK(setConsoleCursorPosition(34, cursorPos.Y - 4))
		printf("%*.*s", 21, 21, " ");

		ERROR_CHECK(setConsoleCursorPosition(34, cursorPos.Y - 4))
		result = getNCharacters(20, username);
		ERROR_CHECK(result)

		if (result == -1)
			return -1;
	}

	ERROR_CHECK(setConsoleCursorPosition(20, cursorPos.Y - 3))
	char password1[21];
	result = getPassword(20, password1);
	ERROR_CHECK(result)

	if (result == -1)
		return -1;

	while (strlen(password1) < 8) {
		ERROR_CHECK(setConsoleCursorPosition(0, cursorPos.Y + 2))
		showWrongInputWarning("Parola trebuie sa aiba minim 8 caractere!");

		ERROR_CHECK(setConsoleCursorPosition(20, cursorPos.Y - 3))
		printf("%*.*s", 21, 21, " ");

		ERROR_CHECK(setConsoleCursorPosition(20, cursorPos.Y - 3))
		result = getPassword(20, password1);
		ERROR_CHECK(result)

		if (result == -1)
			return -1;
	}

	ERROR_CHECK(setConsoleCursorPosition(22, cursorPos.Y - 2))
	char password2[21];
	result = getPassword(20, password2);
	ERROR_CHECK(result)

	if (result == -1)
		return -1;

	while (strcmp(password1, password2)) {
		ERROR_CHECK(setConsoleCursorPosition(0, cursorPos.Y + 2))
		showWrongInputWarning("Parolele nu se potrivesc. Incercati din nou!");

		ERROR_CHECK(setConsoleCursorPosition(22, cursorPos.Y - 2))
		printf("%*.*s", 21, 21, " ");
		
		ERROR_CHECK(setConsoleCursorPosition(20, cursorPos.Y - 3))
		printf("%*.*s", 21, 21, " ");

		ERROR_CHECK(setConsoleCursorPosition(20, cursorPos.Y - 3))
		result = getPassword(20, password1);
		ERROR_CHECK(result)

		if (result == -1)
			return -1;

		ERROR_CHECK(setConsoleCursorPosition(22, cursorPos.Y - 2))
		result = getPassword(20, password2);
		ERROR_CHECK(result)

		if (result == -1)
			return -1;
	}

	char* usersNr = getVariable("dentc.data", "usersNr");
	int usersIntNr;
	char nr[10];

	if (usersNr == NULL) {
		setVariable("dentc.data", "usersNr", "1");
		usersIntNr = 1;
		nr[0] = '1';
		nr[1] = 0;
	}
	else {
		sscanf(usersNr, "%d", &usersIntNr);
		usersIntNr++;
		sprintf(nr, "%d", usersIntNr);
		setVariable("dentc.data", "usersNr", nr);
	}

	char usernameVarName[25];
	char passwordVarName[25];

	strcpy(usernameVarName, "username");
	strcpy(passwordVarName, "password");
	strcat(usernameVarName, nr);
	strcat(passwordVarName, nr);

	int varRes1 = setVariable("dentc.data", usernameVarName, username);
	int varRes2 = setVariable("dentc.data", passwordVarName, password1);

	strcpy(currentUser, username);

	ERROR_CHECK(setConsoleCursorPosition(0, cursorPos.Y + 2))
	if (varRes1 != 1 || varRes2 != 1) {
		showWrongInputWarning("Contul nu a putut fi creat. Incercati din nou!");
		return -1;
	}
	else {
		setConsoleTextColor(10);
		printf("Contul a fost creat cu succes!");
		Sleep(3000);
		setConsoleTextColor(7);
		strcpy(currentUser, username);
		return 1;
	}
}

//Afiseaza un ecran unde utilizatorul trebuie sa intre in cont.
void showLoginScreen() {
	showAppTitle();
	showCreatorInfo();

	char username[21], password[21];
	int result;
	COORD cursorPos;

	while (1) {
		ERROR_CHECK(clearLines(0, 12, 0, 12, 10))
		COORD consoleSize = getConsoleSize();
		setConsoleTextColor(8);
		printf("%*.*s*Autentificare*\n\n\n\n\n", (consoleSize.X - 15) / 2, (consoleSize.X - 15) / 2, " ");
		setConsoleTextColor(7);
		printf("Introduceti numele de utilizator: \n");
		printf("Introduceti parola: \n");
		printf("\n> Creare cont (ESC)");

		cursorPos = getConsoleCursorPosition();
		ERROR_CHECK(setConsoleCursorPosition(34, cursorPos.Y - 3))

		showConsoleCursor();
		result = getNCharacters(20, username);
		ERROR_CHECK(result)

		if (result == -1) {
			hideConsoleCursor();
			result = showSignUpScreen();
			ERROR_CHECK(result)

			if (result == -1)
				continue;
			else
				break;
		}

		ERROR_CHECK(setConsoleCursorPosition(20, cursorPos.Y - 2))
		result = getPassword(20, password);

		if (result == -1) {
			hideConsoleCursor();
			result = showSignUpScreen();
			ERROR_CHECK(result)

			if (result == -1)
				continue;
			else
				break;
		}

		while (!checkUsernameAndPassword("dentc.data", username, password)) {
			ERROR_CHECK(setConsoleCursorPosition(0, cursorPos.Y + 2))
			showWrongInputWarning("Nume de utilizator sau parola incorecte sau nu exista acest utilizator!");

			ERROR_CHECK(setConsoleCursorPosition(20, cursorPos.Y - 2))
			printf("%*.*s", 21, 21, " ");

			ERROR_CHECK(setConsoleCursorPosition(34, cursorPos.Y - 3))
			printf("%*.*s", 21, 21, " ");

			ERROR_CHECK(setConsoleCursorPosition(34, cursorPos.Y - 3))
			result = getNCharacters(20, username);
			ERROR_CHECK(result)

			if (result == -1) {
				hideConsoleCursor();
				result = showSignUpScreen();
				ERROR_CHECK(result)
				break;
			}

			ERROR_CHECK(setConsoleCursorPosition(20, cursorPos.Y - 2))
			result = getPassword(20, password);

			if (result == -1) {
				hideConsoleCursor();
				result = showSignUpScreen();
				ERROR_CHECK(result)
				break;
			}
		}

		if (result == -1)
			continue;
		else {
			if(!strlen(currentUser))
				strcpy(currentUser, username);
			break;
		}
	}
	
	hideConsoleCursor();
	ERROR_CHECK(showMainMenu())
}

//Cauta a n-a aparitie a unui caracter intr-un string, cautarea se face de la coada la cap.
int findNthOcurrenceOfCharacterReverse(c_char* string, c_char ch, int n) {
	NULL_CHECK(string)

	int i = strlen(string) - 1;
	while (n > 0 && i >= 0) {
		if (string[i] == ch)
			n--;
		i--;
	}

	if (i == -1)
		return i;

	return i + 1;
}

//Afiseaza in consola meniul 1.
void showFirstMenu() {
	currentMenu = MENU_1;
	COORD cursorPos = getConsoleCursorPosition();
	COORD consoleSize = getConsoleSize();
	ERROR_CHECK(clearLines(0, 12, 0, 8, cursorPos.Y - 11))

	setConsoleTextColor(8);
	printf("%*.*s*Vizualizare oferte disponibile*\n\n ",
		(consoleSize.X - 32) / 2, (consoleSize.X - 32) / 2, " ");
	setConsoleTextColor(7);

	tableLabelsArray arr = createTableLabelsArray();
	ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Nr", 4)))
	ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Operatia", 63)))
	ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Vechiul pret", 20)))
	ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Noul pret", 20)))
	startTabel(arr);

	srand(time(0));
	int index = (rand() % 5) * 6 + 7;
	int newPrices[5];

	for (int i = 0; i <= 4; i++) {
		char nr[10];
		sprintf(nr, "%d", i+1);
		arr = createTableLabelsArray();

		char* operation = optionsArr.array[index + i]->value.mapValue;
		int parInd = findNthOcurrenceOfCharacterReverse(operation, '(', 1);
		int operLen = strlen(operation);
		operation[parInd - 1] = 0;

		ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(nr, 4)))
		ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(operation, 63)))
		operation[parInd - 1] = ' ';
		operation[operLen - 1] = 0;

		char* priceStr = operation + parInd + 1;
		ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(priceStr, 20)))

		int priceInt;
		sscanf(priceStr, "%d RON", &priceInt);

		priceInt -= (float)priceInt * ((float)1 / (rand() % 5 + 2));
		sprintf(nr, "%d RON", priceInt);
		ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(nr, 20)))

		newPrices[i] = priceInt;
		operation[operLen - 1] = ')';
		addRow(arr);
	}

	endTable();
	setConsoleTextColor(5);
	printf("> Apasati pe o cifra pentru a face o programare in functie de optiunea pe care o doriti\n\n");
	setConsoleTextColor(7);
	printf("> Inapoi (ESC)\n");

	int option , br = 0, i;

	while (!br) {
		option = getch();

		switch (option){
		case '1': 
			i = 0;
			br = 1;
			break;
		case '2':
			i = 1;
			br = 1;
			break;
		case '3':
			i = 2;
			br = 1;
			break;
		case '4':
			i = 3;
			br = 1;
			break;
		case '5':
			i = 4;
			br = 1;
			break;
		case 27:
			ERROR_CHECK(showMainMenu());
			return;
		default:
			showWrongInputWarning("Optiunea aleasa nu e valida!");
			break;
		}
	}

	char* operation = NULL, * initOp;
	initOp = optionsArr.array[index + i]->value.mapValue;
	operation = malloc(strlen(initOp) + 1);
	ERROR_CHECK(operation)

	int parInd = findNthOcurrenceOfCharacterReverse(initOp, '(', 1);
	initOp[parInd - 1] = 0;
	sprintf(operation, "%s (%d RON)", initOp, newPrices[i]);
	initOp[parInd - 1] = ' ';

	showThirdMenu(operation);
	free(operation);
}

//Afiseaza in consola meniul 2.
void showSecondMenu() {
	currentMenu = MENU_2;
	COORD cursorPos = getConsoleCursorPosition();
	COORD consoleSize = getConsoleSize();

	ERROR_CHECK(clearLines(0, 12, 0, 10, cursorPos.Y - 11))

	setConsoleTextColor(6);
	printf(">>> " ANSI_UNDERLINED_PRE "Cabinetul DentC este deschis de luni pana vineri intre orele 8:00 - 16:00" ANSI_UNDERLINED_POST);
	setConsoleTextColor(6);
	printf(" <<< \n");

	setConsoleTextColor(6);
	printf("%*.*s", 11, 11, " ");
	printf(">>> " ANSI_UNDERLINED_PRE "Programarile se pot face doar din 30 in 30 de minute" ANSI_UNDERLINED_POST);
	setConsoleTextColor(6);
	printf(" <<< \n\n\n");

	setConsoleTextColor(8);
	printf("%*.*s*Verificare disponibilitate cabinet*\n\n", (consoleSize.X - 36) / 2, (consoleSize.X - 36) / 2, " ");

	setConsoleTextColor(7);
	printf("Introduceti data (ZZ/LL/AAAA): \n");
	printf("Introduceti ora (OO:MM): \n");
	printf("\n> Inapoi (ESC)\n");

	ERROR_CHECK(setConsoleCursorPosition(31, 16))
	showConsoleCursor();

	dateInfo date;
	char line[10];
	int result = getNCharacters(10, line);
	sscanf(line, "%d/%d/%d", &date.day, &date.month, &date.year);

	while (result != -1 && !checkDate(date)) {
		ERROR_CHECK(setConsoleCursorPosition(0, 20))
		showWrongInputWarning("Data introdusa nu este valida!");

		ERROR_CHECK(setConsoleCursorPosition(31, 16))
		printf("%*.*s", consoleSize.X - 31, consoleSize.X - 31, " ");

		ERROR_CHECK(setConsoleCursorPosition(31, 16))
		result = getNCharacters(10, line);
		sscanf(line, "%d/%d/%d", &date.day, &date.month, &date.year);
	}

	if (result == -1) {
		hideConsoleCursor();
		ERROR_CHECK(showMainMenu())
		return;
	}

	ERROR_CHECK(setConsoleCursorPosition(25, 17))

	hourInfo hour;
	result = getNCharacters(10, line);
	sscanf(line, "%d:%d", &hour.hour, &hour.minute);

	while (result != -1 && !checkHour(hour)) {
		ERROR_CHECK(setConsoleCursorPosition(0, 20))
		showWrongInputWarning("Ora introdusa nu este valida!");

		ERROR_CHECK(setConsoleCursorPosition(25, 17))
		printf("%*.*s", consoleSize.X - 31, consoleSize.X - 31, " ");

		ERROR_CHECK(setConsoleCursorPosition(25, 17))
		result = getNCharacters(10, line);
		sscanf(line, "%d:%d", &hour.hour, &hour.minute);
	}

	if (result == -1) {
		hideConsoleCursor();
		ERROR_CHECK(showMainMenu())
		return;
	}

	hideConsoleCursor();
	ERROR_CHECK(clearLines(0, 16, 0, 16, 5))
	showLoadingAnimation();
	result = searchForAppointment("programari.pdentc", createTimeInfoDH(date, hour));

	if (!result) {
		setConsoleTextColor(10);
		printf("Cabinetul este disponibil la data si ora tastate!\n          Puteti face programarea alegand optiunea 3 din meniul principal.");
		setConsoleTextColor(7);
		Sleep(10000);
	}
	else {
		showError("Cabinetul nu este disponibil la data si ora tastate!\n          Incercati din nou cu alta data sau ora.");
		Sleep(10000);
	}

	ERROR_CHECK(showMainMenu())
}

//Afiseaza submeniul 3.1
void showSubMenu31(dateInfo date, hourInfo hour, c_char* operation) {
	COORD consoleSize = getConsoleSize();
	ERROR_CHECK(clearLines(0, 17, 0, 17, 3))

	printf("Introduceti numele: \n");
	printf("Introduceti prenumele: \n");
	printf("\n > Meniul principal (ESC)");
	showConsoleCursor();

	char fName[21];
	ERROR_CHECK(setConsoleCursorPosition(20, 17))
		int result = getNCharacters(20, fName);
	if (result == -1) {
		hideConsoleCursor();
		ERROR_CHECK(showMainMenu())
			return;
	}

	char lName[21];
	ERROR_CHECK(setConsoleCursorPosition(23, 18))
		result = getNCharacters(20, lName);
	if (result == -1) {
		hideConsoleCursor();
		ERROR_CHECK(showMainMenu())
			return;
	}

	hideConsoleCursor();
	ERROR_CHECK(clearLines(0, 17, 0, 17, 4))

	if(!operation) {
		printf("Alegeti ce tip de serviciu doriti:\n");
		printf("> a. %s\n", findValueByKey(optionsArr, "a"));
		printf("> b. %s\n", findValueByKey(optionsArr, "b"));
		printf("> c. %s\n", findValueByKey(optionsArr, "c"));
		printf("> d. %s\n", findValueByKey(optionsArr, "d"));
		printf("> e. %s\n", findValueByKey(optionsArr, "e"));
		printf("> f. %s\n", findValueByKey(optionsArr, "f"));
		printf("> g. %s\n", findValueByKey(optionsArr, "g"));
		printf("\n> Meniul principal (ESC)");

		int serviceOption = getch();
		if (serviceOption == '1' || serviceOption == 27) {
			ERROR_CHECK(showMainMenu())
			return;
		}

		while (serviceOption != '1' && serviceOption != 27
			&& (serviceOption < 'a' || serviceOption > 'g')) {
			showWrongInputWarning("Optiunea aleasa nu e valida!");
			serviceOption = getch();
		}

		if (serviceOption == '1' || serviceOption == 27) {
			ERROR_CHECK(showMainMenu())
			return;
		}

		ERROR_CHECK(clearLines(0, 17, 0, 17, 10))
		if (serviceOption == 'a' || serviceOption == 'b') {
			char oper[] = { serviceOption, '\0' };
			showAppointment(fName, lName, date, hour, findValueByKey(optionsArr, oper));
			Sleep(20000);
			ERROR_CHECK(showMainMenu());
			return;
		}

		ERROR_CHECK(clearLines(0, 17, 0, 17, 10))
		showOperationsByUserChoice(serviceOption);

		int operOption = getch();
		if (operOption == '1' || operOption == 27) {
			ERROR_CHECK(showMainMenu())
			return;
		}

		while (operOption != '1' && operOption != 27
			&& (operOption < 'a' || operOption > 'f')) {
			showWrongInputWarning("Optiunea aleasa nu e valida!");
			operOption = getch();
		}

		if (operOption == '1' || operOption == 27) {
			ERROR_CHECK(showMainMenu())
			return;
		}

		ERROR_CHECK(clearLines(0, 17, 0, 17, 10))

		char oper[] = { serviceOption, '.', operOption,  '\0' };
		showAppointment(fName, lName, date, hour, findValueByKey(optionsArr, oper));
	}
	else
		showAppointment(fName, lName, date, hour, operation);

	Sleep(20000);
	ERROR_CHECK(showMainMenu());
}

//Afiseaza in consola meniul 3.
void showThirdMenu(c_char* operation) {
	currentMenu = MENU_3;
	COORD cursorPos = getConsoleCursorPosition();
	COORD consoleSize = getConsoleSize();

	ERROR_CHECK(clearLines(0, 8, 0, 10, 19))
	
	setConsoleTextColor(6);
	printf(">>> " ANSI_UNDERLINED_PRE "Cabinetul DentC este deschis de luni pana vineri intre orele 8:00 - 16:00" ANSI_UNDERLINED_POST);
	setConsoleTextColor(6);
	printf(" <<< \n");

	setConsoleTextColor(6);
	printf("%*.*s", 11, 11, " ");
	printf(">>> " ANSI_UNDERLINED_PRE "Programarile se pot face doar din 30 in 30 de minute" ANSI_UNDERLINED_POST);
	setConsoleTextColor(6);
	printf(" <<< \n\n\n");

	setConsoleTextColor(8);
	printf("%*.*s*Programare pentru o anumita zi*\n\n", (consoleSize.X - 30) / 2, (consoleSize.X - 30) / 2, " ");

	setConsoleTextColor(7);
	printf("Introduceti data (ZZ/LL/AAAA): \n");
	printf("Introduceti ora (OO:MM): \n");
	printf("\n> Inapoi (ESC)\n");

	ERROR_CHECK(setConsoleCursorPosition(31, 16))
	showConsoleCursor();

	dateInfo date;
	char line[10];
	int result = getNCharacters(10, line);
	sscanf(line, "%d/%d/%d", &date.day, &date.month, &date.year);

	while (result != -1 && !checkDate(date)) {
		ERROR_CHECK(setConsoleCursorPosition(0, 20))
		showWrongInputWarning("Data introdusa nu este valida!");

		ERROR_CHECK(setConsoleCursorPosition(31, 16))
		printf("%*.*s", consoleSize.X - 31, consoleSize.X - 31, " ");

		ERROR_CHECK(setConsoleCursorPosition(31, 16))
		result = getNCharacters(10, line);
		sscanf(line, "%d/%d/%d", &date.day, &date.month, &date.year);
	}

	if (result == -1) {
		hideConsoleCursor();
		ERROR_CHECK(showMainMenu())
		return;
	}

	ERROR_CHECK(setConsoleCursorPosition(25, 17))

	hourInfo hour;
	result = getNCharacters(10, line);
	sscanf(line, "%d:%d", &hour.hour, &hour.minute);

	while (result != -1 && !checkHour(hour)) {
		ERROR_CHECK(setConsoleCursorPosition(0, 20))
		showWrongInputWarning("Ora introdusa nu este valida!");

		ERROR_CHECK(setConsoleCursorPosition(25, 17))
		printf("%*.*s", consoleSize.X - 31, consoleSize.X - 31, " ");

		ERROR_CHECK(setConsoleCursorPosition(25, 17))
		result = getNCharacters(10, line);
		sscanf(line, "%d:%d", &hour.hour, &hour.minute);
	}

	if (result == -1) {
		hideConsoleCursor();
		ERROR_CHECK(showMainMenu())
		return;
	}

	hideConsoleCursor();
	ERROR_CHECK(clearLines(0, 16, 0, 16, 5))
	showLoadingAnimation();
	result = searchForAppointment("programari.pdentc", createTimeInfoDH(date, hour));

	if (!result) {
		printf("Cabinetul este disponibil la data si ora tastate. Doriti sa faceti programarea?\n");
		printf("> 1. Da\n");
		printf("> 2. Nu\n");

		int opt = getch();
		while (opt != '1' && opt != '2') {
			showWrongInputWarning("Optiunea aleasa nu e valida!");
			opt = getch();
		}

		if (opt == '1') {
			showSubMenu31(date, hour, operation);
			return;
		}
	}
	else {
		showError("Cabinetul nu este disponibil la data si ora tastate!\n");
		Sleep(10000);
	}

	ERROR_CHECK(showMainMenu())
}

//Afiseaza in consola meniul 4.
void showFourthMenu() {
	currentMenu = MENU_4;
	COORD cursorPos = getConsoleCursorPosition();
	COORD consoleSize = getConsoleSize();
	ERROR_CHECK(clearLines(0, 12, 0, 8, cursorPos.Y - 11))

	setConsoleTextColor(8);
	printf("%*.*s*Vedeti istoricul programarilor*\n\n ",
		(consoleSize.X - 30) / 2, (consoleSize.X - 30) / 2, " ");
	setConsoleTextColor(7);

	appointmentDataArray appArr = getUserAppointments("programari.pdentc", currentUser);
	tableLabelsArray arr;
	char nr[4];

	if (appArr.elemNumber > 0) {
		arr = createTableLabelsArray();
		ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Nr", 4)))
		ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Nume", 20)))
		ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Prenume", 20)))
		ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Data", 12)))
		ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Ora", 7)))
		ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Operatia", 42)))
		startTabel(arr);

		if (appArr.elemNumber <= 6) {
			for (int i = 0; i < appArr.elemNumber; i++) {
				arr = createTableLabelsArray();
				sprintf(nr, "%d", i + 1);
				char date[10], hour[5];
				getDateAsString(appArr.array[i]->dateTime.date, date);
				getHourAsString(appArr.array[i]->dateTime.hour, hour);

				arr = createTableLabelsArray();
				ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(nr, 4)))
				ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(appArr.array[i]->fName, 20)))
				ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(appArr.array[i]->lName, 20)))
				ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(date, 12)))
				ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(hour, 7)))
				ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(appArr.array[i]->operation, 42)))
				addRow(arr);
			}

			endTable();
			printf("\n > Inapoi(ESC)");
		}
		else {
			int start = 0, end = 5;
			while (1) {
				Sleep(0.5);
				ERROR_CHECK(clearLines(0, 10, 1, 10, 15))

				arr = createTableLabelsArray();
				ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Nr", 4)))
				ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Nume", 20)))
				ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Prenume", 20)))
				ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Data", 12)))
				ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Ora", 7)))
				ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel("Operatia", 42)))
				startTabel(arr);

				for (int i = start; i <= end; i++) {
					arr = createTableLabelsArray();
					sprintf(nr, "%d", i + 1);
					char date[10], hour[5];
					getDateAsString(appArr.array[i]->dateTime.date, date);
					getHourAsString(appArr.array[i]->dateTime.hour, hour);

					arr = createTableLabelsArray();
					ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(nr, 4)))
					ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(appArr.array[i]->fName, 20)))
					ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(appArr.array[i]->lName, 20)))
					ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(date, 12)))
					ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(hour, 7)))
					ERROR_CHECK(addToTableLabelsArray(&arr, createTableLabel(appArr.array[i]->operation, 42)))
					addRow(arr);
				}

				endTable();
				printf("\n > Inapoi(ESC)%*.*s", 50, 50, " ");
				setConsoleTextColor(6);
				WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), L"˄ Folositi sagetile pentru a naviga prin lista ˅\n", 49, 0, 0);
				setConsoleTextColor(7);

				int updateN = 0, userInput;

				while (!updateN) {
					userInput = getch();
					if (userInput == 224)
						userInput = getch();

					switch (userInput) {
					case 72:
						if (start > 0) {
							start--;
							end--;
							updateN = 1;
						}
						break;
					case 80:
						if (end < appArr.elemNumber - 1) {
							start++;
							end++;
							updateN = 1;
						}
						break;
					case 27:
						ERROR_CHECK(showMainMenu())
							return;
					default:
						break;
					}
				}
			}
		}
	}
	else {
		setConsoleTextColor(5);
		printf("\n\n\n  Nicio programare inregistrata. \n	Pentru a face o programare accesati meniul 3!\n");
		setConsoleTextColor(7);
		printf("\n > Inapoi(ESC)");
	}
}

//Afiseaza ecranul de schimbare a numelui de utilizator.
void showChangeUsernameScreen() {
	COORD consoleSize = getConsoleSize();
	COORD cursorPos = getConsoleCursorPosition();
	ERROR_CHECK(clearLines(0, 12, 0, 12, cursorPos.Y - 11))

	setConsoleTextColor(8);
	printf("%*.*s*Schimbare username*\n\n\n\n\n",
		(consoleSize.X - 19) / 2, (consoleSize.X - 19) / 2, " ");
	setConsoleTextColor(7);

	printf("Introduceti noul nume de utilizator: \n");
	printf("Introduceti parola: \n");
	printf("\n> Meniul principal (ESC)");

	cursorPos = getConsoleCursorPosition();
	ERROR_CHECK(setConsoleCursorPosition(37, cursorPos.Y - 3))
	showConsoleCursor();

	char username[21];
	int result = getNCharacters(20, username);
	ERROR_CHECK(result)

	if (result == -1) {
		hideConsoleCursor();
		ERROR_CHECK(showMainMenu())
		return;
	}

	while (searchForUsername("dentc.data", username)) {
		ERROR_CHECK(setConsoleCursorPosition(0, cursorPos.Y + 2))
		showWrongInputWarning("Nume de utilizator folosit. Introduceti alt nume de utilizator!");

		ERROR_CHECK(setConsoleCursorPosition(37, cursorPos.Y - 3))
		printf("%*.*s", 21, 21, " ");

		ERROR_CHECK(setConsoleCursorPosition(37, cursorPos.Y - 3))
		result = getNCharacters(20, username);
		ERROR_CHECK(result)

		if (result == -1) {
			hideConsoleCursor();
			ERROR_CHECK(showMainMenu())
			return;
		}
	}

	ERROR_CHECK(setConsoleCursorPosition(20, cursorPos.Y - 2))
	char password[21];
	result = getPassword(20, password);
	ERROR_CHECK(result)

	if (result == -1) {
		hideConsoleCursor();
		ERROR_CHECK(showMainMenu())
		return;
	}

	while (!checkUsernameAndPassword("dentc.data", currentUser, password)) {
		ERROR_CHECK(setConsoleCursorPosition(0, cursorPos.Y + 2))
		showWrongInputWarning("Parola incorecta!");

		ERROR_CHECK(setConsoleCursorPosition(20, cursorPos.Y - 2))
		printf("%*.*s", 21, 21, " ");

		ERROR_CHECK(setConsoleCursorPosition(20, cursorPos.Y - 2))
		result = getPassword(20, password);

		if (result == -1) {
			hideConsoleCursor();
			ERROR_CHECK(showMainMenu())
			return;
		}
	}

	int cuResult = changeValue("dentc.data", currentUser, username, 0);
	int coResult = changeEveryOcurrenceOfValue("programari.pdentc", currentUser, username);

	ERROR_CHECK(setConsoleCursorPosition(0, cursorPos.Y + 2))
	hideConsoleCursor();
	if (cuResult == -1 || coResult == -1) {
		showWrongInputWarning("A intervenit o eroare. Numele de utilizator nu a fost setat cu succes!");
		ERROR_CHECK(showMainMenu())
	}
	else {
		strcpy(currentUser, username);
		setConsoleTextColor(10);
		printf("Nume de utilizator schimbat cu succes!");
		Sleep(3000);
		setConsoleTextColor(7);
		ERROR_CHECK(showMainMenu())
	}
}

//Afiseaza ecranul de schimbare a parolei.
void showChangePasswordScreen() {
	COORD consoleSize = getConsoleSize();
	COORD cursorPos = getConsoleCursorPosition();
	ERROR_CHECK(clearLines(0, 12, 0, 12, cursorPos.Y - 11))

	setConsoleTextColor(8);
	printf("%*.*s*Schimbare parola*\n\n\n\n\n",
		(consoleSize.X - 18) / 2, (consoleSize.X - 18) / 2, " ");
	setConsoleTextColor(7);

	printf("Introduceti vechea parola: \n");
	printf("Introduceti noua parola: \n");
	printf("Confirmati parola: \n");
	printf("\n> Meniul principal (ESC)");

	showConsoleCursor();
	ERROR_CHECK(setConsoleCursorPosition(27, cursorPos.Y - 5))
	char oldPassword[21];
	int result = getPassword(20, oldPassword);

	if (result == -1) {
		hideConsoleCursor();
		ERROR_CHECK(showMainMenu())
		return;
	}

	while (!checkUsernameAndPassword("dentc.data", currentUser, oldPassword)) {
		ERROR_CHECK(setConsoleCursorPosition(0, cursorPos.Y + 2))
		showWrongInputWarning("Parola incorecta!");

		ERROR_CHECK(setConsoleCursorPosition(27, cursorPos.Y - 5))
		printf("%*.*s", 21, 21, " ");

		ERROR_CHECK(setConsoleCursorPosition(27, cursorPos.Y - 5))
		result = getPassword(20, oldPassword);

		if (result == -1) {
			hideConsoleCursor();
			ERROR_CHECK(showMainMenu())
			return;
		}
	}

	ERROR_CHECK(setConsoleCursorPosition(25, cursorPos.Y - 4))
	char password1[21];
	result = getPassword(20, password1);

	if (result == -1) {
		hideConsoleCursor();
		ERROR_CHECK(showMainMenu())
		return;
	}

	while (strlen(password1) < 8) {
		ERROR_CHECK(setConsoleCursorPosition(0, cursorPos.Y + 2))
		showWrongInputWarning("Parola trebuie sa aiba minim 8 caractere!");

		ERROR_CHECK(setConsoleCursorPosition(25, cursorPos.Y - 4))
		printf("%*.*s", 21, 21, " ");

		ERROR_CHECK(setConsoleCursorPosition(25, cursorPos.Y - 4))
		result = getPassword(20, password1);
		ERROR_CHECK(result)

		if (result == -1) {
			hideConsoleCursor();
			ERROR_CHECK(showMainMenu())
			return;
		}
	}

	ERROR_CHECK(setConsoleCursorPosition(19, cursorPos.Y - 3))
	char password2[21];
	result = getPassword(20, password2);

	if (result == -1) {
		hideConsoleCursor();
		ERROR_CHECK(showMainMenu())
		return;
	}

	while (strcmp(password1, password2)) {
		ERROR_CHECK(setConsoleCursorPosition(0, cursorPos.Y + 2))
		showWrongInputWarning("Parolele nu se potrivesc. Incercati din nou!");

		ERROR_CHECK(setConsoleCursorPosition(19, cursorPos.Y - 3))
		printf("%*.*s", 21, 21, " ");

		ERROR_CHECK(setConsoleCursorPosition(25, cursorPos.Y - 4))
		printf("%*.*s", 21, 21, " ");

		ERROR_CHECK(setConsoleCursorPosition(25, cursorPos.Y - 4))
		result = getPassword(20, password1);
		ERROR_CHECK(result)

		if (result == -1) {
			hideConsoleCursor();
			ERROR_CHECK(showMainMenu())
			return;
		}

		ERROR_CHECK(setConsoleCursorPosition(19, cursorPos.Y - 3))
		result = getPassword(20, password2);
		ERROR_CHECK(result)

		if (result == -1) {
			hideConsoleCursor();
			ERROR_CHECK(showMainMenu())
			return;
		}
	}

	int cpResult = changeValue("dentc.data", currentUser, password1, 1);

	hideConsoleCursor();
	ERROR_CHECK(setConsoleCursorPosition(0, cursorPos.Y + 2))
	if (cpResult == -1) {
		showWrongInputWarning("A intervenit o eroare. Noua parola nu a fost setat cu succes!");
		ERROR_CHECK(showMainMenu())
	}
	else {
		setConsoleTextColor(10);
		printf("Parola schimbata cu succes!");
		Sleep(3000);
		setConsoleTextColor(7);
		ERROR_CHECK(showMainMenu())
	}
}

//Afiseaza in consola meniul 5.
void showFifthMenu() {
	currentMenu = MENU_5;
	COORD cursorPos = getConsoleCursorPosition();
	COORD consoleSize = getConsoleSize();
	ERROR_CHECK(clearLines(0, 12, 0, 12, cursorPos.Y - 11))

	setConsoleTextColor(8);
	printf("%*.*s*Schimbare detalii cont*\n\n\n\n\n", 
		(consoleSize.X - 24) / 2, (consoleSize.X - 24) / 2, " ");
	setConsoleTextColor(7);

	printf("> 1. Schimbare username\n");
	printf("> 2. Schimbare parola\n");
	printf("\n> Inapoi (ESC)\n\n");

	int option;
	while (1) {
		Sleep(0.5);
		option = getch();
		switch (option) {
		case '1':
			showChangeUsernameScreen();
			return;
		case '2':
			showChangePasswordScreen();
			return;
		case 27:
			ERROR_CHECK(showMainMenu())
			return;
		default:
			showWrongInputWarning("Optiunea aleasa nu e valida!");
			break;
		}
	}

}

//Functie de iesire din program.
void exitProgram() {
	setConsoleTextColor(12);

	COORD cursorPos = getConsoleCursorPosition();
	setConsoleCursorPosition(0, cursorPos.Y);
	printf("Programul se va inchide... La revedere!");

	printf("-");
	for (int i = 1; i <= 100; i++) {
		Sleep(1);
		printf("\b\\");
		Sleep(1);
		printf("\b|");
		Sleep(1);
		printf("\b-");
		Sleep(1);
		printf("\b/");
	}

	exit(0);
}

//Afiseaza un meniu in functie de optiunea aleasa de utilizator.
void parseUserChoice() {
	char option;
	option = getch();

	if ((option < '1' || option > '7') && currentMenu == MAIN_MENU) {
		showWrongInputWarning("Optiunea aleasa nu e valida!");
		return;
	}

	switch (option) {
	case '1':
		if(currentMenu == MAIN_MENU)
			showFirstMenu();
		break;
	case '2':
		if (currentMenu == MAIN_MENU)
			showSecondMenu();
		break;
	case '3':
		if (currentMenu == MAIN_MENU)
			showThirdMenu(NULL);
		break;
	case '4':
		if (currentMenu == MAIN_MENU)
			showFourthMenu();
		break;
	case '5':
		if (currentMenu == MAIN_MENU)
			showFifthMenu();
		break;
	case '6':
		if (currentMenu == MAIN_MENU)
			exitProgram();
		break;
	case '7':
		if (currentMenu == MAIN_MENU)
			showLoginScreen();
		break;
	case 27:
		if (currentMenu != MAIN_MENU)
			ERROR_CHECK(showMainMenu())
		break;
	default:
		break;
	}
}

//Creaza un StringMapArray cu toate optiunile posibile.
StringMapArray createOptionsStringMapArray() {
	StringMapArray arr = createStringMapArray();
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("a", getVariable("servicii-operatii.dentc", "a"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("b", getVariable("servicii-operatii.dentc", "b"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("c", getVariable("servicii-operatii.dentc", "c"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("d", getVariable("servicii-operatii.dentc", "d"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("e", getVariable("servicii-operatii.dentc", "e"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("f", getVariable("servicii-operatii.dentc", "f"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("g", getVariable("servicii-operatii.dentc", "g"))))

	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("c.a", getVariable("servicii-operatii.dentc", "c.a"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("c.b", getVariable("servicii-operatii.dentc", "c.b"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("c.c", getVariable("servicii-operatii.dentc", "c.c"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("c.d", getVariable("servicii-operatii.dentc", "c.d"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("c.e", getVariable("servicii-operatii.dentc", "c.e"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("c.f", getVariable("servicii-operatii.dentc", "c.e"))))

	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("d.a", getVariable("servicii-operatii.dentc", "d.a"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("d.b", getVariable("servicii-operatii.dentc", "d.b"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("d.c", getVariable("servicii-operatii.dentc", "d.c"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("d.d", getVariable("servicii-operatii.dentc", "d.d"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("d.e", getVariable("servicii-operatii.dentc", "d.e"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("d.f", getVariable("servicii-operatii.dentc", "d.f"))))

	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("e.a", getVariable("servicii-operatii.dentc", "e.a"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("e.b", getVariable("servicii-operatii.dentc", "e.b"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("e.c", getVariable("servicii-operatii.dentc", "e.c"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("e.d", getVariable("servicii-operatii.dentc", "e.d"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("e.e", getVariable("servicii-operatii.dentc", "e.e"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("e.f", getVariable("servicii-operatii.dentc", "e.f"))))

	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("f.a", getVariable("servicii-operatii.dentc", "f.a"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("f.b", getVariable("servicii-operatii.dentc", "f.b"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("f.c", getVariable("servicii-operatii.dentc", "f.c"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("f.d", getVariable("servicii-operatii.dentc", "f.d"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("f.e", getVariable("servicii-operatii.dentc", "f.e"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("f.f", getVariable("servicii-operatii.dentc", "f.f"))))

	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("g.a", getVariable("servicii-operatii.dentc", "g.a"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("g.b", getVariable("servicii-operatii.dentc", "g.b"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("g.c", getVariable("servicii-operatii.dentc", "g.c"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("g.d", getVariable("servicii-operatii.dentc", "g.d"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("g.e", getVariable("servicii-operatii.dentc", "g.e"))))
	ERROR_CHECK(addToStringMapArray(&arr, createStringMap("g.f", getVariable("servicii-operatii.dentc", "g.f"))))

	return arr;
}

//Porneste ciclul principal al aplicatie.
void execApplicationMainLoop() {
	if(!initConsole())
		exit(0);

	optionsArr = createOptionsStringMapArray();
	showLoadingBar();

	showLoginScreen();
	ERROR_CHECK(showMainMenu())

	while (1) {
		parseUserChoice();
		Sleep(1);
	}
}