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
#include <unordered_map>
#include <stdexcept>
using namespace std;


//Ionescu Raul-Andrei, 321AC

/*
In acest program s-au folosit urmatoarele concepte: Clase și Obiecte: 2 ori , Constructori și Destructori: 3 ori,Încapsulare: 3 ori, Moștenire: 4 ori, Polimorfism: 4 ori, Abstractizare: 1 ori, Templates: 1 ori, Exceptions: 3 ori, Design Patterns: 1 ori.

Pentru a vedea mai usor toate conceptele de OOP folosite in program am adaugat un comentariu cu 
conceptul folosit de fiecare data pentru a ajuta pe oricine analizeaza acest cod sa le gaseasca
mai usor. De exemplu: ( Încapsulare )
*/

     
//Functii ajutatoare
void enableUTF8() {        //Functie care face posibila afisarea si citirea caracterelor care contin diacritice
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

map<string, float> loadProductsFromCSV(const string& filename) {
    map<string, float> products;
    ifstream file(filename);
    string line;

    // Check if the file was successfully opened
    if (!file.is_open()) {
        cerr << "Error: Could not open the file " << filename << endl;
        return products;
    }

    // Skip the header row (optional, if your CSV has one)
    getline(file, line);

    // Read each line in the file
    while (getline(file, line)) {
        stringstream ss(line);
        string name;
        float price;

        // Read product name and price
        getline(ss, name, ',');  // Read product name, up to the comma
        ss >> price;  // Read the price (after the comma)
        
        // Add to the map
        products[name] = price;
    }

    file.close();
    return products;
}

void applyDiscountAndUpdateCSV(const string& inputFile, const string& outputFile, const string& city, int day) {
    ifstream file(inputFile);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << inputFile << endl;
        return;
    }

    string line;
    vector<string> lines;
    map<string, pair<int, float>> customerOrderInfo; // Order count & accumulated price for discounts
    map<string, int> loyaltyStatus;  // To store if the customer is loyal (0 - not loyal, 1 - loyal)

    // Skip header row
    getline(file, line);

    // Process the CSV and track customers' orders
    while (getline(file, line)) {
        stringstream ss(line);
        string firstName, lastName, order, cityInFile, status, isLoyalStr;
        float price;
        int currentDay;

        getline(ss, firstName, ',');
        getline(ss, lastName, ',');
        getline(ss, order, ',');
        ss >> price;
        ss.ignore(); // Ignore comma separating price and city
        getline(ss, cityInFile, ',');
        ss >> currentDay;
        ss.ignore(); // Ignore the comma separating day and status
        getline(ss, status, ',');
        getline(ss, isLoyalStr, ',');

        string customerKey = firstName + " " + lastName;

        // Only process orders for customers from the specified city
        if (cityInFile == city) {
            customerOrderInfo[customerKey].first++; // Increment order count
            customerOrderInfo[customerKey].second += price; // Add order price to the total

            // Apply discount on the current order
            int totalOrders = customerOrderInfo[customerKey].first;

            // Check for loyalty and apply discount
            if (totalOrders == 5 && loyaltyStatus[customerKey] != 1) {
                // Apply 5% discount for customers with 5 orders
                price *= 0.95;
                isLoyalStr = "1";
                loyaltyStatus[customerKey] = 1;
            } else if (totalOrders == 10 && loyaltyStatus[customerKey] != 2) {
                // Apply 10% discount for customers with 10 orders
                price *= 0.90;
                isLoyalStr = "2"; // Loyalty status for 10 orders
                loyaltyStatus[customerKey] = 2;
            }

            // Modify the price and store it back in the list
            lines.push_back(firstName + "," + lastName + "," + order + "," +
                            to_string(price) + "," + cityInFile + "," + to_string(currentDay) +
                            "," + status + "," + isLoyalStr);
        } else {
            lines.push_back(line); // If not the specified city, just copy the line
        }
    }

    file.close();

    // Now write the updated lines to the output file
    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cerr << "Error: Unable to open file " << outputFile << endl;
        return;
    }

    // Write header back to file
    outFile << "First Name,Last Name,Order,Price,City,Day,Status,Is_loyal\n";

    // Write the modified lines (with discounts applied if applicable)
    for (const auto& line : lines) {
        outFile << line << "\n";
    }

    outFile.close();

    // Replace original file with the updated file
    remove(inputFile.c_str());
    rename(outputFile.c_str(), inputFile.c_str());
}

//( Design Pattern - Abstract Factory )
class Beverage{ // ( Clasa )
    public:
        virtual void prepare() = 0;

