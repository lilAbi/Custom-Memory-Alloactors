
#ifndef MEMORYALLOCATOR_FREELISTALLOCATOR_H
#define MEMORYALLOCATOR_FREELISTALLOCATOR_H

#include "Allocator.h"
#include "DoublyLinkedList.h"


namespace ASL{

    class FreeListAllocator : public Allocator{
    private:
        struct Header{
            int chunkSize;
            unsigned int padding;
        };

    public:
        explicit FreeListAllocator(unsigned int totalSize);
        ~FreeListAllocator() override;

        void* Allocate(int allocationSize, int alignment) override;
        void Free(void* address) override;


        //no move or copy construction
        FreeListAllocator(FreeListAllocator const &rhs) = delete;
        FreeListAllocator(FreeListAllocator const &&rhs) = delete;
    private:
        void* bufferStartPtr{nullptr};
    public:

        DoublyLinkedList<Header> freeList;
        DoublyLinkedList<Header> allocatedList;
    };

}


#endif //MEMORYALLOCATOR_FREELISTALLOCATOR_H
