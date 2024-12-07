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

class UI; // don't know if important or not

class Organizer
{
	LinkedQueue <Patient*>* Requests;
	LinkedQueue <Patient*>* Cancellations;
	Hospital* hospitals;
	priQueue <Cars*>* outCars;
	priQueue <Cars*>* backCars;
	LinkedQueue <Patient*>* finishedPatients;
	int** distanceMatrix;

	int timestep;

	int hospitalCount;
	int requestCount;
	int scarCount; //total number of scars across hospitals
	int ncarCount; //total number of ncars across hospitals

	UI* ui;


public:

	Organizer();
	bool ReadFile(string filename = "Input.txt");
	void Simulate(string filename = "Input.txt");

	int getTimeStep();


	// Print functions
	void printHospitalLists();
	void printOutCarsList();
	void printBackCarsList();
	void printFinishedPatientsList();


	~Organizer();
};

