#include "Mperson.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <stdio.h>
using namespace std;
Mperson::Mperson(int ID)
{
	id = ID;
}

Mperson::Mperson(){
	id = NULL;
}
Mperson::~Mperson()
{
}

int Mperson::getID(){
	return id;
}
void Mperson::setID(int newID){
	id = newID;
}
