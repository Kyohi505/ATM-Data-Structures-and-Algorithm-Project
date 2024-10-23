#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>
#include <filesystem>

using std::cout;
using std::cin;
using std::string;
using std::getline;

struct Acc
{
    string name;
    string bday;
    string contact;
    string accNum;
    string pinCode;
    double balance;
    string encryptedPin;
};

struct Node
{
    Acc data;
    Node* next;
    Node(Acc x) : data(x), next(NULL) {}
};

class System
{
private:
    Node *head;
    Node *currentUser;

    int createAccNumber();
    void checkRegister();
    void showBalance();
    void withdraw();
    void deposit();
    int initialDeposit();
    void changeInfo();
    void changePin();
    void machineMenu();
    void accMenu();
    void showAcc();
    void pinChecker(string &pin);
    void locateAcc(string x);
    void fundTransfer();
    string adam = "ilovedsa";
    string decryptEncrypt(string pin, string adam);

    //usb atm card related variables
    bool isUSBInserted(string usbPath);
    bool AccountFound(string usbPath, string accountPath);
    void ATMCARD();

public:
    System() : head(NULL), currentUser(NULL) {}
    void registerAcc(string usbPath);
    void enterAcc(string n, string p, string accountPath);
    void storeAcc();
    void loadAcc();
};

int mainMenu()
{
    int op;

    system("cls");

    cout << "Menu\n";
    cout << "1 - Deposit\n";
    cout << "2 - Withdraw\n";
    cout << "3 - Balance Inquiry\n";
    cout << "4 - Fund Transfer\n";
    cout << "5 - Account\n";
    cout << "6 - Log Out\n";
    cout << "Select(1-6)\n ";
    cout << "-> ";

    cin >> op;
    return op;
}

void System::machineMenu()
{

    while (true) {
        switch (mainMenu())
        {
        case 1:
            system("cls");
            showBalance();
            system("pause");
            system("cls");
            cout << "Deposit\n";
            deposit();
            showBalance();
            system("pause");
            break;

        case 2:
            system("cls");
            cout << "Withdraw\n";
            withdraw();
            system("pause");
            system("cls");
            showBalance();
            system("pause");
            break;

        case 3:
            system("cls");
            showBalance();
            system("pause");
            break;

        case 4:
            system("cls");
            fundTransfer();
            break;

        case 5:
            system("cls");
            accMenu();
            break;

        case 6:
            system("cls");
            cout << "Thank you and Goodbye!\n";
            system("pause");
            return;
            break;

        default:
            cout << "Please enter only (1 - 6)\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("pause");
        }
    }
}

int infoMenu()
{
    int op;

    system("cls");

    cout << "------------------\n";
    cout << "1 - Account Details\n";
    cout << "2 - Change Account Information\n";
    cout << "3 - Change Pin\n";
    cout << "4 - Cancel\n";
    cout << "-> ";

    cin >> op;
    return op;
}

void System::accMenu()
{

    while (true) {
        switch (infoMenu())
        {
        case 1:
            showAcc();
            break;

        case 2:
            changeInfo();
            break;

        case 3:
            changePin();
            break;

        case 4:
            return;

        default:
            cout << "Enter only (1-4)\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("pause");
        }
    }
}

void System::registerAcc(string usbPath)
{
    string accountPath;
    if(!isUSBInserted(usbPath)){
        cout<<"Please insert your ATM Card to register an Account.\n";
        return;
    }
    if(AccountFound(usbPath, accountPath)){
        cout<<"ATM Card already have an Account. Cannot register another Account.\n";
        return;
    }

    Acc x;
    cout << "Input name: ";
    cin.ignore();
    getline(cin, x.name);

    cout << "Input Birth Date (MM/DD/YYYY): ";
    cin >> x.bday;
    cout << "Input Contact Number: ";
    cin >> x.contact;
    cout << "Input Pin: ";
    cin >> x.pinCode;
    pinChecker(x.pinCode);

    x.encryptedPin = decryptEncrypt(x.pinCode, adam);

    x.accNum = std::to_string(createAccNumber());

    double deposit = initialDeposit();

    if (deposit == -1)
    {
        cout << "Unsuccessful Registration\n";
        system("pause");
        return;
    }
    x.balance = deposit;
    
    std::ofstream accountFile(accountPath);
    accountFile << x.name << '\n'
                << x.bday << '\n'
                << x.contact << '\n'
                << x.accNum << '\n'
                << decryptEncrypt(x.pinCode, adam) << '\n'
                << x.balance << '\n';
    accountFile.close();

    Node* p, * q, * newNode;
    p = q = head;

    newNode = new Node(x);

    while (p != NULL)
    {
        q = p;
        p = p->next;
    }

    if (p == head)
        head = newNode;
    else
    {
        q->next = newNode;
        newNode->next = p;
    }

    cout << "Successful Registration!\n";
    cout << "Your Account Number is: " << newNode->data.accNum << '\n';
    system("pause");

    locateAcc(x.accNum);
}

