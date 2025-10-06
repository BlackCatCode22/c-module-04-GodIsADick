#include <iostream>
#include <fstream>
using namespace std;

int main() {
    fstream myFile;
    myFile.open("blake.txt", ios::out); //write
    if (myFile.is_open()) {
        myFile << "Hello\n";
        myFile << "This is second line\n";
        myFile.close();
    }

    myFile.open("blake.txt", ios::app); //append
    if (myFile.is_open()) {
        myFile << "Hello2\n";
        myFile.close();
    }

    myFile.open("blake.txt", ios::in); //read
    if (myFile.is_open()) {
        string line;
        while (getline(myFile, line)) {
            cout << line << endl;
        }
        myFile.close();
    }

    //Test to see if file was actually created.
    if (myFile) {
        cout << "\nFile blake.txt successfully created in the cmake-build-debug folder.\n";
    }

    system("pause>0");
}