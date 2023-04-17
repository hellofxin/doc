#ifndef LINKED_LIST_INT_H
#define LINKED_LIST_INT_H

#include "iostream"
using namespace std;

template<typename T>struct Node {
	T data;
	Node<T>* next;

	Node() :next(NULL), data(0) { }
	Node(T element): next(NULL), data(element){ }
};

template<typename T> class LinkedListInt {
private:
	Node<T>* head;

public:
	LinkedListInt():head(new Node<T>()){ }
	LinkedListInt<T>& insert(T element);
	bool erase(T element);
	Node<T>* locate(T element);
	T retrieve(Node<T>* p);
	Node<T>* first();
	Node<T>* end();
	void toString();
	void toList();
};


template<typename T> LinkedListInt<T>& LinkedListInt<T>::insert(T element) {
	Node<T>* pNode = new Node<T>();
	pNode->data = element;
	pNode->next = head->next;
	head->next = pNode;
	cout << "LinkedList insert element: "<<element << endl;
	return *this;
}

template<typename T> bool LinkedListInt<T>::erase(T element) {
	Node<T>* pNode = new Node<T>();
	Node<T>* pDeleted = new Node<T>();
	bool bDeleted = false;
	for (pNode = head; pNode->next != NULL; pNode = pNode->next) {
		if (pNode->next->data == element) {
			pDeleted = pNode->next;
			pNode->next = pNode->next->next;
			delete pDeleted;
			pDeleted = NULL;
			bDeleted = true;
			cout << "LinkedList erase element: " << element << endl;
		}
	}
	return bDeleted;
}

template<typename T> Node<T>* LinkedListInt<T>::locate(T element) {
	Node<T>* pNode = new Node<T>();
	for (pNode = head; pNode->next != NULL; pNode = pNode->next) {
		if (pNode->next->data == element) {
			cout << "LinkedList locate element: " << element << endl;
			return pNode->next;
		}
	}
	cout << "LinkedList locate element: failed" << endl;
	return NULL;
}

template<typename T> T LinkedListInt<T>::retrieve(Node<T>* p) {
	Node<T>* pNode = new Node<T>();
	for (pNode = head; pNode->next != NULL; pNode = pNode->next) {
		if (pNode->next == p) {
			cout << "LinkedList retrieve: " << pNode->next->data << endl;
			return pNode->next->data;
		}
	}
	cout << "LinkedList retrieve: falied" << endl;
	return 0;
}

template<typename T> Node<T>* LinkedListInt<T>::first() {
	return head->next;
}

template<typename T> Node<T>* LinkedListInt<T>::end() {
	Node<T>* pNode = new Node<T>();
	for (pNode = head; pNode->next != NULL; pNode = pNode->next) { }
	return pNode;
}

template<typename T> void LinkedListInt<T>::toString() {
	int n = 0;
	Node<T>* pNode = new Node<T>();
	cout << endl;
	for (pNode = head; pNode->next != NULL; pNode = pNode->next) {
		cout << n << ": pNode->next->data = " << pNode->next->data << endl;
		n++;
	}
	return ;
}

template<typename T> void LinkedListInt<T>::toList() {
	Node<T>* pNode = new Node<T>();
	cout << "\nLinkedList head->";
	for (pNode = head; pNode->next != NULL; pNode = pNode->next) {
		cout << pNode->next->data << "-->";
	}
	cout << "NULL" << endl;
}


#endif