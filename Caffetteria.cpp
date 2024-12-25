//Ionescu Raul-Andrei, 321AC

#include <bits/stdc++.h>
#include <vector>
#include <sstream>
#include <regex>
#include <ctime>
#include <stdexcept>
using namespace std;





//Interfata platformei de lucru atat pentru angajati dar si pentru clienti (ca un fel de site) facuta in 2 limbi
void generateMenu(){

    char lan;
    
    cout<<"Welcome to Royal Coffe Shop"<<endl;

restart:    
    cout<<"Choose one of the languages below: "<<endl;
    cout<<"1.English"<<endl;
    cout<<"2.Română"<<endl;

    cin>>lan;

    while(true){
        if(lan == '1'){
            getchar();
            char choice;
            cout<<"Please choose one of the operations below: "<<endl;
            cout<<"1.Operate as an ADMIN"<<endl; // Aceasta este o functie EXTRA adaugata de mine unde arata centralizat date despre continutul fisierelor
            cout<<"2.Operate as an EMPLOYEE"<<endl;
            cout<<"3.Operate as a CUSTOMER"<<endl;
            cin>>choice;

            if(choice == '1'){
                cout<<"Welcome ADMIN!"<<endl;
            }
            else if(choice == '2'){
                cout<<"Welcome EMPLOYEE!"<<endl;
            }
            else if(choice == '3'){
                cout<<"Welcome Customer!"<<endl;
            }
            else{
                char ans;
                cout<<"Invalid option! Do you want to continue? Y/N"<<endl;

                cin>>ans;

                if(ans == 'n' || ans == 'N'){
                    cout<<"Bye!"<<endl;
                    break;
                }
            }
        }
        else if(lan == '2'){

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

        }
        else{
            cout<<"Please choose a valid language"<<endl;
            goto restart;
        }
    }
    
}

int main(void){

    generateMenu();


    return 0;
}