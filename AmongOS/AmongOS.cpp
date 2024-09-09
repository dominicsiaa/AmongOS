// AmongOS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

void printHeader () {
    cout << R"(  
   ____    ___ ___   ___   ____    ____       ___   _____
 _/__  |_ |   |   | /   \ |    \  /    |     /   \ / ___/
|____| | || _   _ ||     ||  _  ||   __|    |     (   \_ 
 |     | ||  \_/  ||  O  ||  |  ||  |  |    |  O  |\__  |
 |  _  | ||   |   ||     ||  |  ||  |_ |    |     |/  \ |
 |  |  |_||   |   ||     ||  |  ||     |    |     |\    |
 |__|__|  |___|___| \___/ |__|__||___,_|     \___/  \___|
)" << endl;

    // Initial welcome message (w/ color)
    cout << "\033[1;32m" << "Hello, Welcome to Among OS commandline!" << endl;
    cout << "\033[1;33m" << "Type 'exit' to quit, 'clear' to clear the screen" << endl;
    
}

int main() {

    printHeader();
    
    string command;

    while (true) {
        cout << "\033[0m" << "Enter a command: " << endl;
        cin >> command;

        if (command == "initialize") {
            cout << "\033[1;32m" << command + " command recognized. Doing Something" << endl;
        }
        else if (command == "screen") {
            cout << "\033[1;32m" << command + " command recognized. Doing Something" << endl;
        }
        else if (command == "scheduler-test") {
            cout << "\033[1;32m" << command + " command recognized. Doing Something" << endl;
        }
        else if (command == "scheduler-stop") {
            cout << "\033[1;32m" << command + " command recognized. Doing Something" << endl;
        }
        else if (command == "report-util") {
            cout << "\033[1;32m" << command + " command recognized. Doing Something" << endl;
        }
        else if (command == "clear") {
            system("cls");
            printHeader();
        }
        else if (command == "exit") {
            cout << "\033[1;32m" << command + " command recognized. Doing Something"  << endl;
            // TODO: working exit command
        }
        else {
             cout << "\033[1;31m" << "Error: command not recognized. Please try again"  << endl;
        }
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
