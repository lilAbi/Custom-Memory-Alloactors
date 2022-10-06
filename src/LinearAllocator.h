#ifndef MEMORYALLOCATOR_LINEARALLOCATOR_H
#define MEMORYALLOCATOR_LINEARALLOCATOR_H

#include "Allocator.h"

namespace ASL{

    class LinearAllocator : public Allocator {
    public:
        explicit LinearAllocator(int _bufferSize);
        ~LinearAllocator() override;

        void* Allocate(int allocationSize, int alignment) override;
        void Free(void* ptr) override;

        //"free" memory;
        virtual void Reset();

        //no copy & no move
        LinearAllocator(LinearAllocator const &rhs) = delete;
        LinearAllocator(LinearAllocator const &&rhs) = delete;

    private:
        //offset of free memory
        void* bufferOffsetPtr{nullptr};
        //ptr to buffer
        void* bufferStartPtr{nullptr};
    };

}


#endif //MEMORYALLOCATOR_LINEARALLOCATOR_H
