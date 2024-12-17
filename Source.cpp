#include "Organizer.h"
int main()
{
	Organizer* org = new Organizer;
	org->mainSimulation();
	delete org;
}