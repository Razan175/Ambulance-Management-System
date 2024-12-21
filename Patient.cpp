#include "Patient.h"

Patient::Patient(int rt, int pid, int hid, Type t, int d, int s)
{

    RT = rt;
    PT = -1;
    P_ID = pid;
    near_hospital_ID = hid;
    distance = d;
    P_type = t;
    Severity = s;
}

int Patient::getID()
{
    return P_ID;
}

int Patient::getRequestTime()
{
    return RT;
}

int Patient::getNearestHospital()
{
    return near_hospital_ID;
}

int Patient::getPatientWaitingTime()
{
    return this->getPickupTime() - this->getRequestTime();
}

int Patient::getBusyCarAssigned()
{
    // busy time = when car assigned to patient when he request it + car going to patient and bring  him back to hospital
    return this->getFinishTime() - this->getRequestTime();
}

int Patient::getDistance()
{
    return distance;
}

Type Patient::getPatientType()
{
    return P_type;
}

int Patient::getPickupTime()
{
    return PT;
}

int Patient::getSeverity()
{
    return Severity;
}

void Patient::setPickupTime(int time)
{
    PT = time;
}

void Patient::setPatientType(Type PyT)
{
    P_type = PyT;
}

void Patient::setFinishTime(int ft)
{
    finishTime = ft;
}
//comment