int System::createAccNumber()
{
    int uniqueAccNum;
    bool unique;

    do
    {
        unique = true;
        uniqueAccNum = 10000 + rand() % 90000;

        Node* p = head;
        while (p != NULL)
        {
            if (std::stoi(p->data.accNum) == uniqueAccNum) {
                unique = false;
                break;
            }
            p = p->next;
        }
    } while (!unique);

    return uniqueAccNum;
}

void System::enterAcc(string n, string pin, string accountPath)
{
    string usbPath;
    if(!isUSBInserted(usbPath)){
        cout<<"Please insert your ATM Card to register an Account.\n";
        return;
    }
    if(AccountFound(usbPath, accountPath)){
        cout<<"No Account found on this ATM Card. Please register first.\n";
        return;
    }
    std::ifstream accountFile(accountPath);
    Acc z;
    getline(accountFile, z.name);
    getline(accountFile, z.bday);
    getline(accountFile, z.contact);
    getline(accountFile, z.accNum);
    string encryptedPin;
    getline(accountFile, encryptedPin);
    z.encryptedPin = encryptedPin;
    accountFile >> z.balance;
    accountFile.close();


    Node* p, * q;
    p = q = head;
    while (p != NULL && n != p->data.accNum)
    {
        q = p;
        p = p->next;
    }

    if (p == NULL)
    {
        cout << "Account not found\n";
        system("pause");
        return;
    }

    int attempts = 0;
    while (attempts < 3)
    {
        if (p->data.pinCode == pin) {
            cout << "Account Login Successful\n";
            currentUser = p;
            locateAcc(n);
            machineMenu();
            return;
        }
        else
        {
            attempts++;
            cout << "Incorrect Pin. Attempt " << attempts << " of 3.\n";
            if (attempts < 3) {
                cout << "Please try again: ";
                cin >> pin;
            }
        }
    }

    if (attempts == 3)
    {
        cout << "Too many failed attempts. Returning to main menu.\n";
        system("pause");
        return;
    }
}

void System::showBalance() {
    cout << "Your current balance is: " << currentUser->data.balance << '\n';
}

void System::showAcc()
{
    system("cls");
    cout << "Your account number is: " << currentUser->data.accNum << '\n';
    cout << "Account name: " << currentUser->data.name << '\n';
    cout << "Birthdate: " << currentUser->data.bday << '\n';
    cout << "Contact number: " << currentUser->data.contact << '\n';
    system("pause");
}

void System::locateAcc(string x)
{
    Node* p, * q;
    p = q = head;

    while (p != NULL && p->data.accNum != x)
    {
        q = p;
        p = p->next;
    }
    if (p == NULL)
    {
        cout << "Account not found.\n";
        system("pause");
    }

    else
    {
        system("cls");
        cout << "Your account number is: " << p->data.accNum << '\n';
        cout << "Account name: " << p->data.name << '\n';
        cout << "Birthdate: " << p->data.bday << '\n';
        cout << "Contact number: " << p->data.contact << '\n';
        cout << "Pin code: " << p->data.pinCode << '\n';

        system("pause");
    }
}

void System::withdraw()
{
    double amount = 0;
    cout << "Enter amount to withdraw: \n";
    cin >> amount;

    if (amount > currentUser->data.balance || amount <= 0)
    {
        cout << "Invalid amount\n";
        return;
    }
    else
    {
        currentUser->data.balance -= amount;
        cout << "You have successfully withdrawn: " << amount << '\n';
    }
    saveToUSB();
}

