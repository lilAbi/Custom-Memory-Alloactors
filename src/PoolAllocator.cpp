
#include "PoolAllocator.h"

ASL::PoolAllocator::PoolAllocator(int _totalSize, int _chunkSize) : Allocator(_totalSize), chunkSize{_chunkSize} {
    assert(chunkSize > sizeof(Node<Header>) && "size of chunk must be bigger than tag size");
    assert(_totalSize % _chunkSize == 0 && "Buffer Size is not a multiple of chunk size");

    //allocate memory for the buffer
    bufferStartPointer = std::malloc(_totalSize);

    //construct nodes at each chunkSize and add to the link list
    for(int i = 0; i <= (_totalSize-_chunkSize); i+=chunkSize){
        void* offsetPtr = (void*)(uintptr_t(bufferStartPointer) + i);
        auto* node = new(offsetPtr) Node<Header>{{chunkSize, false}, nullptr};
        std::cout << "Allocating ptr: " << node << std::endl;
        linkedList.AddNode(node);
    }

}

ASL::PoolAllocator::~PoolAllocator() {
    std::free(bufferStartPointer);
}

void* ASL::PoolAllocator::Allocate(int allocationSize, int alignment) {
    //ptr to be returned
    void* ptr = nullptr;

    //check if allocation size is big enough
    if((allocationSize + sizeof(Node<Header>)) > chunkSize) {
        assert("Allocation size is too small");
        return ptr;
    }

    //find node given a condition function
    auto* node = linkedList.GetNodeBy([](Node<Header>* node) -> bool {
        //if node is in use
        return !(node->data.used);
    });


    //if node is not null
    if(node != nullptr){
        node->data.used = true;
        linkedList.RemoveNode(node);
        ptr = (void*)(uintptr_t(node) + sizeof(Node<Header>));
    }

    return ptr;
}

void ASL::PoolAllocator::Free(void* address) {
    if(address == nullptr){
        return;
    }

    //move address pointer back to node
    auto* node = (Node<Header>*)(uintptr_t(address) - sizeof(Node<Header>));

    //assign true
    node->data.used = false;
    node->next = nullptr;

    //add node back into linked list
    linkedList.AddNode(node);
}
