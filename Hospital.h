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

	Type GetAvailableCar(Type patientType);
	bool AssignPatient(Patient* p, Cars*& c);
public:
	bool AddCar(Cars* &car);
	bool RemoveSCar(Cars*& car);
	bool RemoveNCar(Cars*& car);
	void AddPatient(Patient*& p);
	bool RemovePatient(Patient*& p, Type t); 

	bool sendEPCar(Cars* &c);
	bool sendSPCar(Cars*& c);
	bool sendNPCar(Cars*& c);
	/*
		patient-car assignment functions:
		Type getAvailableCar(Type patientType): peeks the car lists for the suitable available car type
		if no car are available, return cancellation

		bool assignPatient(patient* p, Cars* &c): links given patient to a car and returns the car in c(calls getAvailableCar())
		note that it returns false if there are no available cars

		bool sendXPCar(): these are the functions called by the organizer at each timestep

	*/
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