void System::deposit()
 {
    double amount = 0;
    cout << "Enter amount to deposit: \n";
    cin >> amount;

    if (amount <= 0)
    {
        cout << "Invalid amount\n";
        return;
    }
    else
    {
        currentUser->data.balance += amount;
    }
    saveToUSB();
}

int System::initialDeposit()
{
    double amount = 0;
    do
    {
        cout << "Initial Deposit for Account Registration (Min. ₱5000)\n";
        cout << "(Enter '1' to cancel)\n";
        cout << "-> ";
        cin >> amount;

        if (amount == 1)
        {
            return -1;
        }
        else if (amount < 5000)
        {
            cout << "You must deposit minimum of ₱5000!\n";
            system("pause");
            system("cls");
        }
    } while (amount < 5000);

    if (amount >= 5000)
    {
        cout << "Successful Deposit!\n";
        system("pause");
        system("cls");
        return amount;
    }
    return 0;
}

void System::fundTransfer()
{
    string receiver;
    double amount;

    system("cls");
    cout << "Fund Transfer\n";
    cout << "Enter the recipient's Account Number: ";
    cin >> receiver;

    Node *receiverNode = head;
    while (receiverNode != NULL && receiverNode->data.accNum != receiver)
    {
        receiverNode = receiverNode->next;
    }
    if (receiverNode == NULL)
    {
        cout << "Recipient account not found.\n";
        system("pause");
        return;
    }
    cout << "Enter the amount to transfer: ";
    cin >> amount;

    if (amount <= 0)
    {
        cout << "Invalid amount.\n";
        system("pause");
        return;
    }
    if (amount > currentUser->data.balance)
    {
        cout << "Insufficient funds. Your current balance is: " << currentUser->data.balance << '\n';
        system("pause");
        return;
    }
    currentUser->data.balance -= amount;
    receiverNode->data.balance += amount;

    cout << "Successfully transferred " << amount << " to account number " << receiver << ".\n";
    cout << "Your new balance is: " << currentUser->data.balance << '\n';
    storeAcc();
    saveToUSB();
    system("pause");
}

void System::changeInfo()
{
    int op;
    string newInfo;

    while (true)
    {
        system("cls");

        cout << "Update Information\n";
        cout << "1 - Name\n";
        cout << "2 - Birthdate\n";
        cout << "3 - Contact Number\n";
        cout << "4 - Exit\n";
        cout << "-> ";
        cin >> op;

        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (op)
        {

        case 1:
            system("cls");
            cout << "Update Current Name(Input 'q' to cancel)\n";
            cout << currentUser->data.name << '\n';
            cout << "-> ";
            cin >> newInfo;
            if (newInfo == "q")
            {
                return;
            }
            currentUser->data.name = newInfo;
            system("cls");
            cout << "Updated name: " << currentUser->data.name;
            break;

        case 2:
            cout << "Update Current Birthdate(Input 'q' to cancel)\n";
            cout << currentUser->data.bday << '\n';
            cout << "-> ";
            cin >> newInfo;
            if (newInfo == "q")
            {
                return;
            }
            currentUser->data.bday = newInfo;
            system("cls");
            cout << "Updated Birthdate: " << currentUser->data.bday;
            break;

        case 3:
            cout << "Update Current Contact Number(Input 'q' to cancel)\n";
            cout << currentUser->data.contact << '\n';
            cout << "-> ";
            cin >> newInfo;
            if (newInfo == "q")
            {
                return;
            }
            currentUser->data.contact = newInfo;
            system("cls");
            cout << "Updated contact number: " << currentUser->data.contact;
            break;

        case 4:
            return;

        default:
            cout << "Enter only (1-4)\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("pause");

    cout << "Account information updated successfully!\n";
    saveToUSB();
           }
    }
}

void System::pinChecker(string &pin)
{
    while (pin.length() < 4 || pin.length() > 6)
    {
        cout << "Please input a pin code with 4 to 6 digits: ";
        cin >> pin;
    }
}

