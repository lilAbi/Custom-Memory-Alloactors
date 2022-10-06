
#include "RBFreeListAllocator.h"

ASL::RBFreeListAllocator::RBFreeListAllocator(unsigned int totalSize) : Allocator(totalSize) {
    assert(totalSize > sizeof(Node<Header>) && "Total Size smaller than header");
    bufferStartPtr = std::malloc(totalSize);
    //create node with size of allocation block and add it to the freelist
    auto* node = new(bufferStartPtr) Node<Header>{{static_cast<int>(totalSize), 0}, RED, nullptr, nullptr, nullptr};
    freeList.InsertNode(node);
}

ASL::RBFreeListAllocator::~RBFreeListAllocator() {
    free(bufferStartPtr);
}

void *ASL::RBFreeListAllocator::Allocate(int allocationSize) {
    return Allocate(allocationSize, 1);
}


void *ASL::RBFreeListAllocator::Allocate(int allocationSize, int alignment) {
    //return object of aligned ptr to construct w/e object
    void* alignedPtr = nullptr;

    //find node that has a big enough size + satisfy alignment needs
    auto* node = freeList.FindNode([allocationSize, alignment, &alignedPtr](Node<Header>* currentNode) -> std::strong_ordering {
        Header& header = currentNode->data;
        int neededSize = allocationSize + (int)sizeof(Node<Header>);

        //if node has enough size to contain
        if( header.chunkSize >= neededSize ){
            //test if splittable
            if( ((header.chunkSize - neededSize) < (sizeof(Node<Header>) + 1)) && (header.chunkSize - neededSize != 0)){
                return std::strong_ordering::less;
            }

            void* chunkPtrStart = (void*)(uintptr_t(currentNode) + sizeof(Node<Header>));
            size_t trueChunkSize = header.chunkSize - sizeof(Node<Header>);

            void* adjustedPointer = std::align(alignment, allocationSize, chunkPtrStart, trueChunkSize);
            if(adjustedPointer != nullptr){
                alignedPtr = adjustedPointer;
                //return true;
                return std::strong_ordering::equal;
            }
        }

        return std::strong_ordering::less;
    });

    //split free block
    if(node != nullptr){
        //remove node from freelist
        freeList.RemoveNode(node);

        //get old size of free block
        auto oldHeaderSize = node->data.chunkSize;

        //calculate allocated block size & padding
        unsigned int padding = static_cast<int>(uintptr_t(alignedPtr) - uintptr_t(node)) - sizeof(Node<Header>);
        unsigned allocatedBlockSize = padding + allocationSize + sizeof(Node<Header>);

        //update node header to reflect an allocated block
        node->data.chunkSize = static_cast<int>(allocatedBlockSize);
        node->data.padding = padding;

        //if old header size doesn't match allocated block size then split
        if(oldHeaderSize != allocatedBlockSize){
            //Get address at the start of the free node & calculate new free node size
            void* freeNodeStartPtr = (void*)(uintptr_t(node) + allocatedBlockSize);
            unsigned int chunkSize = oldHeaderSize - allocatedBlockSize;
            //construct new free node at that position
            auto* freeNode = new(freeNodeStartPtr) Node<Header>{{static_cast<int>(chunkSize), 0}, RED, nullptr, nullptr, nullptr};
            //add newly made free block
            freeList.InsertNode(freeNode);
        }

        //add node to allocated list
        allocatedList.InsertNode(node);
    }

    return alignedPtr;
}

void ASL::RBFreeListAllocator::Free(void* address) {
    assert(true && "Calling wrong Free function");
}

void ASL::RBFreeListAllocator::Free(void* address, int sizeOfObject) {

    int blockSize = static_cast<int>(sizeof(Node<Header>)) + sizeOfObject;


    //return node that has a matching address
    auto* freeNode = allocatedList.FindNode([address, blockSize](Node<Header>* currentNode) -> std::strong_ordering{
        auto& header = currentNode->data;
        auto order = header.chunkSize <=> blockSize;

        if(order == std::strong_ordering::less){
            return std::strong_ordering::less;
        } else if(order == std::strong_ordering::equal || order == std::strong_ordering::equivalent){
            if(void* objAddr = (void*)(uintptr_t(currentNode) + sizeof(Node<Header>) + header.padding); objAddr == address) {
                std::cout << "found\n";
                return std::strong_ordering::equal;
            } else {
                return std::strong_ordering::less;
            }
        } else {
            return std::strong_ordering::greater;
        }

    });

    //if node is valid remove from allocated list and add it to the free list

    if(freeNode != nullptr){
        allocatedList.RemoveNode(freeNode);
        freeList.InsertNode(freeNode);
    }
}
