#ifndef QUEUE_LINKED_LIST_H
#define QUEUE_LINKED_LIST_H

/*	队尾插入，对头删除。顺着链接箭头的方向，对头为第一个元素，队尾为最后一个元素。
	对头插入：pNode->next=head; head=pNode;
	队尾插入：rear->next=pNode; rear=pNode;
*/


template<typename T> struct NodeQueue {
	T data;
	NodeQueue<T>* next;

	NodeQueue(T element = 0) :data(element), next(NULL) { }
};

template<typename T> class QueueLinkedList {
private:
	NodeQueue<T>* front;
	NodeQueue<T>* rear;

public:
	QueueLinkedList():front(NULL), rear(NULL){ }
	bool isEmpty();
	QueueLinkedList<T>& enQueue(T element);
	T deQueue();
	void showtime();
};

template<typename T> bool QueueLinkedList<T>::isEmpty() {
	if (front == NULL) {
		cout << "QueueLinkedList isEmpty: true" << endl;
		return true;
	}
	cout << "QueueLinkedList isEmpty: false" << endl;
	return false;
}

template<typename T> QueueLinkedList<T>& QueueLinkedList<T>::enQueue(T element) {
	NodeQueue<T>* pNode = new NodeQueue<T>(element);
	/**	这里是尾插，而不是头插。
	/**/
	if (rear == NULL) {
		rear = pNode;
		front = pNode;
	}
	else {
		rear->next = pNode;
		rear = pNode;
	}
	cout << "QueueLinkedList enQueue: " <<element<< endl;
	return *this;
}

template<typename T> T QueueLinkedList<T>::deQueue() {
	NodeQueue<T>* pNode = NULL;
	T dataTemp = (T)0;
	if (front == NULL) {
		cout << "QueueLinkedList deQueue: failed" << endl;
		return (T)0;
	}
	pNode = front;
	front = front->next;
	/*	!!! 注意，deQueue到队列为空时，调整rear。
	*/
	if (!front) {
		rear = NULL;
	}
	dataTemp = pNode->data;
	delete pNode;
	pNode = NULL;
	cout << "QueueLinkedList deQueue: " << dataTemp << endl;
	return dataTemp;
}

template<typename T> void QueueLinkedList<T>::showtime() {
	NodeQueue<T>* pNode = front;
	cout << "\nQueueLinkedList showtime front->";
	while (pNode) {
		cout << pNode->data << "->";
		pNode = pNode->next;
	}
	cout << "NULL\n" << endl;
}

#endif