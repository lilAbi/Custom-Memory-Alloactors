
#ifndef MEMORYALLOCATOR_POOLALLOCATOR_H
#define MEMORYALLOCATOR_POOLALLOCATOR_H
#include "Allocator.h"
#include "LinkedList.h"

namespace ASL {

    class PoolAllocator: public Allocator {
    private:
        struct Header{
            int size;
            bool used;
        };
    public:
        PoolAllocator(int _totalSize, int _chunkSize);
        ~PoolAllocator() override;

        //search through linked list and find block that is free and set it used and return ptr
        void* Allocate(int allocationSize, int alignment) override;
        //mark block as free and add it back to the linked list
        void Free(void* address) override;

        //no copy & no move
        PoolAllocator(PoolAllocator const &rhs) = delete;
        PoolAllocator(PoolAllocator const &&rhs) = delete;

    private:
        LinkedList<Header> linkedList;
        int chunkSize;
        void* bufferStartPointer{nullptr};
    };


}


#endif //MEMORYALLOCATOR_POOLALLOCATOR_H
