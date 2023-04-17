// list_Int.cpp : 定义控制台应用程序的入口点。
//

#include "linked_list.h"
#include "iostream"
#include "DLinked_list.h"

using namespace std;

int main(){
	LinkedListInt<int> linkedListInt;
	linkedListInt.insert(1).insert(2).insert(3).insert(4).insert(5).insert(6);
	linkedListInt.erase(4);
	Node<int>* p = linkedListInt.first();
	cout << "first->data = " << p->data << "\n\n"<<endl;
	p = linkedListInt.end();
	cout << "end->data = " << p->data << "\n\n" << endl;
	p = linkedListInt.locate(3);
	cout << "retrieve(p) = " << linkedListInt.retrieve(p) << endl;
	linkedListInt.toString();
	linkedListInt.toList();

	DLinkedList<int> dLinkedList;
	dLinkedList.insertHead(11).insertHead(22).insertHead(33).insertHead(4).insertHead(5).insertHead(6).insertTail(77).insertTail(88).insertTail(99);
	/**/
	dLinkedList.erase(11);
	/**/
	dLinkedList.erase(5);
	dLinkedList.erase(66);
	/**/
	dLinkedList.toList();

	system("pause");
    return 0;
}

