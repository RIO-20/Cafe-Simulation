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
In acest program s-au folosit urmatoarele concepte: Clase și Obiecte: 2 ori , Constructori și Destructori: 1 ori,Încapsulare: 1 ori, Moștenire: 4 ori, Polimorfism: 1 ori, Abstractizare: 1 ori, Templates: 0 ori, Exceptions: 3 ori, Design Patterns: 1 ori.

Pentru a vedea mai usor toate conceptele de OOP folosite in program am adaugat un comentariu cu 
conceptul folosit de fiecare data pentru a ajuta pe oricine analizeaza acest cod sa le gaseasca
mai usor. De exemplu: ( Încapsulare )
*/

//Functii ajutatoare
void enableUTF8() { //Functie care face posibila afisarea si citirea caracterelor care contin diacritice
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);  
#endif
}

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string item;
    while (getline(ss, item, delimiter)) {
        tokens.push_back(item);
    }
    return tokens;
}

//Transforma string-urile care au Uppercase in Lowercase
string toLowercase(const string& input) {
    string result = input;
    transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return tolower(c); });
    return result;
}

//Functie care se ocupa cu stocarea tuturor produselor din meniu intr-un vector de string-uri
vector<string> getProductNames(const string& csv) {
    vector<string> productNames;
    vector<string> lines = split(csv, '\n');

  
    for (size_t i = 1; i < lines.size(); ++i) {
        vector<string> fields = split(lines[i], ',');
        if (!fields.empty()) {
            productNames.push_back(toLowercase(fields[0]));
        }
    }
    return productNames;
}

//( Design Pattern - Abstract Factory )
class Beverage{
    public:
        virtual void prepare() = 0;

        Beverage(){ cout<<"Beverage created"<<endl; } // ( Constructor )
        virtual ~Beverage() {}; // ( Destructor )
};

class Drink: public Beverage{

    void prepare() override {
        cout<<"Preparing the drink"<<endl;
    }
        
};

class Snack: public Beverage{

    void prepare() override {
        cout<<"Preparing the snack"<<endl;
    }

};

class BeverageFactory{
    public:
        virtual Beverage* createDrink() = 0;
        virtual Beverage* createSnack() = 0;
        virtual ~BeverageFactory() = default;
};

class DrinkFactory: public BeverageFactory{

    Beverage* createDrink() override{
        return new Drink();
    }

    Beverage* createSnack(){
        return nullptr;
    }
};


class SnackFactory: public BeverageFactory{

    Beverage* createDrink() override{
       return nullptr;
    }

    Beverage* createSnack(){
        return new Snack();
    }
};

string checkBeverage(BeverageFactory* factory){
    
    Beverage* drink = factory->createDrink();
    Beverage* snack = factory->createSnack();

    if(drink && snack)
        return "menu";
    else if(drink)
        return "drink";
    else if(snack)
        return "snack";
    else
        return "none";

}



// Clasa Abstracta pentru angajati (Abstractizare)
class Employee { 
//( Încapsulare )
protected:
    string fname;       // First name
    string lname;       // Last name
    int hourly_rate;    // Hourly rate
    int start_hour;     // Start hour of shifts
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


class Admin : public Employee { // ( Moștenire )
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

    void getAction() override { // ( Polimorfism )
        string firstName, lastName, city, role;
        int day;
        
        cout << "Enter First Name: ";
        cin >> firstName;
        cout << "Enter Last Name: ";
        cin >> lastName;
        cout << "Enter City: ";
        cin >> city;
        
        while(true){
            cout << "Enter Role: ";
            cin >> role;

            
            if(role != "manager" && role !="barista" && role != "ospătar")
                cout<<"This role doesn't exist!"<<endl;
            else
                break;
        }

        cout << "Enter Day: ";
        cin >> day;

        
        string inputFile = "CSV_files/angajati.csv";
        string tempFile = "CSV_files/aux.csv";

        
        ifstream inFile(inputFile);
        ofstream outFile(tempFile);

        if (!inFile.is_open() || !outFile.is_open()) {
            cerr << "Error opening file." << endl;
            return;
        }

        string line;
        bool found = false;

        
        while (getline(inFile, line)) {
            stringstream ss(line);
            vector<string> row;
            string cell;

            
            while (getline(ss, cell, ',')) {
                row.push_back(cell);
            }

            
            if (row.size() == 9 && 
                row[0] == firstName && 
                row[1] == lastName && 
                row[6] == city && 
                row[7] == role && 
                stoi(row[8]) == day) {
                found = true;
                continue; 
            }

        
            outFile << line << "\n";
        }

        inFile.close();
        outFile.close();

        if (found) {
            
            if (remove(inputFile.c_str()) != 0) {
                cerr << "Error deleting original file." << endl;
            } else if (rename(tempFile.c_str(), inputFile.c_str()) != 0) {
                cerr << "Error renaming temporary file." << endl;
            } else {
                cout << "Record deleted and rows updated successfully." << endl;
            }
        } else {
            cerr << "Record not found." << endl;
            remove(tempFile.c_str()); 
        }
    }

