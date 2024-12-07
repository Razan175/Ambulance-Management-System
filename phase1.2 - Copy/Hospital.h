#pragma once
#include "DSfile/LinkedQueue.h"
#include "DSfile/priQueue.h"
#include "Patient.h"
#include "Cars.h"


class Hospital
{
	LinkedQueue <Patient*> SPList;
	priQueue <Patient*> EPList;
	LinkedQueue <Patient*> NPList;

	LinkedQueue <Cars*> NCars;
	LinkedQueue <Cars*> SCars;

public:
	bool AddCar(Cars* &car);
	bool RemoveSCar(Cars*& car);
	bool RemoveNCar(Cars*& car);
	void AddPatient(Patient*& p);
	void AssignPatient(Patient*& p); //assign patient to car, not needed for this phase
	bool RemovePatient(Patient*& p, Type t); //dequeues patient from list given their type(can be modified to be for cancelled patients)
									//but for phase 1.2 they wanted to remove patients to finish list directly so we'll use it for that

	// counts
	int getEmergencyRequestsCount();
	int getSpecialRequestsCount();
	int getNormalRequestsCount();
	int getSCarsCount();
	int getNCarsCount();
	LinkedQueue <Patient*>& getSPList();
	priQueue <Patient*>& getEPList();
	LinkedQueue <Patient*>& getNPList();
};

