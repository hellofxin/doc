#ifndef STACK_ARRAY_H
#define STACK_ARRAY_H

/*  stack 的顺序存储结构的定义、实现及操作，
*/

#define STACK_SIZE 50

template<typename T> class StackArray {
private:
	T elements[STACK_SIZE];
	int top;

public:
	StackArray():top(-1){ }
	bool isEmpty();
	int push(T element);
	T pop();
	T getTop();
	void showtime();
};

template<typename T> bool StackArray<T>::isEmpty() {
	if (top >= 0) {
		cout << "StackArray isEmpty: false" << endl;
		return false;
	}
	cout << "StackArray isEmpty: true" << endl;
	return true;
}

template<typename T> int StackArray<T>::push(T element) {
	if (top >= STACK_SIZE) {
		cout << "stackArray push: failed" << endl;
		return -1;
	}
	top++;
	elements[top] = element;
	cout << "stackArray push: " << top << endl;
	return top;
}

template<typename T> T StackArray<T>::pop() {
	if (top < 0) {
		cout << "stackArray pop: failed" << endl;
		return 0;
	}
	cout << "stackArray pop: " << elements[top] << endl;
	return elements[top--];
}

template<typename T> T StackArray<T>::getTop() {
	if (top < 0) {
		cout << "stackArray top: failed" << endl;
		return 0;
	}
	cout << "stackArray top: " << elements[top] << endl;
	return elements[top];
}

template<typename T> void StackArray<T>::showtime() {
	int index = top;
	cout << "stackArray: top->";
	while (index >= 0) {
		cout<< elements[index--] << "->";
	}
	cout << "NULL\n" << endl;
}

#endif