        Beverage(){ cout<<"Beverage created"<<endl; } // ( Constructor )
        virtual ~Beverage() {}; // ( Destructor )
};

class Drink: public Beverage{ // ( Clasa )

    void prepare() override {
        cout<<"Preparing the drink"<<endl;
    }
        
};

class Snack: public Beverage{ // ( Clasa )

    void prepare() override {
        cout<<"Preparing the snack"<<endl;
    }

};

class BeverageFactory{ // ( Clasa )
    public:
        virtual Beverage* createDrink() = 0;
        virtual Beverage* createSnack() = 0;
        virtual ~BeverageFactory() = default;
};

class DrinkFactory: public BeverageFactory{ // ( Clasa )

    Beverage* createDrink() override{
        return new Drink();
    }

    Beverage* createSnack(){
        return nullptr;
    }
};


class SnackFactory: public BeverageFactory{ // ( Clasa )

    Beverage* createDrink() override{
       return nullptr;
    }

    Beverage* createSnack(){
        return new Snack();
    }
};

class MenuFactory : public BeverageFactory { // ( Clasa )
public:
    Beverage* createDrink() override {
        return new Drink(); 
    }

    Beverage* createSnack() override {
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

class FinancialReport {
private:
    unordered_map<string, unordered_map<int, float>> financial_report;

    // Helper function to clean up string and convert to float
    float stringToFloat(const string& value) {
        string cleaned = value;
        
        // Remove spaces
        cleaned.erase(remove(cleaned.begin(), cleaned.end(), ' '), cleaned.end());
        
        // Remove " lei" or any other non-numeric symbols
        if (cleaned.find("lei") != string::npos) {
            cleaned.replace(cleaned.find("lei"), 3, "");
        }
        
        try {
            return stof(cleaned);  // Try to convert the cleaned string to a float
        } catch (const invalid_argument& e) {
            cerr << "Invalid argument: Could not convert '" << cleaned << "' to a float." << endl;
            return 0.0;  // Default to 0 in case of an error
        }
    }

    // Helper function to convert string to integer
    int stringToInt(const string& value) {
        try {
            return stoi(value);  // Try to convert to integer
        } catch (const invalid_argument& e) {
            cerr << "Invalid argument: Could not convert '" << value << "' to an integer." << endl;
            return 0;  // Default to 0 in case of an error
        }
    }

    // Process salary expenses (reading salaries data)
    void processSalaries(const string& fileName) {
        ifstream file(fileName);
        string line;
        
        // Skip the header row
        getline(file, line);
        
        while (getline(file, line)) {
            stringstream ss(line);
            string firstName, lastName, hourRate, startHour, endHour, daySalary, city, role;
            int day;
            
            getline(ss, firstName, ',');
            getline(ss, lastName, ',');
            getline(ss, hourRate, ',');
            getline(ss, startHour, ',');
            getline(ss, endHour, ',');
            getline(ss, daySalary, ',');
            getline(ss, city, ',');
            getline(ss, role, ',');
            ss >> day;

            // Skip rows with invalid or empty values
            if (daySalary.empty() || city.empty()) {
                continue;
            }

            // Clean and convert salary to float
            float salary = stringToFloat(daySalary);
            financial_report[city][day] -= salary;  // Salaries are expenses, subtract from total
        }
    }

    // Process sales data as expenses
    void processSales(const string& fileName) {
        ifstream file(fileName);
        string line;
        
        // Skip the header row
        getline(file, line);
        
        while (getline(file, line)) {
            stringstream ss(line);
            string name, cost, city;
            int day;

            getline(ss, name, ',');
            getline(ss, cost, ',');
            getline(ss, city, ',');
            ss >> day;

            // Skip rows with invalid or empty values
            if (cost.empty() || city.empty()) {
                continue;
            }

            // Clean and convert cost to float
            float saleAmount = stringToFloat(cost);
            financial_report[city][day] -= saleAmount;  // Sales are expenses, subtract from total
        }
    }

    // Process inventory data as expenses
    void processInventory(const string& fileName) {
        ifstream file(fileName);
        string line;
        
        // Skip the header row
        getline(file, line);
        
        while (getline(file, line)) {
            stringstream ss(line);
            string name, quantity, price, city;
            int day;

            getline(ss, name, ',');
            getline(ss, quantity, ',');
            getline(ss, price, ',');
            getline(ss, city, ',');
            ss >> day;

            // Skip rows with invalid or empty values
            if (quantity.empty() || price.empty() || city.empty()) {
                continue;
            }

            // Clean and convert price to float and quantity to int
            float totalPrice = stringToFloat(price) * stringToInt(quantity);  // Calculate total cost for the items
            financial_report[city][day] -= totalPrice;  // Inventory costs are expenses
        }
    }

    // Process orders as income
    void processOrders(const string& fileName) {
        ifstream file(fileName);
        string line;

        // Skip the header row
        getline(file, line);
        
        while (getline(file, line)) {
            stringstream ss(line);
            string firstName, lastName, order, price, city, status, isLoyal;
            int day;
            
            getline(ss, firstName, ',');
            getline(ss, lastName, ',');
            getline(ss, order, ',');
            getline(ss, price, ',');
            getline(ss, city, ',');
            ss >> day;
            getline(ss, status, ',');
            getline(ss, isLoyal, ',');

            // Skip rows with invalid or empty values
            if (price.empty() || city.empty()) {
                continue;
            }

            // Clean and convert order price to float
            float orderAmount = stringToFloat(price);
            financial_report[city][day] += orderAmount;  // Orders are income, add to total
        }
    }

public:
    // Function to process all CSV files and populate the financial report
    void processReport() {
        processSales("CSV_Files/even_spec.csv");
        processSalaries("CSV_Files/angajati.csv");
        processInventory("CSV_Files/ingrediente.csv");
        processOrders("CSV_Files/comenzi.csv");
    }

    // Function to save the financial report to "rap_finan.csv"
    void saveReport() {
        ofstream file("CSV_Files/rap_finan.csv");
        
        if (file.is_open()) {
            file << "City,Day,Total\n"; // CSV header

            for (const auto& cityEntry : financial_report) {
                const string& city = cityEntry.first;
                for (const auto& dayEntry : cityEntry.second) {
                    int day = dayEntry.first;
                    float totalAmount = dayEntry.second;

                    // Write to CSV (expenses negative, income positive)
                    file << city << "," << day << "," << totalAmount << " lei\n";
                }
            }

            file.close();
            cout << "Financial report saved to rap_finan.csv" << endl;
        } else {
            cout << "Error: Unable to open the file for writing." << endl;
        }
    }
};


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


class Admin : public Employee { // ( Moștenire ) // ( Clasa )
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
        
        enableUTF8();
        string firstName, lastName, oras, rol;
        int day;
        
        cout << "Enter First Name: ";
        cin >> firstName;
        cout << "Enter Last Name: ";
        cin >> lastName;
        cout << "Enter City: ";
        cin >> oras;
        
        while(true){
            cout << "Enter Role: ";
            cin >> rol;

            
            if(rol != "manager" && rol !="barista" && rol != "ospătar")
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
                row[6] == oras && 
                row[7] == rol && 
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
        enableUTF8();
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

            for(unsigned i = 0 ; i < row.size(); ++i){
                cout<<row[i];

                if(i < row.size() - 1)
                    cout<<" | ";
            }

            cout<<endl;

        }
    }
};




class Manager : public Employee { // ( Moștenire ) // ( Clasa )
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
        enableUTF8();
        string firstName, lastName, oras, rol;
        int day;
        
