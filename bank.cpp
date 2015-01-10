/********************************************
Banking Program

* Group 27
* Authors: Alexander Gyori, Yaqzan Ali
* CS3307 Assignment 1

********************************************/
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <stdio.h>
#include <time.h> 
#include <fstream>
#include "Mperson.h"
#include "client.h"
#include "Manager.h"
#define ON true
#define OFF false
using namespace std;

// The Customer class

//Manager Menus and Functions
void managerMenu(vector<client>& clients);					 // Brings user to the manager screen
bool openAccount(vector<client>& clients, int accID, string firstName, string lastName);
bool deleteAccount(vector<client>& clients, int accID); //
void printManMenu(void);    //Prints manager menu

//Maintenance Menus and Functions
void maintenanceMenu(void);		 // Brings user to the maintenance screen
void toggleExecutionTraces(void);   // Sets execution traces from on --> off or from off --> on.
void setExecutionTraces(bool x);   // Turns execution traces on or off depending on x. (true = on, false = off)
void writeToET(string str);  // Writes str to the file execution_traces.txt 
void printExecutionTraces(void);
void clearExecutionTracesLog(void);
void saveExternals(Manager manager, Mperson maintain);

//Client Menus and Functions
void clientMenu(client& theClient, vector<client>& clients); //Brings user to the screen for clients
void printClientMenu(client& theClient, bool cheq, bool sav);// Prints the options available to the client
void purchaseMenu(client& theClient);
void savePurchases(vector<client>& clients);
void viewPurchases(client& theClient);
void saveVendor(client& theClient, double price);
void deposit(client& theClient);
void withdraw(client& theClient);
void withdrawChequing(client& theClient);
void withdrawSavings(client& theClient);
void transfer(client& theClient);
void transferChequing(client& theClient);
void transferSavings(client& theClient);
void chequing(client& theClient, bool cheq, bool sav);
void savings(client& theClient, bool sav, bool cheq);
void updateMonth(vector<client>& clients);
void payInFullMenu(client& theClient);
void writeFailedPayments(client& theClient);
//Helper functions
int getNumber();        // Retrieves an int value from the user
double getDouble();     // Retrieves a double value from the user
void saveClients(vector<client>& clients);					 // Writes client data to text file/ saves the changes
int getAccountIndex(vector<client>& clients, int targetAccNum, int low, int high);
bool compareAccountLastName(client a, client b);
bool compareAccountNums(client a, client b);
bool existsInDatabase(vector<client>& clients, int targetAccID);
void sortByAccountNumber(vector<client>& clients);
vector<client> sortByLastName(vector<client> clients);
bool penalty(client& theClient, double amount);

// Global Variables
bool executionTraces; // This is defined main().
stringstream et;
int endOfMonth;	// The end of the month for credit
int main()
{
	et << "main():\n"; writeToET(et.str()); et.str("");
	cout.precision(2);
	srand(time(NULL));

	// Initialize executionTraces.
	// Read from the file execution_traces.txt and check the first line.  The first line saves the state of whether the ET is on/off.
	// 1 = ON, 0 = OFF
	ifstream etFile("execution_traces.txt");
	if (etFile.is_open()){
		// Read the file's first line.
		string etStr;
		getline(etFile, etStr);
		if (etStr.compare("1") == 0){  // If 1, then on
			executionTraces = ON;
		}
		else if (etStr.compare("0") == 0){  // If 0, then off
			executionTraces = OFF;
		}
		else{  // If neither, then off and error message
			executionTraces = OFF;
			cout << "Error: Could not read current state of execution traces. Execution traces are OFF by default.\n";
		}

	}
	else{
		cout << "Could not open execution_traces.txt. Execution traces are OFF by default.\n";
		executionTraces = OFF;
	}

	// initialize everything else
	const int managerId = NULL;
	const int maintenId = NULL;
	vector<client> clients;		// THe client vector array
	int id, i = 0, getId, pin;;
	double chequings, savings, credLimit, cred, limit, owed;
	string line, name1, name2, freeze, label, full;
	client *f1;
	ifstream myfile("clients.txt");


	// Read client info from text file into vector array
	if (myfile.is_open()){
		while (getline(myfile, line))
		{
			stringstream ss(line);
			ss >> id >> chequings >> savings >> name1 >> name2 >> pin >> freeze >> limit >> full >> owed;
			f1 = new client;
			f1->setId(id);
			f1->setChequing(chequings);
			f1->setSavings(savings);
			f1->setFirstName(name1);
			f1->setLastName(name2);
			f1->setPin(pin);
			f1->setCredLimit(limit);
			if (freeze == "frozen")
				f1->setFrozen(true);
			else
				f1->setFrozen(false);
			if (full == "full")
				f1->setPayInFull(true);
			else
				f1->setPayInFull(false);

			f1->setInterest(owed);
			clients.push_back(*f1);
			i++;
		}
		myfile.close();
	}
	else{
		cout << "Error: Client file not found\n";
	}
	i = 0;
	double item;
	ifstream purchaseFile("client_purchases.txt");
	// Read client purchases from text file into vector array
	if (purchaseFile.is_open()){
		while (getline(purchaseFile, line))
		{
			istringstream ss(line);
			ss >> name1;
			do
			{
				double item = NULL;
				ss >> item; 
				if (item!=NULL)
					clients[i].purchase(item);
			} while (ss);
			
			i++;
		}
		purchaseFile.close();
	}
	else{
		cout << "Error: Client purchases file not found\n";
	}
	
	Mperson maintainPerson;
	Manager manager;
	ifstream maintainFile("externals.txt");

	// Read external info from text file;
	if (maintainFile.is_open()){
		// Get maintainance person ID from file
		getline(maintainFile, line);
		stringstream ss(line);
		ss >> label >> id;
		maintainPerson.setID(id);

		// Get manager ID from file
		getline(maintainFile, line);
		stringstream ss2(line);
		ss2>> label >> id;
		manager.setID(id);

		// Get end of month data from file
		getline(maintainFile, line);
		stringstream ss3(line);
		ss3 >> label >> id;
		endOfMonth = id;
		// Close file
		myfile.close();
		
	}
	else{
		cout << "Error: Externals file not found\n";
	}


	if (endOfMonth == 0){ // if end of month, time to pay up
		et << "End of Month\n"; writeToET(et.str()); et.str("");
		updateMonth(clients);
		endOfMonth = 30;
		saveClients(clients);
		savePurchases(clients);
	}
	et << "A day goes by\n"; writeToET(et.str()); et.str("");
	endOfMonth--; // A day goes by
	saveExternals(manager, maintainPerson);


	cout << endl << "***********---Welcome to Group 27's Banking System!---***********\n";

	while (true){
		bool found = false;
		while (!found){
			cout << endl << "Please enter your login ID: ";
			getId = getNumber();
			if (getId == manager.getID()){
				managerMenu(clients);
				found = true;
			}
			else if (getId == maintainPerson.getID()){
				maintenanceMenu();
				found = true;
			}
			else if (existsInDatabase(clients, getId)){
				int x = getAccountIndex(clients, getId, 0, clients.size() - 1);
				clientMenu(clients[x], clients);
				found = true;
			}
			if (!found)
				cout << endl << "Sorry, that ID is not in our servers\n";

		}
	}

	et << "End of main()\n"; writeToET(et.str()); et.str("");
	return 0;
}

