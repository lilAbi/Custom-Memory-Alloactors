
#ifndef MEMORYALLOCATOR_RBTREE_H
#define MEMORYALLOCATOR_RBTREE_H


enum TreeColor{
    RED,
    BLACK,
};

template<typename T>
struct Node{
    T data;
    TreeColor color;
    Node* parent;
    Node* left;
    Node* right;
};

template<typename T>
class RBTree {
public:
    using NodeType = Node<T>;
    RBTree() = default;
    ~RBTree() = default;

    //insert
    void InsertNode(NodeType* node);

    //remove
    void RemoveNode(NodeType* node);

    //find
    template <typename CondFunc>
    NodeType* FindNode(CondFunc &&func);

    //get min element
    NodeType* minElement(NodeType* node);

    //get max element
    NodeType* maxElement(NodeType* node);

private:

    NodeType* GetGrandParent(NodeType* node);

    void LeftRotate(NodeType* node);
    void RightRotate(NodeType* node);
    void InsertFix(NodeType* node);

    void Transplant(NodeType* nodeA, NodeType* nodeB);
    void RemoveFix(NodeType* node);

public:
    NodeType* root{nullptr};
};

template<typename T>
void RBTree<T>::InsertNode(RBTree::NodeType* node) {
    auto* currentNode = root;
    NodeType* parentNode = nullptr;
    //traverse through the node
    while(currentNode != nullptr){
        parentNode = currentNode;
        //if node is smaller than current node, address wise
        if(node->data < currentNode->data){
            currentNode = currentNode->left;
        } else if(node->data > currentNode->data) {
            currentNode = currentNode->right;
        }
    }

    node->parent = parentNode;
    //if parentNode is null that mean the root node is null and set as new root
    if(parentNode == nullptr){
        root = node;
        return;
    } else if(node->data < parentNode->data){
        //if node address is smaller than the parent node, add as left child
        parentNode->left = node;
    } else if (node->data > parentNode->data){
        //if node address is bigger than the parent node address, add as right
        parentNode->right = node;
    }

    node->left = nullptr;
    node->right = nullptr;
    node->color = RED;

    //fix the rb tree
    InsertFix(node);
}

template<typename T>
void RBTree<T>::RemoveNode(RBTree::NodeType* node) {
    auto* removedNode = node;
    auto removeNodeOldColor = node->color;
    NodeType* nodeToFix = nullptr;

    if(node->left == nullptr){
        nodeToFix = node->right;
        Transplant(node, node->right);

    } else if(node->right == nullptr){
        nodeToFix = node->left;
        Transplant(node, node->left);

    } else {
        removedNode = minElement(node->right);
        removeNodeOldColor = removedNode->color;
        nodeToFix = removedNode->right;

        if(removedNode != node->right){
            Transplant(removedNode, removedNode->right);
            removedNode->right = node->right;
            removedNode->right->parent = removedNode;
        } else {
            nodeToFix->parent = removedNode;
            Transplant(node, removedNode);
            removedNode->left = node->left;
            removedNode->left->parent = removedNode;
            removedNode->color = node->color;
        }

    }

    if( removeNodeOldColor == BLACK && (nodeToFix != nullptr) ){
        RemoveFix(nodeToFix);
    }

}

template<typename T>
template<typename CondFunc>
typename RBTree<T>::NodeType* RBTree<T>::FindNode(CondFunc&& func) {
    auto* currentNode = root;

    while(currentNode != nullptr){
        std::strong_ordering ordering = func(currentNode);



        if(ordering == std::strong_ordering::less){
            currentNode = currentNode->right;
        } else if (ordering == std::strong_ordering::equal){
            return currentNode;
        } else if (ordering == std::strong_ordering::greater){
            currentNode = currentNode->left;
        }
    }

    return nullptr;
}

template<typename T>
typename RBTree<T>::NodeType* RBTree<T>::minElement(RBTree::NodeType *node) {
    if(auto* currentNode = node; currentNode){
        while(currentNode->left != nullptr){
            currentNode = currentNode->left;
        }
        return currentNode;
    } else {
        return currentNode;
    }
}

template<typename T>
typename RBTree<T>::NodeType* RBTree<T>::maxElement(RBTree::NodeType *node) {
    if(auto* currentNode = node; currentNode){
        while(currentNode->right != nullptr){
            currentNode = currentNode->right;
        }
        return currentNode;
    } else {
        return currentNode;
    }
}

//----------utility ------------------------------------------------------


template<typename T>
typename RBTree<T>::NodeType *RBTree<T>::GetGrandParent(RBTree::NodeType *node) {
    if(node != nullptr && node->parent != nullptr){
        return node->parent->parent;
    }
    return nullptr;
}

