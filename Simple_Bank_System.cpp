#include <iostream>
#include<string>
#include<cctype>
#include<vector>
#include<fstream>
#include<iomanip>
using namespace std;
const string UsersFileName = "Users.txt";
const string ClientsFileName = "Clients.txt";

void ShowTransactionMenu();
void ShowMainMenu();
void ShowManageUserMenue();
void Login();
struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;

};
struct stUser
{
	string UserName;
	string Password;
	int Permissions;
	bool MarkForDelete = false;
};
stUser CurrentUser;
bool CheckAccessPermission();
enum enMainMenuOptions
{
	eShowClientList = 1, eAddNewClient = 2, eDelteClient = 3, eUpdateClientInfo = 4, eFindClient = 5,
	eTransactions = 6, eManageUSers = 7,eExit=8
};
enum enTransactionMenu { enDeposit = 1, enWithdraw = 2, enTotalBalance = 3, enMainMenu = 4 };
enum enManageUsersMenueOption{eListUSers=1,eAddNewUser=2,eDeleteUser=3,eUpdateUser=4,eFindUSer=5,eMainMenue=6};
enum enMainMenuPremissions { eAll = -1, pListClients = 1, pAddNewClient=2,pDeleteClient=4,pUpdateClient=8,pFindClient=16,
pTransaction=32,pManageUsers=64};//Permissions are implemented using the bitwise concept.
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
stUser ConvertUserLineToRecord(string Line, string Seperator = "#//#")
{
	stUser User;
	vector<string>vUserData;
	vUserData = SplitString(Line, Seperator);
	User.UserName = vUserData[0];
	User.Password = vUserData[1];
	User.Permissions = stoi(vUserData[2]);
	return User;
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
stUser ConverUserLineToRecord2(string Line, string Seperator = "#//#")
{
	stUser User;
	vector<string>vUserData;
	vUserData = SplitString(Line, Seperator);
	User.UserName = vUserData[0];
	User.Password = vUserData[1];
	User.Permissions = stoi(vUserData[3]);
	return User;
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
string ConvertRecordToLine(stUser User, string Seperator = "#//#")
{
	string stUserRecord = "";
	stUserRecord += User.UserName + Seperator;
	stUserRecord += User.Password + Seperator;
	stUserRecord += to_string(User.Permissions);
	return stUserRecord;
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
bool UserExistsByUserName(string UserName, string FileName)
{
	vector<stUser>vUser;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		stUser User;
		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);
			if (User.UserName == UserName)
			{
				MyFile.close();
				return true;
			}
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
int ReadPermissionToSet()
{
	int Permission = 0; 
	char Answer = 'n';
	cout << "\nDo you want to give full access?y/n? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			return -1;//Full access 
		}
		cout << "\nDo you want to give access to : \n";
		cout << "\nShow client list?y/n? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			Permission += enMainMenuPremissions::pListClients;
		}
		cout << "\nAdd New Client?y/n? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			Permission += enMainMenuPremissions::pAddNewClient;
		}
		cout << "\nDelete Client?y/n? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			Permission += enMainMenuPremissions::pDeleteClient;
		}
		cout << "\nUpdate Client ?y/n? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			Permission += enMainMenuPremissions::pUpdateClient;
		}
		cout << "\nFind Client?y/n? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			Permission += enMainMenuPremissions::pFindClient;
		}
		cout << "\nTransaction?y/n? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			Permission += enMainMenuPremissions::pTransaction;
		}
		cout << "\nManage users?y/n? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			Permission += enMainMenuPremissions::pManageUsers;
		}
		return Permission;
}
stUser ReadNewUser()
{
	stUser User;
	cout << "\nPlase Enter Username?\n  ";
	getline(cin >> ws, User.UserName);
	while (UserExistsByUserName(User.UserName, UsersFileName))
	{
		cout << "\nUser with [" << User.UserName << "] already exists, Enter another username? ";
		getline(cin >> ws, User.UserName);
	}
	cout << "Enter Password :\n";
	getline(cin, User.Password);
	User.Permissions = ReadPermissionToSet();
	return User;
}
vector<stUser>LoadUsersDataFromFile(string FileName)
{
	vector<stUser>vUser;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		stUser User;
		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);
			vUser.push_back(User);
		}
		MyFile.close();
	}
	return vUser;
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
void PrintClientRecordLine(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}
void PrintUserRecordLine(stUser User)
{
	cout << "| " << setw(15) << left << User.UserName;
	cout << "| " << setw(10) << left << User.Password;
	cout << "| " << setw(40) << left << User.Permissions;
}
void ShowAccessDeniedMessage()
{
	cout << "\n---------------------------------------\n";
	cout << "Access Denied,\nYou dont have permission to do this,\nPlease conact your admin.";
	cout << "\n---------------------------------------\n";
}
bool CheckAccessPermission(enMainMenuPremissions Permission)
{
	if (CurrentUser.Permissions == enMainMenuPremissions::eAll)
	{
		return true;
	}
	if ((Permission & CurrentUser.Permissions) == Permission)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void ShowAllClientScreen()
{
	if (!CheckAccessPermission(enMainMenuPremissions::pListClients))
	{
		ShowAccessDeniedMessage();
		return;
	}
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
void ShowAllUsersScreen()
{
	vector<stUser>vUsers = LoadUsersDataFromFile(UsersFileName);
	cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ")User(s).";
	cout << "\n__________________________________________________";
	cout << "________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "User Name";
	cout << "| " << left << setw(15) << "Password";
	cout << "| " << left << setw(40) << "Permissions";
	cout << "\n__________________________________________________";
	cout << "_________________________________________\n";
	if (vUsers.size() == 0)
	{
		cout << "\t\t\t\tNo Users Avilable in the system!";
	}
	else
	{
		for (stUser User : vUsers)
		{
			PrintUserRecordLine(User);
			cout << endl;
		}
	}
	cout << "\n-------------------------------------------------";
	cout << "----------------------------------------\n" << endl;
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
	cout << "\n_______________________________________________________";
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
void PrintClientRecord(sClient Client)
{
	cout << "\n\nThe following is the extracted client record:\n";
	cout << "\nAccout Number: " << Client.AccountNumber;
	cout << "\nPin Code : " << Client.PinCode;
	cout << "\nName : " << Client.Name;
	cout << "\nPhone : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
}
void PrintClientCard(sClient Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "-----------------------------------";
	cout << "\nAccout Number: " << Client.AccountNumber;
	cout << "\nPin Code : " << Client.PinCode;
	cout << "\nName : " << Client.Name;
	cout << "\nPhone : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n-----------------------------------\n";
}
void PrintUsersCard(stUser User)
{
	cout << "\nThe following are the user details :\n";
	cout << "________________________________________";
	cout << "\nUsername     :" << User.UserName;
	cout << "\nPassword     :" << User.Password;
	cout << "\nPermissions  :" << User.Permissions;
	cout << "\n_____________________________________\n";
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
stUser ChangeUsersByRecord(string Username)
{
	stUser User;
	User.UserName = Username;
	cout << "\n\nEnter Password? ";
	getline(cin >> ws, User.Password);
	User.Permissions = ReadPermissionToSet();
	return User;
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
bool MarkUsersForDeleteByUsername(string Username, vector<stUser>& vUsers)
{
	for (stUser& U : vUsers)
	{
		if (Username == U.UserName)
		{
			U.MarkForDelete = true;
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
vector<stUser>SaveUsersDataToFile(string FileName, vector<stUser>vUser)
{
	fstream MyFile;
	string DataLine;
	MyFile.open(FileName, ios::out);
	if (MyFile.is_open())
	{
		for (stUser U : vUser)
		{
			if (U.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(U);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vUser;
}
void AddDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile  << stDataLine << endl;
		MyFile.close();
	}
}
void AddNewClient()
{
	sClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}
void AddNewUsers()
{
	stUser User;
	User = ReadNewUser();
	AddDataLineToFile(UsersFileName, ConvertRecordToLine(User));
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
bool FindUserByUserName(string UserName, vector<stUser>vUsers, stUser& User)
{
	for (stUser U : vUsers)
	{
		if ((U.UserName == UserName))
		{
			User = U;
			return true;
		}
	}
	return false;
}
bool DeleteUserByUsername(string Username, vector<stUser>& vUsers)
{
	if (Username == "Admin")
	{
		cout << "\n\nYou cannot delete this user.";
		return false;
	}
	stUser User;
	char Answer = 'n';
	if (FindUserByUserName(Username, vUsers, User))
	{
		PrintUsersCard(User);
		cout << "\n\nAre you sure you want delete this user?y/n?";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			MarkUsersForDeleteByUsername(Username, vUsers);
			SaveUsersDataToFile(UsersFileName, vUsers);
			vUsers = LoadUsersDataFromFile(UsersFileName);
			cout << "\n\nUser Deleted Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nUser withr username (" << Username << ")is not found!";
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
bool UpdateUserByUserName(string Username, vector<stUser>& vUsers)
{
	stUser User;
	char Answer = 'n';
	if (FindUserByUserName(Username, vUsers, User))
	{
		PrintUsersCard(User);
		cout << "\n\nAre you sure you want uodate this user?y/n? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			for (stUser& U : vUsers)
			{
				if (U.UserName == Username)
				{
					U = ChangeUsersByRecord(Username);
					break;
				}
			}
			SaveUsersDataToFile(UsersFileName, vUsers);
			cout << "\n\nUser Update Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nUser with account number (" << Username << ")is not found!";
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
string ReadUserName()
{
	string Username = "";
	cout << "\nPlease enter username?";
	cin >> Username;
	return Username;
}
void ShowListUsersScreen()
{
	ShowAllUsersScreen();
}
void ShowAddNewUsersScreen()
{
	cout << "\n---------------------------------\n";
	cout << "\tAdd new user screen";
	cout << "\n----------------------------------\n";
	AddNewUsers();
}
void ShowDeleteUserScreen()
{
	cout << "\n------------------------------\n";
	cout << "\tDelete Users Screen";
	cout << "\n-------------------------------\n";
	vector<stUser>vUser = LoadUsersDataFromFile(UsersFileName);
	string Username = ReadUserName();
	DeleteUserByUsername(Username, vUser);
}
void ShowUpdateUserScreen()
{
	cout << "\n----------------------------\n";
	cout << "\tUpdate Users Screen";
	cout << "\n------------------------------\n";
	vector<stUser>vUser = LoadUsersDataFromFile(UsersFileName);
	string Username = ReadUserName();
	UpdateUserByUserName(Username, vUser);

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
void ShowFindUserScreen()
{
	cout << "\n-------------------------------\n";
	cout << "\tFind User Screen";
	cout << "\n-------------------------------\n";
	vector<stUser>vUser = LoadUsersDataFromFile(UsersFileName);
	stUser User;
	string Username = ReadUserName();
	if (FindUserByUserName(Username, vUser, User))
	{
		PrintUsersCard(User);
	}
	else
	{
		cout << "\nUSer with username [" << Username << "] is not found!";
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
void GoBackToMainMenu()
{
	cout << "\n\nPress any key to go back to main menu.... ";
	system("Pause>0");
	ShowMainMenu();
}
void GoBackToTransactionMenu()
{
	cout << "\n\nPress any key to go back to transaction menue....";
	system("pause>0");
	ShowTransactionMenu();
}
void GoBackToManageUserMenue()
{
	cout << "\n\nPress any key to go back to transaction menue...";
	system("pause>0");
	ShowManageUserMenue();
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
	cout << "Choose what do you want to do ?[1 to 7]?";
	cin >> Choice;
	return Choice;
}
void ShowTransactionMenu()
{
	if (!CheckAccessPermission(enMainMenuPremissions::pTransaction))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenu();
		return;
	}
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
	cout << "Choose what do you want to do ?[1 to 8]?";
	short Choice = 0;
	cin >> Choice;
	return Choice;
}
void PerformManageUsermenueoptin(enManageUsersMenueOption ManageUserMenueOption)
{
	switch (ManageUserMenueOption)
	{
	case enManageUsersMenueOption::eListUSers:
	{
		system("cls");
		ShowListUsersScreen();
		GoBackToManageUserMenue();
		break;
	}
	case enManageUsersMenueOption::eAddNewUser:
	{
		system("cls");
		ShowAddNewUsersScreen();
		GoBackToManageUserMenue();
		break;
	}
	case enManageUsersMenueOption::eDeleteUser:
	{
		system("cls");
		ShowDeleteUserScreen();
		GoBackToManageUserMenue();
		break;
	}
	case enManageUsersMenueOption::eUpdateUser:
	{
		system("cls");
		ShowUpdateUserScreen();
		GoBackToManageUserMenue();
		break;
	}
	case enManageUsersMenueOption::eFindUSer:
	{
		system("cls");
		ShowFindUserScreen();
		GoBackToManageUserMenue();
		break;
	}
	case enManageUsersMenueOption::eMainMenue:
		system("cls");
		ShowMainMenu();
		GoBackToMainMenu();
		break;
	}
}
short ReadManageUsersMenueOption()
{
	cout << "Choose what do you want to do? [1 to 6]?";
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
		ShowAllClientScreen();
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

	case enMainMenuOptions::eManageUSers:
	{
		system("cls");
		ShowManageUserMenue();
		break;
	}
	case enMainMenuOptions::eExit:
		system("cls");
		Login();
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
	cout << "\t[7] Manage Users. \n";
	cout << "\t[8] Logout.\n";
	cout << "===================================================\n";
	PerformMainMenuOption((enMainMenuOptions)ReadMainMenueOption());
}
void ShowLoginScreen()
{ 
	cout << "\n";
	cout << "==================================================\n";
	cout << "\t\tLoginScreen";
	cout << "==================================================\n";

}
bool FindUserByUserNameAndPassword(string UserName, string Password, stUser& User)
{
	vector<stUser>vUsers = LoadUsersDataFromFile(UsersFileName);
	for (stUser U : vUsers)
	{
		if (U.UserName == UserName && U.Password == Password)
		{
			User = U;
			return true;
		}
	}
	return false;
}
bool LoadUserInfo(string UserName, string Password)
{
	if (FindUserByUserNameAndPassword(UserName, Password, CurrentUser))
	{
		return true;
	}
	else
	{
		return false;
	}
}
void ShowManageUserMenue()
{
	if (!CheckAccessPermission(enMainMenuPremissions::pManageUsers))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenu();
		return;
	}
	system("cls");//Clean Screen
	cout << "==================================================\n";
	cout << "\t\tManage Users Menue Screen\n";
	cout << "==================================================\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menue\n";
	cout << "==================================================\n";
	PerformManageUsermenueoptin((enManageUsersMenueOption)ReadManageUsersMenueOption());
}
void Login()
{
	bool LoginFaild = false;
	string UserName, Password;
	do
	{
		system("cls");
		cout << "\n==============================\n";
		cout << "\nLogin Screen";
		cout << "\n==============================\n";
		if (LoginFaild)
		{
			cout << "Invalid UserName Or Passwprd .";
		}
		cout << "Enter User Name? \n";
		getline(cin, UserName);
		cout << "Enter Password? \n";
		cin >> Password;
		LoginFaild = !LoadUserInfo(UserName, Password);
	} while (LoginFaild);
	ShowMainMenu();
}
int main()
{
	Login();
	system("pause>0");
	return 0;
}


