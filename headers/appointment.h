#pragma once
#pragma warning(disable : 4996)

#include "defines.h"

//Creaza un pointer de tipul appointmentData.
appointmentData* createAppointmentData(c_char* fName, c_char* lName, c_char* operation, c_char* username, timeInfo dateTime);

//Creaza un appointmentDataArray.
appointmentDataArray createAppointmentDataArray();

//Adauga un appointmentData intr-un appointmentDataArray.
int addToAppointmentDataArray(appointmentDataArray* appArr, appointmentData* app);