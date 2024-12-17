#include "Organizer.h"


Organizer::Organizer()
{
	timestep = 1;
	hospitalCount = 0;
	requestCount = 0;
	scarCount = 0;
	ncarCount = 0;

	Requests = new LinkedQueue <Patient*>;
	Cancellations = new LinkedQueue <Patient*>;
	outCars = new priQueue <Cars*>;
	backCars = new priQueue <Cars*>;
	finishedPatients = new LinkedQueue <Patient*>;

	ui = new UI(this);
}


bool Organizer::ReadFile(string filename)
{
	scarCount = 0;
	ncarCount = 0;

	ifstream file;
	file.open(filename);

	if (file.fail())
	{
		cout << "Cannot open file" << endl;
		return false;
	}

	//Hospital Count
	file >> hospitalCount;

	hospitals = new Hospital[hospitalCount];

	// Scar and Ncar speeds
	int scarSpeed, ncarSpeed;
	file >> scarSpeed >> ncarSpeed;  
	
	distanceMatrix = new int* [hospitalCount];

	//distance matrix
	for (int i = 0; i < hospitalCount; i++)
	{
		distanceMatrix[i] = new int[hospitalCount];

		for (int j = 0; j < hospitalCount; j++)
			file >> distanceMatrix[i][j];
	}

	//Getting number of cars in each hospital and making car objects
	int scars, ncars;
	int carID = 0; 

	for (int i = 0; i < hospitalCount; i++)
	{
		file >> scars >> ncars;
		for (int j = 0; j < scars; j++)
		{
			Cars* newcar = new Cars(++carID,  i + 1, SPECIAL, scarSpeed, READY);
			hospitals[i].AddCar(newcar);
		}
		for (int j = 0; j < ncars; j++)
		{
			Cars* newcar = new Cars(++carID, i + 1, NORMAL, ncarSpeed, READY);
			hospitals[i].AddCar(newcar);
		}
		scarCount += scars;
		ncarCount += ncars;
	}

	//Requests
	file >> requestCount;

	string ptype;
	Type t;
	int rt, pid, hid, d, severity;
	for (int i = 0; i < requestCount; i++)
	{
		severity = 0;
		file >> ptype >> rt >> pid >> hid >> d;

		if (ptype == "NP")
			t = NORMAL;
		else if (ptype == "SP")
			t = SPECIAL;
		else if (ptype == "EP")
		{
			t = EMERGENCY;
			file >> severity;
		}
		else
			continue;

		Patient* p = new Patient(rt,pid,hid,t,d, severity);
		Requests->enqueue(p);
	}

	//Cancellations
	int cancellations;
	file >> cancellations;
	for (int i = 0; i < cancellations; i++)
	{
		file >> rt >> pid >> hid;
		Patient* p = new Patient(rt, pid, hid, CANCELLATION);
		Cancellations->enqueue(p);
	}

	file >> carFailure;

	return true;
}



