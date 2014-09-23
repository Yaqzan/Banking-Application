
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <stdio.h>
#define ON true
#define OFF false
using namespace std;

class client
{
public:
	client(); //Constructor
	~client(){} //Destructor
	//Setters
	void setChequing(double chequing){ chequingAcc = chequing; }
	void setSavings(double saving){ savingsAcc = saving; }
	void setId(int cid){ id = cid; }
	void setFirstName(string name){ name1 = name; }
	void setLastName(string name){ name2 = name; }
	//Getters
	double getSavings(){ return savingsAcc; }		// return amount under savings account
	double getChequing(){ return chequingAcc; }		// Return amount under chequing account
	int getId(){ return id; }						// Return user Id;
	string getFirstName(){ return name1; }			// Return first name;
	string getLastName(){ return name2; }			// Return last name;
private:
	int id;
	double chequingAcc, savingsAcc;
	string name1, name2;
};
client::client()
{
	id = 0;
	chequingAcc = 0.0;
	savingsAcc = 0.0;
	name1 = " ";
	name2 = " ";
}

//Manager Menus and Functions
void managerMenu(vector<client>& clients);					 // Brings user to the manager screen
bool openAccount(vector<client>& clients, int accID, string firstName, string lastName);
bool deleteAccount(vector<client>& clients, int accID);
void printManMenu(void);

//Maintenance Menus and Functions
void maintenanceMenu(void);		 // Brings user to the maintenance screen
void toggleExecutionTraces(void);   // Sets execution traces from on --> off or from off --> on.
void setExecutionTraces(bool x);   // Turns execution traces on or off depending on x. (true = on, false = off)
void writeToET(string str);  // Writes str to the file execution_traces.txt 
void printExecutionTraces(void);
void clearExecutionTracesLog(void);

//Client Menus and Functions
void clientMenu(client& theClient, vector<client>& clients); //Brings user to the screen for clients
void printClientMenu(client& theClient, bool cheq, bool sav);// Prints the options available to the client
void deposit(client& theClient);
void withdraw(client& theClient);
void withdrawChequing(client& theClient);
void withdrawSavings(client& theClient);
void transfer(client& theClient);
void transferChequing(client& theClient);
void transferSavings(client& theClient);
void chequing(client& theClient, bool cheq, bool sav);
void savings(client& theClient, bool sav, bool cheq);

//Helper functions
int getNumber();
double getDouble();
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

int main()
{
        et << "main():\n"; writeToET(et.str()); et.str("");
        
        
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
	const int managerId = 0;
	const int maintenId = 1;
	vector<client> clients;		// THe client vector array
	int id, i = 0, getId;;		
	double chequings, savings;	
	string line, name1, name2;
	client *f1;	
	ifstream myfile("clients.txt");
	

	// Read client info from text file into vector array
	if (myfile.is_open()){
		while (getline(myfile, line))
		{
			stringstream ss(line);
			ss >> id >> chequings >> savings >> name1 >> name2;
			f1 = new client;
			f1->setId(id);
			f1->setChequing(chequings);
			f1->setSavings(savings);
			f1->setFirstName(name1);
			f1->setLastName(name2);
			clients.push_back(*f1);
			i++;
		}
		myfile.close();
	}
	else{
		cout << "Error: Client file not found\n";
	}

	cout << endl<< "***********---Welcome to Group 28's Banking System!---***********\n";
	
	while (true){
		bool found = false;
		while (!found){
			cout << endl<< "Please enter your login ID: ";
			cin >> getId;
			if (getId == managerId){
				managerMenu(clients);
				found = true;
			}
			else if (getId == maintenId){
				maintenanceMenu();
				found = true;
			}
			else if (existsInDatabase(clients, getId)){
                                int x = getAccountIndex(clients, getId, 0, clients.size() - 1);
                                clientMenu(clients[x], clients);
				found = true;
			}
			if (!found)
				cout<<endl << "Sorry, that ID is not in our servers\n";
			
		}
	}
        
        et << "End of main()\n"; writeToET(et.str()); et.str("");
	return 0;
}

