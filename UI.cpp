#include "UI.h"


UI::UI(Organizer* org)
{
	mode = 0;
	organizer = org;
	inputfile = "";
}


void UI::selectMode()
{
	int modeSelection;

	// get mode from user
	cout << "Enter Mode: 1: Interactive Mode, 2: Silent Mode ";

	cin >> modeSelection;


	while (modeSelection != 1 && modeSelection != 2)
	{
		cout << "Error!! Please enter a valid mode: ";
		cin >> modeSelection;
	}

	switch (modeSelection)
	{
	case 1:
		mode = 1;
		cout << "========== Interactive Mode ==========" << endl;
		/*interactiveMode();*/
		break;

	case 2:
		mode = 2;
		cout << "========== Silent Mode ==========" << endl;
		silentMode();
		endSimulation();
		break;
	}

}
void UI::startSimulation()
{

	cout << "======== Starting Simulation =========" << endl;
	cout << "Please Enter File Name: ";
	cin >> inputfile;

}

string UI::getFileName()
{
	return inputfile;
}

void UI::simulateMode()
{
	if (mode == 1) {

	interactiveMode();
}
	/*switch (mode)
	{
	case 1:

		interactiveMode();
		endSimulation();
		break;

	case 2:

		silentMode();
		endSimulation();
		break;
	}*/
}

void UI::endSimulation()
{

	if (mode == 2)
	{

		cout << "Simulation ends, Output file created" << endl;

	}
	else
		cout << "========= Simulation End =========" << endl;
}
 
void UI::interactiveMode()
{
	// clear screen
	system("cls");
	// printLists
	printLists();
	//cout << endl;
	//cout << "======== Press Enter to Go to next timestep ========";
	//cout << endl;
	//cin.get();
	// clear screen
	//system("cls")
}

void UI::silentMode()
{
	system("cls");
	cout << "Simulation Starts..." << endl;

}

void UI::printLists()
{
	// clears all previous inputs
	cin.ignore(); 
	// Print current time
	/*cout << "Current Timestep:  ";
	cout << organizer->getTimeStep() << endl;*/

	organizer->printHospitalLists();
	//organizer->printOutCarsList();
	//organizer->printBackCarsList();
	//organizer->printFinishedPatientsList();

}

void UI::print(string text)
{
	cout << text;
}

void UI::print(int num)
{
	cout << num;
}

void UI::printNewLine()
{
	cout << endl;
}

void UI::waitForEnter()
{
	cin.get();
}

void UI::clearScreen()
{
	system("cls");
}


UI::~UI()
{
}