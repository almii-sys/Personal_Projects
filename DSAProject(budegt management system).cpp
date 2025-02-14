#include <iostream>
#include <iomanip>
using namespace std;

// --- Budget class ---
class Budget {
public:
    int month;
    float rent, groceries, otherExpenses, totalExpenses, income, savings, debt;
    Budget* next;

    // Constructor updated to your requested syntax
    Budget(int m, float r, float g, float o, float i) {
        month = m;
        rent = r;
        groceries = g;
        otherExpenses = o;
        income = i;
        next = nullptr;

        totalExpenses = rent + groceries + otherExpenses;

        if (totalExpenses > income) {
            savings = 0;
            debt = totalExpenses - income;
        } else {
            savings = income - totalExpenses;
            debt = 0;
        }
    }
};

// --- Queue for Budgets ---
class BudgetQueue {
private:
    Budget* head;

public:
    BudgetQueue() : head(nullptr) {}

    void addBudget(int month, float rent, float groceries, float otherExpenses, float income) {
        Budget* newBudget = new Budget(month, rent, groceries, otherExpenses, income);

        if (!head) {
            head = newBudget;
        } else {
            Budget* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newBudget;
        }
    }

    Budget* findBudgetByMonth(int month) {
        Budget* current = head;
        while (current) {
            if (current->month == month) return current;
            current = current->next;
        }
        return nullptr;
    }

    Budget* getBudgets() { return head; }

    void removeLastBudget() {
        if (!head) return;

        if (!head->next) {
            delete head;
            head = nullptr;
            return;
        }

        Budget* prev = nullptr;
        Budget* current = head;
        while (current->next) {
            prev = current;
            current = current->next;
        }
        delete current;
        prev->next = nullptr;
    }
};

// --- Undo Stack ---
class UndoStack {
private:
    Budget* top;

public:
    UndoStack() : top(nullptr) {}

    void push(Budget* budget) {
        Budget* copy = new Budget(budget->month, budget->rent, budget->groceries, budget->otherExpenses, budget->income);
        copy->next = top;
        top = copy;
    }

    Budget* pop() {
        if (!top) return nullptr;

        Budget* temp = top;
        top = top->next;
        temp->next = nullptr;
        return temp;
    }

    bool isEmpty() { return top == nullptr; }
};

// --- BudgetMaker Class ---
class BudgetMaker {
private:
    BudgetQueue budgetQueue;
    UndoStack undoStack;

public:
    void addMonthBudget(int month) {
        if (budgetQueue.findBudgetByMonth(month)) {
            cout << "Budget for month " << month << " already exists. Use the edit option to modify it.\n";
            return;
        }

        float rent, groceries, otherExpenses, income;
        cout << "Enter income for month " << month << ": ";
        cin >> income;
        cout << "Enter rent expense for month " << month << ": ";
        cin >> rent;
        cout << "Enter groceries expense for month " << month << ": ";
        cin >> groceries;
        cout << "Are there other expenses for month " << month << "? (1 for Yes, 0 for No): ";
        int hasOtherExpenses;
        cin >> hasOtherExpenses;
        otherExpenses = 0;

        if (hasOtherExpenses) {
            cout << "Enter other expenses: ";
            cin >> otherExpenses;
        }

        budgetQueue.addBudget(month, rent, groceries, otherExpenses, income);

        // Push the added budget to the undo stack
        Budget* head = budgetQueue.getBudgets();
        while (head->next) {
            head = head->next; // Get the last added budget
        }
        undoStack.push(head);

        cout << "Budget for month " << month << " added successfully.\n";
    }

    void editBudget(int month) {
        Budget* budget = budgetQueue.findBudgetByMonth(month);
        if (!budget) {
            cout << "No budget found for month " << month << ".\n";
            return;
        }

        int choice;
        cout << "What do you want to edit?\n";
        cout << "1. Rent\n2. Groceries\n3. Other Expenses\n4. Income\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter new rent: ";
                cin >> budget->rent;
                break;
            case 2:
                cout << "Enter new groceries expense: ";
                cin >> budget->groceries;
                break;
            case 3:
                cout << "Enter new other expenses: ";
                cin >> budget->otherExpenses;
                break;
            case 4:
                cout << "Enter new income: ";
                cin >> budget->income;
                break;
            default:
                cout << "Invalid choice.\n";
                return;
        }

        // Recalculate totals
        budget->totalExpenses = budget->rent + budget->groceries + budget->otherExpenses;

        if (budget->totalExpenses > budget->income) {
            budget->savings = 0;
            budget->debt = budget->totalExpenses - budget->income;
        } else {
            budget->savings = budget->income - budget->totalExpenses;
            budget->debt = 0;
        }

        cout << "Budget updated successfully.\n";
    }

    // Other methods remain unchanged
    void undoLastBudget() {
        Budget* undone = undoStack.pop();
        if (!undone) {
            cout << "Nothing to undo.\n";
            return;
        }

        budgetQueue.removeLastBudget();
        cout << "Undone entry for month: " << undone->month << endl;
        delete undone;
    }

    void displayBudgets() {
        Budget* head = budgetQueue.getBudgets();

        if (!head) {
            cout << "No budgets to display!\n";
            return;
        }

        cout << left << setw(10) << "Month" << setw(15) << "Rent" << setw(15)
             << "Groceries" << setw(15) << "Other" << setw(15)
             << "Total" << setw(15) << "Savings" << setw(15) << "Debt" << endl;

        for (Budget* current = head; current != nullptr; current = current->next) {
            cout << left << setw(10) << current->month << setw(15) << current->rent
                 << setw(15) << current->groceries << setw(15) << current->otherExpenses
                 << setw(15) << current->totalExpenses << setw(15)
                 << current->savings << setw(15) << current->debt << endl;
        }
    }
};

// --- Main Function ---
int main() {
    BudgetMaker budgetMaker;
    int choice;

    do {
        cout << "--- BudgetMaker ---\n";
        cout << "1. Add Month Budget\n";
        cout << "2. Display Budgets\n";
        cout << "3. Undo Last Entry\n";
        cout << "4. Edit Budget\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int month;
                cout << "Enter month (1 for Jan, 2 for Feb, etc.): ";
                cin >> month;
                budgetMaker.addMonthBudget(month);
                break;
            }
            case 2:
                budgetMaker.displayBudgets();
                break;
            case 3:
                budgetMaker.undoLastBudget();
                break;
            case 4: {
                int month;
                cout << "Enter the month to edit: ";
                cin >> month;
                budgetMaker.editBudget(month);
                break;
            }
            case 5:
                cout << "Exiting BudgetMaker. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