void clientMenu(client& theClient, vector<client>& clients){
	et << "Logged in as client #" << theClient.getId() << "\n"; writeToET(et.str()); et.str("");
	cout << endl << "Welcome " << theClient.getFirstName() << "!\n";
	bool cheq, sav, logout = false;
	while (!logout){
		if (theClient.getChequing() == 0){ cheq = false; }
		else { cheq = true; }
		if (theClient.getSavings() == 0){ sav = false; }
		else { sav = true; }

		printClientMenu(theClient, cheq, sav);

		switch (getNumber()) {
		case 1:
			et << theClient.getId() << " selected 1: Chequing\n"; writeToET(et.str()); et.str("");
			chequing(theClient, cheq, sav);
			saveClients(clients);
			break;
		case 2:
			et << theClient.getId() << " selected 2: Savings\n"; writeToET(et.str()); et.str("");
			savings(theClient, sav, cheq);
			saveClients(clients);
			break;
		case 3:
			et << theClient.getId() << " selected 3: Deposit\n"; writeToET(et.str()); et.str("");
			deposit(theClient);
			saveClients(clients);
			break;
		case 4:
			et << theClient.getId() << " selected 4: Withdraw\n"; writeToET(et.str()); et.str("");
			withdraw(theClient);
			saveClients(clients);
			break;
		case 5:
			et << theClient.getId() << " selected 5: View balance\n"; writeToET(et.str()); et.str("");
			if (cheq){
				cout << endl << "Chequings Account: $" << fixed << theClient.getChequing() << "\n";
			}if (sav){
				cout << endl << "Savings Account:   $" << fixed << theClient.getSavings() << "\n";
			}
			if (!sav && !cheq){
				cout << endl << "You don't have any accounts open. Please open an account first\n";
			}
			break;
		case 6:
			et << theClient.getId() << " selected 6: Tranfer\n"; writeToET(et.str()); et.str("");
			transfer(theClient);
			saveClients(clients);
			break;
		case 7:
			et << theClient.getId() << " selected 7: Purchase\n"; writeToET(et.str()); et.str("");
			if (theClient.isFrozen()){
				et << theClient.getId() << " tried to purchase when account was frozen\n"; writeToET(et.str()); et.str("");
				cout << endl << "Your Account is frozen. You may not purchase anything\n";
			}
			else{
				purchaseMenu(theClient);
				savePurchases(clients);
				saveClients(clients);
			}
			saveClients(clients);
			
			break;
		case 8:
			et << theClient.getId() << " selected 8: View Purchaces\n"; writeToET(et.str()); et.str("");
			viewPurchases(theClient);
			break;
		case 9:
			et << theClient.getId() << " selected 9: Toggle Pay in full\n"; writeToET(et.str()); et.str("");
			payInFullMenu(theClient);
			saveClients(clients);
			break;
		case 10:
			et << theClient.getId() << " selected 10: Logout\n"; writeToET(et.str()); et.str("");
			logout = true;
			break;
		default:
			et << theClient.getId() << " entered invalid input\n"; writeToET(et.str()); et.str("");
			cout << "Invalid input \n";
		}
	}
}
void payInFullMenu(client& theClient){
	
	bool finished = false;
	while (!finished){
		if (theClient.getPayInFull() == true){
			cout << endl << "You are set to pay your FULL debt at the end of the month.\n";
			cout << "Would you like to set it to pay the minimum payment instead?\n";
		}
		else{
			cout << endl << "You are set to pay the minimum debt at the end of the month.\n";
			cout << "Would you like to set it to pay the full debt isntead?\n";
		}
		cout << "1: Yes\n";
		cout << "2: No\n";

		switch (getNumber()){
		case 1:
			et << theClient.getId() << " Toggled Full payment\n"; writeToET(et.str()); et.str("");
			if (theClient.getPayInFull() == true)
				theClient.setPayInFull(false);
			else
				theClient.setPayInFull(true);
			finished = true;
			break;
		case 2:
			et << theClient.getId() << " Cancelled Toggle\n"; writeToET(et.str()); et.str("");
			finished = true;
			break;
		default:
			et << theClient.getId() << " entered invalid input\n"; writeToET(et.str()); et.str("");
			cout << "Invalid input \n";
		}
	}
}
void purchaseMenu(client& theClient){

	double price = rand() % 10000 + 1;
	price /= 100.00;
	bool finished = false;
	bool fin = false;
	while (!finished){
		cout << endl << "Confirm purchase of $" << fixed << price << "?\n";
		cout << "0: No" << endl;
		cout << "1: Yes" << endl;

		switch (getNumber()) {
		case 0:
			et << theClient.getId() << "  Cancelled purchase\n"; writeToET(et.str()); et.str("");
			finished = true;
			break;
		case 1:
			et << theClient.getId() << " Confirmed Purchase\n"; writeToET(et.str()); et.str("");
			cout << "Enter PIN:      Or enter 1 to cancel\n";
			
			while (!fin){
				int thePin = getNumber();
				if (thePin == 1){
					et << theClient.getId() << "  Cancelled purchase\n"; writeToET(et.str()); et.str("");
					fin = true;
				}
				else if (thePin == theClient.getPin()){
					et << "Entered PIN correctly\n"; writeToET(et.str()); et.str("");
						et << theClient.getId() << " Finished Purchase of $"<< price<<"\n"; writeToET(et.str()); et.str("");
						cout << endl<< " Finished Purchase of $" << price << "\n";
						theClient.purchase(price);
						saveVendor(theClient, price);
						fin = true;
					
				}
				else{
					et << " Entered Invalid Pin\n"; writeToET(et.str()); et.str("");
					cout << "Invalid Pin. Try Again\n ";
				}
			}
			finished = true;
			break;
		default:
			et << theClient.getId() << " entered invalid input\n"; writeToET(et.str()); et.str("");
			cout << "Invalid input \n";
		}
	}
}
void writeFailedPayments(client& theClient){
	et << "Notifying Bank of failed payment. Storing Failed payment to file.\n"; writeToET(et.str()); et.str("");
	ofstream failedFile;

	failedFile.open("failed_payments.txt", ios_base::app);
	failedFile << theClient.getId() << " " << fixed << theClient.getAmmountOwing()/10 << endl;
}
void chequing(client& theClient, bool cheq, bool sav){
	bool done = false;
	double chequing = theClient.getChequing();
	while (!done){
		done = true;
		if (cheq == false){ //open chequing account
			cout << endl << "You have opened your chequing account!";
			cout << " How much would you like to deposit?\n";
			double x = getDouble();
			theClient.setChequing(x);
			et << theClient.getId() << " opened a chequing account with $" << x << "\n"; writeToET(et.str()); et.str("");
			cout << endl << "Successfully deposited $" << fixed << x << " into chequing account\n";
		}
		else{
			et << theClient.getId() << " is closing his/her chequing account.\n"; writeToET(et.str()); et.str("");
			cout << endl << "You have closed your chequing account.\n";
			if (sav){
				cout << " Would you like to transfer the money into your savings account?\n";
				cout << "1: Yes\n";
				cout << "2: No\n";

				switch (getNumber()){
				case 1: //Transfer money from chequings to savings
					et << theClient.getId() << " selected 1: Yes, transfer to savings\n"; writeToET(et.str()); et.str("");
					theClient.setSavings(theClient.getSavings() + chequing);
					cout << endl << "Successfully transfered $" << fixed << chequing << " into savings account\n";
					break;
				case 2: // Do not transfer
					et << theClient.getId() << " selected 2: No, do not transfer\n"; writeToET(et.str()); et.str("");
					break;
				default: //invalid handler
					et << theClient.getId() << " entered invalid input\n"; writeToET(et.str()); et.str("");
					cout << endl << "Invalid input.\n";
					done = false;
				}
			}
			cout << "\n";
			theClient.setChequing(0);
			et << "Chequing set to 0.\n"; writeToET(et.str()); et.str("");
		}
	}
}

