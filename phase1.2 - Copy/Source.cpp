#include "Organizer.h"
int main()
{
	Organizer* org = new Organizer;
	org->Simulate("Input.txt");
	delete org;
}