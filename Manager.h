#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <stdio.h>
#include "client.h"
using namespace std;
class Manager
{
public:
	Manager(int ID, vector<client> list);
	Manager();
	~Manager(){};
	void setID(int cid);
	int getID();
private:
	int id;
};

