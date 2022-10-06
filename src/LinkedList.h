
#ifndef MEMORYALLOCATOR_LINKEDLIST_H
#define MEMORYALLOCATOR_LINKEDLIST_H

//object that manages a linked list

template<typename T>
struct Node{
    T data;
    Node* next;
};

template<typename T>
class LinkedList {
public:
    using NodeT = Node<T>;

    LinkedList() = default;
    ~LinkedList() = default;

    template<typename ConditionFunc>
    Node<T>* GetNodeBy(ConditionFunc&& conditionFunc);

    void AddNode(Node<T>* address);
    void RemoveNode(Node<T>* address);

private:
    Node<T>* head{nullptr};
};

template<typename T>
void LinkedList<T>::AddNode(Node<T>* address) {
    //if head ptr is null, set as new head ptr
    if(head == nullptr){
        head = address;
        return;
    }
    //search through list and add at the end
    Node<T>* currentNode = head;
    Node<T>* lastNode = nullptr;
    while(currentNode != nullptr){

        if(currentNode->next == nullptr){
            currentNode->next = address;
            return;
        }

        lastNode = currentNode;
        currentNode = currentNode->next;
    }

}

template<typename T>
void LinkedList<T>::RemoveNode(Node<T>* address) {
    if(head == address){
        head = address->next;
        return;
    }

    Node<T>* currentNode = head;
    Node<T>* lastNode = nullptr;
    while(currentNode != nullptr){
        //if currentNode is address
        if(currentNode == address){
            lastNode->next = currentNode->next;
            return;
        }

        lastNode = currentNode;
        currentNode = currentNode->next;
    }
}

template<typename T>
template<typename ConditionFunc>
typename LinkedList<T>::NodeT* LinkedList<T>::GetNodeBy(ConditionFunc&& conditionFunc) {
    Node<T>* currentNode = head;

    while(currentNode != nullptr){
        bool contains = conditionFunc(currentNode);
        if(contains){
            return currentNode;
        }
        currentNode = currentNode->next;
    }

    return nullptr;
}


#endif //MEMORYALLOCATOR_LINKEDLIST_H
