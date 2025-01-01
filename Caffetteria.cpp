#include <bits/stdc++.h>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
#include <cstdlib>
#include <string>
#include <sstream>
#include <regex>
#include <ctime>
#include <stdexcept>
using namespace std;

//Ionescu Raul-Andrei, 321AC

/*
In acest program s-au folosit urmatoarele concepte: Clase și Obiecte: 2 ori , Constructori și Destructori: 1 ori,Încapsulare: 1 ori,
Moștenire: 0 ori, Polimorfism: 1 ori, Abstractizare: 1 ori, Templates: 0 ori, Exceptions: 3 ori, Design Patterns: 0 ori.

Pentru a vedea mai usor toate conceptele de OOP folosite in program am adaugat un comentariu cu 
conceptul folosit de fiecare data pentru a ajuta pe oricine analizeaza acest cod sa le gaseasca
mai usor. De exemplu: ( Încapsulare )
*/

//Functii ajutatoare
void enableUTF8() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);  // Ensure input also uses UTF-8 encoding
#endif
}



// Clasa Abstracta pentru angajati (Abstractizare)
class Employee {
//( Încapsulare )
protected:
    string fname;       // First name
    string lname;       // Last name
    int hourly_rate;    // Hourly rate
    int start_hour;     // Start hour of shift
    int end_hour;       // End hour of shift
    int salary;         //Salary
    string city;        //City
    string role;        //Role    

public:
    virtual void setData() = 0; // Pure virtual function

    int getSalary(){
        return (this->end_hour - this->start_hour) * this->hourly_rate;
    }

    virtual void getAction() = 0; //Functie specifica pentru fiecare clasa care mosteneste Employee , in functie de rolul pe care il poate avea. 

    virtual ~Employee() {} //( Destructor )
};


class Admin : public Employee {
//( Încapsulare )
public:
    void setData() override { // (Polimorfism)

        enableUTF8();

        cout << "Enter first name: ";
        cin >> fname;
        cout << "Enter last name: ";
        cin >> lname;
        cout << "Enter hourly rate: ";
        cin >> hourly_rate;
        cout << "Enter start hour: ";
        cin >> start_hour;
        cout << "Enter end hour: ";
        cin >> end_hour;
        cout<<"Enter the city: ";
        cin>>city;
        cout<<"You can give the following roles: "<<endl;
        cout<<"1.Barista"<<endl;
        cout<<"2.Waiter/Ospătar"<<endl;
        cout<<"3.Manager"<<endl;
        cout<<"Which role do you want to give: ";

        while(true){    
            try{
                char pos;
                cin>>pos;

                if(pos == '1'){
                    role = "barista";
                    break;
                }
                else if(pos == '2'){
                    role = "ospătar";
                    break;
                }
                else if(pos == '3'){
                    role = "manager";
                    break;
                }
                else
                    throw runtime_error("Invalid option.");               
            }
            catch(const runtime_error& e){
                cout << e.what() << " Please try again." << endl;
            }
        }

        
        try { //(Exceptions )
            ofstream file("CSV_files/angajati.csv", ios::app);  

        
            if (!file.is_open()) {
                throw runtime_error("Error: Unable to open file for writing.");
            }

        
            file << fname << "," << lname << "," << hourly_rate << " lei," 
                << start_hour << ":00," << end_hour << ":00," 
                << getSalary() << " lei," 
                << city << ","<<role<<",";

        
            file.close();

            cout << "Data saved to angajati.csv successfully!" << endl;

        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
        }
    }
};



//Clasa Manager mosteneste clasa Employee (Moștenire)
class Manager : public Employee {
//( Încapsulare )
public:
    void setData() override { // (Polimorfism)

        enableUTF8();

        cout << "Enter first name: ";
        cin >> fname;
        cout << "Enter last name: ";
        cin >> lname;
        cout << "Enter hourly rate: ";
        cin >> hourly_rate;
        cout << "Enter start hour: ";
        cin >> start_hour;
        cout << "Enter end hour: ";
        cin >> end_hour;
        cout<<"Enter the city: ";
        cin>>city;
        cout<<"You can give the following roles: "<<endl;
        cout<<"1.Barista"<<endl;
        cout<<"2.Waiter/Ospătar"<<endl;
        cout<<"Which role do you want to give: ";

        while(true){    
            try{ //( Exceptions )
                char pos;
                cin>>pos;

                if(pos == '1'){
                    role = "barista";
                    break;
                }
                else if(pos == '2'){
                    role = "ospătar";
                    break;
                }
                else
                    throw runtime_error("Invalid option.");               
            }
            catch(const runtime_error& e){
                cout << e.what() << " Please try again." << endl;
            }
        }

        
        try { //(Exceptions)
            ofstream file("CSV_files/angajati.csv", ios::app);  

        
            if (!file.is_open()) {
                throw runtime_error("Error: Unable to open file for writing.");
            }

        
            file << fname << "," << lname << "," << hourly_rate << " lei," 
                << start_hour << ":00," << end_hour << ":00," 
                << getSalary() << " lei," 
                << city << ","<<role<<",";

        
            file.close();

            cout << "Data saved to angajati.csv successfully!" << endl;

        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
        }
    }
};

