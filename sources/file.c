#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "appointment.h"
#include "file.h"
#include "time_f.h"

//Returneaza valoarea unei variabile dintr-un fisier cu variabile.
char* getVariable(c_char* fileName, c_char* variable) {
	NULL_CHECK(variable)
	FILE* file = fopen(fileName, "r");
	NULL_CHECK(file)

	char buffer[500], *value = malloc(255);
	NULL_CHECK(value)

	while (fgets(buffer, 500, file)) {
		int vLength = strlen(variable);
		if (strncmp(buffer, variable, vLength) == 0 && buffer[vLength] == ' ') {
			strcpy(value, buffer + vLength + 3); 
			value[strlen(value) - 1] = 0;
			fclose(file);
			return value;
		}
	}

	fclose(file);
	return NULL;
}

//Genereaza si returneaza un nume de fisier la intamplare.
char* randFileName(size_t length, c_char* extension) {
	static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	char* randomString = NULL;
	srand(time(0));

	if (length) {
		randomString = malloc(length + strlen(extension) + 2);
		if (randomString) {
			for (int n = 0; n < length; n++) {
				int key = rand() % (int)(sizeof(charset) - 1);
				randomString[n] = charset[key];
			}

			randomString[length] = '.';
			randomString[length + 1] = 0;
			strcat(randomString, extension);
		}
	}

	return randomString;
}

//Seteaza valoarea unei variabile dintr-un fisier cu variabile.
int setVariable(c_char* fileName, c_char* variable, c_char* newValue) {
	FILE* file = fopen(fileName, "r");
	NULL_CHECK_EX(file, 
		file = fopen(fileName, "w");
		NULL_CHECK(file)
		fputs("/*!NU MODIFICATI ACEST FISIER*/\n", file);
		fprintf(file, "%s = %s\n", variable, newValue);
		fclose(file);
		return 1;
	)

	char* randFile = randFileName(10, "tmp");
	FILE* tempFile = fopen(randFile, "w");

	NULL_CHECK_EX(tempFile, fclose(file); printf("%s", strerror(errno)); return 0;)

	int valueFound = 0;
	char buffer[500];

	while (fgets(buffer, 500, file)) {
		int vLength = strlen(variable);
		if (strncmp(buffer, variable, vLength) == 0 && buffer[vLength] == ' ') {
			fprintf(tempFile, "%s = %s\n", variable, newValue);
			valueFound = 1;
			continue;
		}
		
		fputs(buffer, tempFile);
	}

	if (!valueFound) 
		fprintf(tempFile, "%s = %s\n", variable, newValue);

	fclose(file);
	fclose(tempFile);

	int removeResult = remove(fileName);
	int renameResult = rename(randFile, fileName);
	free(randFile);
	
	if (removeResult == -1 || renameResult == -1) 
		return 0;

	return 1;
}

//Returneaza indexul primei aparitie in string a lui ch.
int findNthOcurrenceOfCharacter(c_char* string, c_char ch, int n) {
	NULL_CHECK(string)

	int i = 0, len = strlen(string);
	while ((string[i] != ch || n) && n && i < len) {
		if (string[i] == ch)
			n--;
		i++;
	}

	if (!i)
		return -1;

	return i-1;
}

//Verifica daca exista un utilizator cu username si password introduse de utilizator.
int checkUsernameAndPassword(c_char* filename, c_char* username, c_char* password) {
	FILE* file = fopen(filename, "r");
	NULL_CHECK(file)

	char buffer[500];
	while (fgets(buffer, 500, file)) {
		buffer[strlen(buffer) - 1] = 0;
		if (strncmp(buffer, "username", 8) == 0 &&
			strcmp(buffer + findNthOcurrenceOfCharacter(buffer, '=', 1) + 2, username) == 0) {
			FAIL_CHECK(fgets(buffer, 500, file))
			
			buffer[strlen(buffer) - 1] = 0;
			if (strncmp(buffer, "password", 8) == 0 &&
				strcmp(buffer + findNthOcurrenceOfCharacter(buffer, '=', 1) + 2, password) == 0) {
				fclose(file);
				return 1;
			}
		}
	}

	fclose(file);
	return 0;
}

