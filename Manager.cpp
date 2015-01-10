#include "Manager.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <stdio.h>
using namespace std;

Manager::Manager()
{
	id = NULL;
}
void Manager::setID(int cid){ id = cid; }
 int Manager::getID(){ return id; }