    void displayAll(const string& filePath){
        ifstream inFile(filePath);

        
        if(!inFile.is_open()){
            cerr<<"Error opening file: "<<filePath<<endl;
        }

        string line;
        int row_num = 0;

        while(getline(inFile,line)){
            row_num++;
            stringstream ss(line);
            vector<string> row;
            string elem;

            while(getline(ss,elem,','))
                row.push_back(elem);

            for(int i = 0 ; i < row.size(); ++i){
                cout<<row[i];

                if(i < row.size() - 1)
                    cout<<" | ";
            }

            cout<<endl;

        }
    }
};




class Manager : public Employee { // ( Moștenire )
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

    //Aceasta functie scoate angajatii din sistem.
    void getAction() override { // ( Polimorfism )
        string firstName, lastName, city, role;
        int day;
        
        cout << "Enter First Name: ";
        cin >> firstName;
        cout << "Enter Last Name: ";
        cin >> lastName;
        cout << "Enter City: ";
        cin >> city;
        
        while(true){
            cout << "Enter Role: ";
            cin >> role;

            if(role == "manager")
                cout<<"You are not allowed to remove this person"<<endl;
            else if(role != "manager" && role !="barista" && role != "ospătar")
                cout<<"This role doesn't exist!"<<endl;
            else
                break;
        }

        cout << "Enter Day: ";
        cin >> day;

        
        string inputFile = "CSV_files/angajati.csv";
        string tempFile = "CSV_files/aux.csv";

        
        ifstream inFile(inputFile);
        ofstream outFile(tempFile);

        if (!inFile.is_open() || !outFile.is_open()) {
            cerr << "Error opening file." << endl;
            return;
        }

        string line;
        bool found = false;

        
        while (getline(inFile, line)) {
            stringstream ss(line);
            vector<string> row;
            string cell;

            
            while (getline(ss, cell, ',')) {
                row.push_back(cell);
            }

            
            if (row.size() == 9 && 
                row[0] == firstName && 
                row[1] == lastName && 
                row[6] == city && 
                row[7] == role && 
                stoi(row[8]) == day) {
                found = true;
                continue; 
            }

        
            outFile << line << "\n";
        }

        inFile.close();
        outFile.close();

        if (found) {
            
            if (remove(inputFile.c_str()) != 0) {
                cerr << "Error deleting original file." << endl;
            } else if (rename(tempFile.c_str(), inputFile.c_str()) != 0) {
                cerr << "Error renaming temporary file." << endl;
            } else {
                cout << "Record deleted and rows updated successfully." << endl;
            }
        } else {
            cerr << "Record not found." << endl;
            remove(tempFile.c_str()); 
        }
    }

    void getIngridient(int day){
        enableUTF8();
        string name,type,city;
        int quantity;
        double price;
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout<<"Introduce the product name: ";
        getline(cin, name);
        cout<<"Introduce the product type: ";
        getline(cin, type);
        cout<<"Introduce the product quantity: ";

        while(true){
            try{
                cin>>quantity;
                if(quantity <= 0)
                    throw runtime_error("Invalid quantity.");
                break;
            }
            catch(const runtime_error& e){
                cout << e.what() << " Please try again." << endl;
            }
        }

        cout<<"Introduce the price of the product: ";
        cin>>price;

        cout<<"Introduce the city where do you want to receive: ";
        cin>>city;

        ofstream file;
        file.open("CSV_files/ingrediente.csv", ios::app);

         file << name << ","
         << quantity << ","
         << price << ","
         << city << ","
         << day << "\n";
        
        file.close();

    }
};

class Barista:public Employee{ // ( Moștenire )

    void getAction() override{
       
    }

};

/*class Waiter:public Employee{ // ( Moștenire )



};*/


class Customer{
    protected:
        string first_name;
        string last_name;
        string order;
        float total_price;
        string city;
        vector<pair<string, float>> products;
    
    public:

