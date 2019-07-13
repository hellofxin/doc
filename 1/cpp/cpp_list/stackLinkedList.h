#ifndef STACK_LINKED_LIST_H
#define STACK_LINKED_LIST_H


template<typename T> struct NodeStackLinkedList {
	T data;
	NodeStackLinkedList<T>* next;

	NodeStackLinkedList(T element = (T)0) : data(element), next(NULL) { }
};

template<typename T> class StackLinkedList {
private:
	NodeStackLinkedList<T>* top;

public:
	StackLinkedList():top(NULL) { }
	bool isEmpty();
	bool push(T element);
	T pop();
	T getTop();
	void showtime();
};

template<typename T> bool StackLinkedList<T>::isEmpty() {
	if (top == NULL) {
		cout << "StackLinkedList isEmpty: true" << endl;
		return true;
	}
	cout << "StackLinkedList isEmpty: false" << endl;
	return false;
}

template<typename T> bool StackLinkedList<T>::push(T element) {
	NodeStackLinkedList<T>* pNode = new NodeStackLinkedList<T>(element);
	pNode->next = top;
	top = pNode;
	cout << "StackLinkedList push: " << element << endl;
	return true;
}

template<typename T> T StackLinkedList<T>::pop() {
	NodeStackLinkedList<T>* pNode;
	if (!top) {
		cout << "StackLinkedList pop: failed" << endl;
		return 0;
	}
	pNode = top;
	top = top->next;
	cout << "StackLinkedList pop: " << pNode->data << endl;
	T dataTemp = pNode->data;
	delete pNode;
	return dataTemp;
}

template<typename T> T StackLinkedList<T>::getTop() {
	if (!top) {
		cout << "StackLinkedList getTop: failed" << endl;
		return 0;
	}
	cout << "StackLinkedList getTop: " << top->data << endl;
	return top->data;
}

template<typename T> void StackLinkedList<T>::showtime() {
	NodeStackLinkedList<T>* pNode = top;
	cout << "StackLinkedList showtime top-->";
	while (pNode) {
		cout << pNode->data << "->";
		pNode = pNode->next;
	}
	cout << "NULL\n" << endl;
}


#endif