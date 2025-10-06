#include <iostream>
using namespace std;

struct Node {
public:
    char Data;
    Node* pNext;
};

void printList(Node* head) {
    while (head != nullptr) {
        cout << head->Data << " -> ";
        head = head->pNext;
    }
    cout << "null" << endl;
}

int main() {
    Node* pHead = nullptr; // start with an empty list

    // Insert 'a'
    Node* pNew = new Node;
    pNew->Data = 'a';
    pNew->pNext = pHead;
    pHead = pNew;

    // Insert 'b' at front
    pNew = new Node;
    pNew->Data = 'b';
    pNew->pNext = pHead;
    pHead = pNew;

    // Insert 'c' at front
    pNew = new Node;
    pNew->Data = 'c';
    pNew->pNext = pHead;
    pHead = pNew;

    printList(pHead);

    return 0;
}