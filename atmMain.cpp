#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <ctime>
#include <time.h>
#include <set>

using std::cout;
using std::cin;
using std::string;
using std::getline;

struct Acc{
    string name;
    string bday;
    string contact;
    string accNum;
    string pinCode;
    double balance = 5000;
};

struct Node{
    Acc data;
    Node* next;
    Node(Acc x) : data(x), next(NULL){}
};

class System{
    private:
        Node* head;
        Node* currentUser;
        int createAccNumber();
        std::set<int> generatedAccNumbers;
    public:
        System() : head(NULL), currentUser(NULL){}
        void registerAcc();
        void enterAcc(string n, string p);
        void checkRegister();
        void showBalance();
        int withdraw();
        void deposit();
        void fundTransfer(int x);
        void changePin();
        void showAcc(string x);
        void machineMenu();

};

int mainMenu(){
    int op;

    system("cls");

    cout<<"Menu\n";
    cout<<"1. Deposit\n";
    cout<<"2. ---\n";
    cout<<"3. ---\n"; 
    cout<<"4. ---\n";
    cout<<"5. ---\n";
    cout<<"6. ---\n"; 
    cout<<"7. Exit\n";
    cout<<"Select(1-7)\n ";
    cout<<"-> ";

    cin >> op;
    return op;
}


int registerMenu(){
    int op;

    system("cls");

    cout<<"Menu\n";
    cout<<"1. Not Register Acc\n";
    cout<<"2. Open Acc\n";
    cout<<"-> ";

    cin >> op;
    return op;
}

void System::registerAcc(){

Acc x;
cout<<"Input name: ";
cin.ignore();
getline(cin, x.name);

cout <<"Input Birth Date(MM/DD/YYYY): "; cin >> x.bday;
cout <<"Input Contact Number: "; cin >> x.contact;
cout <<"Input Pin: "; cin >> x.pinCode;
x.accNum = std::to_string(createAccNumber());

Node* p, *q, *newNode;
p = q = head;

newNode = new Node(x);

while(p != NULL){
    q = p;
    p = p->next;
}

if(p == head)
    head = newNode;
else{
    q->next = newNode;
    newNode->next = p;
    }

showAcc(x.accNum);

}

int System::createAccNumber(){
    int accNum;
    srand(time(0));

    do{
        accNum = rand() % 90000 + 10000;
    }while(generatedAccNumbers.count(accNum));

    generatedAccNumbers.insert(accNum);
    return accNum;
}

void System::enterAcc(string n, string pin){
Node* p, *q;
p = q = head;
while(p != NULL && n != p->data.accNum && pin !=p->data.pinCode){
        q = p;
        p = p->next;
    }

if(p == NULL){
    cout << "Unsuccessful. PLS TRY AGAIN";
    return;
    }
else{
    cout<<"Account Login Successful";
    currentUser = p;
    showAcc(n);
}

}


void System::showBalance(){
    cout << "Your balance is: "<<currentUser->data.balance;
}

void System::showAcc(string x){
Node *p, *q;
p = q = head;

while (p != NULL && p->data.accNum != x){
    q = p;
    p = p->next;
  }
if (p==NULL){
    cout<<"Acc not found.\n";
    system("pause");
}

else{
    system("cls");
    cout << "Your acc number is: " << p->data.accNum << '\n';
    cout << "Acc name: " << p->data.name << '\n';
    cout << "Bday: " << p->data.bday << '\n';
    cout << "Contact: " << p->data.contact << '\n';
    cout << "Pin code: " << p->data.pinCode << '\n';

    system("pause");
}

}

int System::withdraw(){
    return 0;
}

void System::deposit(){

    double amount = 0;
    cout<<"Enter amount to deposit: \n";
    cin>>amount;

    if(amount < 0){
        cout <<"insufficient amount\n";
    }
    else{
    currentUser->data.balance += amount; 
    
    }
}

void System::fundTransfer(int x){

}

void System::changePin(){

}

void System::machineMenu(){
    while(true){
        switch(mainMenu()){
            case 1: 
                    showBalance();
                    system("pause");
                    cout<<"Deposit\n";
                    deposit();
                    system("pause");
                    showBalance();
                    break;
            
            case 2: break;
            case 3: break;
            case 4: break;
            case 5: break;
            case 6: break;
            case 7: break;
            default:
            cout << "Enter only (1 - 7)";
        }
    }
} 

int main(){

System atm;
string num, pin;
    while(true){

        switch(registerMenu()){
        case 1:
            system("cls");
            atm.registerAcc();
            cout << "Successful Account Registration\n";
            system("pause");
            break;

        case 2:
            system("cls");
            cout<<"Enter Account Number: "; cin >> num;
            cout<<"Enter Pin: "; cin >> pin;
            atm.enterAcc(num, pin);
            atm.machineMenu();
            break;

        default:
            cout<<"Invalid input.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("pause");
        }
    }

    return 0;
}