void savings(client& theClient, bool sav, bool cheq){
	bool done = false;
	double saving = theClient.getSavings();
	while (!done){
		done = true;
		if (sav == false){ //if the savings account is closed
			et << theClient.getId() << " is opening a savings account.\n"; writeToET(et.str()); et.str("");
			cout << endl << "You have opened your savings account!";
			cout << " How much would you like to deposit?\n";
			double x = getDouble();
			theClient.setSavings(x);
			cout << endl << "Successfully deposited $" << fixed << x << " into Savings account\n";
			et << theClient.getId() << " opened a savings account with $" << fixed << x << "\n"; writeToET(et.str()); et.str("");
		}
		else{ //if the savings account is open
			et << theClient.getId() << " is closing his/her savings account.\n"; writeToET(et.str()); et.str("");
			cout << endl << "You have closed your savings account.";
			if (cheq){
				cout << " Would you like to transfer the money into your chequing account?\n";
				cout << "1: Yes\n";
				cout << "2: No\n";

				switch (getNumber()){

				case 1: // Transfer from savings to chequings
					et << theClient.getId() << " selected 1: Transfer to chequing\n"; writeToET(et.str()); et.str("");
					theClient.setChequing(theClient.getChequing() + saving);
					cout << endl << "Successfully transfered $" << fixed << saving << " into chequing account\n";
					break;
				case 2: // Do not transfer
					et << theClient.getId() << " selected 2: Do not transfer to chequing\n"; writeToET(et.str()); et.str("");
					break;
				default: // invalid
					cout << endl << "Invalid input.\n";
					done = false;
				}
			}
			cout << "\n";
			theClient.setSavings(0);
			et << "Savings account set to $0\n"; writeToET(et.str()); et.str("");
		}
	}
}
void transfer(client& theClient){
	bool done = false;
	while (!done){
		cout << endl << "***********---Which account would you like to transfer from?---***********\n";
		cout << "---1: Chequing Account---\n";
		cout << "---2: Savings Account----\n";
		cout << "---3: Cancel-------------\n";
		switch (getNumber()){
		case 1:
			et << theClient.getId() << " selected 1: Transfer from Chequing\n"; writeToET(et.str()); et.str("");
			transferChequing(theClient);
			return;
		case 2:
			et << theClient.getId() << " selected 2: Transfer from Savings\n"; writeToET(et.str()); et.str("");
			transferSavings(theClient);
			return;
		case 3:
			et << theClient.getId() << " selected 3: Cancel\n"; writeToET(et.str()); et.str("");
			return;
		default:
			et << theClient.getId() << " entered invalid input\n"; writeToET(et.str()); et.str("");
			cout << endl << "Error: Invalid input\n";
		}
	}


}
void deposit(client& theClient){
	bool done = false;
	double x;
	while (!done){
		done = true;
		cout << endl << "***********---Which account would you like to deposit in?---***********\n";
		cout << "---1: Chequing Account---\n";
		cout << "---2: Savings Account----\n";
		cout << "---3: Cancel-------------\n";

		switch (getNumber()){

		case 1:  // Deposit into chequing account
			et << theClient.getId() << " selected 1: Deposit in Chequing\n"; writeToET(et.str()); et.str("");
			cout << endl << "How much would you like to deposit?\n";
			x = getDouble();
			theClient.setChequing(theClient.getChequing() + x);
			cout << endl << "Deposited $" << fixed << x << " into Chequing Account\n";
			et << theClient.getId() << " deposited $" << fixed << x << " in to his/her chequing\n"; writeToET(et.str()); et.str("");

			if (theClient.isFrozen() == true){
				if (theClient.getChequing() > (theClient.getAmmountOwing()/10)){
					theClient.setFrozen(false);
					cout << endl << "Your account has been unfrozen\n";
					et << " Client #" << theClient.getId() << "'s account has been immediately unfrozen.\n"; writeToET(et.str()); et.str("");
				}
			}

			break;
		case 2:     // Deposit into savings account
			et << theClient.getId() << " selected 2: Deposit in Savings\n"; writeToET(et.str()); et.str("");
			cout << endl << "How much would you like to deposit?\n";
			x = getDouble();
			theClient.setSavings(theClient.getSavings() + x);
			cout << endl << "Deposited $" << fixed << x << " into Savings Account\n";
			et << theClient.getId() << " deposited $" << fixed << x << " in to his/her savings\n"; writeToET(et.str()); et.str("");
			break;
		case 3:     // Cancel
			et << theClient.getId() << " canceled.\n"; writeToET(et.str()); et.str("");
			return;
		default:    // Invalid input
			et << theClient.getId() << " entered invalid input\n"; writeToET(et.str()); et.str("");
			cout << "Invalid input\n";
			done = false;
		}
	}
}
void withdraw(client& theClient){
	et << "withdraw(client& theClient #:" << theClient.getId() << ")\n"; writeToET(et.str()); et.str("");
	bool done = false;
	while (!done){
		cout << endl << "***********---Which account would you like to withdraw from?---***********\n";
		cout << "---1: Chequing Account---\n";
		cout << "---2: Savings Account----\n";
		cout << "---3: Cancel-------------\n";
		switch (getNumber()){
			//Withdraw from chequing account.
		case 1:{
			et << theClient.getId() << " selected 1: Withdraw from Chequing\n"; writeToET(et.str()); et.str("");
			withdrawChequing(theClient);
			return;
		}
			   // Withdraw from savings account.
		case 2:{
			et << theClient.getId() << " selected 2: Withdraw from Savings\n"; writeToET(et.str()); et.str("");
			withdrawSavings(theClient);
			return;
		}
			   // Cancel.
		case 3:{
			et << theClient.getId() << " canceled\n"; writeToET(et.str()); et.str("");
			return;
		}
		default:{
			et << theClient.getId() << " entered invalid input\n"; writeToET(et.str()); et.str("");
			cout << endl << "Error: Invalid input\n";
		}

		}

	}
}