template<typename T>
void RBTree<T>::LeftRotate(RBTree::NodeType* node) {
    auto* nodeRightSubTree = node->right;  //get node's right subtree
    node->right = nodeRightSubTree->left;  //set node's right child to nodeRightSubTree's left subtree

    //if y's left child is not null
    if(nodeRightSubTree->left != nullptr){
        nodeRightSubTree->left->parent = node; //its new parent is node
    }
    nodeRightSubTree->parent = node->parent;   //node's parent becomes nodeRightSubTree's parent

    //if node is root
    if(node->parent == nullptr){
        root = nodeRightSubTree;
    } else if (node == node->parent->left){
        //if node was a left child
        node->parent->left = nodeRightSubTree;
    } else {
        //if node was a left child
        node->parent->right = nodeRightSubTree;
    }
    nodeRightSubTree->left = node;     //set node as nodeRightSubTree's left child
    node->parent = nodeRightSubTree;   //set node parent to nodeRightSubTree
}

template<typename T>
void RBTree<T>::RightRotate(RBTree::NodeType* node) {
    auto* nodeLeftSubTree = node->left;
    node->left = nodeLeftSubTree->right;

    if(nodeLeftSubTree->right != nullptr){
        nodeLeftSubTree->right->parent = node;
    }

    nodeLeftSubTree->parent = node->parent;

    if(node->parent == nullptr){
        root = nodeLeftSubTree;
    } else if (node == node->parent->left){
        node->parent->left = nodeLeftSubTree;
    } else {
        node->parent->right = nodeLeftSubTree;
    }
    nodeLeftSubTree->right = node;
    node->parent = nodeLeftSubTree;
}

template<typename T>
void RBTree<T>::InsertFix(RBTree::NodeType* node) {
    //while the node is red

    while(node != nullptr && node->parent != nullptr && node->parent->color == RED){
        if(node->parent == nullptr || node->parent->parent == nullptr){
            break;
        }
        if(node->parent == node->parent->parent->left){
            auto* uncle = node->parent->parent->right;
            if(uncle == nullptr) break;
            if(uncle->color == RED){
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if(node == node->parent->right){
                    node = node->parent;
                    LeftRotate(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                RightRotate(node);
            }

        } else {
            auto* uncle = node->parent->parent->left;
            if(uncle == nullptr) break;
            if(uncle->color == RED){
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if(node == node->parent->left){
                    node = node->parent;
                    RightRotate(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                LeftRotate(node);
            }
        }
    }
    root->color = BLACK;
}

template<typename T>
void RBTree<T>::Transplant(RBTree::NodeType* nodeA, RBTree::NodeType *nodeB) {

    if(nodeA->parent == nullptr){
        root = nodeB;
        return;
    } else if(nodeA == nodeA->parent->left){
        nodeA->parent->left = nodeB;
    } else{
        nodeA->parent->right = nodeB;
    }
    if(nodeB) {
        nodeB->parent = nodeA->parent;
    }
}

template<typename T>
void RBTree<T>::RemoveFix(RBTree::NodeType* node) {

    while( (node != root) && (node->color == BLACK) ){
        if(node == node->parent->left){
            auto* nodeRightSibling = node->parent->right;

            if(nodeRightSibling->color == RED){
                nodeRightSibling->color = BLACK;
                node->parent->color = RED;
                LeftRotate(node->parent);
                nodeRightSibling = node->parent->right;
            }

            if((nodeRightSibling->left->color == BLACK) && (nodeRightSibling->right->color == BLACK)){
                nodeRightSibling->color = RED;
                node = node->parent;
            } else {
                if(nodeRightSibling->right->color == BLACK){
                    nodeRightSibling->left->color = BLACK;
                    nodeRightSibling->color = RED;
                    RightRotate(nodeRightSibling);
                    nodeRightSibling = node->parent->right;
                }
                nodeRightSibling->color = node->parent->color;
                node->parent->color = BLACK;
                nodeRightSibling->right->color = BLACK;
                LeftRotate(node->parent);
                node = root;
            }

        } else {
            auto* nodeLeftSibling = node->parent->left;

            if(nodeLeftSibling->color == RED){
                nodeLeftSibling->color = BLACK;
                node->parent->color = RED;
                RightRotate(node->parent);
                nodeLeftSibling = node->parent->left;
            }

            if((nodeLeftSibling->right->color == BLACK) && (nodeLeftSibling->left->color == BLACK)){
                nodeLeftSibling->color = RED;
                node = node->parent;
            } else {
                if(nodeLeftSibling->left->color == BLACK){
                    nodeLeftSibling->right->color = BLACK;
                    nodeLeftSibling->color = RED;
                    LeftRotate(nodeLeftSibling);
                    nodeLeftSibling = node->parent->left;
                }
                nodeLeftSibling->color = node->parent->color;
                node->parent->color = BLACK;
                nodeLeftSibling->left->color = BLACK;
                RightRotate(node->parent);
                node = root;
            }
        }
    }
    node->color = BLACK;
}


#endif //MEMORYALLOCATOR_RBTREE_H
