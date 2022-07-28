#include <stdlib.h>
#include <string.h>

#include "appointment.h"

//Creaza un pointer de tipul appointmentData.
appointmentData* createAppointmentData(c_char* fName, c_char* lName, c_char* operation, 
											c_char* username, timeInfo dateTime) {
	NULL_CHECK(fName)
	NULL_CHECK(lName)
	NULL_CHECK(operation)
	NULL_CHECK(username)
	
	appointmentData* app = malloc(sizeof(appointmentData));
	NULL_CHECK(app)

	app->fName = malloc(strlen(fName) + 1);
	NULL_CHECK_EX(app->fName, free(app); return NULL;);

	app->lName = malloc(strlen(lName) + 1);
	NULL_CHECK_EX(app->fName, free(app); free(app->fName); return NULL;);

	app->username = malloc(strlen(username) + 1);
	NULL_CHECK_EX(app->fName, free(app); free(app->fName); free(app->lName); return NULL;);

	app->operation = malloc(strlen(operation) + 1);
	NULL_CHECK_EX(app->fName, free(app); free(app->fName); free(app->lName); free(app->username); return NULL;);

	strcpy(app->fName, fName);
	strcpy(app->lName, lName);
	strcpy(app->username, username);
	strcpy(app->operation, operation);
	app->dateTime = dateTime;

	return app;
}

//Creaza un appointmentDataArray.
appointmentDataArray createAppointmentDataArray() {
	appointmentDataArray arr;
	arr.array = NULL;
	arr.elemNumber = 0;

	return arr;
}

//Adauga un appointmentData intr-un appointmentDataArray.
int addToAppointmentDataArray(appointmentDataArray* appArr, appointmentData* app) {
	NULL_CHECK(appArr)
	NULL_CHECK(app)

	NULL_CHECK_EX(appArr->array,
			appArr->array = malloc(sizeof(appointmentData*));
			NULL_CHECK(appArr->array)

			appArr->array[0] = app;
			appArr->elemNumber++;
			return 1;
	)

	appointmentData** testApp = realloc(appArr->array, (appArr->elemNumber + 1) * sizeof(appointmentData*));
	NULL_CHECK(testApp)

	appArr->array = testApp;
	appArr->array[appArr->elemNumber] = app;
	appArr->elemNumber++;

	return 1;
}