void transferChequing(client& theClient) {
start:
	cout << endl << "How much would you like to transfer?\n";
	cout << "Enter 0 to cancel.\n";
	double x = getDouble();
	//Ask user for how much they want to transfer
	//Cancel
	if (x == 0) {
		et << theClient.getId() << " entered 0 to cancel\n"; writeToET(et.str()); et.str("");
		return;
	}//if the client tries to transfer more than they have 
	else if (x > theClient.getChequing()) {
		et << theClient.getId() << " tried to transfer too much money\n"; writeToET(et.str()); et.str("");
		cout << endl << "Error: Insufficient funds. You have $" << fixed << theClient.getChequing() << " in your chequing account.\n";
		goto start;
	}
	else {
		et << theClient.getId() << " entered a potential valid transfer amount\n"; writeToET(et.str()); et.str("");
		// Check if this puts the user under $1000 in the chequing account and apply a penalty if this is true.
		if (penalty(theClient, x)) { // There is a penalty.
			et << theClient.getId() << " faces a penalty for the withdrawal\n"; writeToET(et.str()); et.str("");
		warning:
			// Warn the user if they want to proceed with the withdrawal.
			cout << "Because this transaction will bring your chequing account's balance below $1000.00, a\n";
			cout << "$2.00 fee will applied. Do you wish to continue?\n";
			cout << "1: Yes\n";
			cout << "2: No\n";
			switch (getNumber()) {
			case 1:
			{
				et << theClient.getId() << " accepted the penalty\n"; writeToET(et.str()); et.str("");
				// Go through with the transaction.
				// Check if the $2.00 penalty will put the balance below $0.
				if (theClient.getChequing() - x - 2.00 < 0) {
					et << theClient.getId() << " does not have enough balance to accept the penalty\n"; writeToET(et.str()); et.str("");
					cout << endl << "Error: Insufficient funds. You have $" << fixed << theClient.getChequing() << " in your chequing account.\n";
					goto start;
				}
				else {
					// Perform the transaction.
					theClient.setChequing(theClient.getChequing() - x - 2.00);
					theClient.setSavings(theClient.getSavings() + x);
					cout << "Successfully transferred $" << fixed << x << " from your chequing account to your savings account.\n";
					cout << "Your chequing balance is $" << fixed << theClient.getChequing() << endl;
					cout << "Your savings balance is $" << fixed << theClient.getSavings() << endl;
					et << theClient.getId() << " successfully transferred with a penalty\n"; writeToET(et.str()); et.str("");
					return;
				}
				break;
			}
			case 2:
			{
				et << theClient.getId() << " cancelled the withdrawal\n"; writeToET(et.str()); et.str("");
				// Cancel the transaction and exit the withdraw menu.
				cout << "Transfer canceled. You have not been charged.\n";
				return;
			}
			default:
			{
				et << theClient.getId() << " entered invalid input\n"; writeToET(et.str()); et.str("");
				cout << endl << "Error: Invalid input\n";
				goto warning;
			}
			}
		}
		else { // No penalty.
			theClient.setChequing(theClient.getChequing() - x);
			theClient.setSavings(theClient.getSavings() + x);
			cout << endl << "Successfully transferred $" << fixed << x << " from chequing account\n";
			cout << "Your current chequing balance is $" << fixed << theClient.getChequing() << endl;
			cout << "Your current savings balance is $" << fixed << theClient.getSavings() << endl;
			et << theClient.getId() << " transferred $" << fixed << x << " without a penalty from chequing to savings\n"; writeToET(et.str()); et.str("");
		}

	}
}

void transferSavings(client& theClient) {
	cout << endl << "How much would you like to withdraw?\n";
	cout << "Enter 0 to cancel.\n";
	double ammount;
	ammount = getDouble();
	if (ammount == 0) {
		et << theClient.getId() << " entered 0 to cancel\n"; writeToET(et.str()); et.str("");
		return;
	}
	else if (ammount > theClient.getSavings()) {
		et << theClient.getId() << " tried to transfer too much money\n"; writeToET(et.str()); et.str("");
		cout << endl << "Error: Insufficient funds. You have $" << fixed << theClient.getSavings() << " in your Savings account\n";
		return;
	}
	theClient.setChequing(theClient.getChequing() + ammount);
	theClient.setSavings(theClient.getSavings() - ammount);
	cout << "Successfully transfered $" << fixed << ammount << " from Savings account to Chequing Account\n";
	cout << "Your current chequing balance is $" << fixed << theClient.getChequing() << endl;
	cout << "Your current savings balance is $" << fixed << theClient.getSavings() << endl;
	et << theClient.getId() << " successfully transferred $" << fixed << ammount << " from savings to chequing\n"; writeToET(et.str()); et.str("");
	if (theClient.isFrozen() == true){
		if (theClient.getChequing() > (theClient.getAmmountOwing() / 10)){
			theClient.setFrozen(false);
			cout << endl << "Your account has been unfrozen\n";
			et << " Client #" << theClient.getId() << "'s account has been immediately unfrozen.\n"; writeToET(et.str()); et.str("");
		}
	}
}

void withdrawChequing(client& theClient) {
start:
	cout << endl << "How much would you like to withdraw?\n";
	cout << "Enter 0 to cancel.\n";
	double x = getDouble();
	if (x == 0) {
		et << theClient.getId() << " entered 0 to cancel\n"; writeToET(et.str()); et.str("");
		return;
	}
	else if (x > theClient.getChequing()) {
		et << theClient.getId() << " tried to withdraw too much money\n"; writeToET(et.str()); et.str("");
		cout << endl << "Error: Insufficient funds. You have $" << theClient.getChequing() << " in your chequing account.\n";
		goto start;
	}
	else {
		// Check if this puts the user under $1000 in the chequing account and apply a penalty if this is true.
		if (penalty(theClient, x)) { // There is a penalty.
			et << theClient.getId() << " will take a penalty if he/she proceeds with the withdrawal\n"; writeToET(et.str()); et.str("");
		warning:
			// Warn the user if they want to proceed with the withdrawal.
			cout << "Because this transaction will bring your chequing account's balance below $1000.00, a\n";
			cout << "$2.00 fee will applied. Do you wish to continue?\n";
			cout << "1: Yes\n";
			cout << "2: No\n";
			switch (getNumber()) {
			case 1:
			{
				et << theClient.getId() << " selected 1: Proceed with penalty\n"; writeToET(et.str()); et.str("");
				// Go through with the transaction.
				// Check if the $2.00 penalty will put the balance below $0.
				if (theClient.getChequing() - x - 2.00 < 0) {
					et << theClient.getId() << " could not complete the withdrawal with the penalty\n"; writeToET(et.str()); et.str("");
					cout << endl << "Error: Insufficient funds. You have $" << fixed << theClient.getChequing() << " in your chequing account.\n";
					goto start;
				}
				else {
					// Perform the transaction.
					theClient.setChequing(theClient.getChequing() - x - 2.00);
					cout << "Successfully withdrew $" << fixed << x << " from chequing account.\n";
					cout << "Your current chequing balance is $" << fixed << theClient.getChequing();
					et << theClient.getId() << " completed the withdrawal with a penalty\n"; writeToET(et.str()); et.str("");
					return;
				}
				break;
			}
			case 2:
			{
				et << theClient.getId() << " selected 2: Cancel\n"; writeToET(et.str()); et.str("");
				// Cancel the transaction and exit the withdraw menu.
				cout << "Withdrawal canceled. You have not been charged.\n";
				return;
			}
			default:
			{
				et << theClient.getId() << " entered invalid input\n"; writeToET(et.str()); et.str("");
				cout << endl << "Error: Invalid input\n";
				goto warning;
			}

			}
		}
		else { // No penalty.
			theClient.setChequing(theClient.getChequing() - x);
			cout << endl << "Successfully withdrew $" << fixed << x << " from chequing account\n";
			cout << "Your current chequing balance is $" << fixed << theClient.getChequing();
			et << theClient.getId() << " successfully withdrew money without a penalty\n"; writeToET(et.str()); et.str("");
		}

	}
}

void withdrawSavings(client& theClient){
start:
	cout << endl << "How much would you like to withdraw?\n";
	cout << "Enter 0 to cancel.\n";
	double x = getDouble();
	if (x == 0) {
		et << theClient.getId() << " entered 0 to cancel\n"; writeToET(et.str()); et.str("");
		return;
	}
	else if (x > theClient.getSavings()) {
		et << theClient.getId() << " tried to withdraw too much money\n"; writeToET(et.str()); et.str("");
		cout << endl << "Error: Insufficient funds. You have $" << fixed << theClient.getSavings() << " in your savings account.\n";
		goto start;
	}
	else{ // Perform the transaction.
		theClient.setSavings(theClient.getSavings() - x);
		cout << "Successfully withdrew $" << fixed << x << " from savings account\n";
		cout << "Your current savings balance is $" << fixed << theClient.getSavings();
		et << theClient.getId() << " successfully withdrew money from his/her savings\n"; writeToET(et.str()); et.str("");
	}
}


