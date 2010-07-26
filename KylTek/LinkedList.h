#pragma once
#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

/*	- Linked Lists -


	- Functions -

	Constructor - creates the first node for the system
	Destructor - Clears the list (deleting everything inside), then deletes itself
	first (void) - returns the first node* in the list
	add (object) - Adds a new object to the end of the list.
	remove (node) - deletes the object at the specified node and moves the list back, returns the previous node, or the first node in list
	clear (void) - deletes everything inside of the list

	- usage syntax -
	Node = list->first();
	while(Node!=NULL){
		object = Node->object;
		object -> step(); //the objects reoccuring event
		if (object->isDead) //deadflag set?
			delete object;
			Node = list->remove(Node);
		Node = Node -> next;
	}
*/

template <class _Ty>
struct Node{
Node* next;
Node* prev;
_Ty* object;
int sortID;
};

template <class _Ty>
class LinkedList{
private:
	Node<_Ty>* firstnode;
	Node<_Ty>* lastnode;
	int size;
public:

	LinkedList(){
		Init();
	}

	~LinkedList(){
		clear();
	}
	void Init(){
		firstnode = new Node<_Ty>();
		lastnode = firstnode;
		size=0;
	}
	//Returns First Node
	Node<_Ty>* first(){
		return firstnode;
	} 

	//Adds a new element to the array
	void add(_Ty* _object){
		if (_object!=NULL){
			if (lastnode -> object != NULL){
				lastnode->next = new Node<_Ty>();
				lastnode->next->prev = lastnode;
				lastnode = lastnode->next;
				lastnode -> next = NULL;
				lastnode -> object = _object;
			}else{
				lastnode -> object = _object;
			}
			size++;
		}
	}

	int getSize(){
		return size;
	}

	//Parses the node to be deleted
	Node<_Ty>* remove(Node<_Ty>* _node){
		if (_node == lastnode){
			if (_node == firstnode){
				delete lastnode;
				firstnode = new Node<_Ty>;
				lastnode = firstnode;
				--size;
				return firstnode;
			}else{
				lastnode = lastnode ->prev;
				delete lastnode ->next;
				lastnode ->next = NULL;
				--size;
				return lastnode;
			}
		}else if (_node == firstnode){
			if (firstnode->next!=NULL){
				firstnode = firstnode ->next;
				delete firstnode ->prev;
				firstnode ->prev = NULL;
				--size;
				return firstnode;
			}else{
				--size;
				delete firstnode;
				firstnode = new Node<_Ty>;
			}
		}else{
			Node<_Ty>* returnnode = _node ->prev;
			returnnode ->next = _node ->next;
			returnnode ->next->prev = returnnode;
			delete _node;
			--size;
			return returnnode;
		}
		return NULL;
	}

	//permantly deletes everything inside the array
	void clear(){
		Node<_Ty>* clear = firstnode;
		while(clear!=NULL){
			Node<_Ty>* remove = clear;
			clear = clear->next;
			delete remove;
		}
		firstnode = NULL;
		lastnode = NULL;
		clear = NULL;
		size=0;
	}
};

#endif