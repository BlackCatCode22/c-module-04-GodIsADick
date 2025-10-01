#include <iostream>
#include <C:/Users/bwils/CLionProjects/Beginner Projects/hashMapsAndJSON/include/json.hpp.txt>
#include <string>
#include <unordered_map>
using json = nlohmann::json;
using namespace std;

int main() {
    cout << "\nWelcome to JSON!\n" << endl;

    json myStuList = {
        {"Arturo", 1234},
        {"Blake", 5432},
        {"Chase", 4321}
    };

    // Access the values like a hash map
    cout << "Arturo's Student ID: " << myStuList["Arturo"] << " (this is the value of the key/value pair)" << endl;
    cout << "Blake's Student ID: " << myStuList["Blake"] << endl;
    cout << "Chase's Student ID: " << myStuList["Chase"] << endl;

    /* Create a C++ hash map that uses three student names as keys and int values for
     * their test scores - name(string) : score(int) */
    unordered_map<string, int>myStuTestScores;

    myStuTestScores["Arturo"] = 95;
    myStuTestScores["Blake"] = 100;
    myStuTestScores["Chase"] = 87;

    cout << "\nArturo's test score is: " << myStuTestScores["Arturo"] << endl;
    cout << "Blake's test score is: " << myStuTestScores["Blake"] << endl;
    cout << "Chase's test score is: " << myStuTestScores["Chase"] << endl;
    cout << "\n";

    // Loop through the hash map with a C++ programming idiom
    for (auto &pair : myStuTestScores) {
        cout << pair.first << ": " << pair.second << endl;
    }


    return 0;
}
