
#include "StackAllocator.h"


ASL::StackAllocator::StackAllocator(int size) : Allocator(size) {
    bufferStartPtr = malloc(size);
    bufferOffsetPtr = bufferStartPtr;
    std::cout << "Starting Buffer Pointer: " << bufferStartPtr << std::endl;
}

ASL::StackAllocator::~StackAllocator() {
    std::free(bufferStartPtr);
}

void* ASL::StackAllocator::Allocate(int allocationSize, int alignment) {
    //get remaining size left in buffer
    std::size_t remainingSize = totalSize - usedAmount;
    //calculate adjusted size
    std::size_t adjustedSize = allocationSize + sizeof(Header);

    //find aligned ptr
    void* alignedPtr = std::align(alignment, adjustedSize, bufferOffsetPtr, remainingSize);

    if(alignedPtr != nullptr){
        //calculate how much space did the alignment take up
        unsigned long alignmentSize = std::uintptr_t(alignedPtr) - std::uintptr_t(bufferOffsetPtr);
        //update used amount
        usedAmount += (alignmentSize + adjustedSize);
        //construct header
        new(alignedPtr) Header(allocationSize);
        //update new offsetBufferPtr
        bufferOffsetPtr = (void*)(std::uintptr_t(alignedPtr) + adjustedSize);
        //update alignedPtr
        alignedPtr = (void*)(std::uintptr_t(alignedPtr) + sizeof(Header));
    }

    std::cout << "Object pointer: " << alignedPtr << std::endl;
    return alignedPtr;
}

//should be freeing object on the top of the stack
void ASL::StackAllocator::Free(void *address) {
    //check if pointer is withing the range of the buffer
    if((std::uintptr_t(bufferStartPtr) < std::uintptr_t(address)) && (std::uintptr_t(address) < std::uintptr_t(bufferOffsetPtr)) ){

    }



}
