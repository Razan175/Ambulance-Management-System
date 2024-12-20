#include "Cars.h"

Cars::Cars(int id, int hid, Type type, int speed, int checkuptime, CarStatus status)
{
	carID = id;
	hospitalID = hid;
	carType = type;
	carSpeed = speed;
	carStatus = status;
	checkupTime = checkuptime;
	patientAssigned = nullptr;
}

// setters

void Cars::setHospitalID(int id)
{
	hospitalID = id;
}
void Cars::setCarType(Type t)
{
	carType = t;
}

void Cars::setSpeed(int s)
{
	carSpeed = s;
}

void Cars::setCarStatus(CarStatus cs)
{
	carStatus = cs;
}

void Cars::setchecktimestep(int timestep) {
	checkedtimestep = timestep;
}

// getters
//comment
int Cars::getID()
{
	return carID;
}

int Cars::getHID()
{
	return hospitalID;
}

Type Cars::getCarType()
{
	return carType;
}

int Cars::getSpeed()
{
	return carSpeed;
}

CarStatus Cars::getCarStatus()
{
	return carStatus;
}

Patient* Cars::getPatientAssigned()
{
	return patientAssigned;
}

int Cars::getCarcheckTime() {
	return checkupTime+checkedtimestep;
}

//bool pickupPatient(Patient* p);
//bool dropOffPatient(Patient* p); // should i use Patient*& p ?? ***

//bool Cars::pickupPatient(Patient* p)
//{
//	if (!p)
//		return false;
//
//	if (this->getCarStatus() == READY)
//	{
//		this->patientAssigned = p; 
//		this->carStatus = ASSIGNED; // assigned or loaded?? ***
//
//		return true;
//	}
//	return false;
//}
//
//
//bool Cars::dropOffPatient(Patient* p)
//{
//	if (patientAssigned)
//	{
//		patientAssigned = nullptr;
//		return true;
//	}
//	return false;
//
//}

bool Cars::pickupPatient(Patient*& p)
{
	if (patientAssigned)
		return false;
	patientAssigned = p;
	return true;
}

bool Cars::dropOffPatient(Patient* &p)
{
	if (!patientAssigned)
		return false;
	p = patientAssigned;
	patientAssigned = nullptr;

	return true;
}

Cars::~Cars()
{
	// is it important ?? ***	
}