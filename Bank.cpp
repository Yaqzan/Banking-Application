
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
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

int main()
{
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
	return 0;
}

void clientMenu(client& theClient, vector<client>& clients){
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
			chequing(theClient, cheq, sav);
			saveClients(clients);
			break;
		case 2:
			savings(theClient, sav, cheq);
			saveClients(clients);
			break;
		case 3:
			deposit(theClient);
			saveClients(clients);
			break;
		case 4:
			withdraw(theClient);
			saveClients(clients);
			break;
		case 5:
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
			transfer(theClient);
			saveClients(clients);
			break;
		case 7:
			logout = true;
			break;
		default:
			cout << "Invalid input \n";
		}
	}
}

void chequing(client& theClient, bool cheq, bool sav){
	bool done = false;
	double chequing = theClient.getChequing();
	while (!done){
		done = true;
		if (cheq == false){

			cout << endl << "You have opened your chequings account!";
			cout << " How much would you like to deposit?\n";
			double x = getDouble();
			theClient.setChequing(x);
			cout << endl << "Successfully deposited $" << x << " into Chequing account\n";
		}
		else{
			cout << endl << "You have closed your chequing account.";
			if (sav){
				cout << " Would you like to transfer the money into your savings account?\n";
				cout << "1: Yes\n";
				cout << "2: No\n";
		
				switch (getNumber()){
				case 1:
					theClient.setSavings(theClient.getSavings() + chequing);
					cout << endl << "Successfully transfered $" << chequing << " into savings account\n";
					break;
				case 2:
					break;
				default:
					cout << endl << "Invalid input.\n";
					done = false;
				}
			}
			cout << "\n";
			theClient.setChequing(0);
			}
		}
	}

