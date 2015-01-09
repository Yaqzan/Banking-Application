#include "client.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <stdio.h>
using namespace std;
client::client()
{
	id = 0;
	chequingAcc = 0.0;
	savingsAcc = 0.0;
	name1 = " ";
	name2 = " ";
	limit = 0.0;
	cred = 0.0;
	pin = 0;
	frozen = false;
	payInFull = false;
}
//Setters
void client::setChequing(double chequing){ chequingAcc = chequing; }
void client::setSavings(double saving){ savingsAcc = saving; }
void client::setId(int cid){ id = cid; }
void client::setFirstName(string name){ name1 = name; }
void client::setLastName(string name){ name2 = name; }
void client::setCredLimit(double credL){ limit = credL; }
void client::setCred(double cre){ cred = cre; }
void client::setPin(int PIN){ pin = PIN; }
void client::setFrozen(bool setit){ frozen=setit; }
void client::setPayInFull(bool set){ payInFull = set; }
void client::setInterest(double inter){ interest = inter; }
//Getters
double client::getSavings(){ return savingsAcc; }		// return amount under savings account
double client::getChequing(){ return chequingAcc; }		// Return amount under chequing account
int client::getId(){ return id; }						// Return user Id;
string client::getFirstName(){ return name1; }			// Return first name;
string client::getLastName(){ return name2; }			// Return last name;
double client::getCredLimit(){ return limit; }
double client::getCred(){ return cred; }
int  client::getPin(){ return pin; }
bool client::isFrozen(){ return frozen; }
int client::getSizeOfPurchases(){ return purchases.size(); }
bool client::getPayInFull(){ return payInFull; }
double client::getInterest(){ return interest; }
//Purchases
void client::purchase(double ammount){ purchases.push_back(ammount); }
double client::getPurchase(int i){ return purchases[i]; }
double client::payDay(){
	double sum = 0;
	for (int i = 0; i < purchases.size(); i++){
		sum += purchases[i];
	}
	purchases.clear();
	if (sum > 0){
		
		if (payInFull == true){
			sum += interest;
			return sum;
		}
		else{
			interest += (sum*0.9);
			interest += (interest*0.02);
			return sum*0.10;
		}
	}
	else
		return 0;
}
double client::getAmmountOwed(){
	double sum = 0;
	for (int i = 0; i < purchases.size(); i++){
		sum += purchases[i];
	}
	return sum;
}
double client::getAmmountOwing(){return interest;}
