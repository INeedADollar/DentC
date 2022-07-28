#pragma once

#include "defines.h"

//Creaza un tableLabel.
tableLabel* createTableLabel(c_char* text, uint width);

//Creaza un tableLabelsArray.
tableLabelsArray createTableLabelsArray();

//Adauga un tableLabel in tableLabelsArray.
int addToTableLabelsArray(tableLabelsArray* arr, tableLabel* label);

//Incepe afisarea unui tabel.
int startTabel(tableLabelsArray arr);

//Adauga o linie in tabel.
int addRow(tableLabelsArray arr);

//Incheie afisarea unui tabel.
int endTable();