void saveClients(vector<client>& clients){
	// Sort the clients vector by account number.
	sortByAccountNumber(clients);
	ofstream outputFile("clients.txt");
	for (int i = 0; i < clients.size(); i++){
		outputFile << clients[i].getId() << " " << fixed << clients[i].getChequing() << " " << fixed << clients[i].getSavings() << " " << clients[i].getFirstName();
		outputFile << " " << clients[i].getLastName() << " " << clients[i].getPin() << " ";
		if (clients[i].isFrozen() == true){
			outputFile << "frozen ";
		}else{
			outputFile << "ok ";
		}
		outputFile << fixed <<clients[i].getCredLimit();
		if (clients[i].getPayInFull() == true){
			outputFile << " full ";
		}
		else{
			outputFile << " part ";
		}
		outputFile << clients[i].getInterest()<< endl;
	}
	outputFile.close();
}
void savePurchases(vector<client>& clients){
	// Sort the clients vector by account number.
	sortByAccountNumber(clients);
	ofstream outputFile("client_purchases.txt");
	et << "Purchases are being saved.\n"; writeToET(et.str()); et.str("");
	for (int i = 0; i < clients.size(); i++){
		outputFile << clients[i].getId();
		for (int j = 0; j < clients[i].getSizeOfPurchases(); j++){
			outputFile <<" "<< clients[i].getPurchase(j);
		}
		outputFile  << "\n";
	}
	outputFile.close();
}
void saveVendor(client& theClient, double price){

	et << "Purchases are being saved to vendor.\n"; writeToET(et.str()); et.str("");
	ofstream vendorFile;

	vendorFile.open("vendor.txt", ios_base::app);
	vendorFile << theClient.getId() << " " << fixed << price << endl;
}
void saveExternals(Manager manager, Mperson maintain){
	
	ofstream outputFile("externals.txt");

	outputFile << "MaintenancePerson " << maintain.getID() << endl;
	outputFile << "Manager " << manager.getID() << endl;
	outputFile << "EndOfMonth " << endOfMonth;
	outputFile.close();
}
void viewPurchases(client& theClient){
	et << "Veiwing purchases.\n"; writeToET(et.str()); et.str("");

	double sum = 0;
	int number = 1;
	cout << "**--Purchases for this month:--**\n";
	for (int i = 0; i < theClient.getSizeOfPurchases(); i++){
		cout << number <<") $" << fixed << theClient.getPurchase(i) << endl;
		sum += theClient.getPurchase(i);
		number++;
	}

	cout << endl << "Total: $" << fixed << sum << endl;
	if ((0.75* theClient.getChequing()) < sum){
		cout << endl << "Warning: You have spent more than 75% of what is in your chequing account.\n";
		cout << "Please moderate your spending. \n";
		cout << "You have $" << fixed << theClient.getChequing() << " in your chequing account. \n";
		et << "Client spent more than 75% of checquing account.\n"; writeToET(et.str()); et.str("");
	}

}

