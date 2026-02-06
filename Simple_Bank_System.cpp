
#include <iostream>
#include<string>
#include<cctype>
#include<vector>
#include<fstream>
#include<iomanip>
using namespace std;
const string ClientsFileName = "Clients.txt";
void ShowTransactionMenu();
void ShowMainMenu();
struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;

};
vector<string>SplitString(string S1, string Delim)
{
	string sWord;
	vector<string>vString;
	short Pos = 0;
	while ((Pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, Pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		S1.erase(0, Pos + Delim.length());
	}
	if (S1 != "")
	{
		vString.push_back(S1);
	}
	return vString;
}
sClient ConvertLineToRecord(string Line, string Seperator = "#//#")
{
	sClient Client;
	vector<string>vClientData;
	vClientData = SplitString(Line, Seperator);
	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);
	return Client;
}
string ConvertRecordToLine(sClient Client, string Seprator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seprator;
	stClientRecord += Client.PinCode + Seprator;
	stClientRecord += Client.Name + Seprator;
	stClientRecord += Client.Phone + Seprator;
	stClientRecord += to_string(Client.AccountBalance);
	return stClientRecord;
}
bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	vector<sClient>vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return false;
}
sClient ReadNewClient()
{
	sClient Client;
	cout << "Enter Account Number ? ";
	getline(cin >> ws, Client.AccountNumber);
	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
	{
		cout << "\nClient With[" << Client.AccountNumber << "]already existis ,Enter another Account Number?";
		getline(cin >> ws, Client.AccountNumber);
	}
	cout << "Enter PinCode?";
	getline(cin, Client.PinCode);
	cout << "Enter Name ? ";
	getline(cin, Client.Name);
	cout << "Enter Phone ? ";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance ? ";
	cin >> Client.AccountBalance;
	return Client;
}
vector <sClient>LoadClientsDataFromFile(string FileName)
{
	vector<sClient>vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			if (Line == "")
			{
				continue;
			}
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}
void GoBackToTransactionMenu()
{
	cout << "\n\nPress any key to go back to transaction menue....";
	system("pause>0");
	ShowTransactionMenu();
}
void PrintClientRecordLine(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}
void PrintAllClients()
{
	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	cout << "\n\t\t\t\t\tClient List(" << vClients.size() << ")Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n";
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Avilable in the system!";
	else

	for (sClient Client : vClients)
	{
		PrintClientRecordLine(Client);
		cout << endl;
	}
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}
void PrintClientRecord(sClient Client)
{
	cout << "\n\nThe following is the extracted client record:\n";
	cout << "\nAccout Number: " << Client.AccountNumber;
	cout << "\nPin Code : " << Client.PinCode;
	cout << "\nName : " << Client.Name;
	cout << "\nPhone : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
}
bool FindClientByAccountNumber(string AccounNumber, vector<sClient>&vClients, sClient& Client)
{

	for (sClient C : vClients)
	{
		if (C.AccountNumber == AccounNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}
sClient ChangeClientRecord(string AccountNumber)
{
	sClient Client;
	Client.AccountNumber = AccountNumber;
	cout << "\n\nEnter PinCode ? ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name ? ";
	getline(cin, Client.Name);
	cout << "Enter Phone ? ";
	getline(cin, Client.Phone);
	cout << "Enter AccoutBalance ? ";
	cin >> Client.AccountBalance;
	return Client;
}
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	for (sClient& C : vClients)
	{
		if (AccountNumber == C.AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
vector<sClient>SaveClientsDataToFile(string FileName, vector<sClient>vClients)
{
	string DataLine;
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}
void AddDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}
void AddNewClient()
{
	sClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}
bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	sClient Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientRecord(Client);
		cout << "\n\nAre you sure you want delete this client? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber,
				vClients);
			SaveClientsDataToFile(ClientsFileName, vClients);
			//Refresh Clients
			vClients = LoadClientsDataFromFile(ClientsFileName);
			cout << "\n\nClient Deleted Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber
			<< ") is Not Found!";
		return false;
	}
}
bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClienst)
{
	sClient Clients;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, vClienst, Clients))
	{
		PrintClientRecord(Clients);
		cout << "Are you sure you want update this client?y/n?";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			for (sClient& C : vClienst)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientsDataToFile(ClientsFileName, vClienst);
			cout << "\n\nClient Update successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClients with account Number (" << AccountNumber << ")is not found !";
		return false;
	}
}
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<sClient>& vClients)
{
	char Answer = 'n';
	cout << "Are you sure you want performe this transaction ? y/n ? ";
	cin>> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		for (sClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
			 C.AccountBalance+= Amount;
				SaveClientsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Successfully.New balance is:" << C.AccountBalance;
				return true;
			}
		}
	}
	return false;
}
string ReadClientAccountNumber()
{
	string AccountNumber = "";
	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}
void ShowDepositScreen()
{
	cout << "\n------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n------------------------------\n";
	sClient Client;
	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber= ReadClientAccountNumber();
	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "Client with [" << AccountNumber << "]does not exist.\n";
		AccountNumber=ReadClientAccountNumber();
	}
	PrintClientRecord(Client);
	double Amount = 0;
	cout << "\nPlease enter deposit amount ? ";
	cin >> Amount;
	DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}
