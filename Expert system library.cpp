#include <iostream>
#include <limits>
#include <cctype>
using namespace std;

// Ask yes/no question
bool ask_question(const string& question) {
    char response;
    while (true) {
        cout << question << " (y/n): ";
        cin >> response;
        response = tolower(response);

        if (response == 'y' || response == 'n') {
            return response == 'y';
        } else {
            cout << "Please enter 'y' or 'n' only." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

// Rule-based questions
bool is_book_available() {
    return ask_question("Is the book available?");
}

bool is_user_eligible() {
    return ask_question("Is the user eligible for borrowing?");
}

bool is_book_damaged() {
    return ask_question("Is the book damaged?");
}

bool has_user_overdue_books() {
    return ask_question("Does the user have overdue books?");
}

bool is_book_in_demand() {
    return ask_question("Is the book currently in demand?");
}

bool is_book_low_in_stock() {
    return ask_question("Is the book stock below threshold?");
}

// Main expert system using score-based evaluation
int main() {
    cout << "Library Management Expert System\n" << endl;

    int score = 0;

    if (is_book_available()) score++;
    if (is_user_eligible()) score++;
    if (!has_user_overdue_books()) score++;
    if (!is_book_damaged()) score++;
    if (is_book_in_demand()) score++;
    if (!is_book_low_in_stock()) score++;

    cout << "\nBook Issue Evaluation Summary:\n";
    cout << "Total Score: " << score << "/6\n";

    if (score == 6) {
        cout << "Excellent conditions. Book can be issued immediately.\n";
    } else if (score >= 4) {
        cout << "Good conditions. Book can be issued with caution.\n";
    } else if (score >= 2) {
        cout << "Poor conditions. Recommend reviewing the issue case.\n";
    } else {
        cout << "Very poor conditions. Book should NOT be issued.\n";
    }

    return 0;
}
