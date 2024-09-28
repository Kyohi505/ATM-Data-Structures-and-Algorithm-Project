#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <ctime>
#include <set>

using std::cout;
using std::cin;
using std::string;
using std::getline;

struct Acc{
    string name;
    string bday;
    string contact;// changed int to strings cuz erros and mas maganda ata kung strings
    string accNum;
    string pinCode;
    double balance;
};

struct Node{
    Acc data;
    Node* next;
    Node(Acc x) : data(x), next(NULL){}
};

class System{
    private:
        Node* head;
        //Node* currentUser; 
        int createAccNumber();
        std::set<int> generatedAccNumbers;
    public:
        System() : head(NULL){}//currentUser(NULL){}
        void registerAcc();
        void enterAcc(string n, string p);
        void checkRegister();
        void showBalance();
        int withdraw();
        void deposit(double x);
        void fundTransfer(int x);
        void changePin();
        void showAcc(string x);
        void machineMenu();

};

int registerMenu(){
    int op;

    system("cls");

    cout<<"Menu\n";
    cout<<"1. Register Account\n";
    cout<<"2. Open Acc\n";
    cout<<"-> ";

    cin >> op;
    return op;
}

int mainMenu(){ //options for withdraw, balance inquiry, deposity...etc
    int op;

    system("cls");

    cout<<"Menu\n";
    cout<<"1. ---\n";
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

void System::machineMenu(){

    while(true){

        switch(mainMenu()){
            case 1:break;
            case 2:break;
            case 3:break;
            case 4:break;
            case 5:break;
            case 6:break;
            case 7:break;
            default:
                cout << "Enter Only (1-7)\n";
        }
    }
}

void System::registerAcc(){

Acc x;
cout<<"Input name: ";
cin.ignore();
getline(cin, x.name);

cout <<"Input Birth Date(MM/DD/YY): "; cin >> x.bday;
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
    //p = currentUser;
    showAcc(n);
}

}


void System::showBalance(){

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

void System::deposit(double x){

}

void System::fundTransfer(int x){

}

void System::changePin(){

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
            break;

        default:
            cout<<"Invalid input.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//input error handling para if aksidente ma input anything other than numbers
            system("pause");
        }
    }

    return 0;
}
