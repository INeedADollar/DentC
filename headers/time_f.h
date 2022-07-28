#pragma once
#pragma warning(disable : 4996)

#include "defines.h"

//Creaza un obiect de tipul timeInfo din parametrii specificati.
timeInfo createTimeInfo(uint minutes, uint hour, uint day, uint month, uint year);

//Creaza un obiect de tipul timeInfo din date si hour.
timeInfo createTimeInfoDH(dateInfo date, hourInfo hour);

//Creaza un obiect de tipul dateInfo.
dateInfo createDateInfo(uint day, uint month, uint year);

//Creaza un obiect de tipul hourInfo.
hourInfo createHourInfo(uint minutes, uint hour);

//Compara doua date.
int compareDates(dateInfo date1, dateInfo date2);

//Compara doua ore.
int compareHours(hourInfo hour1, hourInfo hour2);

//Verifica daca data specificata este valida.
int checkDate(dateInfo date);

//Verifica daca ora specificata este valida.
int checkHour(hourInfo hour);

//Returneaza timpul curent.
timeInfo* getLocalDatetime();

//Salveaza data sub forma de string.
void getDateAsString(dateInfo date, char* dateStr);

//Salveaza ora sub forma de string.
void getHourAsString(hourInfo hour, char* hourStr);