#include "Hospital.h"

bool Hospital::AddCar(Cars* &car)
{
	if (car->getCarType() == NORMAL)
		NCars.enqueue(car);
	else if (car->getCarType() == SPECIAL)
		SCars.enqueue(car);
	else
	{
		car = nullptr;
		return false;
	}
	return true;

}
//comment
bool Hospital::RemoveSCar(Cars*& car)
{
	Cars* p;
	if (SCars.peek(p))
	{
		if (p->getCarStatus() == ASSIGNED)
			return SCars.dequeue(car);
	}
	return false;
}

bool Hospital::RemoveNCar(Cars*& car)
{
	Cars* p;
	if (NCars.peek(p))
	{
		if (p->getCarStatus() == ASSIGNED)
			return NCars.dequeue(car);
	}
	return false;
}

void Hospital::AddPatient(Patient*& p)
{
	if (p->getPatientType() == EMERGENCY) {
		EPList.enqueue(p, p->getSeverity());
	}
	else if (p->getPatientType() == SPECIAL) {
		SPList.enqueue(p);
	}
	else if (p->getPatientType() == NORMAL) {
		NPList.enqueue(p);
	}
}

Type Hospital::GetAvailableCar(Type patientType)
{
	return Type();
}

//Finds the suitable car for patient p , assigns p to it and returns the car in variable c
//bool Hospital::AssignPatient(Patient* p, Cars*& c)
//{
	/*
	Cars* amp;
	if (p->getPatientType() == EMERGENCY) {
		if (!NCars.isEmpty()) {
			NCars.dequeue(amp);
			amp->setCarStatus(ASSIGNED);
		}
		else {
			SCars.dequeue(amp);
			amp->setCarStatus(ASSIGNED);
		}
	}
	else if (p->getPatientType() == SPECIAL) {
		SCars.dequeue(amp);
		amp->setCarStatus(ASSIGNED);
	}
	else if (p->getPatientType() == NORMAL) {
		NCars.dequeue(amp);
		amp->setCarStatus(ASSIGNED);
	}

	*/
	//return false;
//}

bool Hospital::RemovePatient(Patient*& p, Type t)
{
	int pri;

	if (t == EMERGENCY)
	{
		EPList.dequeue(p,pri);
	}
	else if (t == SPECIAL)
	{
		SPList.dequeue(p);
	}
	else if (t == NORMAL)
	{
		NPList.dequeue(p);
	}
	//else if (t == CANCELLATION)
	//{
	//	//loop on all Ncars list "another linkedque"
	//	Cars* nc = nullptr;
	//	while (NCars.isEmpty()) {
	//		NCars.peek(nc);
	//		if (nc->getPatientAssigned() == p && !nc->getCarStatus() == LOADED) {
	//			//patient is not picked up by the assigned car yet
	//			NPList.dequeue(p);
	//		}
	//	}
	//}
	else
	{
		p = nullptr;
		return false;
	}

	return true;
}

bool Hospital::movetotop(Patient* p) {
	int pri;
	Patient* temp = nullptr;
	Patient* target = nullptr;
	LinkedQueue <Patient*>* Tempp = new LinkedQueue <Patient*>;

	if (p->getPatientType() == EMERGENCY) { //emergency patient list 
		EPList.peek(temp, pri);
		EPList.enqueue(p, pri + 1); // patient enqued with the highest pri to get moved to front 
	}
	else if (p->getPatientType() == NORMAL) {
		while (NPList.peek(temp)) { //searching for target patient
			if (temp == p) { //patient found
				NPList.dequeue(target);
			}
			else { 
				NPList.dequeue(temp);
				Tempp->enqueue(temp);
			}
		}
		NPList.enqueue(target); 
		while (Tempp->peek(temp))
		{ // returnung patients to queue
			Tempp->dequeue(temp);
			NPList.enqueue(temp);
		}
	}
	else if (p->getPatientType() == SPECIAL) {
		while (SPList.peek(temp)) { //searching for target patient
			if (temp == p) { //patient found
				SPList.dequeue(target);
			}
			else {
				SPList.dequeue(temp);
				Tempp->enqueue(temp);
			}
		}
		SPList.enqueue(target); //enqued first to be assigned first
		while (Tempp->peek(temp))
		{ // returnung patients to queue
			Tempp->dequeue(temp);
			SPList.enqueue(temp);
		}
	}
}

