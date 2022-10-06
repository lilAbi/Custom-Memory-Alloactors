#include "LinearAllocator.h"

ASL::LinearAllocator::LinearAllocator(int const _bufferSize) : Allocator(_bufferSize){
    bufferStartPtr = std::malloc(totalSize);
    bufferOffsetPtr = bufferStartPtr;
}


ASL::LinearAllocator::~LinearAllocator() {
    free(bufferStartPtr);
}

void* ASL::LinearAllocator::Allocate(int allocationSize, int alignment) {
    //remaining size
    std::size_t remainingSize = totalSize-usedAmount;

    //returns aligned ptr, or nullptr if space too small
    void* alignedPtr = std::align(alignment, allocationSize, bufferOffsetPtr, remainingSize);

    if(alignedPtr != nullptr){
        //get size that the alignment took up
        unsigned long alignmentSize = std::uintptr_t(alignedPtr) - std::uintptr_t(bufferOffsetPtr);
        //add the alignment size and allocation size to used amount
        usedAmount += int(alignmentSize + allocationSize);
        //advance the buffer offset by the allocation size
        bufferOffsetPtr = (void*)(std::uintptr_t(alignedPtr) + allocationSize);
    }

    return alignedPtr;
}

void ASL::LinearAllocator::Free(void *address) {
    assert(false && "This allocator type does not support this feature,use Reset()");
}

void ASL::LinearAllocator::Reset() {
    //free and reallocated buffe
    std::free(bufferStartPtr);
    bufferStartPtr = std::malloc(totalSize);
    //set offsetPtr to starting ptr
    bufferOffsetPtr = bufferStartPtr;
    //reset used amount
    usedAmount = 0;
}