//Cauta un username intr-un fisier cu variabile.
int searchForUsername(c_char* filename, c_char* username) {
	FILE* file = fopen(filename, "r");
	NULL_CHECK(file)

	char buffer[500];
	while (fgets(buffer, 500, file)) {
		buffer[strlen(buffer) - 1] = 0;
		if (strncmp(buffer, "username", 8) == 0 &&
			strcmp(buffer + findNthOcurrenceOfCharacter(buffer, '=', 1) + 2, username) == 0) {
			fclose(file);
			return 1;
		}
	}

	fclose(file);
	return 0;
}

//Schimba o valoare salvata intr-un fisier cu variabile.
int changeValue(c_char* filename, c_char* currentValue, c_char* newValue, int pair) {
	FILE* file = fopen(filename, "r");
	NULL_CHECK(file)

	char* randFile = randFileName(10, "tmp");
	FILE* tempFile = fopen(randFile, "w");

	NULL_CHECK_EX(tempFile, fclose(file); return 0;)
	
	char buffer[500];
	while (fgets(buffer, 500, file)) {
		int ocIndex = findNthOcurrenceOfCharacter(buffer, '=', 1);
		buffer[strlen(buffer) - 1] = 0;

		if (strcmp(buffer + ocIndex + 2, currentValue) == 0) {
			if (pair) {
				if (!fprintf(tempFile, "%s\n", buffer))
					break;

				fgets(buffer, 500, file);
				ocIndex = findNthOcurrenceOfCharacter(buffer, '=', 1);
				buffer[strlen(buffer) - 1] = 0;
			}

			char variable[15];
			for (int i = 0; i < ocIndex - 1; i++)
				variable[i] = buffer[i];
			variable[ocIndex - 1] = 0;

			fprintf(tempFile, "%s = %s\n", variable, newValue);
		}
		else 
			fprintf(tempFile, "%s\n", buffer);
	}	

	fclose(file);
	fclose(tempFile);

	int removeResult = remove(filename);
	int renameResult = rename(randFile, filename);
	free(randFile);

	if (removeResult == -1 || renameResult == -1)
		return -1;

	return 1;
}

//Schimba fiecare aparitie a unei valori intr-un fisier.
int changeEveryOcurrenceOfValue(c_char* filename, c_char* currentValue, c_char* newValue) {
	FILE* file = fopen(filename, "r");
	NULL_CHECK(file)

	char* randFile = randFileName(10, "tmp");
	FILE* tempFile = fopen(randFile, "w");

	NULL_CHECK_EX(tempFile, fclose(file); return 0;)
	int start = 0, stop = 0, dif = 0, usernameLen = strlen(currentValue);
	char buffer[500], newLine[500];

	while (fgets(buffer, 500, file)) {
		buffer[strlen(buffer) - 1] = 0;
		start = findNthOcurrenceOfCharacter(buffer, ',', 2) + 1;
		stop = findNthOcurrenceOfCharacter(buffer, ',', 3);

		if (start == -1 || stop == -1 || strlen(buffer) < usernameLen || (stop - start) != usernameLen) {
			fprintf(tempFile, "%s\n", buffer);
			continue;
		}

		for (int i = start; i < stop; i++) {
			if (buffer[i] != currentValue[i - start]) {
				dif = 1;
				break;
			}
		}

		if (!dif) {
			for (int i = 0; i < start; i++)
				newLine[i] = buffer[i];

			newLine[start] = 0;
			strcat(newLine, newValue);
			strcat(newLine, buffer + stop);
			fprintf(tempFile, "%s\n", newLine);
		}
		else {
			fprintf(tempFile, "%s\n", buffer);
			dif = 0;
		}
	}

	fclose(file);
	fclose(tempFile);

	int removeResult = remove(filename);
	int renameResult = rename(randFile, filename);
	free(randFile);

	if (removeResult == -1 || renameResult == -1)
		return -1;

	return 1;
}

