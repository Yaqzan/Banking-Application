
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
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


void managerMenu(vector<client>& clients);					 // Brings user to the manager screen
void maintenanceMenu(void);									 // BRings user to the maintenance screen
void printManMenu(void);
//Client Menues
void clientMenu(client& theClient, vector<client>& clients); //Brings user to the screen for clients
void printClientMenu(client& theClient, bool cheq, bool sav);// Prints the options available to the client
void deposit(client& theClient);
void withdraw(client& theClient);
void transfer(client& theClient);
void chequing(client& theClient, bool cheq, bool sav);
void savings(client& theClient, bool sav, bool cheq);

//Helper functions
int getNumber();
double getDouble();
void saveClients(vector<client>& clients);					 // Writes client data to text file/ saves the changes

int main()
{
	// initialize everything
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
			else{

				for (int i = 0; i < clients.size(); i++){
					if (getId == clients[i].getId()){
						clientMenu(clients[i], clients);
						found = true;
						break;
					}
				}
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
	string input;
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
	double ammount;
	while (!done){
		cout <<endl<< "***********---Which account would you like to trasnfer from?---***********\n";
		cout << "***********---1: Chequings Account---***********\n";
		cout << "***********---2: Savings Account---***********\n";
		cout << "***********---3: Cancel---***********\n";
		switch (getNumber()){
		case 1:
			cout <<endl<< "How much would you like to withdraw?\n";
			ammount = getDouble();
			if (ammount > theClient.getChequing()){
				cout <<endl << "Error: Insufficient funds. You have $"<< theClient.getChequing()<< " in your Chequings account\n";
				break;
			}
			theClient.setChequing(theClient.getChequing() - ammount);
			theClient.setSavings(theClient.getSavings() + ammount);
			done = true;
			cout << endl <<"Successfully transfered $" << ammount << " from Chequings account to Savings Account\n";
			break;
		case 2:
			cout << endl<<"How much would you like to withdraw?\n";
			ammount = getDouble();
			if (ammount > theClient.getSavings()){
				cout <<endl<< "Error: Insufficient funds. You have $" << theClient.getSavings() << " in your Savings account\n";
				break;
			}
			theClient.setChequing(theClient.getChequing() + ammount);
			theClient.setSavings(theClient.getSavings() - ammount);
			done = true;
			cout << endl <<"Successfully transfered $" << ammount << " from Savings account to Chequings Account\n";
			break;
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
		cout << "***********---1: Chequings Account---***********\n";
		cout << "***********---2: Savings Account---***********\n";
		cout << "***********---3: Cancel---***********\n";
		cin >> y;
		switch (y){
		case 1:
			cout << endl<< "How much would you like to deposit?\n";
			x = getDouble();
			theClient.setChequing(theClient.getChequing() + x);
			cout <<endl<< "Deposited $" << x << " into Chequings Account\n";
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

		cout <<endl << "How much would you like to withdraw?\n";
		double x = getDouble();
		if (x == 0){
			return;
		}
		else if (x > theClient.getChequing()){
			cout << endl <<"Error: Insufficient funds. You have $"<<theClient.getChequing() <<" in your chequings account.\n";
		}else{
			theClient.setChequing(theClient.getChequing() - x);
			cout <<endl << "Successfully withdrew $" << x << " from chequing account\n";
			done = true;
		}
	}
}
void saveClients(vector<client>& clients){
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
	cout << endl<< "***********---What woudld you like to do?---***********\n";
	cout << "***********---1: Create an account---***********\n";
	cout << "***********---2: Delete an account---***********\n";
	cout << "***********---3: Display an account---***********\n";
	cout << "***********---4: Display all Accounts---***********\n";
	cout << "***********---5: Logout---***********\n";
}
void managerMenu(vector<client>& clients){
	cout <<endl << "Welcome Manager!\n";
	bool logout = false;
	int x;
	while (true){
		printManMenu();
		switch (getNumber()){
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			return;
		default:
			cout << "Error: Invalid input\n";

		}
	}
}
void maintenanceMenu(void){
	cout <<endl << "Welcome System Maintenance Person!\n";
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