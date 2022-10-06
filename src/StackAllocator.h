
#ifndef MEMORYALLOCATOR_STACKALLOCATOR_H
#define MEMORYALLOCATOR_STACKALLOCATOR_H

#include "Allocator.h"

namespace ASL{


    class StackAllocator: public Allocator{
    private:
        struct Header{
            explicit Header(int _size): size{_size} {}
            int size;
        };
    public:
        explicit StackAllocator(int size);
        ~StackAllocator() override;

        void* Allocate(int allocationSize, int alignment) override;
        void Free(void *address) override;

        //no copy & no move
        StackAllocator(StackAllocator const &rhs) = delete;
        StackAllocator(StackAllocator const &&rhs) = delete;

    private:
        void* bufferStartPtr{nullptr};
        void* bufferOffsetPtr{nullptr};
    };

}


#endif //MEMORYALLOCATOR_STACKALLOCATOR_H
