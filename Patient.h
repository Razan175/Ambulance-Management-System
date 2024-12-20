#pragma once
#include "Enums.h"
#include <iostream>
#include <string>
#include "DSfile/LinkedQueue.h"
#include "DSfile/priQueue.h"

using namespace std;
// in general han3ml list l kol kind of patient.

#define MAX_SEVERITY 5
class Patient
{
private:
    // data members 
    int RT;
    int PT;
    int near_hospital_ID;
    int P_ID;
    int distance;
    int Severity;
    Type P_type; // get type of patient 
public:
    // constructor + member functions

    Patient(int rt, int pid, int hid, Type t, int d = -1, int s = 0);
    // getters
    int getID();
    int getRequestTime();
    int getNearestHospital();
    int getDistance();
    Type getPatientType();
    int getPickupTime();
    int getSeverity();
    // setter
    void setPickupTime(int time);
    void setPatientType(Type PyT);

};