void updateMonth(vector<client>& clients){
	et << "Recieving credit payment from clients.\n"; writeToET(et.str()); et.str("");
	for (int i = 0; i < clients.size(); i++){
		double cheq = clients[i].getChequing();		// Get ammount in chequing
		if (cheq > 0){ // Check if client has a credit card
			double owe = clients[i].payDay();	//Get amount owed
	
			bool full = clients[i].getPayInFull();	// Check if paying in full or in part
			if (full == true && owe > cheq){	// If you're trying to pay the full, but dont have enough
				if (owe/10 > cheq){ //if you cant pay the minimum ammount either
					clients[i].setFrozen(true);	// Freeze account
					clients[i].setInterest(clients[i].getInterest() + owe); // incure interest
					clients[i].setInterest(clients[i].getInterest() + (clients[i].getInterest()*0.02));
					writeFailedPayments(clients[i]);	// Write to file;
					et << "Client #" << clients[i].getId() << " failed to pay the minimum payment and their account has been frozen.\n"; writeToET(et.str()); et.str("");
				}
				else{	// if you can pay minimum ammount
					double owe2 = (owe *0.9);	// Get 90%
					owe = owe*0.1;	//Get 10% 
					clients[i].setInterest(clients[i].getInterest() + owe2); // incure interest
					clients[i].setInterest(clients[i].getInterest() +(clients[i].getInterest()*0.02));
					clients[i].setChequing(cheq - owe);		// Take payment
					et << "Client #" << clients[i].getId() << " has payed the minimum ammount.\n"; writeToET(et.str()); et.str("");
				}
			}
			else if (full ==false && owe > cheq){	// If you're trying to pay part, but don't have enough
				clients[i].setFrozen(true);	// Freeze account
				clients[i].setInterest(clients[i].getInterest() + owe + (owe*0.02)); // Incur interest
				writeFailedPayments(clients[i]);	// Write to file;
				et << "Client #" << clients[i].getId() << " failed to pay the minimum payment and their account has been frozen.\n"; writeToET(et.str()); et.str("");
			}
			else if (full == true && owe < cheq){	// If you have enough and paying the full ammount
				clients[i].setChequing(cheq - owe);		// Take payment
				clients[i].setInterest(0.0);			// Remove any ammount oweing
				et << "Client #" << clients[i].getId() << " has payed the full ammount of his/her debt.\n"; writeToET(et.str()); et.str("");
			}
			else if (full == false && owe < cheq){	// if you have enough to pay the minimum payment
				clients[i].setChequing(cheq - owe);		// Take payment
				et << "Client #" << clients[i].getId() << " has payed the minimum ammount.\n"; writeToET(et.str()); et.str("");
			}
			
		}
	}
	saveClients(clients);
}
void printClientMenu(client& theClient, bool cheq, bool sav){
	
	if (theClient.getChequing()>0){
		cout << endl << "****************************************************** \n";
		cout << endl << "You are $" << fixed << theClient.getAmmountOwing()+ theClient.getAmmountOwed() << " in debt\n";
		cout << "Your next credit card bill: $" << fixed << (theClient.getAmmountOwing() + theClient.getAmmountOwed()) / 10 << "\n";
		cout << "Your bill is set to pay in " << endOfMonth << " days\n";
	}
	cout << endl << "***********---What would you like to do?---*********** \n";
	if (!cheq){ 
	cout << "---1: Open Chequing account---\n"; }
	else		
	cout << "---1: Close chequing account--\n";

	if (!sav){
	cout << "---2: Open savings account----\n"; }
	else		
	cout << "---2: Close savings account---\n";

	cout << "---3: Deposit-----------------\n";
	cout << "---4: Withdraw----------------\n";
	cout << "---5: View Balance------------\n";
	cout << "---6: Transfer----------------\n";
	cout << "---7: Purchase----------------\n";
	cout << "---8: View Purchases----------\n";
	cout << "---9: Toggle Full Payment-----\n";
	cout << "---10: Logout-----------------\n" << endl;

}
void printManMenu(){
	cout << endl << "***********---What would you like to do?---***********\n"; writeToET(et.str()); et.str("");
	cout << "---1: Create an account------\n";
	cout << "---2: Delete an account------\n";
	cout << "---3: Display an account-----\n";
	cout << "---4: Display all Accounts---\n";
	cout << "---5: Display Failed payments\n";
	cout << "---6: Logout-----------------\n";
}
void managerMenu(vector<client>& clients){
	cout << endl << "Welcome Manager!\n";
start:
	bool logout = false;
	while (!logout){

		printManMenu();

		switch (getNumber()){

			// Create an account.   
		case 1:
		{
			et << "Manager selected 1: Create an account\n"; writeToET(et.str()); et.str("");
			// Automatically generate a new account number.
			int newAccNumber;
			if (clients.empty()){
				et << "First user account #100 created\n"; writeToET(et.str()); et.str("");
				newAccNumber = 100;
			}
			else { // Generate a new account number by taking the last number on the on the clients vector and adding 1 to it.
				et << "Generating a new client account number\n"; writeToET(et.str()); et.str("");
				newAccNumber = clients[clients.size() - 1].getId() + 1;
			}

			// Get the first name of the new client.
			bool valid1 = false;
			valid1 = false;
			string firstName;
			while (!valid1){
				cout << "Please enter the first name of the new client.\n";
				cout << "To cancel, enter 0.\n";
				cin >> firstName;
				if (firstName.compare("0") == 0){ // If the first name is "0" then...
					et << "Manager canceled\n"; writeToET(et.str()); et.str("");
					// Escape back to the manager menu.
					goto start;
				}
				else if (firstName.size() >= 1){
					et << "Manager entered a valid name\n"; writeToET(et.str()); et.str("");
					valid1 = true;
				}
				else {
					et << "Manager entered an invalid name \n"; writeToET(et.str()); et.str("");
					cout << "Please enter a valid name.\n";
				}
			}

			// Get the last name of the new client.
			valid1 = false;
			string lastName;
			while (!valid1){
				cout << "Please enter the last name of the new client.\n";
				cout << "To cancel, enter 0.\n";
				cin >> lastName;
				if (lastName.compare("0") == 0){ // If the last name is "0" then...
					et << "Manager canceled\n"; writeToET(et.str()); et.str("");
					// Escape back to the manager menu.
					goto start;
				}
				else if (lastName.size() >= 1){
					et << "Manager entered a valid name\n"; writeToET(et.str()); et.str("");
					valid1 = true;
				}
				else {
					et << "Manager entered an invalid name\n"; writeToET(et.str()); et.str("");
					cout << "Please enter a valid name.\n";
				}
			}

			// Attempt to open the account.
			bool success = openAccount(clients, newAccNumber, firstName, lastName);
			if (!success){
				et << "Manager successfully opened an account\n"; writeToET(et.str()); et.str("");
				cout << "Could not open account: account number already exists.\n";
			}
			else {
				et << "Manager could not open the account\n"; writeToET(et.str()); et.str("");
				cout << "Account #" << newAccNumber << " for user " << firstName << " " << lastName << " was successfully created.\n";
			}
			break;
		}

		// Delete an account.   
		case 2:
		{
			et << "Manager selected 2: Delete account\n"; writeToET(et.str()); et.str("");
			bool valid2 = false;
			// Get the account number.
			while (!valid2){
				cout << "Please enter an account number to delete.\n";
				cout << "To cancel, enter 0.\n";
				int accNumber = getNumber();
				if (accNumber == 0){
					et << "Manager canceled\n"; writeToET(et.str()); et.str("");
					goto start;   //Escape back to the manager menu.  
				}
				// Do not allow the account to be deleted if there is still money in it
				int x = getAccountIndex(clients, accNumber, 0, clients.size() - 1);
				if (x == -1){
					cout << "Account #" << accNumber << " could not be found.\n";
				}
				else if (clients[x].getSavings() != 0 || clients[x].getChequing() != 0){
					et << "Manager tried to delete an account with a balance on it\n"; writeToET(et.str()); et.str("");
					cout << "Account #" << accNumber << " could not be deleted since there exists a balance on their savings and/or chequing account.\n";
				}
				// Otherwise, attempt to delete the account.
				else{
					et << "Manager is asked for confirmation of the deletion\n"; writeToET(et.str()); et.str("");
					// Ask for confirmation.
					cout << "Are you sure you want to delete account #" << accNumber << " owned by " << clients[x].getLastName() << ", " << clients[x].getFirstName() << "?\n";
					cout << "1: Yes\n";
					cout << "2: No\n";

					switch (getNumber()){
					case 1:{ // Attempt to delete.
						et << "Manager selected 1: Proceed\n"; writeToET(et.str()); et.str("");
						bool success = deleteAccount(clients, accNumber);
						if (!success){
							cout << "Account #" << accNumber << " could not be deleted.\n";
						}
						else{
							cout << "Account #" << accNumber << " successfully removed.\n";
							valid2 = true;
							break;
						}
					}
					case 2:{ // Go back to the main menu.
						et << "Manager selected 2: Cancel\n"; writeToET(et.str()); et.str("");
						cout << "Deletion canceled.\n";
						goto start;
					}
					default:
					{
						et << "Manager entered invalid input\n"; writeToET(et.str()); et.str("");
						cout << "Error: Invalid input\n";
					}

					}

				}
			}
			break;
		}

		// Display an account.    
		case 3:
		{
			et << "Manager selected 3: Display an account\n"; writeToET(et.str()); et.str("");
			bool valid3 = false;
			while (!valid3){
				if (clients.size() == 0){
					cout << "No accounts to display.\n";
					goto start;
				}
				cout << "Please enter an account to view.\n";
				cout << "To cancel, enter 0.\n";
				int accNumber = getNumber();
				if (accNumber == 0) goto start;
				// Check if the account is in the database.
				if (existsInDatabase(clients, accNumber)){
					// Find the location at which the account is stored in the vector
					int i = getAccountIndex(clients, accNumber, 0, clients.size() - 1);
					// Display the data.
					cout << "********************************************" << endl;
					cout << "Account Number:      " << clients[i].getId() << endl;
					cout << "Name:                " << clients[i].getLastName() << ", " << clients[i].getFirstName() << endl;
					cout << "Savings Account:     $" << fixed << clients[i].getSavings() << endl;
					cout << "Chequing Account:    $" << fixed << clients[i].getChequing() << endl;
					cout << "Dept:                $" << fixed << clients[i].getAmmountOwing() + clients[i].getAmmountOwed() << endl;
					cout << "********************************************" << endl;
					valid3 = true;
					et << "Manager successfully displayed an account\n"; writeToET(et.str()); et.str("");
				}
				else {
					et << "Manager entered an invalid account number\n"; writeToET(et.str()); et.str("");
					cout << "Account " << accNumber << " could not be found.\n";
				}
			}
			break;
		}

		// Display all accounts.    
		case 4:
		{
			et << "Manager selected 4: Display all accounts\n"; writeToET(et.str()); et.str("");
			if (clients.size() == 0){
				cout << "No accounts to display.\n";
			}
			else{
				et << "Manager is asked how to display accounts\n"; writeToET(et.str()); et.str("");
				// Ask which order to display accounts in.
				cout << "***********---Sort by:---***********\n";
				cout << "---1: Account Number---\n";
				cout << "---2: Last Name--------\n";
				cout << "---0: Cancel-----------\n";
				switch (getNumber())
				{
				case 1:
				{
					et << "Manager selected 1: By account number\n"; writeToET(et.str()); et.str("");
					// Display all accounts by account number.
					for (int i = 0; i < clients.size(); i++){
						// Display the data.
						cout << "********************************************" << endl;
						cout << "Account Number:       " << clients[i].getId() << endl;
						cout << "Name:                 " << clients[i].getLastName() << ", " << clients[i].getFirstName() << endl;
						cout << "Savings Account:      $" << fixed << clients[i].getSavings() << endl;
						cout << "Chequing Account:     $" << fixed << clients[i].getChequing() << endl;
						cout << "Dept:                $" << fixed << clients[i].getAmmountOwing() + clients[i].getAmmountOwed() << endl;
					}
					cout << "********************************************" << endl;
					break;
				}
				case 2:
				{
					et << "Manager selected 2: By last name\n"; writeToET(et.str()); et.str("");
					// Display all accounts by last name.
					vector<client> alphaClients = sortByLastName(clients);
					for (int i = 0; i < alphaClients.size(); i++){
						// Display the data.
						cout << "********************************************" << endl;
						cout << "Account Number:       " << alphaClients[i].getId() << endl;
						cout << "Name:                 " << alphaClients[i].getLastName() << ", " << alphaClients[i].getFirstName() << endl;
						cout << "Savings Account:      $" << fixed << alphaClients[i].getSavings() << endl;
						cout << "Chequing Account:     $" << fixed << alphaClients[i].getChequing() << endl;
						cout << "Dept:                $" << fixed << clients[i].getAmmountOwing() + clients[i].getAmmountOwed()<< endl;
					}
					cout << "********************************************" << endl;
					break;
				}
				case 0:
				{
					et << "Manager selected 0: Cancel\n"; writeToET(et.str()); et.str("");
					goto start;
				}
				default:
				{
					et << "Manager entered invalid input\n"; writeToET(et.str()); et.str("");
					cout << "Error: Invalid input\n";
				}
				}

			}
			break;
		}
		// Display Failed payments.        
		case 5:
		{
			et << "Manager selected 5: Display failed payments\n"; writeToET(et.str()); et.str("");
			string line;
			int id;
			double value;
			ifstream failedFile("failed_payments.txt");
			// Read client purchases from text file into vector array
			cout << endl;
			if (failedFile.is_open()){
				while (getline(failedFile, line))
				{
					stringstream ss(line);
					ss >> id >> value;
					cout << "Client #" << id << " failed to make a payment of $" << fixed << value << endl;
				}
				failedFile.close();
			}
			else{
				cout << "Error: failed_purchases.txt file not found\n";
			}
			break;
		}
		// Log out.        
		case 6:
		{
			et << "Manager selected 6: Logout\n"; writeToET(et.str()); et.str("");
			return;
		}
		default:
			et << "Manager entered invalid input\n"; writeToET(et.str()); et.str("");
			cout << "Error: Invalid input\n";

		}
	}
}
bool openAccount(vector<client>& clients, int accID, string firstName, string lastName)
{
	et << "openAccount(vector<client>& clients, int accID = " << accID << "string firstName = "
		<< firstName << "string lastName = " << lastName << ")\n"; writeToET(et.str()); et.str("");
	// Check if the account ID already exists in the database.
	if (existsInDatabase(clients, accID)){
		et << "The account ID already exists in the database\n"; writeToET(et.str()); et.str("");
		return false;
	}
	else {
		// Create a new client object.
		client newClient = client();
		newClient.setFirstName(firstName);
		newClient.setLastName(lastName);
		newClient.setId(accID);
		// Add to the vector.
		clients.push_back(newClient);
		// Add to the database.
		saveClients(clients);
		et << "Manager successfully opened an account.\n"; writeToET(et.str()); et.str("");
		return true;
	}
}


