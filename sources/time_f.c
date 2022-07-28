#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "time_f.h"

//Creaza un timeInfo din parametrii specificati.
timeInfo createTimeInfo(uint minutes, uint hour, uint day, uint month, uint year) {
	timeInfo time;
	time.hour.minute = minutes;
	time.hour.hour = hour;
	time.date.day = day;
	time.date.month = month;
	time.date.year = year;

	return time;
}

//Creaza un timeInfo din date si hour.
timeInfo createTimeInfoDH(dateInfo date, hourInfo hour) {
	timeInfo time;
	time.hour.minute = hour.minute;
	time.hour.hour = hour.hour;
	time.date.day = date.day;
	time.date.month = date.month;
	time.date.year = date.year;

	return time;
}

//Creaza un dateInfo.
dateInfo createDateInfo(uint day, uint month, uint year) {
	dateInfo date;
	date.day = day;
	date.month = month;
	date.year = year;

	return date;
}

//Creaza un hourInfo.
hourInfo createHourInfo(uint minutes, uint hour) {
	hourInfo sHour;
	sHour.hour = hour;
	sHour.minute = minutes;

	return sHour;
}

//Compara doua date.
int compareDates(dateInfo date1, dateInfo date2) {
	if (date1.year < date2.year)
		return -1;
	else if (date1.year > date2.year)
		return 1;
	else {
		if (date1.month < date2.month)
			return -1;
		else if (date1.month > date2.month)
			return 1;
		else {
			if (date1.day < date2.day)
				return -1;
			else if (date1.day > date2.day)
				return 1;
			else
				return 0;
		}
	}
}

//Compara doua ore.
int compareHours(hourInfo hour1, hourInfo hour2) {
	if (hour1.hour > hour2.hour)
		return 1;
	else if (hour1.hour < hour2.hour)
		return -1;
	else if (hour1.minute > hour2.minute)
		return 1;
	else if (hour1.minute < hour2.minute)
		return -1;
	else
		return 0;
}

//Verifica daca ora specificata este valida.
int checkHour(hourInfo hour) {
	if ((hour.hour >= 8 && hour.hour <= 15) && (hour.minute == 0 || hour.minute == 30))
		return 1;

	return 0;
}

//Calculeaza si returneaza a cata zi din saptamana e ziua specificata.
int getWeekDay(uint d, uint m, uint y) {
	return (d += m < 3 ? y-- : y - 2, 23 * m / 9 + d + 4 + y / 4 - y / 100 + y / 400) % 7;
}

//Verifica daca data specificata este valida.
int checkDate(dateInfo date) {
	timeInfo* nTime = getLocalDatetime();
	int wDay = getWeekDay(date.day, date.month, date.year);
	if (wDay == 0 || wDay == 6)
		return 0;

	if (date.year == nTime->date.year) {
		if (date.month >= 1 && date.month <= 12 && date.month >= nTime->date.month) {
			if (date.month > nTime->date.month) {
				if ((date.day >= 1 && date.day <= 31) && (date.month == 1 || date.month == 3 || date.month == 5 || date.month == 7 || date.month == 8 || date.month == 10 || date.month == 12)) {
					free(nTime);
					return 1;
				}
				else if ((date.day >= 1 && date.day <= 30) && (date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11)) {
					free(nTime);
					return 1;
				}
				else if ((date.day >= 1 && date.day <= 28) && (date.month == 2)) {
					free(nTime);
					return 1;
				}
				else if (date.day == 29 && date.month == 2 && (date.year % 400 == 0 || (date.year % 4 == 0 && date.year % 100 != 0))) {
					free(nTime);
					return 1;
				}
			}
			else {
				if ((date.day > nTime->date.day) && (date.day >= 1 && date.day <= 31) && (date.month == 1 || date.month == 3 || date.month == 5 || date.month == 7 || date.month == 8 || date.month == 10 || date.month == 12)) {
					free(nTime);
					return 1;
				}
				else if ((date.day > nTime->date.day) && (date.day >= 1 && date.day <= 30) && (date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11)) {
					free(nTime);
					return 1;
				}
				else if ((date.day > nTime->date.day) && (date.day >= 1 && date.day <= 28) && (date.month == 2)) {
					free(nTime);
					return 1;
				}
				else if ((date.day > nTime->date.day) && date.day == 29 && date.month == 2 && (date.year % 400 == 0 || (date.year % 4 == 0 && date.year % 100 != 0))) {
					free(nTime);
					return 1;
				}
			}
		}
	}
	else if (date.year > nTime->date.year && date.year <= nTime->date.year + 5) {
		if (date.month >= 1 && date.month <= 12) {
			if ((date.day >= 1 && date.day <= 31) && (date.month == 1 || date.month == 3 || date.month == 5 || date.month == 7 || date.month == 8 || date.month == 10 || date.month == 12)) {
				free(nTime);
				return 1;
			}
			else if ((date.day >= 1 && date.day <= 30) && (date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11)) {
				free(nTime);
				return 1;
			}
			else if ((date.day >= 1 && date.day <= 28) && (date.month == 2)) {
				free(nTime);
				return 1;
			}
			else if (date.day == 29 && date.month == 2 && (date.year % 400 == 0 || (date.year % 4 == 0 && date.year % 100 != 0))) {
				free(nTime);
				return 1;
			}
		}
	}

	free(nTime);
	return 0;
}

//Returneaza timpul curent.
timeInfo* getLocalDatetime() {
	time_t nowTime = time(0);
	struct tm* nTime = malloc(sizeof(struct tm));

	NULL_CHECK(nTime)

	localtime_s(nTime, &nowTime);
	timeInfo* timeI = malloc(sizeof(struct time));

	NULL_CHECK(timeI)
	
	timeI->hour.minute = nTime->tm_min;
	timeI->hour.hour = nTime->tm_hour;
	timeI->date.day = nTime->tm_mday;
	timeI->date.month = nTime->tm_mon + 1;
	timeI->date.year = nTime->tm_year + 1900;

	free(nTime);
	return timeI;
}

//Salveaza data sub forma de string.
void getDateAsString(dateInfo date, char* dateStr) {
	if (date.day < 10) {
		if (date.month < 10)
			sprintf(dateStr, "%d%d/%d%d/%d", 0, date.day, 0, date.month, date.year);
		else
			sprintf(dateStr, "%d%d/%d/%d", 0, date.day, date.month, date.year);
	}
	else {
		if (date.month < 10)
			sprintf(dateStr, "%d/%d%d/%d", date.day, 0, date.month, date.year);
		else
			sprintf(dateStr, "%d/%d/%d", date.day, date.month, date.year);
	}
}

//Salveaza ora sub forma de string.
void getHourAsString(hourInfo hour, char* hourStr) {
	if (hour.minute == 0) 
		sprintf(hourStr, "%d:%d%d", hour.hour, 0, hour.minute);
	else
		sprintf(hourStr, "%d:%d", hour.hour, hour.minute);
}