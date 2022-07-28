#include <stdlib.h>
#include <string.h>

#include "string_map.h"

//Creaza un obiect de tipul StringMap.
StringMap* createStringMap(c_char* key, c_char* value) {
	NULL_CHECK(key)
	NULL_CHECK(value)

	StringMap* map = malloc(sizeof(StringMap));
	NULL_CHECK(map)

	map->key.mapKey = malloc(strlen(key) + 1);
	NULL_CHECK_EX(map->key.mapKey, free(map); return NULL;)

	map->value.mapValue = malloc(strlen(value) + 1);
	NULL_CHECK(map->value.mapValue, free(map); free(map->key.mapKey); return NULL;)
	
	strcpy(map->key.mapKey, key);
	strcpy(map->value.mapValue, value);

	return map;
}

//Returneaza valoarea continuta de unul din obiectele din stringMapArray cu cheia key.
char* findValueByKey(StringMapArray array, c_char* key) {
	NULL_CHECK(key)

	for (int i = 0; i < array.elNumber; i++)
		if (strcmp(array.array[i]->key.mapKey, key) == 0)
			return array.array[i]->value.mapValue;

	return NULL;
}

//Creaza un obiect de tipul StringMapArray.
StringMapArray createStringMapArray() {
	StringMapArray arr = { 0 };
	return arr;
}

//Adauga un StringMap intr-un StringMapArray.
int addToStringMapArray(StringMapArray* mapArr, StringMap* map) {
	NULL_CHECK(mapArr)
	NULL_CHECK(map)

	NULL_CHECK_EX(mapArr->array,
		mapArr->array = malloc(sizeof(StringMap*));
		NULL_CHECK(mapArr->array)

		mapArr->array[0] = map;
		mapArr->elNumber++;
		return 1;
	)

	StringMap** testMap = realloc(mapArr->array, (mapArr->elNumber + 1) * sizeof(StringMap*));
	NULL_CHECK(testMap)

	mapArr->array = testMap;
	mapArr->array[mapArr->elNumber] = map;
	mapArr->elNumber++;
	
	return 1;
}