bool deleteAccount(vector<client>& clients, int accID)
{
	// Check if the vector is empty.
	if (clients.empty()){
		return false;
	}
	// Check if the account ID exists in the database.
	else if (existsInDatabase(clients, accID)){
		// Perform a binary search method to get the index value of the account.
		int i = getAccountIndex(clients, accID, 0, clients.size() - 1);
		// Remove the account from the vector.
		clients.erase(clients.begin() + i);
		// Update the database.
		saveClients(clients);
		et << "Manager successfully deleted an account\n"; writeToET(et.str()); et.str("");
		return true;
	}
	else {
		et << "Manager failed to delete an account\n"; writeToET(et.str()); et.str("");
		return false;
	}

}

void sortByAccountNumber(vector<client>& clients)
{
	if (clients.empty()){
		return;
	}
	// Sort by account number.
	std::sort(clients.begin(), clients.end(), compareAccountNums);

}

vector<client> sortByLastName(vector<client> clients)
{
	if (clients.empty()){
		return clients;
	}
	// Sort the vector, but do not save the changes.
	sort(clients.begin(), clients.end(), compareAccountLastName);
	return clients;

}

bool existsInDatabase(vector<client>& clients, int targetAccID)
{
	if (clients.empty()){
		return false;
	}
	// First ensure that the vector is ordered (by account number).
	sortByAccountNumber(clients);
	// Search using the binary search method.
	int result = getAccountIndex(clients, targetAccID, 0, clients.size() - 1);
	if (result == -1){
		return false;
	}
	else {
		return true;
	}

}



bool compareAccountLastName(client a, client b){
	return (a.getLastName() < b.getLastName());
}

bool compareAccountNums(client a, client b){ 
	return (a.getId() < b.getId()); 
}
int getAccountIndex(vector<client>& clients, int targetAccNum, int low, int high){
	et << "int getAccountIndex(vector<client>& clients, int targetAccNum = " << targetAccNum << " int low = "
		<< low << "int high = " << high << "\n"; writeToET(et.str()); et.str("");
	// Perform a binary search for the account.
	// This assumes we have an ordered list.
	if (high < low) return -1; // Not found.
	int mid = low + ((high - low) / 2);
	if (clients[mid].getId() > targetAccNum){
		return getAccountIndex(clients, targetAccNum, low, mid - 1);
	}
	else if (clients[mid].getId() < targetAccNum){
		return getAccountIndex(clients, targetAccNum, mid + 1, high);
	}
	else{
		et << "int getAccountIndex() found " << mid << "\n"; writeToET(et.str()); et.str("");
		return mid; // Found.
	}
}

