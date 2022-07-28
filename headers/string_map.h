#pragma once
#pragma warning(disable : 4996)

#include "defines.h"

//Creaza un pointer de tipul StringMap.
StringMap* createStringMap(c_char* key, c_char* value);

//Returneaza valoarea continuta de unul din obiectele din stringMapArray cu cheia key.
char* findValueByKey(StringMapArray array, c_char* key);

//Creaza un StringMapArray.
StringMapArray createStringMapArray();

//Adauga un StringMap intr-un StringMapArray.
int addToStringMapArray(StringMapArray* mapArr, StringMap* map);