void Organizer::randSimulation(string filename)
{
	
	ui->startSimulation();
	int rnd;
	this->ReadFile(filename);
	ui->selectMode();
	Patient* p;
	Cars* c;
	int fpatients = 0;
	while (fpatients != requestCount)
	{
		while (Requests->peek(p) && p->getRequestTime() <= timestep)
		{
			Requests->dequeue(p);
			hospitals[p->getNearestHospital() - 1].AddPatient(p);
			p = nullptr;
		}
		/*do
		{
			p = nullptr;
			Requests->peek(p);
			if (p && p->getRequestTime() <= timestep)
			{
				Requests->dequeue(p);
				hospitals[p->getNearestHospital() - 1].AddPatient(p);
			}
			else
				break;
		} while (!Requests->isEmpty());
		*/
		for (int i = 0; i < hospitalCount; i++)
		{
			srand(time(0));
			rnd = rand() % 100;
			c = nullptr;
			p = nullptr;
			if (rnd >= 10 && rnd < 20)
			{
				hospitals[i].RemovePatient(p,SPECIAL);
				if (p != nullptr)
				{
					finishedPatients->enqueue(p);
					fpatients++;
				}
					
			}
			else if (rnd >= 20 && rnd < 25)
			{
				hospitals[i].RemovePatient(p,EMERGENCY);
				if (p != nullptr)
				{
					finishedPatients->enqueue(p);
					fpatients++;
				}
			}
			else if (rnd >= 30 && rnd < 40)
			{
				hospitals[i].RemovePatient(p,NORMAL);
				if (p != nullptr)
				{
					finishedPatients->enqueue(p);
					fpatients++;
				}
			}
			else if (rnd >= 40 && rnd < 45)
			{
				hospitals[i].RemoveSCar(c);
				if (c != nullptr)
					outCars->enqueue(c,timestep + 20);
			}
			else if (rnd >= 70 && rnd < 75)
			{
				hospitals[i].RemoveNCar(c);
				if (c != nullptr)
					outCars->enqueue(c,timestep + 20);   //since there is no patient assigned
			}											//there is no pickup time, so we 
														//assumed any number after the current timestep 
														//i.e timestep + 20
			else if (rnd >= 80 && rnd < 90)
			{
				outCars->dequeue(c,rnd);
				if (c != nullptr)
					backCars->enqueue(c,timestep + 20);
			}
			else if (rnd >= 91 && rnd < 95)
			{
				backCars->dequeue(c, rnd);
				if (c != nullptr)
					hospitals[c->getHID() - 1].AddCar(c);
			}
		}

		ui->simulateMode();

		timestep++;
		
	}
	
	// cancellations 
	Patient* cp;
	Cars* cancelledc;
	while (!Cancellations->isEmpty()) {
		cp = nullptr;
		cancelledc = nullptr;
		int prior;
		Cancellations->peek(cp);
		if (cp && cp->getRequestTime() <= timestep)
		{
			Cancellations->dequeue(cp);
			hospitals[cp->getNearestHospital() - 1].RemovePatient(cp, CANCELLATION);
			delete cp; //removing patient from the system 
			// TODO add car assigned to patient from out to back cars list 
		}
		else
			break;
	}

	
	ui->endSimulation();
	
}

void Organizer::mainSimulation(string filename)
{
	ui->startSimulation();
	int rnd;
	this->ReadFile(filename);
	ui->selectMode();
	Patient* p = nullptr;
	Cars* c;
	int fpatients = 0;
	while (fpatients != requestCount)
	{
		while (Requests->peek(p) && p->getRequestTime() <= timestep)
		{
			Requests->dequeue(p);
			hospitals[p->getNearestHospital() - 1].AddPatient(p);
			p = nullptr;
		}


	}
}
// Print Functions

void Organizer::printHospitalLists()
{

		for (int i = 0; i < hospitalCount; i++)
		{

			ui->print("Current Timestep:  ");
			ui->print(getTimeStep());
			ui->printNewLine();

			ui->print("==============   HOSPITAL #");
			ui->print(i + 1);
			ui->print(" data   ==============");
			ui->printNewLine();

			// Emergency Patients
			ui->print(hospitals[i].getEmergencyRequestsCount());
			ui->print(" EP requests: ");
			if (!hospitals[i].getEPList().isEmpty())
				hospitals[i].getEPList().printQueue();
			ui->printNewLine();

			// Special Patients
			ui->print(hospitals[i].getSpecialRequestsCount());
			ui->print(" SP requests: ");
			if (!hospitals[i].getSPList().isEmpty())
				hospitals[i].getSPList().printQueue();
			ui->printNewLine();

			// Normal Patients
			ui->print(hospitals[i].getNormalRequestsCount());
			ui->print(" NP requests: ");
			if (!hospitals[i].getNPList().isEmpty())
				hospitals[i].getNPList().printQueue();
			ui->printNewLine();

			// Free Cars
			ui->print("Free Cars: ");
			ui->print(hospitals[i].getSCarsCount());
			ui->print(" SCars, ");
			ui->print(hospitals[i].getNCarsCount());
			ui->print(" NCars");
			ui->printNewLine();
			ui->print("==============   HOSPITAL #");
			ui->print(i + 1);
			ui->print(" data end   ==============");
			ui->printNewLine();

			printOutCarsList();
			printBackCarsList();
			printFinishedPatientsList();

			ui->waitForEnter();
			ui->clearScreen();
		}
		ui->print("======== Press Enter to Go to next timestep ========");
		ui->waitForEnter();
	
}

