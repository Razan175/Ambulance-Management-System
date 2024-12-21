#include "Organizer.h"



bool Organizer::WriteFile(string filename, int timestamp)
{

	ofstream outputFile;
	outputFile.open("output6.txt", ios::out);

	Patient* patient = nullptr;

	int totalbusytime = 0;

	int totalWaitingTime = 0;

	if (outputFile.is_open())
	{
		outputFile << "FT		PID			QT			WT" << endl;


		for (int i = 0, z = finishedPatients->getSize(); i < z; i++)
		{
			finishedPatients->dequeue(patient);

			totalWaitingTime += patient->getPatientWaitingTime();
			totalbusytime += patient->getBusyCarAssigned();

			outputFile << patient->getFinishTime() << "		" << patient->getID() << "		"
				<< patient->getRequestTime() << "		" << patient->getPatientWaitingTime() << endl;

			finishedPatients->enqueue(patient);
		}


		outputFile << "----------------------------------------------" << endl;

		outputFile << "Patients: " << NPCount + SPCount + EPCount << " [NP: " << NPCount << ", SP: " << SPCount << ", VP: " << EPCount << "]" << endl;
		outputFile << "Hospitals = " << hospitalCount << endl;
		outputFile << "Cars: " << scarCount + ncarCount << " [SCar: " << scarCount << ", NCar: " << ncarCount << "]" << endl;


		outputFile << "Avg utilization = " << ((float)(totalbusytime / (scarCount + ncarCount)) / timestamp) * 100 << endl;



		outputFile.close();

		return true;

	}
	return false;
}
Organizer::Organizer()
{
	timestep = 1;
	hospitalCount = 0;
	requestCount = 0;
	scarCount = 0;
	ncarCount = 0;
	cancellations = 0;
	SPCount = 0;
	NPCount = 0;
	EPCount = 0;
	carFailure = 0;
	CheckUpList = new LinkedQueue<Cars*>;
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
	int SCheckUpTime, NCheckUpTime;
	file >> SCheckUpTime>> NCheckUpTime;

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
			Cars* newcar = new Cars(++carID,  i + 1, SPECIAL, scarSpeed,SCheckUpTime, READY);
			hospitals[i].AddCar(newcar);
		}
		for (int j = 0; j < ncars; j++)
		{
			Cars* newcar = new Cars(++carID, i + 1, NORMAL, ncarSpeed, NCheckUpTime, READY);
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
		{
			t = NORMAL;
			NPCount++;
		}
		else if (ptype == "SP")
		{
			t = SPECIAL;
			SPCount++;
		}
		else if (ptype == "EP")
		{
			t = EMERGENCY;
			EPCount++;
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
	file.close();
	return true;
}

void Organizer::mainSimulation(string filename)
{
	if (!ReadFile(filename))
		return;

	ui->startSimulation();
	ui->selectMode();

	Patient* p = nullptr;
	Cars* c;
	int pri;
	int fpatients = 0;

	while (fpatients <  requestCount)
	{

		//sending all patients to their hospital at the given timestep
		while (Requests->peek(p) && p->getRequestTime() <= timestep)
		{
			Requests->dequeue(p);
			hospitals[p->getNearestHospital() - 1].AddPatient(p);
			p = nullptr;
		}

		//moving cars from free to out
		for (int i = 0; i < hospitalCount; i++)
		{

			//handling emergency patients first

			while (hospitals[i].sendEPCar(c, p))
			{
				if (c == nullptr && p!= nullptr)
				{
					Hospital* randomHospital = FindEP_Random_Hospital(i + 1);
					if (randomHospital)
					{
						randomHospital->AddPatient(p);
					}
					p = nullptr;
				}
				else if (p != nullptr)
				{
					p = c->getPatientAssigned();
					p->setPickupTime((timestep + (p->getDistance() / c->getSpeed())));

					//we want the highest priority to the smallest pickup time
					//the given pri-queues give the highest priority to the biggest integer number
					//so to solve this, we pass the pickup time with a negative sign

					outCars->enqueue(c, -1 * p->getPickupTime());
					p = nullptr;
				}

			}
			//handling special patients next
			while (hospitals[i].sendSPCar(c))
			{
				p = c->getPatientAssigned();
				p->setPickupTime(timestep + (p->getDistance() / c->getSpeed()));
				outCars->enqueue(c, -1 * p->getPickupTime());	
			}

			//handling normal patients
			while (hospitals[i].sendNPCar(c))
			{
				p = c->getPatientAssigned();
				p->setPickupTime(timestep + (p->getDistance() / c->getSpeed())); 
				outCars->enqueue(c, -1 * p->getPickupTime());
			}
		}

		//CarFailure(); //called after cars are out 
		//cancelled(); //called after cars are out

		//move from out to back
		while (outCars->peek(c, pri) && (pri * -1) <= timestep)
		{
			outCars->dequeue(c, pri);
			p = c->getPatientAssigned();

			pri += (-1 * (p->getDistance() / c->getSpeed()));
			c->setCarStatus(LOADED);

			backCars->enqueue(c, pri);
		}

		//move from back to free
		while (backCars->peek(c, pri) && (pri * -1) <= timestep)
		{
			backCars->dequeue(c, pri);
			// if car is not cancelled or failed
			if (c->dropOffPatient(p) && c->getCarStatus() != FAILED && p->getPatientType() != CANCELLATION)
			{
				c->setCarStatus(READY);
				AddFinishedPatient(p);
				//fpatients++;
				hospitals[c->getHID() - 1].AddCar(c);
			}
			else if (c->getCarStatus() == FAILED) {
				CheckUpList->enqueue(c);
				c->setchecktimestep(timestep); //recording when the car entered the checkup list 
			}
			else
			{
				c->setCarStatus(READY);
				//fpatients++;
				hospitals[c->getHID() - 1].AddCar(c);
			}
		}

		//move cars from checkup list to free
		while (CheckUpList->peek(c) && c->getCarcheckTime()==timestep) {
			CheckUpList->dequeue(c);
			c->setCarStatus(READY);
			// c->setchecktimestep(0); not sure ab this bs isa sah 
			hospitals[c->getHID() - 1].AddCar(c);
		}

		timestep++;
		ui->simulateMode();
	}
	WriteFile("output.txt", timestep);
	ui->endSimulation();
}

void Organizer::cancelled() {
	// cancellations 

		Patient* cp;
		Cars* ccar;
		int prior;
		while (!Cancellations->isEmpty())
		{
			cp = nullptr;
			ccar = nullptr;
			Cancellations->peek(cp);
			if (cp->getRequestTime() <= timestep)
			{
				Cancellations->dequeue(cp);
				priQueue <Cars*>* TempCars = new priQueue <Cars*>;
				Cars* tempcar;
				int tprior;
				//search for patient in outcars list 
				while (!outCars->isEmpty()) {
					outCars->peek(tempcar,tprior);
					if (tempcar->getPatientAssigned() == cp && !tempcar->getCarStatus() == LOADED) {
						//patient found and not picked up yet 
						outCars->dequeue(ccar, prior);

						//move from out to back
						prior += (-1 * (cp->getDistance() / ccar->getSpeed()));
						backCars->enqueue(ccar, prior);
						delete cp; //remove patient from the system 
						break;
					}
					else {
						outCars->dequeue(tempcar, tprior);
						TempCars->enqueue(tempcar, tprior);
					}
				}
				//patient found and handelled 
				while (!TempCars->isEmpty()) {
					TempCars->dequeue(tempcar,tprior);
					outCars->enqueue(tempcar, tprior);
				}
			}
			
		}



}

/////////////PHASE 1.2 SIMULATION, DON'T ADD TO IT, ADD TO mainSimulation()////////
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
	ui->endSimulation();
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
	Cars* tempcar;// = new Cars;
	int pri;
	int Fpri;
	Cars* failedcar;// = new Cars;
	//Patient* failedpatient;

	{
		srand(time(0));
		rndd = rand() % 101; //generates random number between 0 and 100
		if (rndd == carFailure)
		{
			rndc = (rand() % (outCars->getSize())) + 1; // generates random number between 1 and outcars number

			for (int j = 1; j <= rndc - 1; j++) { //deques all elements before desired car
				outCars->peek(tempcar, pri);
				outCars->dequeue(tempcar, pri); //deques first item
				TempCars->enqueue(tempcar, pri); // enques it in temp prique
				tempcar = nullptr;
			}

			outCars->peek(failedcar, Fpri);
			outCars->dequeue(failedcar, Fpri); //picked car is pointed at by failedcar ptr

			while (!TempCars->isEmpty()) { // returns cars back to outcars
				TempCars->peek(tempcar, pri);
				TempCars->dequeue(tempcar, pri);
				outCars->enqueue(tempcar, pri);
				tempcar = nullptr;
			}

			FailureAction(failedcar,Fpri);
		}
	}


}