//Cauta o programare in lista de programari, verificand daca e posibila noua programare.
int searchForAppointment(c_char* filename, timeInfo dateTime) {
	FILE* file = fopen(filename, "r");
	NULL_CHECK(file)

	char buffer[500];
	timeInfo fileDate;
	dateInfo date1, date2;
	hourInfo hour1, hour2;
	date1 = createDateInfo(dateTime.date.day, dateTime.date.month, dateTime.date.year);
	hour1 = createHourInfo(dateTime.hour.minute, dateTime.hour.hour);

	while (fgets(buffer, 500, file)) {
		sscanf(buffer + findNthOcurrenceOfCharacter(buffer, ',', 3) + 1, "%d,%d,%d,%d,%d", &fileDate.hour.minute,
			&fileDate.hour.hour, &fileDate.date.day, &fileDate.date.month, &fileDate.date.year);
		
		date2 = createDateInfo(fileDate.date.day, fileDate.date.month, fileDate.date.year);
		hour2 = createHourInfo(fileDate.hour.minute, fileDate.hour.hour);

		if (!checkDate(date2) || !checkHour(hour2))
			continue;

		if (compareDates(date1, date2) == 0 && compareHours(hour1, hour2) == 0) {
			fclose(file);
			return 1;
		}
	}

	fclose(file);
	return 0;
}

//Returneaza un appointmentData cu datele despre a n-a programare a unui utilizator.
appointmentDataArray getUserAppointments(c_char* filename, c_char* username) {
	FILE* file = fopen(filename, "r");
	NULL_CHECK_EX(file, return createAppointmentDataArray();)
	
	char buffer[500], fName[21], lName[21], user[21], operation[80];
	int start = 0, stop = 0, dif = 0, usernameLen = strlen(username);
	appointmentDataArray arr = createAppointmentDataArray();
	timeInfo time;

	while (fgets(buffer, 500, file)) {
		buffer[strlen(buffer) - 1] = 0;
		start = findNthOcurrenceOfCharacter(buffer, ',', 2) + 1;
		stop = findNthOcurrenceOfCharacter(buffer, ',', 3);

		if (start == -1 || stop == -1 || strlen(buffer) < usernameLen || (stop - start) != usernameLen)
			continue;

		for (int i = start; i < stop; i++) {
			if (buffer[i] != username[i - start]) {
				dif = 1;
				break;
			}
		}

		if (!dif) {
			sscanf(buffer, "%[^,],%[^,],%[^,],%d,%d,%d,%d,%d,%[^,]", fName, lName, user, &time.hour.minute,
				&time.hour.hour, &time.date.day, &time.date.month, &time.date.year, operation);
			
			addToAppointmentDataArray(&arr, createAppointmentData(fName, lName, operation, username, time));						
		}
		else
			dif = 0;
	}

	fclose(file);
	return arr;
}

//Scrie in fisier o programare.
int saveAppointment(c_char* filename, appointmentData app) {
	FILE* file = fopen(filename, "r");
	NULL_CHECK_EX(file, 
		file = fopen(filename, "w");
		fputs("/*!NU MODIFICATI ACEST FISIER*/\n", file);
		fclose(file);
	)

	fclose(file);
	file = fopen(filename, "a+");
	NULL_CHECK(file)

	fprintf(file, "%s,%s,%s,%d,%d,%d,%d,%d,%s\n", app.fName, app.lName, app.username, app.dateTime.hour.minute,
		app.dateTime.hour.hour, app.dateTime.date.day, app.dateTime.date.month, app.dateTime.date.year, app.operation);

	fclose(file);
	return 1;
}