        cout << "Enter First Name: ";
        cin >> firstName;
        cout << "Enter Last Name: ";
        cin >> lastName;
        cout << "Enter City: ";
        cin >> oras;
        
        while(true){
            cout << "Enter Role: ";
            cin >> rol;

            if(rol == "manager")
                cout<<"You are not allowed to remove this person"<<endl;
            else if(rol != "manager" && role !="barista" && rol != "ospătar")
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
                row[6] == oras && 
                row[7] == rol && 
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
        string name,type,oras;
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
        cin>>oras;

        ofstream file;
        file.open("CSV_files/ingrediente.csv", ios::app);

         file << name << ","
         << quantity << ","
         << price << ","
         << oras << ","
         << day << "\n";
        
        file.close();

    }

    void createEvent(int day) {
        string eventName;
        float eventCost;
        string eventCity;

        // Prompt for user input to fill event details
        cout << "Enter event name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer before reading the event name
        getline(cin, eventName);  // Read the full event name (getline handles spaces)

        cout << "Enter event cost: ";
        cin >> eventCost;         // Read the event cost as a float

        // Clear the input buffer to avoid skipping the next `getline`
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        cout << "Enter event city: ";
        getline(cin, eventCity);  // Read the event city

        // Open the file and append the event details
        ofstream file("CSV_files/even_spec.csv", ios::app);

        if (!file.is_open()) {
            cerr << "Error: Unable to open even_spec.csv" << endl;
            return;
        }

        // Check if it's the first line in the file and write the header if it's empty
        file.seekp(0, ios::end); // Move to the end of the file to check if it's empty
        if (file.tellp() == 0) {
            file << "Name,Cost,City,Day\n"; // Write the header if the file is empty
        }

        // Write the event details into the CSV
        file << eventName << ","
            << fixed << setprecision(2) << eventCost << "," // Format the cost as float with 2 decimals
            << eventCity << ","
            << day << "\n"; // Use the passed `day` parameter

        file.close();
        cout << "Event created successfully!" << endl;
    }
};

class Barista {
private:
    // Set to store items that the barista can prepare
    std::unordered_set<std::string> canPrepareItems = {
        "Espresso", "Latte", "Orange juice", "Lemonade", "Green tea", "Black tea"
    };

public:
    // Method to check ingredients and prepare a specific product
    void checkAndPrepare(const std::string& ingredientFile, const std::string& productFile) {
        std::unordered_map<std::string, std::pair<int, std::string>> ingredients;
        
        // Step 1: Read ingredients from ingrediente.csv
        std::ifstream inFile(ingredientFile);
        std::string line;
        std::vector<std::string> lines;
        
        while (std::getline(inFile, line)) {
            lines.push_back(line);
            std::stringstream ss(line);
            std::string name, quantityStr, priceStr, city, dayStr;
            int quantity, day;

            std::getline(ss, name, ',');
            std::getline(ss, quantityStr, ',');
            std::getline(ss, priceStr, ',');
            std::getline(ss, city, ',');
            std::getline(ss, dayStr, ',');

            // Check if quantity or dayStr are empty or invalid before converting
            try {
                if (quantityStr.empty() || dayStr.empty()) {
                    throw std::invalid_argument("Invalid data (empty field).");
                }

                // Convert strings to integers (with checks for valid format)
                quantity = std::stoi(quantityStr);
                day = std::stoi(dayStr);
            } catch (const std::invalid_argument& e) {
                std::cout << "Error: " << e.what() << " in line: " << line << std::endl;
                continue;  // Skip the line if there's a conversion issue
            }

            // Store the ingredient in the map
            ingredients[name] = {quantity, city};
        }

        inFile.close();
        
        // Step 2: Process each individual order (based on product name)
        std::vector<std::string> orders = {"Espresso", "Latte", "Orange juice", "Lemonade", "Green tea", "Black tea"};
        for (const auto& order : orders) {
            if (canPrepareItems.find(order) != canPrepareItems.end()) {
                // Check if ingredients for the order are available
                if (canMakeProduct(order, ingredients)) {
                    prepareProduct(order, ingredients, productFile);
                    updateIngredientsFile(ingredientFile, ingredients, lines);
                } else {
                    std::cout << "Insufficient ingredients to make " << order << ".\n";
                }
            } else {
                std::cout << order << " is pre-made and should be served by the waiter.\n";
            }
        }
    }

private:
    bool canMakeProduct(const std::string& productName, std::unordered_map<std::string, std::pair<int, std::string>>& ingredients) {
        // Logic to check if the ingredients for a specific product are available
        if (productName == "Espresso") {
            return ingredients["Coffee beans"].first >= 10;
        } else if (productName == "Latte") {
            return ingredients["Coffee beans"].first >= 10 && ingredients["Milk"].first >= 10;
        } else if (productName == "Orange juice") {
            return ingredients["Orange"].first >= 20;
        } else if (productName == "Lemonade") {
            return ingredients["Lemon"].first >= 10 && ingredients["Sugar"].first >= 5;
        } else if (productName == "Green tea") {
            return ingredients["Tea leaves"].first >= 5;
        } else if (productName == "Black tea") {
            return ingredients["Tea leaves"].first >= 5;
        }
        return false;
    }