//Hospital* Organizer::findSuitableHospital(int currentHospitalID)
//{
//	if (currentHospitalID < 1 || currentHospitalID > hospitalCount) {
//		return nullptr; 
//	}
//	int currentIndex = currentHospitalID - 1;
//
//	// get the minEP list 
//	int minEPCount = INT_MAX;
//	for (int i = 0; i < hospitalCount; i++) {
//		if (i != currentIndex) 
//		{  // Skip current hospital
//			int epCount = hospitals[i].getEmergencyRequestsCount();
//			if (epCount < minEPCount) 
//			{
//				minEPCount = epCount;
//			}
//		}
//	}
//
//	// Second pass: Among hospitals with minimum EP count, find nearest
//	int shortestDistance = INT_MAX;
//	Hospital* bestHospital = nullptr;
//
//	for (int i = 0; i < hospitalCount; i++) {
//		if (i != currentIndex &&
//			hospitals[i].getEmergencyRequestsCount() == minEPCount) {
//
//			int distance = distanceMatrix[currentIndex][i];
//			if (distance < shortestDistance) {
//				shortestDistance = distance;
//				bestHospital = &hospitals[i];
//			}
//		}
//	}
//
//	return bestHospital;
//}

Hospital* Organizer::FindEP_Random_Hospital(int currentHospitalID)
{
	if (hospitalCount <= 1)
	{
		return nullptr;
	}
	int currentIndex = currentHospitalID - 1;
	int randomIndex;

	do 
	{
		randomIndex = rand() % hospitalCount;
	}
	while (randomIndex == currentIndex);

	return &hospitals[randomIndex];


}

void  Organizer::FailureAction(Cars* fc, int pri) {

	Patient* failedp = nullptr;

	fc->setCarStatus(FAILED);

	backCars->enqueue(fc, pri); // car added to back list 

	failedp = fc->getPatientAssigned(); //gets the patient that was assigned to the failedcar

	//put patient on top of list to be assigned next
	hospitals[failedp->getNearestHospital()-1].movetotop(failedp);
}

void Organizer::AddFinishedPatient(Patient* p)
{
	if (p != nullptr && p->getPatientType() != CANCELLATION) 
	{
		finishedPatients->enqueue(p);
		fpatients++;
	}
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