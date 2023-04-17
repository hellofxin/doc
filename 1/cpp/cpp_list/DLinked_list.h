#ifndef DLINKED_LIST_H
#define DLINKED_LIST_H

#include "iostream"

template<typename T> struct NodeDLinkedList {
	T data;
	NodeDLinkedList<T>* previous;
	NodeDLinkedList<T>* next;

	NodeDLinkedList(): previous(NULL),next(NULL), data(0){ }
	NodeDLinkedList(T element): previous(NULL), next(NULL), data(element){ }
};

template<typename T> class DLinkedList {
private:
	NodeDLinkedList<T>* head;
	NodeDLinkedList<T>* tail;

public:
	DLinkedList() : head(NULL), tail(NULL) { }
	DLinkedList<T>& insertHead(T element);
	DLinkedList<T>& insertTail(T element);
	bool erase(T element);
	NodeDLinkedList<T>* locate(T element);
	T retrieve(NodeDLinkedList<T>* p);
	void toString();
	void toList();
	
};

template<typename T> DLinkedList<T>& DLinkedList<T>::insertHead(T element) {
	NodeDLinkedList<T>* pNode = new NodeDLinkedList<T>(element);
	pNode->next = head;
	/* 
	  如果head非空，则维护head->previoud,
	  如果head为空，则pNode是第一个节点，tail也应该指向pNode;
	  第一次写DLinkedList时，insertHead和insertTail部分未连接起来，所以要注意增加!head部分。
	*/
	if (!head) {
		tail = pNode;
	}
	else {
		head->previous = pNode;
	}
	head = pNode;
	cout << "DLinkedList insertHead element: " << element << endl;
	return *this;
}

template<typename T> DLinkedList<T>& DLinkedList<T>::insertTail(T element) {
	NodeDLinkedList<T>* pNode = new NodeDLinkedList<T>(element);
	/* 下面方法有待改进 * 
	if (tail == NULL) {
		pNode->previous = tail;
		tail = pNode;
	}
	pNode->previous = tail;
	tail->next = pNode;
	tail = pNode;
	/* 这种方法简洁 */
	pNode->previous = tail;
	if (!tail) {
		head = pNode;
	}
	else {
		tail->next = pNode;
	}
	tail = pNode;
	cout << "DLinkedList insertTail element: " << element << endl;
	return *this;
}

template<typename T> bool DLinkedList<T>::erase(T element) {
	NodeDLinkedList<T>* pNode = new NodeDLinkedList<T>();
	NodeDLinkedList<T>* pDeleted = NULL;
	bool bDeleted = false;
	for (pNode = head; pNode != NULL; ) {
		if (pNode->data == element) {
			/*	如果pNode指向非头结点，则维护pNode->previous->next,如果指向头结点，则维护头结点；
				如果pNode指向非tail节点，则维护pNode->next->previous，如果指向tail，则维护tail
			*/
			if (pNode == head) {
				head = head->next;
			}
			else {
				pNode->previous->next = pNode->next;
			}
			if (pNode == tail) {
				tail = tail->previous;
			}
			else {
				pNode->next->previous = pNode->previous;
			}
			pDeleted = pNode;			
		}
		/*	对pNode的更新要放在if外面 */
		pNode = pNode->next;
		if (pDeleted) {
			delete pDeleted;
			pDeleted = NULL;
			bDeleted = true;
			cout << "DLinkedList erase element: " << element << endl;
		}
	}
	return bDeleted;
}

template<typename T> NodeDLinkedList<T>* DLinkedList<T>::locate(T element) {

}

/**
template<typename T> NodeDLinkedList<T>* DLinkedList<T>::retrieve(NodeDLinkedList<T>* p) {

}
/**/



template<typename T> void DLinkedList<T>::toList() {
	NodeDLinkedList<T>* pNode = new NodeDLinkedList<T>();
	cout << "\nDLinkedList head->";
	for (pNode = head; pNode!= NULL; pNode = pNode->next) {
		cout << pNode->data << "<-->";
	}
	cout << "NULL\n\nDLinkedList tail->";
	for (pNode = tail; pNode != NULL; pNode = pNode->previous) {
		cout << pNode->data << "<--->";
	}
	cout << "NULL" << endl;
}

#endif