    void prepareProduct(const std::string& productName, std::unordered_map<std::string, std::pair<int, std::string>>& ingredients, const std::string& productFile) {
        // Prepare the product and update the ingredient inventory
        if (productName == "Espresso") {
            ingredients["Coffee beans"].first -= 1;
        } else if (productName == "Latte") {
            ingredients["Coffee beans"].first -= 1;
            ingredients["Milk"].first -= 1;
        } else if (productName == "Orange juice") {
            ingredients["Orange"].first -= 2;
        } else if (productName == "Lemonade") {
            ingredients["Lemon"].first -= 2;
            ingredients["Sugar"].first -= 1;
        } else if (productName == "Green tea") {
            ingredients["Tea leaves"].first -= 1;
        } else if (productName == "Black tea") {
            ingredients["Tea leaves"].first -= 1;
        }

        // Step 3: Write the product details into produse.csv
        std::ofstream outFile(productFile, std::ios_base::app); // Open in append mode
        outFile << productName << "," << "Beverage" << "," << 1 << "," << "Cluj-Napoca" << "," << 1 << "\n";
        outFile.close();

        std::cout << productName << " is prepared and recorded in " << productFile << std::endl;
    }

    // Method to update the ingrediente.csv file
    void updateIngredientsFile(const std::string& ingredientFile, std::unordered_map<std::string, std::pair<int, std::string>>& ingredients, const std::vector<std::string>& lines) {
        // Open the file in write mode
        std::ofstream outFile(ingredientFile);

        // Write back all lines to the file, updating the quantities of ingredients
        for (const auto& line : lines) {
            std::stringstream ss(line);
            std::string name, quantityStr, priceStr, city, dayStr;

            std::getline(ss, name, ',');
            std::getline(ss, quantityStr, ',');
            std::getline(ss, priceStr, ',');
            std::getline(ss, city, ',');
            std::getline(ss, dayStr, ',');

            // If the ingredient is found, update the quantity
            if (ingredients.find(name) != ingredients.end()) {
                quantityStr = std::to_string(ingredients[name].first);
            }

            // Rebuild the line
            outFile << name << "," << quantityStr << "," << priceStr << "," << city << "," << dayStr << "\n";
        }

        outFile.close();
    }
};

// Helper function to check if a string is a valid integer
bool isValidInteger(const std::string& str) {
    try {
        std::stoi(str);  // Try converting string to integer
        return true;
    } catch (const std::invalid_argument&) {
        return false; // If invalid argument is thrown, return false
    } catch (const std::out_of_range&) {
        return false; // If number is out of range
    }
}

//Inca nu functioneaza cum trebuie , mai trebuie lucrata la aceasta functie
// Waiter class
class Waiter {

public:
     void checkAndFinalizeOrder(const std::string& orderFile, const std::string& ingredientFile, const std::string& productFile) {
        // Step 1: Read ingredients (pre-packaged items) from ingrediente.csv
        std::unordered_map<std::string, int> ingredients;
        readIngredients(ingredientFile, ingredients);

        // Step 2: Read order list from comenzi.csv
        std::ifstream orderFileIn(orderFile);
        std::string line;
        std::vector<std::string> orderLines;

        while (std::getline(orderFileIn, line)) {
            orderLines.push_back(line);
            std::stringstream ss(line);

            std::string firstName, lastName, order, price, city, dayStr, status, isLoyalStr;
            std::getline(ss, firstName, ',');
            std::getline(ss, lastName, ',');
            std::getline(ss, order, ',');
            std::getline(ss, price, ',');
            std::getline(ss, city, ',');
            std::getline(ss, dayStr, ',');
            std::getline(ss, status, ',');
            std::getline(ss, isLoyalStr, ',');

            // Ensure 'dayStr' is valid before converting to integer
            if (!isValidInteger(dayStr)) {
                std::cout << "Skipping invalid order line: " << line << "\n";
                continue;  // Skip invalid rows
            }
            int day = std::stoi(dayStr);  // Now safe to call std::stoi

            // Ensure loyalty column is valid
            bool isLoyal = false;
            if (!isLoyalStr.empty() && isValidInteger(isLoyalStr)) {
                isLoyal = std::stoi(isLoyalStr); // Assuming loyalty is 0 or 1
            }

            std::unordered_map<std::string, int> orderedItems = getOrderedItems(order);

            // Check availability of each ordered item
            bool canFinalize = checkItemsAvailability(orderedItems, ingredients, productFile);

            if (canFinalize) {
                finalizeOrder(line, orderFile, orderedItems, ingredients, city);
            } else {
                std::cout << "Order could not be finalized: Insufficient items\n";
            }
        }
    }

private:
    // Set of pre-packaged items to check
    std::unordered_set<std::string> prePackagedItems = {
        "Pepsi", "Water", "Chocolate chip biscuits", "Banana bread", "Sandwich", "Croissant"
    };

