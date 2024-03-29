/*
 * list.h
 *
 *  Created on: May 30, 2019
 *      Author: OS1
 */

#ifndef H_LIST_H_
#define H_LIST_H_

#include <iostream.h>
#include "utils.h"

class ThreadList {
private:
	struct ThreadListElement {
		ThreadListElement(Thread *elem) {
			next = 0;
			data = elem;
		}
		ThreadListElement *next;
		Thread *data;
	};

public:
	ThreadList() {
		head = tail = 0;
	}

	void add(Thread *elem) {
		if (head == 0) {
			head = tail = new ThreadListElement(elem);
			return;
		}
		ThreadListElement *new_item = new ThreadListElement(elem);
		tail->next = new_item;
		tail = new_item;
	}

	void remove(Thread *elem) {
		if (head == 0) return;
		if (head->data == elem) {
			if (tail == head) tail = 0;
			ThreadListElement *tmp = head;
			head = head->next;
			delete tmp;
			return;
		}
		ThreadListElement *it = head;
		while (it->next != tail) {
			if (it->next->data == elem) {
				ThreadListElement *tmp = it->next;
				it->next = tmp->next;
				if (tail == tmp) tail = it;
				delete tmp;
				return;
			}
			it = it->next;
		}
		if (tail->data == elem) {
			ThreadListElement *tmp = tail;
			tail = it;
			delete tmp;
			tail->next = 0;
		}
		return;
	}

	Thread *find(ID id) {
		if (head == 0) return 0;
		if (head == tail && head->data->getId() == id) return head->data;
		ThreadListElement *it = head;
		while (it != 0) {
			if (it->data->getId() == id) return it->data;
			it = it->next;
		}
		return 0;
	}

	// TODO: remove this
	int count() {
		ThreadListElement *it = head;
		int ret = 0;
		while (it != 0) {
			it = it->next;
			ret++;
		}
		return ret;
	}

	// TODO: remove this
	void print_state() {
		ThreadListElement *it = head;
		while (it != 0) {
//			lock;
			cout << it->data->getId() << ", ";
//			unlock;
			it = it->next;
		}
	}

private:
	ThreadListElement *head;
	ThreadListElement *tail;
};

extern ThreadList threads;

class PCBList {
private:
	struct PCBListElement {
		PCBListElement(PCB *elem, int rt = -1) {
			next = 0;
			data = elem;
			restore_time = rt;
		}
		PCBListElement *next;
		PCB *data;
		unsigned long restore_time;
	};

public:
	PCBList() {
		head = tail = 0;
	}

	void add(PCB *elem) {
		if (head == 0) {
			head = tail = new PCBListElement(elem);
			return;
		}
		PCBListElement *new_item = new PCBListElement(elem);
		tail->next = new_item;
		tail = new_item;
	}

	void add_sleeper(PCB *elem, unsigned long sleep_amount) {
		Time wakeup_time = total_time + sleep_amount;
		PCBListElement *add = new PCBListElement(elem, wakeup_time);
		if (head == 0) {
			head = tail = add;
			return;
		}
		if (head == tail) {
			if (head->restore_time < wakeup_time) {
				tail = add;
			} else {
				head = add;
			}
			head->next = tail;
			return;
		}
		if (head->restore_time > wakeup_time) {
			add->next = head;
			head = add;
			return;
		}
		PCBListElement *it = head;
		while (it->next != 0 && it->next->restore_time < wakeup_time) it = it->next;
		if (it->next == 0) {
			it->next = add;
			tail = add;
		}
		add->next = it->next;
		it->next = add;
	}

	int get_head_restore_time() {
		if (head == 0) return -1;
		else return head->restore_time;
	}

	int has_head() {
		if (head == 0) return 0;
		else return 1;
	}

	void remove(PCB *elem) {
		if (head == 0) return;
		if (head == tail) {
			delete head;
			head = tail = 0;
			return;
		}
		if (head->data == elem) {
			if (tail == head) tail = 0;
			PCBListElement *tmp = head;
			head = head->next;
			delete tmp;
			return;
		}
		PCBListElement *it = head;
		while (it->next != tail) {
			if (it->next->data == elem) {
				PCBListElement *tmp = it->next;
				it->next = tmp->next;
				if (tail == tmp) tail = it;
				delete tmp;
				return;
			}
			it = it->next;
		}
		if (tail->data == elem) {
			PCBListElement *tmp = tail;
			tail = it;
			delete tmp;
			tail->next = 0;
		}
		return;
	}

	PCB* pop() {
		if (tail == 0) return 0;
		if (head == tail) {
			PCB *ret = head->data;
			delete head;
			head = tail = 0;
			return ret;
		}
		PCBListElement *tmp = head;
		head = head->next;
		PCB *ret = tmp->data;
		delete tmp;
		return ret;
	}

private:
	PCBListElement *head;
	PCBListElement *tail;
};

// doubly linked list

//template <typename T>
//class list {
//private:
//	struct list_element {
//		list_element(T& new_data) {
//			next = prev = 0;
//			data = new_data;
//		}
//		list_element *next, *prev;
//		T data;
//	};
//
//public:
//	list() {
//		len = 0;
//		head = tail = 0;
//	}
//
//	void push_back(const T& elem) {
//		len++;
//		if (len == 0) {
//			head = tail = new list_element(elem);
//			return;
//		}
//		list_element *new_item = new list_element(elem);
//		new_item->prev = tail;
//		tail->next = new_item;
//		tail = new_item;
//	}
//
//	void push_front(const T& elem) {
//		len++;
//		if (len == 0) {
//			head = tail = new list_element(elem);
//			return;
//		}
//		list_element *new_item = new list_element(elem);
//		new_item->next = head;
//		head->prev = new_item;
//		head = new_item;
//	}
//
//	T& pop_back() {
//		len--;
//		if (len == 1) {
//			T ret = head->data;
//			delete head;
//			head = tail = 0;
//			return ret;
//		}
//		list_element *ret_element = tail;
//		tail = tail->prev;
//		T ret = ret_element->data;
//		delete ret_element;
//		return ret;
//	}
//
//	T& pop_front() {
//		len--;
//		if (len == 1) {
//			T ret = head->data;
//			delete head;
//			head = tail = 0;
//			return ret;
//		}
//		list_element *ret_element = head;
//		head = head->next;
//		T ret = ret_element->data;
//		delete ret_element;
//		return ret;
//	}
//
//	unsigned size() {
//		return len;
//	}
//
////	void find(T& elem);
//
//private:
//	unsigned len;
//	list_element *head;
//	list_element *tail;
//};

#endif /* H_LIST_H_ */