class Waiter:public Employee{

};



//Interfata platformei de lucru atat pentru angajati dar si pentru clienti (ca un fel de site) facuta in 2 limbi
void generateMenu(){

    enableUTF8();

    char lan;
    
    cout<<"Welcome to Royal Coffe Shop"<<endl;
    restart:    
        cout<<"Choose one of the languages below: "<<endl;
        cout<<"1.English"<<endl;
        cout<<"2.Română"<<endl;

    cin>>lan;

    while(true){
        int day = 1;
        if(lan == '1'){
        start:    
            char ch;

            cout<<"Please choose one of the operations below: "<<endl;
            cout<<"1.Operate as an ADMIN"<<endl; /* Aceasta este o functie EXTRA adaugata de mine facuta pentru a adauga orice 
                                                    persoana ca si angajat dar arata si date despre continutul tuturor fisierelor CSV*/
            /*Am folosit rolul de ADMIN pentru a pune persoane in orice rol (manager, barista, ospatari), e ca un fel de patron 
            al restaurantului care poate sa faca ce doreste*/                                                    
            cout<<"2.Operate as an EMPLOYEE"<<endl;
            cout<<"3.Operate as a CUSTOMER"<<endl;
            cin>>ch;

            if(ch == '1'){
                cout<<"Welcome ADMIN!"<<endl;

                while(true){
                    
                    int choice;
                    cout<<"You can make the following actions: "<<endl;
                    cout<<"1.Hire a person"<<endl;
                    cout<<"2.Inspect one of the database"<<endl;
                    cout<<"3.See the financial report for the current day"<<endl;
                    cout<<"4.Exit/ Go to next day"<<endl;

                    cin>>choice;

                    switch(choice){
                        case 1:{
                            break;
                        }
                        case 2:{
                            break;
                        }
                        case 3:{
                            break;
                        }
                        case 4:{
                            day++;
                            goto start;
                            break;
                        }
                        default:
                            cout << "Please introduce one of the options above." << endl;
                            break;
                        
                    }
                    /*Manager man; // ( Clase și Obiecte )
                    man.setData();
                    ofstream file("CSV_files/angajati.csv", ios::app);  

                    if (!file.is_open()) {
                        throw runtime_error("Error: Unable to open file for writing.");
                    }

                    file<<day<<endl;*/
                }
            }
            else if(ch == '2'){
                cout<<"Welcome EMPLOYEE!"<<endl;
            }
            else if(ch == '3'){
                cout<<"Welcome Customer!"<<endl;
            }
            else{
                char ans;
                cout<<"Invalid option! Do you want to continue? Y/N"<<endl;

                cin>>ans;

                if(ans == 'y' || ans == 'Y'){
                    day++;
                    cout<<"See you next day( "<<day<<" )!"<<endl;
                }

                if(ans == 'n' || ans == 'N'){
                    cout<<"Bye!"<<endl;
                    break;
                }
            }
        }
        /*else if(lan == '2'){

            getchar();
            cout<<"Alegeți o operație din cele de mai jos: "<<endl;
            cout<<"1.Rol de ADMINISTARTOR"<<endl; // Aceasta este o functie EXTRA adaugata de mine unde arata centralizat date despre continutul fisierelor
            cout<<"2.Rol de ANGAJAT"<<endl;
            cout<<"3.Rol de CLIENT"<<endl;

            char choice;

            cin>>choice;

            if(choice == '1'){
                cout<<"Bun venit ADMIN!"<<endl;
            }
            else if(choice == '2'){
                cout<<"Bun venit ANGAJAT!"<<endl;
            }
            else if(choice == '3'){
                cout<<"Bun venit CLIENT!"<<endl;
            }
            else{
                char ans;
                cout<<"Opțiune invalidă! Doriți să continuați D/N"<<endl;

                cin>>ans;

                if(ans == 'n' || ans == 'N'){
                    cout<<"La revedere!"<<endl;
                    break;
                }
            }

        }*/
        else{
            cout<<"Please choose a valid language"<<endl;
            goto restart;
        }
    }
}

int main(void){

    enableUTF8();

    /*Manager manager;
    manager.setData();
    */

    generateMenu();


    return 0;
}
