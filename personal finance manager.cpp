#include<iostream>
#include<iomanip>
#include<cmath>
#include<string>

using namespace std;

// Base class representing a general transaction
class Transaction {
protected:
    double amount;
    string description;

public:
    // Constructor to initialize transaction details
    Transaction(double amt, const string &desc) : amount(amt), description(desc) {}

    // Virtual function to display the transaction
    virtual void display() const {
        cout << setw(15) << amount << setw(25) << description << endl;
    }

    // Virtual destructor to clean up derived objects properly
    virtual ~Transaction() {}
};

// Derived class for Income transactions
class Income : public Transaction {
public:
    // Constructor to initialize income details
    Income(double amt, const string& desc) : Transaction(amt, desc) {}

    // Overriding display function to indicate it's an income
    void display() const override {
        cout << setw(15) << "Income" << setw(20);
        Transaction::display();
    }
};

// Derived class for Expenditure transactions
class Expenditure : public Transaction {
public:
    // Constructor to initialize expenditure details
    Expenditure(double amt, const string &desc) : Transaction(amt, desc) {}

    // Overriding display function to indicate it's an expenditure
    void display() const override {
        cout << setw(15) << "Expenditure" << setw(20);
        Transaction::display();
    }
};

// Base class representing an investment
class Investment {
protected:
    double amount;
    int duration;

public:
    // Constructor to initialize investment details
    Investment(double amt, int dur) : amount(amt), duration(dur) {}

    // Virtual function to display investment details
    virtual void display() const {
        cout << setw(15) << amount << setw(15) << duration;
    }

    // Virtual function to calculate the maturity amount of the investment
    virtual double maturityAmount() const {
        return amount;
    }

    // Virtual destructor to clean up derived objects properly
    virtual ~Investment() {}
};

// SIP (Systematic Investment Plan) derived from Investment
class SIP : public Investment {
private:
    double monthlyContribution;

public:
    // Constructor to initialize SIP details
    SIP(double amt, int dur, double monthly) : Investment(amt, dur), monthlyContribution(monthly) {}

    // Overriding display function to indicate it's an SIP
    void display() const override {
        cout << setw(15) << "SIP";
        Investment::display();
        cout << setw(20) << monthlyContribution << endl;
    }

    // Overriding maturityAmount function for SIP calculation
    double maturityAmount() const override {
        double totalInvestment = amount * pow(1 + (0.096 / 12), duration * 12);
        return totalInvestment + (monthlyContribution * 12 * duration);
    }
};

// FD (Fixed Deposit) derived from Investment
class FD : public Investment {
public:
    // Constructor to initialize FD details
    FD(double amt, int dur) : Investment(amt, dur) {}

    // Overriding display function to indicate it's an FD
    void display() const override {
        cout << setw(15) << "FD";
        Investment::display();
        cout << endl;
    }

    // Overriding maturityAmount function for FD calculation
    double maturityAmount() const override {
        return amount * pow((1 + 0.071), duration);
    }
};

// Class to manage all financial transactions and investments
class FinanceManager {
public:
    Transaction* transactions[100]; // Array to store transactions
    Investment* investments[50]; // Array to store investments
    int tCount; // Transaction count
    int iCount; // Investment count

    // Constructor initializes counters to zero
    FinanceManager() : tCount(0), iCount(0) {}

    // Function to add a transaction
    void addTransaction(Transaction* t) {
        if (tCount < 100)
            transactions[tCount++] = t;
        else
            cout << "Transaction limit reached!\n";
    }

    // Function to add an investment
    void addInvestment(Investment* i) {
        if (iCount < 50)
            investments[iCount++] = i;
        else
            cout << "Investment limit reached!\n";
    }

    // Display all records with balance info
    void displayRecord(double balance) const {
        cout << "-----------------------------------\n";
        cout << "|        Personal Finance        |\n";
        cout << "-----------------------------------\n";

        cout << "\n||--BALANCE--: " << balance << "||" << endl;

        cout << "\n--TRANSACTIONS--:\n";
        cout << setw(15) << "Type" << setw(15) << "Amount" << setw(25) << "Description" << endl;
        for (int i = 0; i < tCount; i++) {
            transactions[i]->display();
        }

        cout << "\n--INVESTMENTS--\n";
        cout << setw(15) << "Type" << setw(15) << "Amount" << setw(15) << "Duration" << setw(30) << "Monthly Contribution" << endl;
        for (int i = 0; i < iCount; i++) {
            investments[i]->display();
        }
    }