void Organizer::printOutCarsList()
{

	ui->print("---------------------------------------------------------------");
	ui->printNewLine();
	ui->print(outCars->getSize());
	ui->print(" <== Out cars: ");


	if (outCars->isEmpty())
	{
		ui->print("None");
	}
	else
	{

		for (int j = 0; j < outCars->getSize(); j++)
		{
			Cars* temp;
			int pri;  // may make some problems in phase 2

			outCars->dequeue(temp, pri);
			if (j == outCars->getSize())
			{
				if (temp->getCarType() == NORMAL)
					ui->print("N");
				else
					ui->print("S");
				ui->print(temp->getID());
				ui->print("_H");
				ui->print(temp->getHID());
				//ui->print("_P");
				//ui->print(temp->getPatientAssigned()->getID()); to be done in phase 2
			}
			else
			{
				if (temp->getCarType() == NORMAL)
					ui->print("N");
				else
					ui->print("S");
				ui->print(temp->getID());
				ui->print("_H");
				ui->print(temp->getHID());
				//ui->print("_P");
				//ui->print(temp->getPatientAssigned()->getID()); to be done in phase 2
				ui->print(", ");
			}
			outCars->enqueue(temp, pri);
		}
	}
	ui->printNewLine();

}

void Organizer::printBackCarsList()
{

	ui->print(backCars->getSize());
	ui->print(" ==> Back cars: ");


	if (backCars->isEmpty())
	{
		ui->print("None");
	}
	else
	{

		Cars* temp;
		int pri;  // may make some problems in phase 2

		for (int i = 0; i < backCars->getSize(); i++)
		{
			backCars->dequeue(temp, pri);
			if (i == backCars->getSize())
			{
				if (temp->getCarType() == NORMAL)
					ui->print("N");
				else
					ui->print("S");
				ui->print(temp->getID());
				ui->print("_H");
				ui->print(temp->getHID());
				//ui->print("_P");
				//ui->print(temp->getPatientAssigned()->getID()); to be done in phase 2
			}
			else
			{
				ui->print("S");
				ui->print(temp->getID());
				ui->print("_H");
				ui->print(temp->getHID());
				//ui->print("_P");
				//ui->print(temp->getPatientAssigned()->getID()); to be done in phase 2
				ui->print(", ");
			}
			backCars->enqueue(temp, pri);
		}
	}
	ui->printNewLine();

	ui->print("---------------------------------------------------------------");
}

void Organizer::printFinishedPatientsList()
{

	ui->printNewLine();
	ui->print(finishedPatients->getSize());
	ui->print(" finished patients: ");

	if (finishedPatients->isEmpty())
	{
		ui->print("None");
	}
	else
	{
		finishedPatients->printQueue();
	}
}


int Organizer::getTimeStep()
{
	return timestep;
}

void Organizer::CarFailure() {
	int rndd;
	int rndc;
	priQueue <Cars*>* TempCars = new priQueue <Cars*>;
	Cars* tempcar = new Cars;
	int pri;
	int Fpri;
	Cars* failedcar = new Cars;
	Patient* failedpatient;

	{
		srand(time(0));
		rndd = rand() % 101; //generates random number between 0 and 100
		if (rndd == carFailure)
		{
			rndc = (rand() % (outCars->getSize())) + 1; // generates random number between 1 and outcars number

			for (int j = 1; j <= rndc - 1; j++) { //enques all elements before desired car
				outCars->peek(tempcar, pri);
				outCars->dequeue(tempcar, pri); //deques first item
				TempCars->enqueue(tempcar, pri); // enques it in temp prique
			}

			outCars->peek(failedcar, Fpri);
			outCars->dequeue(failedcar, Fpri); //picked car is pointed at by failedcar ptr

			while (!TempCars->isEmpty()) { // returns cars back to outcars
				TempCars->peek(tempcar, pri);
				TempCars->dequeue(tempcar, pri);
				outCars->enqueue(tempcar, pri);
			}

			failedcar->setCarStatus(FAILED);
			backCars->enqueue(failedcar, Fpri); // car added to back list 

			//Question2: after backcars arrive to hispital are they added to free cars list???

			CheckUpList->enqueue(failedcar); // car added to checkup list 

			//Question3: datastructure necessary for checkup list??

			failedpatient = failedcar->getPatientAssigned(); //gets the patient that was assigned to the failedcar

			//put patient on top of list to be assigned next
			hospitals[failedpatient->getNearestHospital() - 1].movetotop(failedpatient);
		}
	}

	//Delete dynamic allocations 
	delete TempCars;
	delete tempcar;
	delete failedcar;
}

Organizer::~Organizer()
{
	delete[] hospitals;
	for (int i = 0; i < hospitalCount; i++)
		delete[] distanceMatrix[i];
	delete[] distanceMatrix;


	delete ui;
	delete Requests;
	delete Cancellations;
	delete outCars;
	delete backCars;
	delete finishedPatients;
}