void System::changePin()
{
    string oldPin, newPin;

    do
    {
        system("cls");
        cout << "Enter Current Pin\n";
        cout << "(Input 'q' to cancel)\n";
        cout << "-> ";
        cin >> oldPin;

        if (oldPin == "q")
        {
            return;
        }

        if (oldPin != currentUser->data.pinCode)
        {
            cout << "Incorrect Pin\n";
            system("pause");
        }

        else
        {
            while (true)
            {
                system("cls");
                cout << "Enter New Pin\n";
                cout << "(Input 'q' to cancel)\n";
                cout << "-> ";
                cin >> newPin;
                pinChecker(newPin);

                if (newPin == "q")
                {
                    return;
                }
                else if (newPin == currentUser->data.pinCode)
                {
                    cout << "That is already your pin\n";
                    system("pause");
                    return;
                }
                else
                {
                    currentUser->data.pinCode = newPin;
                    cout << "Pin Successfully Changed!\n";

                    system("pause");
                    system("cls");

                    cout << "This is your current Pin: " << currentUser->data.pinCode << '\n';
                    cout << "Do not share to others!!\n";
                    system("pause");
                    return;
                }
            }
        }
    } while (oldPin != "q" || newPin != "q");
    saveToUSB();
}

int registerMenu()
{
    int op;

    system("cls");

    cout << "Menu\n";
    cout << "1 - Register Account\n";
    cout << "2 - Enter Account\n";
    cout << "3 - Exit\n";
    cout << "-> ";

    cin >> op;
    return op;
}
void System::storeAcc()
{
    Node *p = head;
    std::ofstream file("pinCode.txt");

    while (p != NULL)
    {
        file << p->data.name << '\n'
             << p->data.bday << '\n'
             << p->data.contact << '\n'
             << p->data.accNum << '\n'
             << p->data.balance << '\n'
             << p->data.encryptedPin << '\n';
        p = p->next;
    }

    file.close();
}

void System::loadAcc()
{
    std::ifstream file("pinCode.txt");
    Acc d;

    while (getline(file, d.name) &&
           getline(file, d.bday) &&
           getline(file, d.contact) &&
           getline(file, d.accNum) &&
           file >> d.balance)
    {
        file.ignore();
        getline(file, d.encryptedPin);

        d.pinCode = decryptEncrypt(d.encryptedPin, adam);

        Node *p = new Node(d);
        p->next = head;
        head = p;
    }

    file.close();
}

string System::decryptEncrypt(string pin, string adam)
{
    string result = pin;
    for (size_t i = 0; i < pin.length(); i++)
    {
        result[i] = pin[i] ^ adam[i % adam.length()];
    }
    return result;
}

bool System::isUSBInserted(string usbPath)
{
    for (char drive = 'D'; drive <= 'Z'; ++drive) {//checks for common usb letters (d,e,f,g,h,etc.)
        string drivePath(1, drive);
        drivePath += ":\\";

        if (std::filesystem::is_directory(drivePath)) {
            usbPath = drivePath; //sets usbPath if usb detected
            return true; //if usb found
        }
    }
    return false; //if no usb found
}

bool System::AccountFound(string usbPath,string accountPath)
{
    accountPath = usbPath + "account.txt";
    return std::filesystem::exists(accountPath);
}

void System::ATMCARD()
{
    string usbPath;
    string accountPath;
    system("cls");
    cout << "Please insert your ATM Card to proceed.\n";
    while (!isUSBInserted(usbPath)) {
        cout << "No ATM Card detected. Please insert your ATM Card.\n";
        system("pause");
        system("cls");
    }
    cout << "ATM Card detected!\n";
    if (AccountFound(usbPath, accountPath))
    {
        enterAcc(accountPath);
    } else
    {
        cout << "No account found on ATM Card. Registering a new account.\n";
        registerAcc(usbPath);
    }
    mainMenu();
}

int main() {
{

    System atm;
    atm.loadAcc();
    atm.insertATM();
    string num, pin;
    while (true)
    {

        switch (registerMenu())
        {
        case 1:
            system("cls");
            atm.registerAcc();
            atm.storeAcc();
            break;

        case 2:
            system("cls");
            cout << "Enter Account Number: ";
            cin >> num;
            cout << "Enter Pin: ";
            cin >> pin;
            atm.enterAcc(num, pin);
            break;

        case 3:
            system("cls");
            cout << "Thank You! and Goodbye!";
            atm.storeAcc();
            exit(0);

        default:
            cout << "Invalid input.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("pause");
        }
    }

    return 0;
}
