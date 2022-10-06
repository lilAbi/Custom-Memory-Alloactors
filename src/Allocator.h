
#ifndef MEMORYALLOCATOR_ALLOCATOR_H
#define MEMORYALLOCATOR_ALLOCATOR_H

#include <memory>
#include <cassert>
#include <cstdint>
#include <iostream>

namespace ASL {

    //interface for standard allocator
    class Allocator{
    public:
        //default construction
        explicit Allocator(unsigned long _totalSize): totalSize{_totalSize} {};
        //don't forget to make the destructor virtual
        virtual  ~Allocator() = default;

        //return a ptr to the start of the allocated chunk
        virtual void* Allocate(int allocationSize, int alignment) = 0;
        //free memory
        virtual void Free(void* address) = 0;

        //no copy & no moving
        Allocator(Allocator const &rhs) = delete;
        Allocator(Allocator const &&rhs) = delete;

    protected:
        unsigned long totalSize;
        unsigned long usedAmount{0};
    };


}

#endif //MEMORYALLOCATOR_ALLOCATOR_H
