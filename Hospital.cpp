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

//Modify this function
bool Hospital::AssignPatient(Patient* p, Cars*& c)
{
	if (p->getPatientType() == EMERGENCY) {
		if (!NCars.isEmpty())
		{
			NCars.dequeue(c);
			c->pickupPatient(p);
			c->setCarStatus(ASSIGNED);
			return true;
		}
		else if (!SCars.isEmpty())
		{
			SCars.dequeue(c);
			c->pickupPatient(p);
			c->setCarStatus(ASSIGNED);
			return true;
		}
	}
	else if (p->getPatientType() == SPECIAL) 
	{
		if (!SCars.isEmpty())
		{
			SCars.dequeue(c);
			c->pickupPatient(p);
			c->setCarStatus(ASSIGNED);
			return true;
		}
	}
	else if (p->getPatientType() == NORMAL)
	{
		if (!NCars.isEmpty())
		{
			NCars.dequeue(c);
			c->pickupPatient(p);
			c->setCarStatus(ASSIGNED);
			return true;
		}
	}
	return false;

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
	return false;
	*/
}

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
	else if (t == CANCELLATION)
	{
		//Question1: is this the correct way to get the car or should i loop on all Ncars list "another linkedque"
		Cars* nc = nullptr;
		while (!NCars.isEmpty()) {
			NCars.peek(nc);
			if (nc->getPatientAssigned() == p && !nc->getCarStatus() == LOADED) {
				//patient is not picked up by the assigned car yet
				NPList.dequeue(p);
				//nc->setCarStatus(READY);
			}
		}
	}
	else
	{
		p = nullptr;
		return false;
	}

	return true;
}

//////////////////////////
/// SEND CAR FUNCTIONS///
////////////////////////

bool Hospital::sendEPCar(Cars* &c)
{
	Patient* p;
	int pri;
	if (!EPList.dequeue(p, pri))  //if list is empty ,return false
		return false;
	return AssignPatient(p, c);		//if there are no available cars, return false
}

bool Hospital::sendSPCar(Cars*& c)
{
	Patient* p;
	if (!SPList.dequeue(p))  //if list is empty ,return false
		return false;
	return AssignPatient(p, c);		//if there are no available cars, return false
}

bool Hospital::sendNPCar(Cars*& c)
{
	Patient* p;
	if (!NPList.dequeue(p))  //if list is empty ,return false
		return false;
	return AssignPatient(p, c);		//if there are no available cars, return false
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