void savings(client& theClient, bool sav, bool cheq){
	bool done = false;
	double saving = theClient.getSavings();
	while (!done){
		done = true;
		if (sav == false){

			cout << endl << "You have opened your savings account!";
			cout << " How much would you like to deposit?\n";
			double x = getDouble();
			theClient.setSavings(x);
			cout << endl << "Successfully deposited $" <<x << " into Savings account\n";
		}
		else{
			cout << endl << "You have closed your savings account.";
			if (cheq){
				cout << " Would you like to transfer the money into your chequing account?\n";
				cout << "1: Yes\n";
				cout << "2: No\n";

				switch (getNumber()){
				case 1:
					theClient.setChequing(theClient.getChequing() + saving);
					cout << endl << "Successfully transfered $" << saving << " into chequing account\n";
					break;
				case 2:
					break;
				default:
					cout << endl << "Invalid input.\n";
					done = false;
				}
			}
			cout << "\n";
			theClient.setSavings(0);
		}
	}
}
void transfer(client& theClient){
	bool done = false;
	while (!done){
		cout <<endl<< "***********---Which account would you like to transfer from?---***********\n";
		cout << "***********---1: Chequing Account---***********\n";
		cout << "***********---2: Savings Account---***********\n";
		cout << "***********---3: Cancel---***********\n";
		switch (getNumber()){
		case 1:
                        transferChequing(theClient);
			return;
		case 2:
                        transferSavings(theClient);
			return;
		case 3:
			return;
		default:
			cout << endl << "Error: Invalid input\n";
		}
	}


}
void deposit(client& theClient){
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
			cout << endl<< "How much would you like to deposit?\n";
			x = getDouble();
			theClient.setChequing(theClient.getChequing() + x);
			cout <<endl<< "Deposited $" << x << " into Chequing Account\n";
			break;
		case 2:
			cout <<endl<< "How much would you like to deposit?\n";
			x = getDouble();
			theClient.setSavings(theClient.getSavings() + x);
			cout <<endl<< "Deposited $" << x << " into Savings Account\n";
			break;
		case 3:
			return; 
		default:
			cout << "Invalid input\n";
			done = false;
		}

			
	}
}
void withdraw(client& theClient){
	bool done = false;
	while (!done){
            cout <<endl<< "***********---Which account would you like to withdraw from?---***********\n";
            cout << "***********---1: Chequing Account---***********\n";
            cout << "***********---2: Savings Account---***********\n";
            cout << "***********---3: Cancel---***********\n";
            switch (getNumber()){
                //Withdraw from chequing account.
                case 1:{
                    withdrawChequing(theClient);
                    return;
                }
                // Withdraw from savings account.
                case 2:{
                    withdrawSavings(theClient);
                    return;
                }
                // Cancel.
                case 3:{
                    return;
                }
                default:{
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
    if (x == 0) {
        return;
    } 
    else if (x > theClient.getChequing()) {
        cout << endl << "Error: Insufficient funds. You have $" << theClient.getChequing() << " in your chequing account.\n";
        goto start;
    } 
    else {
        // Check if this puts the user under $1000 in the chequing account and apply a penalty if this is true.
        if (penalty(theClient, x)) { // There is a penalty.
            warning:
            // Warn the user if they want to proceed with the withdrawal.
            cout << "Because this transaction will bring your chequing account's balance below $1000.00, a\n";
            cout << "$2.00 fee will applied. Do you wish to continue?\n";
            cout << "1: Yes\n";
            cout << "2: No\n";
            switch (getNumber()) {
                case 1:
                { // Go through with the transaction.
                    // Check if the $2.00 penalty will put the balance below $0.
                    if (theClient.getChequing() - x - 2.00 < 0) {
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
                        return;
                    }
                    break;
                }
                case 2:
                { // Cancel the transaction and exit the withdraw menu.
                    cout << "Transfer canceled. You have not been charged.\n";
                    return;
                }
                default:
                {
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
        }
        
    }
    
    /**
    cout << endl << "How much would you like to withdraw?\n";
    ammount = getDouble();
    if (ammount > theClient.getChequing()) {
        cout << endl << "Error: Insufficient funds. You have $" << theClient.getChequing() << " in your Chequing account\n";
        break;
    }
    else if //Check if there is a $2.00 penalty.
    theClient.setChequing(theClient.getChequing() - ammount);
    theClient.setSavings(theClient.getSavings() + ammount);
    done = true;
    cout << endl << "Successfully transfered $" << ammount << " from Chequing account to Savings Account\n";
     */
}

void transferSavings(client& theClient) {
    cout << endl << "How much would you like to withdraw?\n";
    double ammount;
    ammount = getDouble();
    if (ammount > theClient.getSavings()) {
        cout << endl << "Error: Insufficient funds. You have $" << theClient.getSavings() << " in your Savings account\n";
        return;
    }
    theClient.setChequing(theClient.getChequing() + ammount);
    theClient.setSavings(theClient.getSavings() - ammount);
    cout << "Successfully transfered $" << ammount << " from Savings account to Chequing Account\n";
    cout << "Your current chequing balance is $" << theClient.getChequing() << endl;
    cout << "Your current savings balance is $" << theClient.getSavings() << endl;
}

void withdrawChequing(client& theClient) {
    start:
    cout << endl << "How much would you like to withdraw?\n";
    cout << "Enter 0 to cancel.\n";
    double x = getDouble();
    if (x == 0) {
        return;
    } 
    else if (x > theClient.getChequing()) {
        cout << endl << "Error: Insufficient funds. You have $" << theClient.getChequing() << " in your chequing account.\n";
        goto start;
    } 
    else {
        // Check if this puts the user under $1000 in the chequing account and apply a penalty if this is true.
        if (penalty(theClient, x)) { // There is a penalty.
            warning:
            // Warn the user if they want to proceed with the withdrawal.
            cout << "Because this transaction will bring your chequing account's balance below $1000.00, a\n";
            cout << "$2.00 fee will applied. Do you wish to continue?\n";
            cout << "1: Yes\n";
            cout << "2: No\n";
            switch (getNumber()) {
                case 1:
                { // Go through with the transaction.
                    // Check if the $2.00 penalty will put the balance below $0.
                    if (theClient.getChequing() - x - 2.00 < 0) {
                        cout << endl << "Error: Insufficient funds. You have $" << theClient.getChequing() << " in your chequing account.\n";
                        goto start;
                    } 
                    else {
                        // Perform the transaction.
                        theClient.setChequing(theClient.getChequing() - x - 2.00);
                        cout << "Successfully withdrew $" << x << " from chequing account.\n";
                        cout << "Your current chequing balance is $" << theClient.getChequing();
                        return;
                    }
                    break;
                }
                case 2:
                { // Cancel the transaction and exit the withdraw menu.
                    cout << "Withdrawal canceled. You have not been charged.\n";
                    return;
                }
                default:
                {
                    cout << endl << "Error: Invalid input\n";
                    goto warning;
                }

            }
        } 
        else { // No penalty.
            theClient.setChequing(theClient.getChequing() - x);
            cout << endl << "Successfully withdrew $" << x << " from chequing account\n";
            cout << "Your current chequing balance is $" << theClient.getChequing();
        }
        
    }
}

void withdrawSavings(client& theClient){
    start:
    cout << endl << "How much would you like to withdraw?\n";
    cout << "Enter 0 to cancel.\n";
    double x = getDouble();
    if (x == 0) {
        return;
    } 
    else if (x > theClient.getSavings()) {
        cout << endl << "Error: Insufficient funds. You have $" << theClient.getSavings() << " in your savings account.\n";
        goto start;
    } 
    else{ // Perform the transaction.
        theClient.setSavings(theClient.getSavings() - x);
        cout << "Successfully withdrew $" << x << " from savings account\n";
        cout << "Your current savings balance is $" << theClient.getSavings();
    }
}


void saveClients(vector<client>& clients){
    // Sort the clients vector by account number.
    sortByAccountNumber(clients);
    ofstream outputFile("clients.txt");
	for (int i = 0; i < clients.size(); i++){
		outputFile << clients[i].getId() << " " << clients[i].getChequing() << " " << clients[i].getSavings() << " " << clients[i].getFirstName() << " " << clients[i].getLastName()<<"\n";
	}
	outputFile.close(); 
}
void printClientMenu(client& theClient, bool cheq, bool sav){
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
	cout << endl<< "***********---What would you like to do?---***********\n";
	cout << "***********---1: Create an account---***********\n";
	cout << "***********---2: Delete an account---***********\n";
	cout << "***********---3: Display an account---***********\n";
	cout << "***********---4: Display all Accounts---***********\n";
	cout << "***********---5: Logout---***********\n";
}
void managerMenu(vector<client>& clients){
        cout <<endl << "Welcome Manager!\n";
        start:
	bool logout = false;
	while (!logout){
            
		printManMenu();
                
		switch (getNumber()){
                    
                // Create an account.   
		case 1:
                {
                    // Automatically generate a new account number.
                    int newAccNumber;
                    if (clients.empty()){
                        newAccNumber = 100;
                    }
                    else { // Generate a new account number by taking the last number on the on the clients vector and adding 1 to it.
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
                            // Escape back to the manager menu.
                            cout << "Hey guy";
                            goto start;
                        }
                        else if (firstName.size() >= 1){
                            valid1 = true;
                        }
                        else {
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
                            // Escape back to the manager menu.
                            goto start;
                        }
                        else if (lastName.size() >= 1){
                            valid1 = true;
                        }
                        else {
                            cout << "Please enter a valid name.\n";
                        }
                    }
                    
                    // Attempt to open the account.
                    bool success = openAccount(clients, newAccNumber, firstName, lastName);
                    if (!success){
                        cout << "Could not open account: account number already exists.\n";
                    }
                    else {
                        cout << "Account #" << newAccNumber << " for user " << firstName << " " << lastName << " was successfully created.\n";
                    }
                    break;
                }
                    
                // Delete an account.   
		case 2:
                {
                    bool valid2 = false;
                    // Get the account number.
                    while (!valid2){
                        cout << "Please enter an account number to delete.\n";
                        cout << "To cancel, enter 0.\n";
                        int accNumber = getNumber();
                        if (accNumber == 0) goto start;   //Escape back to the manager menu.
                        // Do not allow the account to be deleted if there is still money in it
                        int x = getAccountIndex(clients, accNumber, 0, clients.size() - 1);
                        if (x == -1){
                            cout << "Account #" << accNumber << " could not be found.\n";
                        }
                        else if (clients[x].getSavings() != 0 || clients[x].getChequing() != 0){
                            cout << "Account #" << accNumber << " could not be deleted since there exists a balance on their savings and/or chequing account.\n";
                        }
                        // Otherwise, attempt to delete the account.
                        else{
                            // Ask for confirmation.
                            cout << "Are you sure you want to delete account #" << accNumber << " owned by " << clients[x].getLastName() << ", " << clients[x].getFirstName() << "?\n";
                            cout << "1: Yes\n";
                            cout << "2: No\n";
                            
                            switch (getNumber()){
                                case 1:{ // Attempt to delete.
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
                                    cout << "Deletion canceled.\n";
                                    goto start;
                                }
                                default:
                                    {
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
                        }
                        else {
                            cout << "Account " << accNumber << " could not be found.\n";
                        }
                    }
                    break;
                }
                
                // Display all accounts.    
		case 4:
                {
                    if (clients.size() == 0){
                        cout << "No accounts to display.\n";
                    }
                    else{
                        // Ask which order to display accounts in.
                        cout << "***********---Sort by:---***********\n";
                        cout << "***********---1: Account Number---***********\n";
                        cout << "***********---2: Last Name---***********\n";
                        cout << "***********---0: Cancel---***********\n";
                        switch(getNumber())
                        {
                            case 1:
                            {
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
                                goto start;
                            }
                            default:
                            {
                                cout << "Error: Invalid input\n";
                            }
                        }
                        
                    }
                    break;
                }
                    
                // Log out.        
		case 5:
                {
                    return;
                }
		default:
			cout << "Error: Invalid input\n";

		}
	}
}
bool openAccount(vector<client>& clients, int accID, string firstName, string lastName)
{
    // Check if the account ID already exists in the database.
    if (existsInDatabase(clients, accID)){
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
        return true;
    }
    else {
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

bool compareAccountNums(client a, client b){
    return (a.getId() < b.getId());
}

bool compareAccountLastName(client a, client b){
    return (a.getLastName() < b.getLastName());
}

int getAccountIndex(vector<client>& clients, int targetAccNum, int low, int high){
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
        return mid; // Found.
    }
}

void maintenanceMenu(void){
	cout <<endl << "Welcome System Maintenance Person!\n";
        
        bool logout = false;
        while (!logout){
            cout << endl << "***********---What would you like to do?---*********** \n";
            cout << "***********---1: Toggle execution tracing---***********\n";
            cout << "***********---2: Print execution traces---***********\n";
            cout << "***********---3: Logout---***********\n";
            switch (getNumber()){
                
                // Toggle execution tracing.
                case 1:{
                    toggleExecutionTraces();
                    break;
                }
                
                // Print execution traces.
                case 2:{
                    printExecutionTraces();
                    break;
                }
                
                // Logout.
                case 3:{
                    return; // Exit the maintenance menu.
                }
                
                default:{
                    cout << "Invalid input\n";
                }   
            }
        }
}
int getNumber(){
	int ammount;
	cin >> ammount;
	while (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << endl << "Invalid input.  Enter a NUMBER: \n";
		cin >> ammount;
	}
	return ammount;
}
double getDouble(){
	double ammount;
	cin >> ammount;
	while (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << endl << "Invalid input.  Enter a NUMBER: \n";
		cin >> ammount;
	}
	return ammount;
}

bool penalty(client& theClient, double amount){
    // Check if the transaction will bring the client's chequing account down below $1000.
    if (theClient.getChequing() - amount < 1000.00){
        return true;
    }
    else{
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
                executionTraces = OFF;
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
                executionTraces = ON;
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
    ofstream etFile;
    // Open the file in append mode.
    etFile.open("execution_traces.txt", ios::app);
    // Write.
    etFile << str;
    // Close.
    etFile.close();
}

void printExecutionTraces(){
    // Open execution_traces.txt and print the file (excluding the first line).
    
}