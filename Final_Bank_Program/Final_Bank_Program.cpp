#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <limits>

using namespace std;

// Enumeration for menu options
enum MenuOption {
    MAKE_DEPOSIT = 1,
    MAKE_WITHDRAWAL,
    DEPOSIT_HISTORY,
    WITHDRAWAL_HISTORY,
    ALL_HISTORY,
    CHECK_BALANCE,
    EXIT
};

// BankAccount class definition
class BankAccount {
private:
    // Private data members
    double balance;
    vector<double> balanceHistory;
    vector<string> depositHistory;
    vector<string> withdrawalHistory;

public:
    // Constructor
    BankAccount() : balance(0.0) {}

    // Setter for balance with history recording
    void setBalance(double newBalance) {
        balance = newBalance;
        balanceHistory.push_back(newBalance);
    }

    // Getter for balance
    double getBalance() const {
        return balance;
    }

    // Deposit method
    void deposit(double amount) {
        // Check if the amount is valid
        if (amount > 0) {
            // Update balance and record transaction history
            balance += amount;
            balanceHistory.push_back(balance);
            string transaction = getTimeStamp() + " Deposit: " + to_string(amount);
            depositHistory.push_back(transaction);
            // Display success message
            cout << "Deposit successful. " << transaction << ". Your new balance is: " << balance << "\n";
        }
        else {
            // Display error message for invalid deposit amount
            cout << "Invalid deposit amount.\n";
        }
    }

    // Withdrawal method
    void withdrawal(double amount) {
        // Check if the withdrawal amount is valid and sufficient funds are available
        if (amount > 0 && amount <= balance) {
            // Update balance and record transaction history
            balance -= amount;
            balanceHistory.push_back(balance);
            string transaction = getTimeStamp() + " Withdrawal: " + to_string(amount);
            withdrawalHistory.push_back(transaction);
            // Display success message
            cout << "Withdrawal successful. " << transaction << ". Your new balance is: " << balance << "\n";
        }
        else {
            // Display error message for invalid withdrawal amount or insufficient funds
            cout << "Invalid withdrawal amount or insufficient funds.\n";
        }
    }

    // Getter for deposit history
    string getDepositHistory() const {
        string result;
        for (const string& deposit : depositHistory) {
            result += deposit + "\n";
        }
        return result;
    }

    // Getter for withdrawal history
    string getWithdrawalHistory() const {
        string result;
        for (const string& withdrawal : withdrawalHistory) {
            result += withdrawal + "\n";
        }
        return result;
    }

    // Getter for all transaction history
    string getAllTransactionHistory() const {
        string result;
        for (const string& deposit : depositHistory) {
            result += deposit + "\n";
        }
        for (const string& withdrawal : withdrawalHistory) {
            result += withdrawal + "\n";
        }
        return result;
    }

private:
    // Private method to get timestamp for transactions
    string getTimeStamp() const {
        time_t now;
        struct tm timeinfo;
        char buffer[80];

        time(&now);
        localtime_s(&timeinfo, &now);
        strftime(buffer, sizeof(buffer), "%m/%d/%Y %H:%M:%S", &timeinfo);
        return string(buffer);
    }

    // Friend functions for file input/output
    friend ofstream& operator<<(ofstream& file, const BankAccount& account);
    friend ifstream& operator>>(ifstream& file, BankAccount& account);
};

// Function to write transaction history to a file
void writeTransactionHistoryToFile(const string& fileName, const BankAccount& account) {
    ofstream file(fileName, ios::trunc);
    if (file.is_open()) {
        file << account;
        file.close();
    }
    else {
        // Display error message if file cannot be opened for writing
        cout << "Error opening file for writing: " << fileName << "\n";
    }
}

// Overloaded output operator for file output
ofstream& operator<<(ofstream& file, const BankAccount& account) {
    file << account.balance << "\n";
    for (const double& balance : account.balanceHistory) {
        file << balance << " ";
    }
    file << "\n";
    for (const string& deposit : account.depositHistory) {
        file << deposit << "\n";
    }
    for (const string& withdrawal : account.withdrawalHistory) {
        file << withdrawal << "\n";
    }
    return file;
}

// Overloaded input operator for file input
ifstream& operator>>(ifstream& file, BankAccount& account) {
    file >> account.balance;
    double balanceValue;
    while (file >> balanceValue) {
        account.balanceHistory.push_back(balanceValue);
    }
    string transaction;
    getline(file, transaction);
    while (getline(file, transaction)) {
        if (!transaction.empty()) {
            if (transaction.find("Deposit") != string::npos) {
                account.depositHistory.push_back(transaction);
            }
            else if (transaction.find("Withdrawal") != string::npos) {
                account.withdrawalHistory.push_back(transaction);
            }
        }
    }
    return file;
}

// Function to display the main menu
void displayMenu(const string& name, const BankAccount& account) {
    cout << "********************************" << endl;
    cout << "Welcome " << name << "\nYour balance is: " << account.getBalance() << endl;
    cout << "1) Make a deposit\n2) Make a withdrawal\n3) Deposit History\n4) Withdrawal History\n"
        "5) Deposit and Withdrawal History\n6) Balance\n7) Exit" << endl;
}

// Function to display transaction history
void displayHistory(const string& history) {
    cout << history << "\n";
}

// Main function
int main() {
    BankAccount account;
    int selection;
    cout << "Welcome to our Bank Management System" << endl;
    cout << "Enter your name: ";
    string name;
    getline(cin, name);

    ifstream file("bank_data.txt");
    if (!file.is_open()) {
        // Display error message if file cannot be opened for reading
        cout << "Error opening file for reading: " << "bank_data.txt" << "\n";
        return 1;
    }

    try {
        // Read account data from file
        file >> account;
    }
    catch (const std::exception& e) {
        // Display error message if there is an exception while reading the file
        cout << "Error reading file: " << e.what() << "\n";
    }

    file.close();

    bool isRunning = true;
    while (isRunning) {
        // Display main menu
        displayMenu(name, account);
        cin >> selection;

        // Process user selection
        switch (selection) {
        case MAKE_DEPOSIT: {
            double deposit;
            cout << "Enter your deposit: ";
            cin >> deposit;

            if (cin.fail()) {
                // Display error message for invalid input
                cout << "Invalid input. Please enter a valid number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            // Make a deposit
            account.deposit(deposit);
            break;
        }
        case MAKE_WITHDRAWAL: {
            double withdrawal;
            cout << "Enter your withdrawal: ";
            cin >> withdrawal;

            if (cin.fail()) {
                // Display error message for invalid input
                cout << "Invalid input. Please enter a valid number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            // Make a withdrawal
            account.withdrawal(withdrawal);
            break;
        }
        case DEPOSIT_HISTORY:
            // Display deposit history
            displayHistory(account.getDepositHistory());
            break;
        case WITHDRAWAL_HISTORY:
            // Display withdrawal history
            displayHistory(account.getWithdrawalHistory());
            break;
        case ALL_HISTORY:
            // Display all transaction history
            displayHistory(account.getAllTransactionHistory());
            break;
        case CHECK_BALANCE:
            // Display current balance
            cout << "Your current balance is: " << account.getBalance() << "\n";
            break;
        case EXIT:
            // Write transaction history to file and exit the program
            writeTransactionHistoryToFile("bank_data.txt", account);
            cout << "Thank you for using the banking program. Your final balance is: " << account.getBalance() << "\n";
            isRunning = false;
            break;
        default:
            // Display error message for invalid selection
            cout << "Invalid selection. Please try again.\n";
        }
    }

    return 0;
}
