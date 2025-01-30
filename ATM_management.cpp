#include <bits/stdc++.h>
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
using namespace std;

class Account {
private:
    string fullName, dob, nationality, accountNumber, email, pin;
    double balance = 0.0;
    bool isLoggedIn = false;

public:
    // Masked PIN input for security

    string getMaskedInput() {
        string inputPin;
        char ch;

        #ifdef _WIN32
            while ((ch = _getch()) != '\r') {
                if (ch == '\b' && !inputPin.empty()) {
                    inputPin.pop_back();
                    cout << "\b \b";
                } else if (ch != '\b') {
                    inputPin.push_back(ch);
                    cout << '*';
                }
            }
        #else
            struct termios oldt, newt;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~ECHO;
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);

            while ((ch = getchar()) != '\n') {
                if (ch == 127 && !inputPin.empty()) {
                    inputPin.pop_back();
                    cout << "\b \b";
                } else if (ch != 127) {
                    inputPin.push_back(ch);
                    cout << '*';
                }
            }

            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        #endif

        cout << endl;
        return inputPin;
    }

    // Get current date and time

    string getCurrentDateTime() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        stringstream ss;

        ss << setfill('0') << setw(2) << ltm->tm_mday << "/"
           << setw(2) << ltm->tm_mon + 1 << "/"
           << setw(2) << ltm->tm_year + 1900 << " "
           << setw(2) << ltm->tm_hour << ":"
           << setw(2) << ltm->tm_min << ":"
           << setw(2) << ltm->tm_sec;

        return ss.str();
    }

    // Email validation

    bool isValidEmail(const string &email) {
        return email.find("@gmail.com") != string::npos;
    }

    // Check for unique email
    bool isEmailUnique(const string &email) {
        ifstream file("accounts.csv");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string accNum, name, dob, emailField, pin;
            double bal;

            getline(ss, accNum, ',');
            getline(ss, name, ',');
            getline(ss, dob, ',');
            getline(ss, emailField, ',');
            getline(ss, pin, ',');
            ss >> bal;

            if (email == emailField) {
                return false; // Duplicate email found
            }
        }

        file.close();
        return true; // Email is unique
    }

    // Date validation
    bool isValidDate(const string &dob) {
        regex datePattern(R"(^\d{2}/\d{2}/\d{4}$)");
        if (!regex_match(dob, datePattern)) return false;
        
        int day, month, year;
        sscanf(dob.c_str(), "%d/%d/%d", &day, &month, &year);
        if (year < 1900 || year > 2024 || month < 1 || month > 12 || day < 1 || day > 31) 
            return false;
        if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) 
            return false;
        if (month == 2) {
            bool isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
            if (day > (isLeap ? 29 : 28)) 
                return false;
        }
        return true;
    }

    // Check if user is an adult
    bool issavings(const string &dob) {
        int day, month, year;
        sscanf(dob.c_str(), "%d/%d/%d", &day, &month, &year);
        time_t t = time(0);
        tm *now = localtime(&t);
        int currentYear = now->tm_year + 1900;
        int currentMonth = now->tm_mon + 1;
        int currentDay = now->tm_mday;
        int age = currentYear - year;
        if (currentMonth < month || (currentMonth == month && currentDay < day)) {
            age--;
        }
        return age >= 18;
    }

    // Check for unique account number
    bool isAccountNumberUnique(const string &accountNumber) {
        ifstream file("accounts.csv");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string accNum;
            getline(ss, accNum, ',');
            if (accNum == accountNumber) {
                return false; // Duplicate account number found
            }
        }

        file.close();
        return true;
    }

    // Create new account
    void createAccount() 
        {
        cout <<"\n\t\t\t*****************************************************************************************************\n";
        cout <<"\t\t\t*                                                                                                   *\n";
        cout <<"\t\t\t*                               Create New Account                                                  *\n";
        cout <<"\t\t\t*                                                                                                   *\n";
        cout <<"\t\t\t*****************************************************************************************************\n";
        cin.ignore();
        cout << "\t\t\tEnter Full Name: ";
        getline(cin, fullName);
        do {
            cout << "\t\t\tEnter Date of Birth (DD/MM/YYYY): ";
            cin >> dob;       
        } 
        while (!isValidDate(dob));
        cout << (issavings(dob) ? "\t\t\tAccount Type: Savings Minor\n" : "\t\t\tAccount Type: Savings Majjor \n");
        do 
        {
    cout << "\t\t\tEnter Gmail Address: ";
    cin >> email;
    if (!isValidEmail(email)) {
        cout << "\t\t\tInvalid email. Must end with @gmail.com\n";
    } else if (!isEmailUnique(email)) {
        cout << "\t\t\tThis email is already in use. Please try a different email.\n";
    }
} while (!isValidEmail(email) || !isEmailUnique(email));

        do {
            cout << "\t\t\tEnter an 8-digit Account Number: ";
            cin >> accountNumber;
            if (accountNumber.length() != 8 || !isAccountNumberUnique(accountNumber)) 
            {
                //This Account number is already in used. Please try a different Account number.
                cout << "\t\t\tAccount number must be 8 digits and unique. Please try again.\n";
            }
        }
        
        while (accountNumber.length() != 8 || !isAccountNumberUnique(accountNumber));

        cout << "\t\t\tSet a 4-digit PIN: ";
        pin = getMaskedInput();
        while (pin.length() != 4 || !all_of(pin.begin(), pin.end(), ::isdigit)) {
            cout << "\t\t\tInvalid PIN. Please enter a 4-digit numeric PIN: ";
            pin = getMaskedInput();
        }
        
        ofstream accountFile("accounts.csv", ios::app);
        accountFile << accountNumber << "," << fullName << "," << dob << ","
                    << email << "," << pin << "," << balance << "\n";
        accountFile.close();
        cout <<"\n\n";
        cout <<"\n\t\t\t*****************************************************************************************************\n";
        cout <<"\t\t\t*                                                                                                    *\n";
        cout <<"\t\t\t*                            Account created successfully!                                           *\n";
        cout <<"\t\t\t*                                                                                                    *\n";
        cout <<"\t\t\t******************************************************************************************************\n";
    }


    // Delete account
    void deleteAccount() {
    cout << "\n\t\t\tEnter your Account Number to delete: ";
    string inputAccountNumber;
    cin >> inputAccountNumber;
    cout << "\t\t\tEnter your PIN to confirm: ";
    string inputPin = getMaskedInput();
    ifstream inputFile("accounts.csv");
    ofstream tempFile("temp.csv");
    bool accountFound = false;
    string line;

    while (getline(inputFile, line)) {
        stringstream ss(line);
        string accNum, name, dob, email, pin;
        double bal;

        getline(ss, accNum, ',');
        getline(ss, name, ',');
        getline(ss, dob, ',');
        getline(ss, email, ',');
        getline(ss, pin, ',');
        ss >> bal;

        if (accNum == inputAccountNumber && pin == inputPin) {
            accountFound = true;
            cout << "\n\t\t\tAccount with Account Number " << inputAccountNumber << " has been deleted successfully.\n";
        } else {
            tempFile << line << "\n";
        }
    }

    inputFile.close();
    tempFile.close();

    // Replace original file with updated one
    remove("accounts.csv");
    rename("temp.csv", "accounts.csv");

    if (!accountFound) {
        cout << "\n\t\t\tInvalid account number or PIN. Account not found.\n";
    }
}


    // Login
    bool login() {
    cout << "\n\t\t\tEnter Account Number: ";
    string inputAccountNumber, inputPin;
    cin >> inputAccountNumber;
    cout << "\t\t\tEnter PIN (input will be masked): ";

    inputPin = getMaskedInput();
    ifstream file("accounts.csv");
    string line;
    while (getline(file, line)) {
        string accNum, name, dateOfBirth, emailField, pinCode;
        double bal;
        stringstream ss(line);


        // Parse CSV data
        getline(ss, accNum, ',');
        getline(ss, name, ',');
        getline(ss, dateOfBirth, ',');
        getline(ss, emailField, ',');
        getline(ss, pinCode, ',');
        ss >> bal;

        // Check login credentials
        if (inputAccountNumber == accNum && inputPin == pinCode) {
            this->accountNumber = accNum;
            this->fullName = name;
            this->dob = dateOfBirth;
            this->email = emailField; // Update the email field
            this->balance = bal;
            this->isLoggedIn = true;

        cout <<"\n\t\t\t******************************************************************************************************\n";
        cout <<"\t\t\t*                                                                                                    *\n";
        cout <<"\t\t\t*                                                                                                    *\n";
        cout <<"\t\t\t*                      Login successful! Welcome, " << fullName <<"                                  *\n";
        cout <<"\t\t\t*                                                                                                    *\n";
        cout <<"\t\t\t*                                                                                                    *\n";
        cout <<"\t\t\t******************************************************************************************************\n";
        
           // cout << "\n\t\t\tLogin successful! Welcome, " << fullName << ".\n";
            return true;
        }
    }
    cout << "\n\t\t\tInvalid account number or PIN.\n";
    return false;
}

    // Show account information
    void showAccountInfo() 
    {

        if (isLoggedIn) 
        {
        cout <<"\n\t\t\t******************************************************************************************************\n";
        cout <<"\t\t\t*                                                                                                     *\n";
        cout <<"\t\t\t*                                                                                                     *\n";
        cout <<"\t\t\t*                                 Account Information                                                 *\n";
        cout <<"\t\t\t*                                                                                                     *\n";
        cout <<"\t\t\t*                                                                                                     *\n";
        cout <<"\t\t\t*******************************************************************************************************\n";
        
        cout << "\t\t\tName: " << fullName << "\n";
        cout << "\t\t\tAccount Number: " << accountNumber << "\n";
        cout << "\t\t\tDate of Birth: " << dob << "\n";
        cout << "\t\t\tEmail: " << email << "\n"; // Display the email
        cout << "\t\t\tTotal Balance: $" << fixed << setprecision(2) << balance << "\n";
    } else {
        cout <<"\n\t\t\t*****************************************************************************************************\n";
        cout <<"\t\t\t*                                                                                                    *\n";
        cout <<"\t\t\t*                                                                                                    *\n";
        cout <<"\t\t\t*                                 Please login first.                                                *\n";
        cout <<"\t\t\t*                                                                                                    *\n";
        cout <<"\t\t\t*                                                                                                    *\n";
        cout <<"\t\t\t******************************************************************************************************\n";
        }
    }

    // Deposit money
    void deposit() {
        if (isLoggedIn) 
        {
            double amount;
        cout <<"\t\t\tEnter amount to deposit: ";
            cin >> amount;
            balance += amount;

            // Update balance in CSV file
            updateBalanceInFile();
        cout <<"\t\t\tDeposit successful. Total Balance: $" << fixed << setprecision(2) << balance <<"\n";
        }
        else 
        {
        cout <<"\n\t\t\t******************************************************************************************************\n";
        cout <<"\t\t\t*                                                                                                     *\n";
        cout <<"\t\t\t*                           Please login first.                                                       *\n";
        cout <<"\t\t\t*                                                                                                     *\n";
        cout <<"\t\t\t*******************************************************************************************************\n";
        }
    }

    // Check balance
    void checkBalance() {
    if (isLoggedIn) {
        cout << "\n\t\t\t*****************************************************************************************************\n";
        cout << "\t\t\t*                                                                                                   *\n";
        cout << "\t\t\t*                             Your Current Balance: $" << fixed << setprecision(2) << balance <<"   *\n";
        cout << "\t\t\t*                                                                                                   *\n";
        cout << "\t\t\t*****************************************************************************************************\n";
    } else {
        cout << "\n\t\t\t*****************************************************************************************************\n";
        cout << "\t\t\t*                                                                                                    *\n";
        cout << "\t\t\t*                               Please login first.                                                  *\n";
        cout << "\t\t\t*                                                                                                    *\n";
        cout << "\t\t\t******************************************************************************************************\n";
    }
}


    // Withdraw money
    void withdraw() {
        if (isLoggedIn) {
            double amount;
            cout << "\t\t\tEnter amount to withdraw: ";
            cin >> amount;
            if (amount > balance) {
                cout << "\t\t\tInsufficient funds.";
            } 
            else 
            {
                balance -= amount;

                // Update balance in CSV file

                updateBalanceInFile();

                cout << "\t\t\tWithdrawal successful. Total Balance: $" << fixed << setprecision(2) << balance << "\n";
            }
        } 
        else
        {
        cout <<"\n\t\t\t*****************************************************************************************************\n";
        cout <<"\t\t\t*                                                                                                    *\n";
        cout <<"\t\t\t*                               Please login first.                                                  *\n";
        cout <<"\t\t\t*                                                                                                    *\n";
        cout <<"\t\t\t******************************************************************************************************\n";
        }
    }

    // Logout
    void logout() {
        isLoggedIn = false;
        cout <<"\n\t\t\t*****************************************************************************************************\n";
        cout <<"\t\t\t*                                                                                                   *\n";
        cout <<"\t\t\t*                                Logged out successfully.                                           *\n";
        cout <<"\t\t\t*                                                                                                   *\n";
        cout <<"\t\t\t******************************************************************************************************\n";
    
    }

    // Logged-in menu
    void showLoggedInMenu() {
        int choice;
        do {
        cout <<"\n\t\t\t*****************************************************************************************************\n";
        cout <<"\t\t\t*                                                                                                   *\n";
        cout <<"\t\t\t*                                  1. Balance  Check                                                *\n";
        cout <<"\t\t\t*                                  2. Deposit                                                       *\n";
        cout <<"\t\t\t*                                  3. Withdraw                                                      *\n";
        cout <<"\t\t\t*                                  4. Show Account Information                                      *\n";
        cout <<"\t\t\t*                                  5. Logout                                                        *\n";
        cout <<"\t\t\t*                                                                                                   *\n";
        cout <<"\t\t\t*****************************************************************************************************\n";
        cout <<"\n";
        cout <<"\t\t\tEnter choice: ";
            cin >> choice;
            switch (choice) 
            {
            case 1:
                checkBalance();
                break;
            case 2:
                deposit();
                break;
            case 3:
                withdraw();
                break;
            case 4:
                showAccountInfo();
                break;
                
            case 5:
                logout();
                break;
            default:
                cout << "\t\t\tInvalid choice.\n";
            }
        } while (isLoggedIn);
    }

