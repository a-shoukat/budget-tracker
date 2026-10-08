// ============================================================
//  Budget Tracker - Programming Fundamentals (PF) Project
//  Author : Ayesha Shoukat
//  Description:
//      A console-based budget tracker that records income and
//      expenses, shows balance, gives a category-wise summary
//      and saves all data to a file.
// ============================================================

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

// Structure to store one transaction (income or expense)
struct Transaction {
    string type;         // "Income" or "Expense"
    string category;     // e.g. Food, Transport, Fees
    string description;  // short note
    double amount;
};

vector<Transaction> transactions;   // all records kept in memory

// ---------- Function Declarations ----------
void addTransaction(string type);
void viewTransactions();
void viewBalance();
void categorySummary();
void saveToFile();
void loadFromFile();

// ============================================================
int main() {
    loadFromFile();   // load old data if file exists

    int choice;
    do {
        cout << "\n================================\n";
        cout << "        BUDGET TRACKER\n";
        cout << "================================\n";
        cout << "1. Add Income\n";
        cout << "2. Add Expense\n";
        cout << "3. View All Transactions\n";
        cout << "4. View Current Balance\n";
        cout << "5. Expense Summary by Category\n";
        cout << "6. Save & Exit\n";
        cout << "--------------------------------\n";
        cout << "Enter your choice (1-6): ";
        cin >> choice;
        cin.ignore();   // clear newline from buffer

        switch (choice) {
            case 1: addTransaction("Income");  break;
            case 2: addTransaction("Expense"); break;
            case 3: viewTransactions();        break;
            case 4: viewBalance();             break;
            case 5: categorySummary();         break;
            case 6: saveToFile();
                    cout << "\nData saved. Goodbye!\n";
                    break;
            default:
                cout << "\nInvalid choice! Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}

// ============================================================
// Add a new income or expense record
// ============================================================
void addTransaction(string type) {
    Transaction t;
    t.type = type;

    cout << "\nEnter category (e.g. Food, Transport, Fees, Salary): ";
    getline(cin, t.category);
    cout << "Enter description: ";
    getline(cin, t.description);
    cout << "Enter amount (Rs): ";
    cin >> t.amount;
    cin.ignore();

    if (t.amount <= 0) {
        cout << "Amount must be greater than zero. Record not added.\n";
        return;
    }

    transactions.push_back(t);
    cout << type << " of Rs " << fixed << setprecision(2) << t.amount
         << " added successfully!\n";
}

// ============================================================
// Display every transaction in a neat table
// ============================================================
void viewTransactions() {
    if (transactions.empty()) {
        cout << "\nNo transactions recorded yet.\n";
        return;
    }

    cout << "\n-----------------------------------------------------------------\n";
    cout << left << setw(5)  << "No."
         << setw(10) << "Type"
         << setw(15) << "Category"
         << setw(25) << "Description"
         << right << setw(12) << "Amount(Rs)" << "\n";
    cout << "-----------------------------------------------------------------\n";

    for (int i = 0; i < transactions.size(); i++) {
        cout << left << setw(5)  << i + 1
             << setw(10) << transactions[i].type
             << setw(15) << transactions[i].category
             << setw(25) << transactions[i].description
             << right << setw(12) << fixed << setprecision(2)
             << transactions[i].amount << "\n";
    }
    cout << "-----------------------------------------------------------------\n";
}

// ============================================================
// Calculate and display total income, expenses and balance
// ============================================================
void viewBalance() {
    double income = 0, expense = 0;

    for (int i = 0; i < transactions.size(); i++) {
        if (transactions[i].type == "Income")
            income += transactions[i].amount;
        else
            expense += transactions[i].amount;
    }

    cout << "\n---------------- BALANCE ----------------\n";
    cout << "Total Income   : Rs " << fixed << setprecision(2) << income << "\n";
    cout << "Total Expenses : Rs " << fixed << setprecision(2) << expense << "\n";
    cout << "-----------------------------------------\n";
    cout << "Current Balance: Rs " << fixed << setprecision(2) << income - expense << "\n";
}

// ============================================================
// Show total expenses grouped by category
// ============================================================
void categorySummary() {
    if (transactions.empty()) {
        cout << "\nNo transactions recorded yet.\n";
        return;
    }

    cout << "\n------- EXPENSE SUMMARY BY CATEGORY -------\n";

    vector<string> seen;   // categories already printed

    for (int i = 0; i < transactions.size(); i++) {
        if (transactions[i].type != "Expense")
            continue;

        // skip if this category was already counted
        bool alreadySeen = false;
        for (int k = 0; k < seen.size(); k++) {
            if (seen[k] == transactions[i].category) {
                alreadySeen = true;
                break;
            }
        }
        if (alreadySeen) continue;
        seen.push_back(transactions[i].category);

        // add up this category
        double total = 0;
        for (int j = 0; j < transactions.size(); j++) {
            if (transactions[j].type == "Expense" &&
                transactions[j].category == transactions[i].category)
                total += transactions[j].amount;
        }

        cout << left << setw(20) << transactions[i].category
             << ": Rs " << fixed << setprecision(2) << total << "\n";
    }
    cout << "-------------------------------------------\n";
}

// ============================================================
// Save all transactions to a text file
// ============================================================
void saveToFile() {
    ofstream file("transactions.txt");
    if (!file) {
        cout << "Error: could not open file for saving.\n";
        return;
    }

    for (int i = 0; i < transactions.size(); i++) {
        file << transactions[i].type << "|"
             << transactions[i].category << "|"
             << transactions[i].description << "|"
             << transactions[i].amount << "\n";
    }
    file.close();
}

// ============================================================
// Load transactions back from the text file (if it exists)
// ============================================================
void loadFromFile() {
    ifstream file("transactions.txt");
    if (!file) return;   // no saved data yet — not an error

    string line;
    while (getline(file, line)) {
        Transaction t;
        int p1 = line.find('|');
        int p2 = line.find('|', p1 + 1);
        int p3 = line.find('|', p2 + 1);
        if (p1 == string::npos || p2 == string::npos || p3 == string::npos)
            continue;

        t.type        = line.substr(0, p1);
        t.category    = line.substr(p1 + 1, p2 - p1 - 1);
        t.description = line.substr(p2 + 1, p3 - p2 - 1);
        t.amount      = stod(line.substr(p3 + 1));
        transactions.push_back(t);
    }
    file.close();
}
