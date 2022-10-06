
#ifndef MEMORYALLOCATOR_DOUBLYLINKEDLIST_H
#define MEMORYALLOCATOR_DOUBLYLINKEDLIST_H
#include <iostream>

template<typename T>
struct Node{
    T data;
    Node* next;
    Node* prev;
};



template<typename T>
class DoublyLinkedList {
public:
    using NodeType = Node<T>;

    DoublyLinkedList() = default;
    ~DoublyLinkedList() = default;

    void AddNode(NodeType* node);
    void RemoveNode(NodeType* node);

    template<typename ConditionFunc>
    NodeType* GetNodeBy(ConditionFunc&& conditionFunc);

private:
    NodeType* head{nullptr};
};

template<typename T>
void DoublyLinkedList<T>::AddNode(DoublyLinkedList::NodeType* node) {
    if(head == nullptr){
        head = node;
        node->next = nullptr;
        node->prev = nullptr;
        return;
    }

    //if the node location is before head node location
    if(node < head){
        head->prev = node;
        node->next = head;
        node->prev = nullptr;
        head = node;
        return;
    }

    NodeType* currentNode = head;
    while(currentNode != nullptr){

        if(currentNode->next != nullptr){
            //if between two nodes
            if((currentNode->next > node) && (currentNode < node)){
                currentNode->next->prev = node;
                node->next = currentNode->next;

                currentNode->next = node;
                node->prev = currentNode;
                return;
            }

        } else {
            //end of the list
            currentNode->next = node;
            node->prev = currentNode;
            return;
        }

        currentNode = currentNode->next;
    }
}

template<typename T>
void DoublyLinkedList<T>::RemoveNode(DoublyLinkedList::NodeType* node) {
    if(head == node){
        head = head->next;
        if(head != nullptr) head->prev = nullptr;

        node->prev = nullptr;
        node->next = nullptr;
        return;
    }

    NodeType* currentNode = head;
    while(currentNode != nullptr){

        if(currentNode->next != nullptr){
            if(currentNode == node){
                currentNode->prev->next = currentNode->next;
                currentNode->next->prev = currentNode->prev;

                currentNode->next = nullptr;
                currentNode->prev = nullptr;
                return;
            }
        } else {
            //if node is at the end of the list
            if(currentNode == node){
                currentNode->prev->next = nullptr;

                currentNode->next = nullptr;
                currentNode->prev = nullptr;
                return;
            }

        }
        currentNode = currentNode->next;
    }

}

template<typename T>
template<typename ConditionFunc>
typename DoublyLinkedList<T>::NodeType* DoublyLinkedList<T>::GetNodeBy(ConditionFunc &&conditionFunc) {
    auto* currentNode = head;

    while(currentNode != nullptr){
        //if this is the node we are looking for then return it
        if(bool isNode = conditionFunc(currentNode); isNode){
            return currentNode;
        }
        //move to next node
        currentNode = currentNode->next;
    }

    //no node matching criteria found
    return nullptr;
}


#endif //MEMORYALLOCATOR_DOUBLYLINKEDLIST_H