void ShowDeleteClientScreen()
{
	cout << "\n--------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n--------------------------------\n";
	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}
void ShowUpdateClientInfoScreen()
{
	cout << "\n--------------------------------\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n--------------------------------\n";
	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}
void ShowAddNewClientScreen()
{
	cout << "\n------------------------------\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n------------------------------\n";
	AddNewClient();
}
void ShowFindClientScreen()
{
	sClient Client;
	cout << "\n------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n------------------------------\n";
	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientRecord(Client);
	}
	else
	{
		cout << "\nClient with Account Number(" << AccountNumber << ")is not found.";
	}
}
void ShowEndScreen()
{
	cout << "\n------------------------------\n";
	cout << "\tProgram Ends :-) ";
	cout << "\n------------------------------\n";
}
void ShowWithDrawScreen()
{
	cout << "\n-------------------------------\n";
	cout << "\tWithdraw Screen ";
	cout << "\n-------------------------------\n";
	sClient Client;
	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with[" << AccountNumber << "]does not exist\n";
		AccountNumber = ReadClientAccountNumber();
	}
	PrintClientRecord(Client);
	double Amount = 0;
	cout << "\nPLease enter withdraw amount ? ";
	cin >> Amount;
	while (Amount > Client.AccountBalance)
	{
		cout << "\nAmount Exceeds the balance ,you can withdraw up to : " << Client.AccountBalance << "Please enter another amount ?";
		cin >> Amount;
	}
	DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}
void PrintClientRecordBalanceLine(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}
void ShowTotalBalanceScreen()
{
	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	cout << "\n\t\t\t\t\tBalances List(" << vClients.size() << ")Client(s)";
	cout<< "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	double TotalBalances = 0;
	if (vClients.size() == 0)
	{
		cout << "\t\t\t\tNo Client Avilable in the system !";
	}
	else
	{
		for (sClient C : vClients)
		{
			PrintClientRecordBalanceLine(C);
			TotalBalances += C.AccountBalance;
			cout << endl;
		}
		cout << "\n_______________________________________________________";
		cout << "_________________________________________\n" << endl;
		cout << "\t\t\t\t\t Total Balances = " << TotalBalances;
	}
}
enum enMainMenuOptions
{
	eShowClientList = 1, eAddNewClient = 2, eDelteClient = 3, eUpdateClientInfo = 4, eFindClient = 5, eTransactions = 6, eExit = 7
};
enum enTransactionMenu { enDeposit = 1, enWithdraw = 2, enTotalBalance = 3, enMainMenu = 4 };
void GoBackToMainMenu()
{
	cout << "\n\nPress any key to go back to main menu.... ";
	system("Pause>0");
	ShowMainMenu();
}
void PerformTransactionManiMenu(enTransactionMenu Option)
{
	switch (Option)
	{
	case enTransactionMenu::enDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionMenu();
		break;
	case enTransactionMenu::enWithdraw:
		system("cls");
		ShowWithDrawScreen();
		GoBackToTransactionMenu();
		break;
	case enTransactionMenu::enTotalBalance:
		system("cls");
		ShowTotalBalanceScreen();
		GoBackToTransactionMenu();
		break;
	case enTransactionMenu::enMainMenu:
		ShowMainMenu();
	}
}
short ReadTransactionMenueOption()
{
	short Choice = 0;
	cout << "Choose what do you want to do ?[1 to 4]?";
	cin >> Choice;
	return Choice;
}
void ShowTransactionMenu()
{
	system("cls");
	cout << "==================================================\n";
	cout << "\t\tTranaction Menue Screen : \n";
	cout << "==================================================\n";
	cout << "\t[1] Deposit.\n ";
	cout << "\t[2] WithDraw.\n ";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "==================================================\n";
	PerformTransactionManiMenu((enTransactionMenu)ReadTransactionMenueOption());

}
short ReadMainMenueOption()
{
	cout << "Choose what do you want to do ?[1 to 6]?";
	short Choice = 0;
	cin >> Choice;
	return Choice;
}


void PerformMainMenuOption(enMainMenuOptions Option)
{
	switch (Option)
	{
	case enMainMenuOptions::eShowClientList:
		system("cls");
		PrintAllClients();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eAddNewClient:
		system("cls");
		ShowAddNewClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eDelteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eUpdateClientInfo:
		system("cls");
		ShowUpdateClientInfoScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eTransactions:
		system("cls");
		ShowTransactionMenu();
		GoBackToMainMenu();
		break;

	case enMainMenuOptions::eExit:
		system("cls");
		ShowEndScreen();
		break;
	}
}
void ShowMainMenu()
{
	system("cls");
	cout << "===================================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "===================================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client. \n";
	cout << "\t[4] Update Client Info. \n";
	cout << "\t[5] Find Client. \n";
	cout << "\t[6] Transactions. \n";
	cout << "\t[7] Exit. \n";
	cout << "===================================================\n";
	PerformMainMenuOption((enMainMenuOptions)ReadMainMenueOption());
}

int main()
{
	
	ShowMainMenu();
	system("pause>0");
	return 0;
}