void clientMenu(client& theClient, vector<client>& clients){
        et << "clientMenu(client& theClient #" << theClient.getId() << ", vector<client>& clients)\n"; writeToET(et.str()); et.str("");
	cout << endl << "Welcome " << theClient.getFirstName() << "!\n";
	bool cheq, sav, logout=false;
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
				cout << endl<< "Chequings Account: $" << theClient.getChequing() << "\n";
			}if (sav){
				cout << endl << "Savings Account:   $" << theClient.getSavings() << "\n";
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
                        et << theClient.getId() << " selected 7: Logout\n"; writeToET(et.str()); et.str("");
			logout = true;
			break;
		default:
                        et << theClient.getId()  << " entered invalid input\n"; writeToET(et.str()); et.str("");
			cout << "Invalid input \n";
		}
	}
}

void chequing(client& theClient, bool cheq, bool sav){
        et << "chequing(client& theClient #" << theClient.getId() <<  ", bool cheq = " << cheq << ", bool sav = " << sav << ")\n"; writeToET(et.str()); et.str("");
        bool done = false;
	double chequing = theClient.getChequing();
	while (!done){
		done = true;
		if (cheq == false){
			cout << endl << "You have opened your chequing account!";
			cout << " How much would you like to deposit?\n";
			double x = getDouble();
			theClient.setChequing(x);
                        et << theClient.getId() << " opened a chequing account with $" << x << "\n"; writeToET(et.str()); et.str("");
			cout << endl << "Successfully deposited $" << x << " into chequing account\n";
		}
		else{
                        et << theClient.getId() << " is closing his/her chequing account.\n"; writeToET(et.str()); et.str("");
			cout << endl << "You have closed your chequing account.\n";
			if (sav){
				cout << " Would you like to transfer the money into your savings account?\n";
				cout << "1: Yes\n";
				cout << "2: No\n";
		
				switch (getNumber()){
				case 1:
                                        et << theClient.getId() << " selected 1: Yes, transfer to savings\n"; writeToET(et.str()); et.str("");
					theClient.setSavings(theClient.getSavings() + chequing);
					cout << endl << "Successfully transfered $" << chequing << " into savings account\n";
					break;
				case 2:
                                        et << theClient.getId() << " selected 2: No, do not transfer\n"; writeToET(et.str()); et.str("");
					break;
				default:
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
        et << "savings(client& theClient #" << theClient.getId() << " bool sav = " << sav << " bool cheq = " << cheq << ")\n"; writeToET(et.str()); et.str("");
	bool done = false;
	double saving = theClient.getSavings();
	while (!done){
		done = true;
		if (sav == false){
                        et << theClient.getId() << " is opening a savings account.\n"; writeToET(et.str()); et.str("");
			cout << endl << "You have opened your savings account!";
			cout << " How much would you like to deposit?\n";
			double x = getDouble();
			theClient.setSavings(x);
			cout << endl << "Successfully deposited $" <<x << " into Savings account\n";
                        et << theClient.getId() << " opened a savings account with $" << x << "\n"; writeToET(et.str()); et.str("");
		}
		else{
                        et << theClient.getId() << " is closing his/her savings account.\n"; writeToET(et.str()); et.str("");
			cout << endl << "You have closed your savings account.";
			if (cheq){
				cout << " Would you like to transfer the money into your chequing account?\n";
				cout << "1: Yes\n";
				cout << "2: No\n";

				switch (getNumber()){
				case 1:
                                        et << theClient.getId() << " selected 1: Transfer to chequing\n"; writeToET(et.str()); et.str("");
					theClient.setChequing(theClient.getChequing() + saving);
					cout << endl << "Successfully transfered $" << saving << " into chequing account\n";
					break;
				case 2:
                                        et << theClient.getId() << " selected 2: Do not transfer to chequing\n"; writeToET(et.str()); et.str("");
					break;
				default:
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
        et << "transfer(client& theClient #" << theClient.getId() << ")\n"; writeToET(et.str()); et.str("");
	bool done = false;
	while (!done){
		cout <<endl<< "***********---Which account would you like to transfer from?---***********\n";
		cout << "***********---1: Chequing Account---***********\n";
		cout << "***********---2: Savings Account---***********\n";
		cout << "***********---3: Cancel---***********\n";
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
        et << "deposit(client& theClient #" << theClient.getId() << ")\n"; writeToET(et.str()); et.str("");
	bool done = false;
	double x;
	int y;
	while (!done){
		done = true;
		cout <<endl << "***********---Which account would you like to deposit in?---***********\n";
		cout << "***********---1: Chequing Account---***********\n";
		cout << "***********---2: Savings Account---***********\n";
		cout << "***********---3: Cancel---***********\n";
		cin >> y;
		switch (y){
		case 1:
                        et << theClient.getId() << " selected 1: Deposit in Chequing\n"; writeToET(et.str()); et.str("");
			cout << endl<< "How much would you like to deposit?\n";
			x = getDouble();
			theClient.setChequing(theClient.getChequing() + x);
			cout <<endl<< "Deposited $" << x << " into Chequing Account\n";
                        et << theClient.getId() << " deposited $" << x << " in to his/her chequing\n"; writeToET(et.str()); et.str("");
			break;
		case 2:
                        et << theClient.getId() << " selected 2: Deposit in Savings\n"; writeToET(et.str()); et.str("");
			cout <<endl<< "How much would you like to deposit?\n";
			x = getDouble();
			theClient.setSavings(theClient.getSavings() + x);
			cout <<endl<< "Deposited $" << x << " into Savings Account\n";
                        et << theClient.getId() << " deposited $" << x << " in to his/her savings\n"; writeToET(et.str()); et.str("");
			break;
		case 3:
                        et << theClient.getId() << " canceled.\n"; writeToET(et.str()); et.str("");
			return; 
		default:
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
            cout <<endl<< "***********---Which account would you like to withdraw from?---***********\n";
            cout << "***********---1: Chequing Account---***********\n";
            cout << "***********---2: Savings Account---***********\n";
            cout << "***********---3: Cancel---***********\n";
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
    et << "transferChequing(client& theClient #" << theClient.getId() << ")\n"; writeToET(et.str()); et.str("");
    start:
    cout << endl << "How much would you like to transfer?\n";
    cout << "Enter 0 to cancel.\n";
    double x = getDouble();
    if (x == 0) {
        et << theClient.getId() << " entered 0 to cancel\n"; writeToET(et.str()); et.str("");
        return;
    } 
    else if (x > theClient.getChequing()) {
        et << theClient.getId() << " tried to transfer too much money\n"; writeToET(et.str()); et.str("");
        cout << endl << "Error: Insufficient funds. You have $" << theClient.getChequing() << " in your chequing account.\n";
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
                        cout << endl << "Error: Insufficient funds. You have $" << theClient.getChequing() << " in your chequing account.\n";
                        goto start;
                    } 
                    else {
                        // Perform the transaction.
                        theClient.setChequing(theClient.getChequing() - x - 2.00);
                        theClient.setSavings(theClient.getSavings() + x);
                        cout << "Successfully transferred $" << x << " from your chequing account to your savings account.\n";
                        cout << "Your chequing balance is $" << theClient.getChequing() << endl;
                        cout << "Your savings balance is $" << theClient.getSavings() << endl;
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
            cout << endl << "Successfully transferred $" << x << " from chequing account\n";
            cout << "Your current chequing balance is $" << theClient.getChequing() << endl;
            cout << "Your current savings balance is $" << theClient.getSavings() << endl;
            et << theClient.getId() << " transferred $" << x << " without a penalty from chequing to savings\n"; writeToET(et.str()); et.str("");
        }
        
    }
}

void transferSavings(client& theClient) {
    et << "transferSavings(client& theClient #" << theClient.getId() << ")\n"; writeToET(et.str()); et.str("");
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
        cout << endl << "Error: Insufficient funds. You have $" << theClient.getSavings() << " in your Savings account\n";
        return;
    }
    theClient.setChequing(theClient.getChequing() + ammount);
    theClient.setSavings(theClient.getSavings() - ammount);
    cout << "Successfully transfered $" << ammount << " from Savings account to Chequing Account\n";
    cout << "Your current chequing balance is $" << theClient.getChequing() << endl;
    cout << "Your current savings balance is $" << theClient.getSavings() << endl;
    et << theClient.getId() << " successfully transferred $" << ammount << " from savings to chequing\n"; writeToET(et.str()); et.str("");
}

void withdrawChequing(client& theClient) {
    et << "withdrawChequing(client& theClient #" << theClient.getId() << ")\n"; writeToET(et.str()); et.str("");
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
                        cout << endl << "Error: Insufficient funds. You have $" << theClient.getChequing() << " in your chequing account.\n";
                        goto start;
                    } 
                    else {
                        // Perform the transaction.
                        theClient.setChequing(theClient.getChequing() - x - 2.00);
                        cout << "Successfully withdrew $" << x << " from chequing account.\n";
                        cout << "Your current chequing balance is $" << theClient.getChequing();
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
            cout << endl << "Successfully withdrew $" << x << " from chequing account\n";
            cout << "Your current chequing balance is $" << theClient.getChequing();
            et << theClient.getId() << " successfully withdrew money without a penalty\n"; writeToET(et.str()); et.str("");
        }
        
    }
}

void withdrawSavings(client& theClient){
    et << "withdrawSavings(client& theClient #" << theClient.getId() << ")\n"; writeToET(et.str()); et.str("");
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
        cout << endl << "Error: Insufficient funds. You have $" << theClient.getSavings() << " in your savings account.\n";
        goto start;
    } 
    else{ // Perform the transaction.
        theClient.setSavings(theClient.getSavings() - x);
        cout << "Successfully withdrew $" << x << " from savings account\n";
        cout << "Your current savings balance is $" << theClient.getSavings();
        et << theClient.getId() << " successfully withdrew money from his/her savings\n"; writeToET(et.str()); et.str("");
    }
}


void saveClients(vector<client>& clients){
        et << "saveClients(vector<client>& clients)\n"; writeToET(et.str()); et.str("");
        // Sort the clients vector by account number.
        sortByAccountNumber(clients);
        ofstream outputFile("clients.txt");
        et << "Clients are being saved.\n"; writeToET(et.str()); et.str("");
	for (int i = 0; i < clients.size(); i++){
		outputFile << clients[i].getId() << " " << clients[i].getChequing() << " " << clients[i].getSavings() << " " << clients[i].getFirstName() << " " << clients[i].getLastName()<<"\n";
	}
	outputFile.close(); 
}
void printClientMenu(client& theClient, bool cheq, bool sav){
        et << "printClientMenu(client& theClient #" << theClient.getId() << "bool cheq = " << cheq << "bool sav = " << sav << ")\n"; writeToET(et.str()); et.str("");
        cout<< endl		 << "***********---What would you like to do?---*********** \n";

	if (!cheq){ cout << "***********---1: Open Chequing account---***********\n"; }
	else		cout << "***********---1: Close chequing account---***********\n";

	if (!sav){	cout << "***********---2: Open savings account---***********\n"; }
	else		cout << "***********---2: Close savings account---***********\n";

				cout << "***********---3: Make a deposit---***********\n";
				cout << "***********---4: Withdraw money---***********\n";
				cout << "***********---5: View Balance---***********\n";
				cout << "***********---6: Transfer---***********\n";
				cout << "***********---7: Logout---***********\n" << endl;

}
void printManMenu(){
        et << "printManMenu()\n";
	cout << endl<< "***********---What would you like to do?---***********\n"; writeToET(et.str()); et.str("");
	cout << "***********---1: Create an account---***********\n";
	cout << "***********---2: Delete an account---***********\n";
	cout << "***********---3: Display an account---***********\n";
	cout << "***********---4: Display all Accounts---***********\n";
	cout << "***********---5: Logout---***********\n";
}
void managerMenu(vector<client>& clients){
        et << "managerMenu(vector<client>& clients)\n"; writeToET(et.str()); et.str("");
        cout <<endl << "Welcome Manager!\n";
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
                        newAccNumber = clients[clients.size()-1].getId() + 1;
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
                            cout << "Savings Account:     $" << clients[i].getSavings() << endl;
                            cout << "Chequing Account:    $" << clients[i].getChequing() << endl;
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
                        cout << "***********---1: Account Number---***********\n";
                        cout << "***********---2: Last Name---***********\n";
                        cout << "***********---0: Cancel---***********\n";
                        switch(getNumber())
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
                                    cout << "Savings Account:      $" << clients[i].getSavings() << endl;
                                    cout << "Chequing Account:     $" << clients[i].getChequing() << endl;
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
                                    cout << "Savings Account:      $" << alphaClients[i].getSavings() << endl;
                                    cout << "Chequing Account:     $" << alphaClients[i].getChequing() << endl;
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
                    
                // Log out.        
		case 5:
                {
                    et << "Manager selected 5: Logout\n"; writeToET(et.str()); et.str("");
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
    et << "deleteAccount(vector<client>& clients, int accID = " << accID << ")\n"; writeToET(et.str()); et.str("");
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
    et << "Sorting the clients vector by account number\n"; writeToET(et.str()); et.str("");
    if (clients.empty()){
        return;
    }
    // Sort by account number.
    std::sort(clients.begin(), clients.end(), compareAccountNums);
    
}

vector<client> sortByLastName(vector<client> clients)
{
    et << "vector<client> sortByLastName(vector<client> clients)\n"; writeToET(et.str()); et.str("");
    et << "Sorting the clients by last name\n"; writeToET(et.str()); et.str("");
    if (clients.empty()){
        return clients;
    }
    // Sort the vector, but do not save the changes.
    sort(clients.begin(), clients.end(), compareAccountLastName);
    return clients;
    
}

bool existsInDatabase(vector<client>& clients, int targetAccID)
{
    et << "bool existsInDatabase(vector<client>& clients, int targetAccID = " << targetAccID << ")\n"; writeToET(et.str()); et.str("");
    if (clients.empty()){
        et << "bool existsInDatabase(...) = false\n"; writeToET(et.str()); et.str("");
        return false;
    }
    // First ensure that the vector is ordered (by account number).
    sortByAccountNumber(clients);
    // Search using the binary search method.
    int result = getAccountIndex(clients, targetAccID, 0, clients.size() - 1);
    if (result == -1){
        et << "bool existsInDatabase(...) = false\n"; writeToET(et.str()); et.str("");
        return false;
    }
    else {
        et << "bool existsInDatabase(...) = true\n"; writeToET(et.str()); et.str("");
        return true;
    }
    
}

bool compareAccountNums(client a, client b){
    return (a.getId() < b.getId());
}

bool compareAccountLastName(client a, client b){
    return (a.getLastName() < b.getLastName());
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
	cout <<endl << "Welcome System Maintenance Person!\n";
        start:
        bool logout = false;
        while (!logout){
            cout << endl << "***********---What would you like to do?---*********** \n";
            cout << "***********---1: Toggle execution tracing---***********\n";
            cout << "***********---2: Print execution traces---***********\n";
            cout << "***********---3: Clear execution traces log---***********\n";
            cout << "***********---4: Logout---***********\n";
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
int getNumber(){
        et << "User is entering a number\n"; writeToET(et.str()); et.str("");
	int ammount;
	cin >> ammount;
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
double getDouble(){
        et << "User is entering a double\n"; writeToET(et.str()); et.str("");
	double ammount;
	cin >> ammount;
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
        et << "There is a penalty\n"; writeToET(et.str()); et.str("");
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
                        if(remove("temp.txt") != 0) cout << "Error deleting file temp.txt\n";
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
                        if(remove("temp.txt") != 0) cout << "Error deleting file temp.txt\n";
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
            cout <<"Could not open file execution_traces.txt\n";
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