void maintenanceMenu(void){
	et << "maintenanceMenu()\n"; writeToET(et.str()); et.str("");
	cout << endl << "Welcome System Maintenance Person!\n";
start:
	bool logout = false;
	while (!logout){
		cout << endl << "***********---What would you like to do?---*********** \n";
		cout << "---1: Toggle execution tracing-----\n";
		cout << "---2: Print execution traces-------\n";
		cout << "---3: Clear execution traces log---\n";
		cout << "---4: Logout-----------------------\n";
		switch (getNumber()){

			// Toggle execution tracing.
		case 1:{
			et << "Maintenance selected 1: Toggle execution tracing\n"; writeToET(et.str()); et.str("");
			toggleExecutionTraces();
			break;
		}

			   // Print execution traces.
		case 2:{
			et << "Maintenance selected 2: print Execution traces\n"; writeToET(et.str()); et.str("");
			printExecutionTraces();
			break;
		}

		case 3:{
			et << "Maintenance selected 3: Clear execution traces log\n"; writeToET(et.str()); et.str("");
			// Ask for confirmation.
			cout << "Are you sure you want to clear the execution traces log?\n";
			cout << "1: Yes\n";
			cout << "2: No\n";
			et << "Maintenance is asked for confirmation on the log clear\n"; writeToET(et.str()); et.str("");
			switch (getNumber()){
			case 1:{
				et << "Maintenance selected 1: Proceed\n"; writeToET(et.str()); et.str("");
				clearExecutionTracesLog();
				goto start;
			}

			case 2:{
				et << "Maintenance selected 2: Cancel\n"; writeToET(et.str()); et.str("");
				goto start;
			}
			default:{
				et << "Maintenance entered invalid input\n"; writeToET(et.str()); et.str("");
				cout << "Invalid input\n";
			}
			}
			break;
		}

			   // Logout.
		case 4:{
			et << "Maintenance selected 4: Logout\n"; writeToET(et.str()); et.str("");
			return; // Exit the maintenance menu.
		}

		default:{
			et << "Maintenance entered invalid input\n"; writeToET(et.str()); et.str("");
			cout << "Invalid input\n";
		}
		}
	}
}
//  Call this function to prompt the user for an int value. Returns that value
int getNumber(){
	et << "User is entering a number\n"; writeToET(et.str()); et.str("");
	int ammount;
	cin >> ammount;
	//If the value endered was not of type int
	while (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << endl << "Invalid input.  Enter a NUMBER: \n"; writeToET(et.str()); et.str("");
		et << "User entered an invalid number\n"; writeToET(et.str()); et.str("");
		cin >> ammount;
	}
	et << "User correctly entered the number" << ammount << "\n"; writeToET(et.str()); et.str("");
	return ammount;
}
// Call this function to prompt the user for a double value. Returns that value
double getDouble(){
	et << "User is entering a double\n"; writeToET(et.str()); et.str("");
	double ammount;
	cin >> ammount;
	//If the value entered was not of type double
	while (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << endl << "Invalid input.  Enter a NUMBER: \n";
		et << "User entered an invalid double \n"; writeToET(et.str()); et.str("");
		cin >> ammount;
	}
	et << "User correctly entered a double\n"; writeToET(et.str()); et.str("");
	return ammount;
}

bool penalty(client& theClient, double amount){
	et << "penalty(client& theClient #" << theClient.getId() << " double amount = " << amount << ")\n"; writeToET(et.str()); et.str("");
	// Check if the transaction will bring the client's chequing account down below $1000.
	if (theClient.getChequing() - amount < 1000.00){
		et << "There is a penalty"; writeToET(et.str()); et.str("");
		return true;
	}
	else{
		et << "There is no penalty\n"; writeToET(et.str()); et.str("");
		return false;
	}
}


void toggleExecutionTraces(){
	if (executionTraces == ON){
		cout << "Execution tracing is ON. Do you want to switch if OFF?\n";
		cout << "1: Yes\n";
		cout << "2: No\n";
		switch (getNumber()){

		case 1:{    //Yes, switch OFF
			string line;
			executionTraces = OFF;
			// Change the first line of the file.
			ifstream etFile;
			etFile.open("execution_traces.txt");
			if (etFile.is_open()){
				// We need to make a temporary file to save our exec traces.
				ofstream tempFile;
				tempFile.open("temp.txt");
				if (tempFile.is_open()){
					// Start copying the contents of execution_traces.txt into temp.txt
					// Skip the first line.
					getline(etFile, line);
					while (getline(etFile, line)){
						tempFile << line << endl;
					}
					tempFile.close();
					etFile.close();
					// Overwrite execution_traces.txt.
					ofstream etFileWrite("execution_traces.txt");
					etFileWrite << "0\n"; // The file now only contains "0".
					// Add the rest of the file from the temp.
					ifstream tempFileRead("temp.txt");
					while (getline(tempFileRead, line)){
						etFileWrite << line << endl;
					}
					etFileWrite.close();
					tempFileRead.close();
					// Remove temp.txt since we no longer need it.
					if (remove("temp.txt") != 0) cout << "Error deleting file temp.txt\n";
				}
				else{
					cout << "Could not create temp.txt\n";
					return;
				}

			}
			else{
				cout << "Could not open file execution_traces.txt\n";
				return;
			}
			cout << "Execution tracing is OFF.\n";
			break;
		}

		case 2:{    //No, keep ON
			cout << "Execution tracing is ON.\n";
			break;
		}
		default:{
			cout << "Invalid input\n";
		}
		}
	}
	else{
		cout << "Execution tracing is OFF. Do you want to switch it ON?\n";
		cout << "1: Yes\n";
		cout << "2: No\n";
		switch (getNumber()){

		case 1:{    //Yes, switch ON
			string line;
			executionTraces = ON;
			// Change the first line of the file.
			ifstream etFile;
			etFile.open("execution_traces.txt");
			if (etFile.is_open()){
				// We need to make a temporary file to save our exec traces.
				ofstream tempFile;
				tempFile.open("temp.txt");
				if (tempFile.is_open()){
					// Start copying the contents of execution_traces.txt into temp.txt
					// Skip the first line.
					getline(etFile, line);
					while (getline(etFile, line)){
						tempFile << line << endl;
					}
					tempFile.close();
					etFile.close();
					// Overwrite execution_traces.txt.
					ofstream etFileWrite("execution_traces.txt");
					etFileWrite << "1\n"; // The file now only contains "1".
					// Add the rest of the file from the temp.
					ifstream tempFileRead("temp.txt");
					while (getline(tempFileRead, line)){
						etFileWrite << line << endl;
					}
					etFileWrite.close();
					tempFileRead.close();
					// Remove temp.txt since we no longer need it.
					if (remove("temp.txt") != 0) cout << "Error deleting file temp.txt\n";
				}
				else{
					cout << "Could not create temp.txt\n";
					return;
				}

			}
			else{
				cout << "Could not open file execution_traces.txt\n";
				return;
			}
			cout << "Execution tracing is ON.\n";
			break;
		}

		case 2:{    //No, keep OFF
			cout << "Execution tracing is OFF.\n";
			break;
		}
		default:{
			cout << "Invalid input\n";
		}
		}
	}
}

void writeToET(string str){
	// Only write if execution tracing is on.
	if (executionTraces == ON){
		ofstream etFile;
		// Open the file in append mode.
		etFile.open("execution_traces.txt", ios::app);
		if (etFile.is_open()){
			// Write.
			etFile << str << endl;
			// Close.
			etFile.close();
		}
		else{
			cout << "Could not open file execution_traces.txt\n";
		}
	}
	else {
		return;
	}
}

void printExecutionTraces(){
	et << "Maintenance is printing the execution traces\n"; writeToET(et.str()); et.str("");
	// Open execution_traces.txt and print the file (excluding the first line).
	string line;
	ifstream etFile;
	etFile.open("execution_traces.txt");
	if (etFile.is_open()){
		// Skip the first line.
		getline(etFile, line);
		while (getline(etFile, line))
		{
			cout << line << endl;
		}
		// Close the file.
		etFile.close();
	}
	else{
		cout << "Could not open file execution_traces.txt\n";
	}
}

void clearExecutionTracesLog(){
	et << "Maintenance clearing the execution log\n"; writeToET(et.str()); et.str("");
	// Clear everything in execution_traces.txt EXCEPT for the first line which governs the state of exec tracing.
	// Do this by overwriting the file with the current state of executionTraces.
	ofstream etFile;
	etFile.open("execution_traces.txt");
	if (etFile.is_open()){
		if (executionTraces == ON){
			etFile << "1\n";
		}
		else{
			etFile << "0\n";
		}
		etFile.close();
	}
	else{
		cout << "Could not open file execution_traces.txt\n";
	}
}