    // Method to check if the order can be finalized
   

    // Read ingredients from the CSV file into the map
    void readIngredients(const std::string& ingredientFile, std::unordered_map<std::string, int>& ingredients) {
        std::ifstream inFile(ingredientFile);
        std::string line;

        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            std::string name, quantityStr, priceStr, city, dayStr;
            int quantity;

            std::getline(ss, name, ',');
            std::getline(ss, quantityStr, ',');
            std::getline(ss, priceStr, ',');
            std::getline(ss, city, ',');
            std::getline(ss, dayStr, ',');

            // Skip invalid or empty quantity
            if (!isValidInteger(quantityStr)) {
                std::cout << "Skipping ingredient with invalid quantity: " << line << "\n";
                continue;
            }

            quantity = std::stoi(quantityStr);
            ingredients[name] = quantity;
        }
    }

    // Get a map of ordered items
    std::unordered_map<std::string, int> getOrderedItems(const std::string& orderStr) {
        std::unordered_map<std::string, int> orderedItems;
        std::stringstream orderStream(orderStr);
        std::string item;

        while (std::getline(orderStream, item, '&')) {
            orderedItems[item] += 1;  // Count the item occurrences in the order
        }

        return orderedItems;
    }

    // Check items availability in the inventory and products
    bool checkItemsAvailability(const std::unordered_map<std::string, int>& orderedItems, std::unordered_map<std::string, int>& ingredients, const std::string& productFile) {
        std::ifstream productFileIn(productFile);
        std::unordered_set<std::string> preparedItems;
        std::string productLine;

        // Read the prepared products from producse.csv (Barista made items)
        while (std::getline(productFileIn, productLine)) {
            std::stringstream ss(productLine);
            std::string name;
            std::getline(ss, name, ',');
            preparedItems.insert(name);
        }

        // Check availability of products (both from `produse.csv` and `ingrediente.csv`)
        for (const auto& [item, quantity] : orderedItems) {
            // Check if the item is in the pre-packaged list and is available in ingredients
            if (prePackagedItems.find(item) != prePackagedItems.end()) {
                if (ingredients.find(item) != ingredients.end() && ingredients[item] >= quantity) {
                    // Decrement the available quantity in ingredients
                    ingredients[item] -= quantity;
                } else {
                    std::cout << "Not enough " << item << " in stock.\n";
                    return false;
                }
            }
            // Otherwise, check if it's a product that was prepared (from produse.csv)
            else if (preparedItems.find(item) != preparedItems.end()) {
                // If the product was prepared, assume it's available
                // No need to decrement product file, it's assumed to have already been prepared by Barista
            } else {
                std::cout << item << " is neither a prepared product nor a pre-packaged item.\n";
                return false;
            }
        }

        return true;
    }