        void getOrder(int day){

            cout<<"Enter First Name: ";
            cin>>first_name;
            cout<<"Enter Last Name:";
            cin>>last_name;

            vector<string> selectedProducts;
            vector<float> productPrices;
            string product;

            cout << "Enter products to order (type 'done' to finish):" << endl;
            cin.ignore(); 

            while (true) {
                cout << "Product: ";
                getline(cin, product);

                if (product == "done") {
                    break;
                }

                bool found = false;
                for (const auto& p : products) {
                    if (p.first == product) {
                        selectedProducts.push_back(p.first);
                        productPrices.push_back(p.second);
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    cout << "Invalid product. Please try again." << endl;
                }
            }

            order = buildOrderString(selectedProducts);
            total_price = calculateTotalPrice(productPrices);  

            cout<<"Introduce the city: ";
            cin>>city;

            bool is_loyal = false;      

            ofstream file;
            file.open("CSV_files/comenzi.csv", ios::app);

            file << first_name << ","
            << last_name << ","
            << order << ","
            << total_price << ","
            << city << ","
            <<day<<","
            <<is_loyal<<"\n";

            file.close();
        }

        void getDataMenu() {
            const string& filePath = "CSV_files/menu.csv";
            ifstream file(filePath);
            if (!file.is_open()) {
                cerr << "Error: Unable to open file " << filePath << endl;
            return;
            }

            string line;
            getline(file, line);

      
            while (getline(file, line)) {
                stringstream ss(line);
                string productName, priceString;
                float price;

                if (getline(ss, productName, ',') && getline(ss, priceString)) {
                    try {
                        price = stof(priceString);
                        products.emplace_back(productName, price);
                    } catch (const invalid_argument&) {
                        cerr << "Invalid price format in line: " << line << endl;
                    }
                }
            }

            file.close();
        }


        void showMenu(){
            const string& filePath = "CSV_files/menu.csv";
            ifstream inFile(filePath);

        
            if(!inFile.is_open()){
                cerr<<"Error opening file: "<<filePath<<endl;
            }

            string line;
            int row_num = 0;

            while(getline(inFile,line)){
                row_num++;
                stringstream ss(line);
                vector<string> row;
                string elem;

                while(getline(ss,elem,','))
                    row.push_back(elem);

                for(int i = 0 ; i < row.size(); ++i){
                    cout<<row[i];

                    if(i < row.size() - 1)
                        cout<<" | ";
                }

                cout<<endl;

            }
        }

    private:
        string buildOrderString(const vector<string>& products) {
            if (products.empty()) return "";

            string result = products[0];
            for (size_t i = 1; i < products.size(); ++i) {
                result += " & " + products[i];
            }
            return result;
        }

        float calculateTotalPrice(const vector<float>& prices) {
            float total = 0.0f;
            for (float price : prices) {
                total += price;
            }
            return total;
        }
};

//Interfata platformei de lucru atat pentru angajati dar si pentru clienti (ca un fel de site)
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
                Admin adm;

                while(true){
                    
                    int choice;
                    cout<<"You can make the following actions: "<<endl;
                    cout<<"1.Hire a person"<<endl;
                    cout<<"2.Remove a person"<<endl;
                    cout<<"3.Inspect one of the database"<<endl;
                    cout<<"4.See the financial report for the current day"<<endl;
                    cout<<"5.Exit/ Go to next day"<<endl;

                    cin>>choice;

                    switch(choice){
                        case 1:{
                            adm.setData();
                            break;
                        }
                        case 2:{
                            adm.getAction();
                            break;
                        }
                        case 3:{
                            break;
                        }
                        case 4:{
                            break;
                        }
                        case 5:{
                            char opt;
                            cout<<"Do you want to go to the next day: Y/N? "<<endl;

                            cin>>opt;
                            if(opt == 'y' || opt == 'Y'){
                                day++;
                                goto start;
                            }
                            else
                                goto start;

                            break;
                        }
                        default:
                            cout << "Please introduce one of the options above." << endl;
                            break;
                        
                    }
                }
            }
            else if(ch == '2'){
                char choice;
                cout<<"Welcome EMPLOYEE!"<<endl;
                cout<<"Choose from the following roles: "<<endl;
                cout<<"1.Manager"<<endl;
                cout<<"2.Waiter"<<endl;
                cout<<"3.Barista"<<endl;

                cin>>choice;

                if(choice == '1'){
                    Manager man;
                    cout<<"Welcome Manger!"<<endl;
                    cout<<"You can make the following actions: "<<endl;
                    cout<<"1.Hire a person"<<endl;
                    cout<<"2.Remove a person"<<endl;
                    cout<<"3.Get the neccesary ingredients"<<endl;
                    cout<<"4.Organize special events"<<endl;
                    cout<<"5.Exit/ Go to next day"<<endl;
                    
                    int alg;
                    cin>>alg;

                     switch(alg){
                        case 1:{
                            man.setData();
                            break;
                        }
                        case 2:{
                            man.getAction();
                            break;
                        }
                        case 3:{
                            man.getIngridient(day);
                            break;
                        }
                        case 4:{
                            break;
                        }
                        case 5:{
                            char opt;
                            cout<<"Do you want to go to the next day: Y/N? "<<endl;

                            cin>>opt;
                            if(opt == 'y' || opt == 'Y'){
                                day++;
                                goto start;
                            }
                            else
                                goto start;

                            break;
                        }
                        default:
                            cout << "Please introduce one of the options above." << endl;
                            break;
                        
                    }
                }
                else if(choice == '2'){

                }
                else if(choice == '3'){

                }
                else{
                    cout<<"Invalid choice!"<<endl;
                }

            }
            else if(ch == '3'){
                Customer cust;
                int choice;
                cout<<"Welcome Customer!"<<endl;
                cout<<"You have the following options:"<<endl;
                cout<<"1.Order"<<endl;
                cout<<"2.See the menu"<<endl;

                cin>>choice;

                if(choice == 1){
                    cust.getDataMenu();
                    cust.getOrder(day);
                }
                else if(choice == 2){
                    cust.showMenu();
                }
                else
                    cout<<"Invalid choice!"<<endl;
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
    generateMenu();


    return 0;
}
