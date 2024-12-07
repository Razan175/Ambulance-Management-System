#pragma once
#include "DSfile/LinkedQueue.h"
#include "DSfile/priQueue.h"
#include "Patient.h"
#include "Enums.h"


class Cars
{

	int carID;							// Car ID
	int hospitalID;					// Hospital ID
	Type carType;					// Car type (SC, NC)
	int carSpeed;					// Car speed
	CarStatus carStatus;			// Car status (Ready, Assigned, Loaded)

	Patient* patientAssigned;		// patient assigned to car




public:

	//Cars(int id);	// Create a normal empty car
	Cars(int id, int hid, Type type, int speed, CarStatus status); // Constructor

	// Setters
	void setHospitalID(int id); // idk if it is important or not ***
	void setCarType(Type t);
	void setSpeed(int s);
	void setCarStatus(CarStatus cs);

	// Getters
	int getID(); // We can use it later
	int getHID();
	Type getCarType();
	int getSpeed();
	CarStatus getCarStatus();
	Patient* getPatientAssigned(); // is it important ?? *** 
								


	// Other Functions

	//bool pickupPatient(Patient* p);
	//bool dropOffPatient(Patient* p); // should i use Patient*& p ?? ***
	//								//bayen, msh m7tagenha fel phase de aslan

	// destructor
	~Cars();

};