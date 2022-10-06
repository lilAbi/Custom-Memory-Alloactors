#include <iostream>
#include "RBFreeListAllocator.h"
int main() {

    ASL::RBFreeListAllocator rbFreeListAllocator(2048);


    void* ptr = rbFreeListAllocator.Allocate(32);
    std::cout << rbFreeListAllocator.Allocate(64) << std::endl;
    std::cout << rbFreeListAllocator.Allocate(124) << std::endl;

    std::cout << rbFreeListAllocator.Allocate(45) << std::endl;

    std::cout << rbFreeListAllocator.Allocate(234) << std::endl;
    void* ptr1 = rbFreeListAllocator.Allocate(89);

    std::cout << rbFreeListAllocator.Allocate(321) << std::endl;
    std::cout << rbFreeListAllocator.Allocate(123) << std::endl;

    void* ptr2 = rbFreeListAllocator.Allocate(100);

    std::cout << rbFreeListAllocator.freeList.root->data.chunkSize << std::endl;


    rbFreeListAllocator.Free(ptr, 32);
    rbFreeListAllocator.Free(ptr1, 89);
    rbFreeListAllocator.Free(ptr2, 100);





    return 0;
}
