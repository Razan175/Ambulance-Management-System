#pragma once

#include "DSfile/LinkedQueue.h"
#include "DSfile/priQueue.h"

#include "Hospital.h"
#include "Patient.h"
#include "Cars.h"
#include "UI.h"
#include "Enums.h"

#include <fstream>
#include <string>
#include <iostream>
#include <time.h>
using namespace std;

class UI;

class Organizer
{
	LinkedQueue <Patient*>* Requests;
	LinkedQueue <Patient*>* Cancellations;
	Hospital* hospitals;
	priQueue <Cars*>* outCars;
	priQueue <Cars*>* backCars;
	LinkedQueue <Cars*>* CheckUpList;
	LinkedQueue <Patient*>* finishedPatients;
	int** distanceMatrix;

	int timestep;

	int hospitalCount;
	int requestCount;
	int cancellations;
	int NPCount;
	int SPCount;
	int EPCount;
	int scarCount; //total number of scars across hospitals
	int ncarCount; //total number of ncars across hospitals
	int carFailure;//cars failure probability number 
	

	UI* ui;



public:

	Organizer();
	bool ReadFile(string filename = "InputFiles/Input.txt");
	bool WriteFile(string filename, int timestamp);
	void randSimulation(string filename = "InputFiles/Input.txt");
	void mainSimulation();
	void printFinishedPatientsList();
	int getTimeStep();
	void cancelled();
	void CarFailure();
	void FailureAction(Cars* fc, int pri);	
	void AddFinishedPatient(Patient* p);
	// Print functions
	void printHospitalLists();
	void printOutCarsList();
	void printBackCarsList();
	// Finding closest hospital for EP when no avalible cars
	//Hospital* findSuitableHospital(int currentHospitalID);
	Hospital* FindEP_Random_Hospital(int currentHospitalID);

	~Organizer();
};

