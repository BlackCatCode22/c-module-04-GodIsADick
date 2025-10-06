//Blake Wilson
//Linked List Program

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

struct Node {
    string Data;
    Node* pNext;
};

void printlist(Node* head) {
    while (head != nullptr) {
        cout << head->Data << " -> ";
        head = head->pNext;
    }
    cout << "null" << endl;
}

int main() {
    string myStr = "abcDefg";
    string myStrReversed = myStr;
    reverse(myStrReversed.begin(), myStrReversed.end());

    Node* pHead = nullptr;

    Node* pNew = new Node();
    pNew->Data = myStr;
    pNew->pNext = pHead;
    pHead = pNew;

    pNew = new Node();
    pNew->Data = myStrReversed;
    pNew->pNext = pHead;
    pHead = pNew;

    printlist(pHead);

    return 0;
}