#include "Organizer.h"
int main()
{
	Organizer* org = new Organizer;
	org->randSimulation();
	delete org;
}