    // Finalize the order by writing the updated order and ingredients to CSV
    void finalizeOrder(const std::string& orderLine, const std::string& orderFile, 
                       const std::unordered_map<std::string, int>& orderedItems, 
                       std::unordered_map<std::string, int>& ingredients, 
                       const std::string& city) {

        // Update the order status from preparing to finalized
        std::ifstream inFile(orderFile);
        std::ofstream outFile("temp.csv");  // Temporary file to overwrite original

        std::string line;
        while (std::getline(inFile, line)) {
            // Skip first row (header)
            if (line.find("First Name") != std::string::npos) {
                outFile << line << "\n"; 
                continue;
            }

            if (line.find(orderLine) != std::string::npos) {
                // Update the order status to 'finalized'
                std::string updatedLine = line.substr(0, line.rfind(',')+1) + "finalized," + "0";  // Assuming loyalty remains unchanged
                outFile << updatedLine << "\n";
            } else {
                outFile << line << "\n";
            }
        }
        outFile.close();
        inFile.close();
        
        // Replace original order file with updated file
        std::remove(orderFile.c_str());
        std::rename("temp.csv", orderFile.c_str());

        // Update the ingredients stock
        std::ofstream ingredientFileOut("ingrediente.csv");
        for (const auto& [name, qty] : ingredients) {
            ingredientFileOut << name << "," << qty << ",\n";  // Write updated quantities
        }

        std::cout << "Order finalized and ingredients updated!\n";
    }
};

class Customer{
    protected:
        string first_name;
        string last_name;
        string order;
        float total_price;
        string city;
        vector<pair<string, float>> products;
        map<string, float> availableIngredients;  
        vector<string> unavailableProducts;  
    
    public:

