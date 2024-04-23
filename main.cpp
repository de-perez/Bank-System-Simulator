/*
    Name: Ramon Perez, CS 302-1004
    Description: This program uses a custom created 2D hashMap to simulate a bank system.
	The program will read a .csv file and load the entries into a 2D hashmap of
	<string, hashMap<string, bankType>. bankType holds information like name, balance, pin,
	account number, and locked status.
    Output: Account information and deposit/withdraw transactions made during program execution.
*/
#include "hashMap.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

struct bankType
{
	struct transactionType
	{
		transactionType(bool t, double a)
		{
			type = t;
			amount = a;
		}

		bool type;
		double amount;
	};

	bankType(std::string first, std::string last, std::string acctNo,
		double balance,unsigned int pin)
	{
		name = last + ", " + first;
		this->balance = balance;
		this->pin = pin;
		this->acctNo = acctNo;
		locked = false;
	}

	bankType()
	{
		name = "";
		balance = 0;
		pin = 0;
		locked = false;
	}

	std::string name;
	std::string acctNo;
	double balance;
	unsigned int pin;
	bool locked;

	std::vector<transactionType> transactions;
};


int main()
{
	/*
    FUNCTION_IDENTIFIER: This main function will read in data.csv into a 2D hashmap.
	It will then prompt the user for a location, error check, then for the last 4
	digits of an account, error check, then pin. If successful, the user can deposit,
	withdraw, see transactions, or exit the account. When they exit, they will be asked
	if they want to access another account, or exit the program.
    Return Value: N/A
    */
	std::cout << std::fixed << std::setprecision(2);
	hashMap<std::string, hashMap<std::string, bankType>> bank;

	// Read from CSV File
	std::fstream bankData;
	bankData.open("data.csv");

    std::string line = "";

	// Toss first line
	getline(bankData, line);

	hashMap <string, bool> validLocations;
	
	while (getline(bankData, line))
	{
		std::string firstName = line.substr(0, line.find(","));
		line.erase(0, line.find(",") + 1);

		std::string lastName = line.substr(0, line.find(","));
		line.erase(0, line.find(",") + 1);

		std::string location = line.substr(0, line.find(","));
		line.erase(0, line.find(",") + 1);
		for (int i = 0; i < location.length(); i++)
        {
            location[i] = tolower(location[i]);
        }
		validLocations[location] = true;

		std::string acctNo = line.substr(0, line.find(","));
		line.erase(0, line.find(",") + 1);

		std::string balanceS = line.substr(0, line.find(","));
		line.erase(0, line.find(",") + 1);
		double balance = std::stod(balanceS);

		std::string pinS = line.substr(0, line.find(","));
		line.erase(0, line.find(",") + 1);
		unsigned int pin = std::stoul(pinS);

		std::string acc4Digits = acctNo.substr(acctNo.length() - 4);

		bank[location][acc4Digits] = bankType(firstName, lastName, acctNo, balance, pin);
	}

    // Close the file
    bankData.close();


	// Main Algorithm
	bool promptContinue = false;
	string uLocation = "";
	string uAcc4Digits = "";
	unsigned int uPin = 0;

	// Reset variables
	bool exitOptions = false;
	char uChoice, uContinue = ' ';
	double depAmount, withAmount = 0;

	bool exitProgram = false;
	bool continueSession = true;
	// Step 1:
	do{ // *************************** Step 1 ***************************
	// Reset Variables
	exitOptions = false;
	uChoice, uContinue = ' ';
	depAmount, withAmount = 0;
	continueSession = true;
	string uLocation = "";

	// Check Location
	cout << "\nPlease enter bank branch location: ";
	getline(cin >> ws, uLocation); // IMPORTANT: cin >> ws allows the proper use of getline() after cin 
	for (int i = 0; i < uLocation.length(); i++)
	{
		uLocation[i] = tolower(uLocation[i]);
	}

	if (!validLocations[uLocation])
	{
		cout << "\nBank branch not found\n";
		continueSession = false;
	} 

	// Check Last 4 Account Digits
	if (continueSession){ // These lines will skip to the end if input was invalid
	cout << "\nEnter Last 4 digits of account: ";
	cin >> uAcc4Digits;
	if (bank[uLocation][uAcc4Digits].acctNo == "")
	{
		cout << "Account not found\n";
		continueSession = false;
	} 

	// Check if account is locked
	if (continueSession){
	if (bank[uLocation][uAcc4Digits].locked)
	{
		cout << "Account has a lock\n";
		continueSession = false;
	}

	// Check Pin
	if (continueSession) {
	int pinCount = 0;
	do
	{
		cout << "\nEnter a pin: ";
		cin >> uPin;
		
		if (bank[uLocation][uAcc4Digits].pin != uPin)
		{
			cout << "\nInvalid pin\n";
			pinCount++;
			if (pinCount == 3)
			{
				bank[uLocation][uAcc4Digits].locked = true;
				cout << "\n3 failed login attempts, account is now locked\n";
				continueSession = false;
			}
		}
	}
	while (bank[uLocation][uAcc4Digits].pin != uPin && continueSession);

	// Transaction Type Loop:
	if (continueSession) {
	// Step 8:

	// Reset Variables
	exitOptions = false;
	uChoice = uContinue = ' ';
	depAmount = withAmount = 0;
	do
	{
		cout << "\n(D)eposit\n(W)ithdrawal\n(V)iew account\n(E)xit account\nEnter choice : ";
		cin >> uChoice;

		switch(uChoice)
		{
		case 'D':
		case 'd':
			depAmount = 0;
			cout << "\nEnter deposit amount: ";
			cin >> depAmount;
			bank[uLocation][uAcc4Digits].balance += depAmount;
			// bank[uLocation][uAcc4Digits].transactions.push_back(false, depAmount);
			bank[uLocation][uAcc4Digits].transactions.push_back(bankType::transactionType(true, depAmount));
			// Go to step 8	
			break;
		case 'W':
		case 'w':
			withAmount = 0;
			cout << "\nEnter withdrawal amount: ";
			cin >> withAmount;
			if (bank[uLocation][uAcc4Digits].balance >= withAmount)
			{
				bank[uLocation][uAcc4Digits].balance -= withAmount;
				bank[uLocation][uAcc4Digits].transactions.push_back(bankType::transactionType(false, withAmount));
			}
			else
			{
				cout << "\nNot enough funds to make transaction\n";
			}
			// Go to Step 8
			break;
		case 'V':
		case 'v':
			cout << "\nName: " << bank[uLocation][uAcc4Digits].name
			<< 		"\nBalance: $" << bank[uLocation][uAcc4Digits].balance << endl;

			if (bank[uLocation][uAcc4Digits].transactions.size() > 0)
			{
				cout << "\nList of transactions\n";
				for (auto it : bank[uLocation][uAcc4Digits].transactions)
				{
					if (it.type) // Deposit
					{
						cout << "Deposit amount $" << it.amount << endl;
					}
					else // Withdrawal
					{
						cout << "Withdrawal amount $" << it.amount << endl;
					}
				}
			}
			else
			{
				cout << "\nNo transactions\n";
			}
			// Go to Step 8
			break;
		case 'E':
		case 'e':
			exitOptions = true;
		break;
		default:
			cout << "\nInvalid choice";
			break;
		}
	} while (!exitOptions);
	// These brackets are from error checking with continueSession bool 
	}
	}
	}
	}

	// Step 14
	cout << "\nContinue? (Y/N): ";
	cin >> uContinue;
	if (uContinue == 'y' || uContinue == 'Y')
	{
		// Restart Program (Step 1)
	}
	else
	{
		// End Program
		exitProgram = true;
	}
	} while(!exitProgram); // *************************** End of Step 1 Loop ***************************

	return 0;
}