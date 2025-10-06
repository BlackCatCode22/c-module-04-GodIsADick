//Blake Wilson
// Char Linked List

#include <iostream>
#include <string>
using namespace std;

//Create the struct
struct node {
    char data;
    node* next;
};

void printlist(node* head) {
    while (head != nullptr) {
        cout << head->data << " -> ";
        head = head->next;
    }
    cout << "null" << endl;
}

int main() {

    cout<<"\nWelcome to my Linked List\n";

    // Create a string named myStr
    string myStr = "abcDefg";

    //Test the struct
    //Create a pointer named head
    node* pHead = nullptr;

    //Create a new node named Node
    node* pNew = new node();
    pNew->data = myStr[0];
    pNew->next = nullptr;

    //Output what is in our node
    cout << "\nOutput of data and next: \n";
    cout << pNew->data << endl;
    cout << pNew->next << endl;

    //Create our linked list
    pHead = pNew;

    //Verify this with output
    cout << "\nOutput of the node that pHead is pointing to\n";
    cout << pHead->data << endl;
    cout << pHead->next << endl;

    //Add a node to our linked list using our four steps
    //Create a new node
    pNew = new node();
    pNew->data = myStr[1];
    pNew->next =


    return 0;
}