         void loadAvailableProducts() {
                ifstream file("CSV_files/ingrediente.csv");

                if (!file.is_open()) {
                    cerr << "Error: Unable to open ingredients file" << endl;
                    return;
                }

                string line;
                getline(file, line);  // Skip the header

                // Assuming product availability is based on their names from ingredients (e.g. Coffee, Tea, etc.)
                while (getline(file, line)) {
                    stringstream ss(line);
                    string name, quantityStr, priceStr, city, dayStr;
                    int quantity, day;
                    float price;

                    if (getline(ss, name, ',') &&
                        getline(ss, quantityStr, ',') &&
                        getline(ss, priceStr, ',') &&
                        getline(ss, city, ',') &&
                        getline(ss, dayStr)) {
                        
                        quantity = stoi(quantityStr);
                        price = stof(priceStr);
                        day = stoi(dayStr);

                        if (quantity > 0) {
                            availableIngredients[name] = price;  // Only store products with available quantities
                        } else {
                            unavailableProducts.push_back(name);  // Track unavailable products
                        }
                    }
                }
                file.close();
        }

        void checkAvailableDrinks(vector<string>& allowedProducts) {
            // Check if ingredients for specific drinks are missing
            if (availableIngredients.find("Coffee beans") == availableIngredients.end()) {
                allowedProducts.erase(remove(allowedProducts.begin(), allowedProducts.end(), "espresso"), allowedProducts.end());
                allowedProducts.erase(remove(allowedProducts.begin(), allowedProducts.end(), "latte"), allowedProducts.end());
                cout << "We are out of coffee beans. Espresso and Latte are unavailable." << endl;
            }
            
            if (availableIngredients.find("Milk") == availableIngredients.end()) {
                allowedProducts.erase(remove(allowedProducts.begin(), allowedProducts.end(), "latte"), allowedProducts.end());
                cout << "We are out of milk. Latte is unavailable." << endl;
            }
            
            if (availableIngredients.find("Orange") == availableIngredients.end()) {
                allowedProducts.erase(remove(allowedProducts.begin(), allowedProducts.end(), "orange juice"), allowedProducts.end());
                allowedProducts.erase(remove(allowedProducts.begin(), allowedProducts.end(), "lemonade"), allowedProducts.end());
                cout << "We are out of oranges. Orange juice and Lemonade are unavailable." << endl;
            }
            
            if (availableIngredients.find("Lemon") == availableIngredients.end()) {
                allowedProducts.erase(remove(allowedProducts.begin(), allowedProducts.end(), "lemonade"), allowedProducts.end());
                cout << "We are out of lemons. Lemonade is unavailable." << endl;
            }

            if (availableIngredients.find("Tea bag") == availableIngredients.end()) {
                allowedProducts.erase(remove(allowedProducts.begin(), allowedProducts.end(), "green tea"), allowedProducts.end());
                allowedProducts.erase(remove(allowedProducts.begin(), allowedProducts.end(), "black tea"), allowedProducts.end());
                cout << "We are out of tea bags. Green tea and Black tea are unavailable." << endl;
            }

            // Add other necessary checks for snacks or other items
        }


        void getOrder(int day) {
            const map<string, float> products = loadProductsFromCSV("CSV_files/menu.csv");

            loadAvailableProducts();   
            enableUTF8();
            cout << "Enter First Name: ";
            cin >> first_name;
            cout << "Enter Last Name: ";
            cin >> last_name;

            vector<string> selectedProducts;
            vector<float> productPrices;
            string product;

            cout << "Which type of order do you want to have? " << endl;
            cout << "1.Only drinks" << endl;
            cout << "2.Only snacks" << endl;
            cout << "3.Both types" << endl;
            string res;

            while (true) {
                int alg;
                cin >> alg;

                if (alg == 1) {
                    DrinkFactory drink;
                    res = checkBeverage(&drink);
                    break;
                } else if (alg == 2) {
                    SnackFactory snack;
                    res = checkBeverage(&snack);
                    break;
                } else if (alg == 3) {
                    MenuFactory menu;
                    res = checkBeverage(&menu);
                    break;
                } else
                    cout << "Invalid choice" << endl;
            }

            vector<string> allowedProducts;

            if (res == "drink") {
                    allowedProducts = {"espresso", "latte", "black tea", "green tea", "orange juice", "lemonade"};
            } else if (res == "snack") {
                    allowedProducts = {"chocolate chip biscuits", "banana bread", "sandwich", "croissant"};
            } else if (res == "menu") {
                    allowedProducts = {"espresso", "latte", "black tea", "green tea", "orange juice", "lemonade","chocolate chip biscuits", "banana bread", "sandwich", "croissant"};
            }

            checkAvailableDrinks(allowedProducts);

            cout << "Enter products to order (type 'done' to finish):" << endl;
            cin.ignore();

             while (true) {
                cout << "Product: ";
                getline(cin, product);

                if (product == "done") {
                    break;
                }

                string product_lower = toLowercase(product);  

               
                if (find(allowedProducts.begin(), allowedProducts.end(), product_lower) != allowedProducts.end()) {
                    selectedProducts.push_back(product);
                    productPrices.push_back(availableIngredients[product_lower]);
                } else {
                    cout << "This product is not available or not supported. Please choose a different product." << endl;
                }
            }

            order = buildOrderString(selectedProducts);
            total_price = calculateTotalPrice<float>(productPrices);

            cout << "Introduce the city: ";
            cin >> city;

            bool is_loyal = false;

            ofstream file;
            file.open("CSV_files/comenzi.csv", ios::app);

            file << first_name << ","
                << last_name << ","
                << order << ","
                << total_price << ","
                << city << ","
                << day << ","
                << "preparing" << ","
                << (is_loyal ? "1" : "0") << "\n";

            file.close();

            // Call the function to apply discounts and update the CSV
            applyDiscountAndUpdateCSV("CSV_files/comenzi.csv", "CSV_files/comenzi_temp.csv", city, day);

            cout << "Thank you for your order! If eligible, discounts have been applied!" << endl;
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

                for(unsigned int i = 0 ; i < row.size(); ++i){
                    cout<<row[i];

                    if(i < row.size() - 1)
                        cout<<" | ";
                }

                cout<<endl;

            }
        }

    private:
        string buildOrderString(const vector<string>& prod) {
            if (prod.empty()) return "";

            string result = prod[0];
            for (size_t i = 1; i < prod.size(); ++i) {
                result += " & " + prod[i];
            }
            return result;
        }

        template <typename X>
        X calculateTotalPrice(const vector<X>& prices) { // ( Templates )
            X total = 0.0f;
            for (X price : prices) {
                total += price;
            }
            return total;
        }
};

