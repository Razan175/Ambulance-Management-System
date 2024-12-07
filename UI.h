#pragma once
#include "Enums.h"
#include "Organizer.h"




class Organizer; // don't know important or not


class UI
{

	int mode;
	Organizer* organizer;

public:

	UI(Organizer* org);

	//main functions in UI

	void startSimulation();
	void selectMode();
	void endSimulation();

	// simulate the needed mode
	void simulateMode();

	// Wait for user enter
	void interactiveMode();

	// just print the state of the program
	// "Silent Mode", "Simulation starts...", "Simulation ends, Output file created" 
	// then printes output file when the simulation ends without displaying in the console the lists
	// phase 2
	void silentMode();


	//Print Functions
	void PrintTimeStep();
	void printLists();

	// error when write wrong input
	void print(string text);
	void print(int num);
	void printNewLine();
	void waitForEnter();
	void clearScreen();

	~UI();


};

	