private:
    // Update balance in CSV file
    void updateBalanceInFile()
    {
        ifstream inputFile("accounts.csv");
        ofstream tempFile("temp.csv");

        string line;
        while (getline(inputFile, line)) 
        {
            stringstream ss(line);
            string accNum, name, dob, email, pin;
            double bal;

            getline(ss, accNum, ',');
            getline(ss, name, ',');
            getline(ss, dob, ',');
            getline(ss, email, ',');
            getline(ss, pin, ',');
            ss >> bal;

            if (accNum == accountNumber) {
                tempFile << accNum << "," << name << "," << dob << ","
                         << email << "," << pin << "," << balance << "\n";
            } else {
                tempFile << line << "\n";
            }
        }
        inputFile.close();
        tempFile.close();

        remove("accounts.csv");

        rename("temp.csv", "accounts.csv");
    }
};

int main() {
    Account bankSystem;
    int choice;

    while (true) 
    {
        cout << "\n\t\t\t****************************************************************************************************\n";
        cout << "\t\t\t*                                                                                                  *\n";
        cout << "\t\t\t*                                    ATM   SYSTEM                                                  *\n";
        cout << "\t\t\t*                                                                                                  *\n";
        cout << "\t\t\t*                                  1. Create Account                                               *\n";
        cout << "\t\t\t*                                  2. Login                                                        *\n";
        cout << "\t\t\t*                                  3. Delete Account                                               *\n";
        cout << "\t\t\t*                                  4. Exit                                                         *\n";
        cout << "\t\t\t*                                                                                                  *\n";
        cout << "\t\t\t****************************************************************************************************\n";
        
        cout << "\n\t\t\tEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                bankSystem.createAccount();
                break;
            case 2:
                if (bankSystem.login()) {
                    bankSystem.showLoggedInMenu();
                }
                break;
            case 3:
                bankSystem.deleteAccount();
                break;
            case 4:
                cout << "\n\t\t\tThank you for using the Banking System.\n\n\t\t\tDeveloper By Fahim Sariyar Shovon!\n";
                return 0;
            default:
                cout << "\n\t\t\tInvalid choice. Please try again.\n";
        }
    }
    return 0;
}