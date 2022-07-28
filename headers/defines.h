#pragma once

//Tipuri de date

//unsigned int
typedef unsigned int uint;

//const char
typedef const char c_char;

//Reprezinta lungimea unui array.
typedef unsigned long long a_size;


//Structuri

//Structura ce reprezinta o data.
typedef struct date {
	uint day, month, year;
} dateInfo;

//Structura ce reprezinta o ora.
typedef struct hour {
	uint minute, hour;
} hourInfo;

//Structura ce contine detalii despre timp.
typedef struct time {
	dateInfo date;
	hourInfo hour;
} timeInfo;

//Cheia unui Map.
struct MapKey {
	char* mapKey;
};

//Valoarea unui Map.
struct MapValue {
	char* mapValue;
};

//Map care contine o cheie si o valoare.
typedef struct Map {
	struct MapKey key;
	struct MapValue value;
} StringMap;

//Array ce contine StringMaps.
typedef struct MapArray {
	StringMap** array;
	a_size elNumber;
} StringMapArray;

//Struct ce contine date despre o programare.
typedef struct appData {
	c_char *fName, *lName, *operation, *username;
	timeInfo dateTime;
} appointmentData;

//Array ce contine elemente de tip appointmentData.
typedef struct appDataArr {
	appointmentData** array;
	a_size elemNumber;
} appointmentDataArray;

//Contine textul ce va fi afisat in celula unui tabel.
typedef struct label {
	char* labelText;
	uint width;
} tableLabel;

//Array ce contine elemenete de tip tableLabelsArray.
typedef struct labArr {
	tableLabel** labelsArray;
	a_size arraySize;
} tableLabelsArray;


//Enumeratii

//Enumeratie care contine tipurile de meniuri posibile.
typedef enum Menus
{
	MAIN_MENU, //Meniul principal
	MENU_1,	   //Meniul 1 
	MENU_2,    //Meniul 1 
	MENU_3,    //Meniul 1 
	MENU_4,    //Meniul 1 
	MENU_5,    //Meniul 1 
} MenuTypes;


//defines

//Daca x e NULL returneaza 0.
#define NULL_CHECK(x) if(x == NULL) return 0;

//Daca x e NULL executa y.
#define NULL_CHECK_EX(x, y) if(x == NULL) {y}

//Verifica daca o functie a esuat (daca valoarea returnata e 0).
#define FAIL_CHECK(x) if (!x) return 0;