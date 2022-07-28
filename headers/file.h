#pragma once
#pragma warning(disable : 4996)

#include "defines.h"

//Returneaza valoarea unei variabile dintr-un fisier cu variabile.
char* getVariable(c_char* fileName, c_char* variable);

//Seteaza valoarea unei variabile dintr-un fisier cu variabile.
int setVariable(c_char* fileName, c_char* variable, c_char* newValue);

//Verifica daca exista un utilizator cu username si password introduse de utilizator.
int checkUsernameAndPassword(c_char* filename, c_char* username, c_char* password);

//Cauta un username intr-un fisier cu variabile.
int searchForUsername(c_char* filename, c_char* username);

//Schimba o valoare salvata intr-un fisier cu variabile.
int changeValue(c_char* filename, c_char* currentValue, c_char* newValue, int pair);

//Schimba fiecare aparitie a unei valori intr-un fisier.
int changeEveryOcurrenceOfValue(c_char* filename, c_char* currentValue, c_char* newValue);

//Cauta o programare in lista de programari, verificand daca e posibila noua programare.
int searchForAppointment(c_char* filename, timeInfo dateTime);

//Returneaza un appointmentData cu datele despre a n-a programare a unui utilizator.
appointmentDataArray getUserAppointments(c_char* filename, c_char* username);

//Scrie in fisier o programare.
int saveAppointment(c_char* filename, appointmentData app);