//Interfata platformei de lucru atat pentru angajati dar si pentru clienti (ca un fel de site)
void generateMenu(){

    enableUTF8();

    //char lan;
    
    cout<<"Welcome to Royal Coffe Shop"<<endl;
    restart:    
        /*cout<<"Choose one of the languages below: "<<endl;
        cout<<"1.English"<<endl;
        cout<<"2.Română"<<endl;*/

    //cin>>lan;
    int day = 1;
    while(true){
        
        //if(lan == '1'){
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
                            int rez;
                            cout<<"Please choose one of the files:"<<endl;
                            cout<<"1.CSV_Files/angajati.csv"<<endl;
                            cout<<"2.CSV_Files/comenzi.csv"<<endl;
                            cout<<"3.CSV_Files/even_spec.csv"<<endl;
                            cout<<"4.CSV_Files/ingrediente.csv"<<endl;
                            cout<<"5.CSV_Files/menu.csv"<<endl;
                            cout<<"6.CSV_Files/produse.csv"<<endl;
                            cout<<"7.CSV_Files/rap_finan.csv"<<endl;

                            cin>>rez;
                            string fisier;

                            if(rez == 1)
                                fisier = "CSV_Files/angajati.csv";
                            else if(rez == 2)
                                fisier = "CSV_Files/comenzi.csv";
                            else if(rez == 3)
                                fisier = "CSV_Files/even_spec.csv";
                            else if(rez == 4)
                                fisier = "CSV_Files/ingrediente.csv";
                            else if(rez == 5)
                                fisier = "CSV_Files/menu.csv";
                            else if(rez == 6)
                                fisier = "CSV_Files/produse.csv";
                            else if(rez == 7)
                                fisier = "CSV_Files/rap_finan.csv";
                            else
                                fisier = nullptr;

                            adm.displayAll(fisier);
                            break;
                        }
                        case 4:{
                            FinancialReport report;
                            report.processReport();  
                            report.saveReport();
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
                            man.createEvent(day);
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
                    cout<<"Welcome Waiter!"<<endl;
                    Waiter wai;
                    wai.checkAndFinalizeOrder("CSV_Files/comenzi.csv", "CSV_Files/ingrediente.csv", "CSV_Files/produse.csv");
                }
                else if(choice == '3'){
                    cout<<"Welcome Barista!"<<endl;
                    Barista bar;
                    bar.checkAndPrepare("CSV_Files/ingrediente.csv","CSV_Files/produse.csv");

    
                   
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
                 cout<<"Invalid Option! Do you want to go to the next day: Y/N? "<<endl;

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
        //}
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
        /*else{
            cout<<"Please choose a valid language"<<endl;
            goto restart;
        }*/
    }
}

int main(void){

    enableUTF8();
    generateMenu();


    return 0;
}