//////////////////////////
/// SEND CAR FUNCTIONS///
////////////////////////

bool Hospital::sendEPCar(Cars* &c,Patient *&p)
{
	int pri;
	if (EPList.isEmpty())  //if list is empty ,return false
		return false;
	EPList.dequeue(p, pri);
	return AssignPatient(p, c);		//if there are no available cars, return false and send the patient to the organizer
									//to find them a new hospital
									//if true, assigns patient p to car and puts it in c
}

bool Hospital::sendSPCar(Cars*& c)
{
	Patient* p;
	if (SPList.isEmpty())  //if list is empty ,return false
		return false;
	SPList.peek(p);
	if (AssignPatient(p, c))
	{
		SPList.dequeue(p);
		return true;		//if there are no available cars, return false
	}
							//if true, puts the car with the assigned patient in C

}

bool Hospital::sendNPCar(Cars*& c)
{
	Patient* p;
	if (NPList.isEmpty())  //if list is empty ,return false
		return false;
	NPList.peek(p);
	if (AssignPatient(p, c))
	{
		NPList.dequeue(p);
		return true;
	}								//if there are no available cars, return false
									//if true, puts the car with the assigned patient in C
}
Type Hospital::GetAvailableCar(Type patientType)
{
	Cars* tempCar;

	if (patientType == EMERGENCY) {
		if (!NCars.isEmpty()) {
			NCars.peek(tempCar);
			if (tempCar->getCarStatus() == READY)
				return NORMAL;
		}
		if (!SCars.isEmpty()) {
			SCars.peek(tempCar);
			if (tempCar->getCarStatus() == READY)
				return SPECIAL;
		}
	}
	else if (patientType == SPECIAL) {
		if (!SCars.isEmpty()) {
			SCars.peek(tempCar);
			if (tempCar->getCarStatus() == READY)
				return SPECIAL;
		}
	}
	else if (patientType == NORMAL) {

		if (!NCars.isEmpty()) {
			NCars.peek(tempCar);
			if (tempCar->getCarStatus() == READY)
				return NORMAL;
		}
	}

	return CANCELLATION;
}

bool Hospital::AssignPatient(Patient* p, Cars*& c)
{
	if (!p)
	{
		return false;
	}
	c = nullptr;

	Type availableCarType = GetAvailableCar(p->getPatientType());

	if (availableCarType == CANCELLATION)

	{
		if (p->getPatientType() == EMERGENCY)
		{
			return false;
		}

		return false;
	}
	if (availableCarType == NORMAL)
	{
		NCars.dequeue(c);
	}
	else if (availableCarType == SPECIAL)
	{
		SCars.dequeue(c);
	}

	if (c && c->pickupPatient(p))
	{
		c->setCarStatus(ASSIGNED);
		return true;
	}

	if (c)
	{
		if (c->getCarType() == NORMAL)
			NCars.enqueue(c);
		else
			SCars.enqueue(c);
	}

	return false;
}



// not sure from this
int Hospital::getEmergencyRequestsCount()
{
	return EPList.getSize();
}
int Hospital::getSpecialRequestsCount()
{
	return SPList.getSize();
}
int Hospital::getNormalRequestsCount()
{
	return NPList.getSize();
}

int Hospital::getSCarsCount()
{
	return SCars.getSize();
}
int Hospital::getNCarsCount()
{
	return NCars.getSize();
}

LinkedQueue <Patient*>& Hospital::getSPList()
{
	return SPList;
}
priQueue <Patient*>& Hospital::getEPList()
{
	return EPList;
}
LinkedQueue <Patient*>& Hospital::getNPList()
{
	return NPList;
}
