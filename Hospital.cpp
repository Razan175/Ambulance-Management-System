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
		return SCars.dequeue(car);
		
}

bool Hospital::RemoveNCar(Cars*& car)
{
	return NCars.dequeue(car);

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

void Hospital::AssignPatient(Patient*& p)
{
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
	};
}

bool Hospital::RemovePatient(Patient*& p, Type t)
{
	int pri;// = p->getSeverity();





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