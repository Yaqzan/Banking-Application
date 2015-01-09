#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <stdio.h>
using namespace std;
class client
{
public:
	client(); //Constructor
	~client(){} //Destructor
	//Setters
	void setChequing(double chequing);
		void setSavings(double saving);
		void setId(int cid);
		void setFirstName(string name);
		void setLastName(string name);
		void setCredLimit(double credL);
		void setPin(int PIN);
		void setFrozen(bool setit);
		void setPayInFull(bool set);
		void setInterest(double inter);
	//Getters
	double getSavings();		// return amount under savings account
	double getChequing();	// Return amount under chequing account
	int getId();					// Return user Id;
	string getFirstName();			// Return first name;
	string getLastName();			// Return last name;
	double getCredLimit();
	bool isFrozen();
	int getPin();
	int getSizeOfPurchases();
	void purchase(double ammount);
	double getPurchase(int i);
	double getAmmountOwing();
	double payDay();
	bool getPayInFull();
	double getInterest();
	double getAmmountOwed();
private:
	int id, pin;
	double chequingAcc, savingsAcc, cred, limit, interest;
	string name1, name2;
	bool frozen, payInFull;
	vector<double> purchases;
};
