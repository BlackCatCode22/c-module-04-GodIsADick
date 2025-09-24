// linkedListPopQuizStarterCode.cpp
// dH 9/20/25

#include <iostream>
#include <string>

using namespace std;

struct node {
    char data;
    node* next;
};

int main() {
    cout << "\n    *********** Hello and welcome to the linked list pop quiz! *****************\n";

    // Create a string named myStr and initialize it to "abc"
    string myStr = "abcDefg";

    // Test string output and string indexing
    cout << "myStr = " << myStr << endl;
    cout << "\nmyStr[0] is: " << myStr[0] << endl;
    cout << "\nmyStr[1] is: " << myStr[1] << endl;
    cout << "\nmyStr[2] is: " << myStr[2] << endl;


    // Create two pointers
    node* pHead = nullptr;
    node* pCurrent = nullptr;

    // Linked List Steps
    // 1) Create a new node
    // 2) Fill the data fields
    // 3) Attach the node to the head of list
    // 4) Reposition head of list pointer

    // Boundary Condition: The start of the list/
    pHead = new node();
    pHead->data = myStr[0];
    pHead->next = nullptr;

    // Create two more nodes

    // This is the second node in our list.
    // Step 1: Create a new node
    pCurrent = new node();
    // Step 2: Fill the data field(s)
    pCurrent->data = myStr[1];
    // Step 3: Attach to head of list
    pCurrent->next = pHead;
    // Step 4: Reposition head of list
    pHead = pCurrent;

    // This is the third node in our list.
    // Step 1: Create a new node
    pCurrent = new node();
    // Step 2: Fill the data field(s)
    pCurrent->data = myStr[2];
    // Step 3: Attach to head of list
    pCurrent->next = pHead;
    // Step 4: Reposition head of list
    pHead = pCurrent;

    // Output the linked list.
    cout << "\nLinked list data: " << pHead->data << endl;
    cout << "\nLinked list data: " << pHead->next->data << endl;
    cout << "\nLinked list data: " << pHead->next->next->data << endl;

    // Use a for loop to build a linked list of all chars in myStr

    // TODO: Complete the following code snippet
    // Find myStr's length
    cout << "\nmyStr length is: " << myStr.length() << endl;


    // What is wrong with this code? It creates 100 new nodes with nothing stored in them, wasting memory.
    // Why does it not fail? The code, although not code you'd want to put in your linked list program, is written in correct syntax and will run anyway.
    // TODO: Discuss with your lab partner, be ready to answer your professor's questions about memory management
    for (int i=0; i<100; i++) {
        new node;
    }

    /* What does this code do? This code creates a new node each iteration, stores the character from myStr[i] in it, prints it, and then deletes it.
     * It does not actually form a linked list because the nodes are destroyed right after being printed.*/
    // Is this proper C++ programming? Not really, you usually don’t create and delete nodes inside the same loop unless you’re testing.
    // TODO: Discuss with your lab partner, know what the delete
    for (int i = 0; i < 7; i++) {
        node* myNode = new node;
        myNode->data = myStr[i];
        cout << "\nLinked list data: " << myNode->data << endl;
        delete myNode;
    }


    // Create the linked list using a for loop

    // We must have a starting node outside the for loop.
    // Boundary Condition: The start of the list

    // Note: pHead and pCurrent are already node pointers, we do not redefine them here
    // What would happen if you did this:
    // node* pHead = new node();
    // You would be re-declaring a brand-new local variable pHead inside the current scope of the function.
    pHead = new node();
    pHead->data = myStr[0];
    pHead->next = nullptr;

    // TODO: follow the steps and complete this code snippet
    for (int i = 1; i < 7; i++) {
        // Step 1: Create a new node
        pCurrent = new node();
        // Step 2: Fill the data field(s)
        pCurrent->data = myStr[i];
        // Step 3: Attach to head of list
        pCurrent->next = pHead;
        // Step 4: Reposition head of list
        pHead = pCurrent;
    }

    // Output the linked list!
    // Ensure pHead and pCurrent are both pointing to the head of the list
    // TODO: Complete these C++ statements:
    cout << "\npHead's data: " << pHead->data << endl;
    cout << "\npCurrent's data: " << pCurrent->data << endl;

    // Use a while loop to get the size of the list
    // Remember: the loop control variable must be initialized, checked, and changed.
    int sizeOfList = 0;
    cout << "\nSize of list: " << sizeOfList << endl;
    // TODO: create a while loop that walks thru the list
    while (pCurrent != nullptr) {
        sizeOfList++;
        pCurrent = pCurrent->next;
    }
    cout << "\nSize of list: " << sizeOfList << endl;

    // Get pCurrent back to the head of the list
    // TODO: Where is pCurrent after the while loop? Hint: look at your linked-list drawing.
    // pCurrent is nullptr, as it is the condition that stops the while loop.

    // TODO: Use a for loop to output the list
    pCurrent = pHead;
    cout << endl;
    for (; pCurrent != nullptr; ) {
        cout << pCurrent->data << " -> ";
        pCurrent = pCurrent->next;
    }
    cout << "null" << endl;

    // Get pCurrent back to the head of the list to do more linked-list stuff!
    pCurrent = pHead;

    return 0;
}