    // Function to display total number of investments and transactions
    void displaySummary() const {
        cout << "\nTotal Transactions: " << tCount << endl;
        cout << "Total Investments: " << iCount << endl;
    }
};

// User class representing a person managing their finances
class User {
public:
    FinanceManager manager; // Instance of FinanceManager
    double balance; // User balance

    // Constructor initializes balance
    User(double initialBalance) : balance(initialBalance) {}

    // Function to handle user operations
    void operations() {
        int choice = -1;
        while (choice != 0) {
            cout << "\n--OPTIONS--\n";
            cout << "1. Record INCOME\n";
            cout << "2. Record EXPENDITURE\n";
            cout << "3. Make Investment\n";
            cout << "4. View Finance Information\n";
            cout << "5. View Maturity Amounts\n";
            cout << "6. View Summary\n";
            cout << "0. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    recordIncome();
                    break;
                }

                case 2: {
                    recordExpenditure();
                    break;
                }

                case 3: {
                    makeInvestment();
                    break;
                }

                case 4: {
                    manager.displayRecord(balance);
                    break;
                }

                case 5: {
                    viewMaturityAmounts();
                    break;
                }

                case 6: {
                    manager.displaySummary();
                    break;
                }

                case 0:
                    cout << "Exiting...\n";
                    break;

                default:
                    cout << "\nInvalid option, please try again.";
            }
        }
    }

private:
    // Function to record income
    void recordIncome() {
        double amt;
        string desc;
        cout << "Enter amount: ";
        cin >> amt;
        cout << "Enter description: ";
        cin.ignore();
        getline(cin, desc);
        manager.addTransaction(new Income(amt, desc));
        balance += amt;
    }

    // Function to record expenditure with balance check
    void recordExpenditure() {
        double amt;
        string desc;
        cout << "Enter amount: ";
        cin >> amt;
        if (balance - amt < 1000) {
            cout << "Error: Minimum balance requirement is 1000.\n";
            return;
        }
        cout << "Enter description: ";
        cin.ignore();
        getline(cin, desc);
        manager.addTransaction(new Expenditure(amt, desc));
        balance -= amt;
    }

    // Function to make an investment
    void makeInvestment() {
        int subChoice;
        cout << "\nSelect Investment Type:\n1. SIP\n2. FD\n0. Go Back\n";
        cin >> subChoice;

        switch (subChoice) {
            case 1: {
                double amt, monthly;
                int dur;
                cout << "Enter investment amount: ";
                cin >> amt;
                if (balance - amt < 1000) {
                    cout << "Error: Insufficient balance for SIP.\n";
                    return;
                }
                cout << "Enter duration (years): ";
                cin >> dur;
                cout << "Enter monthly contribution: ";
                cin >> monthly;
                manager.addInvestment(new SIP(amt, dur, monthly));
                balance -= amt;
                break;
            }
            case 2: {
                double amt;
                int dur;
                cout << "Enter FD amount: ";
                cin >> amt;
                if (balance - amt < 1000) {
                    cout << "Error: Insufficient balance for FD.\n";
                    return;
                }
                cout << "Enter duration (years): ";
                cin >> dur;
                manager.addInvestment(new FD(amt, dur));
                balance -= amt;
                break;
            }
            case 0:
                return;
            default:
                cout << "Invalid choice.";
        }
    }

    // Function to view maturity amounts of investments
    void viewMaturityAmounts() const {
        cout << "--MATURITY AMOUNTS--\n";
        for (int i = 0; i < manager.iCount; i++) {
            Investment* inv = manager.investments[i];
            cout << "\nInvestment " << i + 1 << " Maturity Amount: " << inv->maturityAmount() << " Rs" << endl;
            cout << setw(15) << "Type" << setw(15) << "Amount" << setw(15) << "Duration" << setw(30) << "Monthly Contribution" << endl;
            inv->display();
        }
    }
};

int main() {
    cout << "---Welcome to Finance Management System!!---\n";

    // Creating user object with initial balance of 2000
    User user(2000);
    
    // Calling operations function for user interaction
    user.operations();

    return 0;
}
