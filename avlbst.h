
#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the height as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int getHeight () const;
    void setHeight (int height);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int height_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), height_(1)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the height of a AVLNode.
*/
template<class Key, class Value>
int AVLNode<Key, Value>::getHeight() const
{
    return height_;
}

/**
* A setter for the height of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setHeight(int height)
{
    height_ = height;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    bool parentHeightChange(AVLNode<Key, Value>* node);

    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);

    bool checkIfBalanced(AVLNode<Key, Value>* node);

    int identifyImbalanceType(AVLNode<Key, Value>* z, AVLNode<Key, Value>* y, AVLNode<Key, Value>* x);

    AVLNode<Key,Value>* getGreaterChild(AVLNode<Key, Value>* node);

    void updateHeight(AVLNode<Key, Value>* node);

    void rightRotate(AVLNode<Key, Value>* node);

    void leftRotate(AVLNode<Key, Value>* node);

    int caclHeight(AVLNode<Key, Value>* node);

    void removeFix(AVLNode<Key, Value>* n);

    AVLNode<Key,Value>* getG(AVLNode<Key, Value>* c, AVLNode<Key, Value>* n);

    bool willHeightChange( AVLNode<Key,Value>* n);

    void rightRotateForRemove(AVLNode<Key, Value>* z);

    void leftRotateForRemove(AVLNode<Key, Value>* z);

};


template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // TODO 
    // ** BST's Insert**

    // CASE 1: Empty Tree
    if(this->root_ == nullptr){

        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second ,nullptr); 

        return;
    }

    // CASE 2: Non-Empty Tree
    int direction = 0;  // 2 = left child, 1 = right child
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);

    AVLNode<Key, Value>* prev;

    while(temp != nullptr){
        prev = temp;

        if(new_item.first > temp->getKey()){

            direction = 1;

            temp = temp->getRight();
        }
        else if(new_item.first < temp->getKey()){

            direction = 2;

            temp = temp->getLeft();
        }
        else if(new_item.first == temp->getKey()){   // then overwrite value

            temp->setValue(new_item.second);

            return;
        }
    }
    // create the new node with parent set to prev
    temp = new AVLNode<Key, Value>(new_item.first, new_item.second, prev);

    // set parent's child to new node
    if(direction == 2){

        prev->setLeft(temp);

    } 
    else if(direction == 1){

        prev->setRight(temp);

    }
    

    if(prev->getHeight() == 2){
        return;
    }
    else if(prev->getHeight() == 1){
        prev->setHeight(2);
        insertFix(prev, temp);
    }

}



// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************



template<class Key, class Value>
void AVLTree<Key,Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node)
{

    if(parent == nullptr){
        
        return;
    }

    AVLNode<Key, Value>* grandParent = parent->getParent();

    if(grandParent== nullptr){

        return;
    }    
    
    // if grandParent Height did  not change we are done 
    if(!parentHeightChange(parent)){

        return;

    }
    
    updateHeight(grandParent);

    // CHECK if grandParent is balanced
    
    if(checkIfBalanced(grandParent)){

        insertFix(grandParent, parent);

    } else{    // else grandParent is not balanced and grandParent = z

        AVLNode<Key, Value>* z = grandParent;

        AVLNode<Key, Value>* y = getGreaterChild(z);

        AVLNode<Key, Value>* x = getGreaterChild(y);

        int imbalanceType = identifyImbalanceType(z, y, x); // z y x 

        if(imbalanceType == 1){     // Fix left zig-zig 

            // Call right rotate
            rightRotate(z);

        }
        else if(imbalanceType == 2){    // Fix left zig - zag

            // Call left rotate then right rotate
            leftRotate(y);
            rightRotate(z);

        }
        else if(imbalanceType == 3){    // Fix right zig-zig
            
            // Call left rotate
            leftRotate(z);

        }
        else if(imbalanceType == 4){    // Fix right zig-zag

            // Call right rotate then left rotate
            rightRotate(y);
            leftRotate(z);

        }

    }
}


template<class Key, class Value>
int AVLTree<Key,Value>::identifyImbalanceType(AVLNode<Key, Value>* z, AVLNode<Key, Value>* y, AVLNode<Key, Value>* x)
{

    // Check left Zig - Zig:
    if(z->getLeft() == y && y->getLeft() == x){

        return 1;

    }

    // Check left Zig - Zag
    else if(z->getLeft() == y && y ->getRight() == x){

        return 2;

    }

    // Check right Zig - Zig
    else if(z->getRight() == y && y ->getRight() == x){

        return 3;

    }

    // Check right Zig - Zag
    else if(z->getRight() == y && y->getLeft() == x){

        return 4;

    }
    return 0;
}


template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{

    AVLNode<Key, Value>* nodeToRemove = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));

    AVLNode<Key, Value>* p;

    // CASE 1: key not in the tree
    if(nodeToRemove == nullptr) {

        return; // Key not found, nothing to remove
    }

    if((nodeToRemove->getLeft() != nullptr) && (nodeToRemove->getRight() != nullptr)){

        AVLNode<Key, Value>* succ = static_cast<AVLNode<Key,Value>*>(this->successor(nodeToRemove));
        nodeSwap(succ, nodeToRemove);
    }

    p = nodeToRemove->getParent();

    // CASE 2: nodeToRemove has no children
    if(nodeToRemove->getLeft()== nullptr && nodeToRemove->getRight()==nullptr){
        
        if(nodeToRemove->getParent() != nullptr) {  // if no children + existing parent -> make the the parent's child null
            if (nodeToRemove->getParent()->getLeft() == nodeToRemove){
                nodeToRemove->getParent()->setLeft(nullptr);
            }
            else if(nodeToRemove->getParent()->getRight() == nodeToRemove){
                nodeToRemove->getParent()->setRight(nullptr);
            }
            delete nodeToRemove;
        }
        else{   // if no children + null parent = root node
            this->root_ = nullptr;
            p = nullptr;
            delete nodeToRemove;
        }
    }

    // CASE 3: nodeToRemove has one child 
    else if((nodeToRemove->getLeft() != nullptr && nodeToRemove->getRight() == nullptr) || (nodeToRemove->getLeft() == nullptr && nodeToRemove->getRight() != nullptr)){

        AVLNode<Key, Value>* child = (nodeToRemove->getLeft() != nullptr) ? nodeToRemove->getLeft() : nodeToRemove->getRight();

        if(nodeToRemove == this->root_){   // if that node with one child is the root 
            this->root_ = child;
            child->setParent(nullptr);
            p = nullptr;
            delete nodeToRemove;
        } else{   // if not a root node then it has a parent 
            AVLNode<Key, Value>* parent = nodeToRemove->getParent();

            if(nodeToRemove->getParent()->getLeft() == nodeToRemove){   // if nodeToRemove is its parent's left child then 
                parent->setLeft(child);
            }else{
                parent->setRight(child);
            }
            child->setParent(parent);

            p = parent;

            delete nodeToRemove;
        }
    }

    removeFix(p);
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n)
{

    // If n is null, return
    if(n == nullptr){

        return;

    }
    
    AVLNode<Key, Value>* p = n->getParent();

    
    // CASE 1: n is out of balance
    if(!checkIfBalanced(n)){  

        AVLNode<Key, Value>* c = getGreaterChild(n);

        AVLNode<Key, Value>* g = getG(c, n);

        int typeImbalance = identifyImbalanceType(n, c, g);

        if(typeImbalance == 1){     // Fix left zig-zig 

            // Call right rotate
            rightRotateForRemove(n);

            updateHeight(n);
            updateHeight(c);

            removeFix(p);
        }
        else if(typeImbalance == 2){    // Fix left zig - zag

            // Call left rotate then right rotate

            leftRotateForRemove(c);
            rightRotateForRemove(n);

            updateHeight(n);
            updateHeight(c);
            updateHeight(g);

            removeFix(p);

        }
        else if(typeImbalance == 3){    // Fix right zig-zig
            
            // Call left rotate
            leftRotateForRemove(n);

            updateHeight(n);
            updateHeight(c);

            removeFix(p);

        }
        else if(typeImbalance == 4){    // Fix right zig-zag

            // Call right rotate then left rotate
            rightRotateForRemove(c);
            leftRotateForRemove(n);

            updateHeight(n);
            updateHeight(c);
            updateHeight(g);

            removeFix(p);
        }   

    }
    else if(!willHeightChange(n)){  // n's height does not to change then done

        return;

    } else{

        updateHeight(n);

        removeFix(p);
    }
}



template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int tempH = n1->getHeight();
    n1->setHeight(n2->getHeight());
    n2->setHeight(tempH);
}


// ************* Rotations *******************
// ********************************************
// ********************************************
// ********************************************
// ********************************************



template<class Key, class Value>
void AVLTree<Key,Value>::leftRotate(AVLNode<Key, Value>* z)
{

    if(z == nullptr){

    }

    AVLNode<Key, Value>* y = z->getRight();

    AVLNode<Key, Value>* p = z -> getParent();

    if(p != nullptr){   // CASE 1: z is not the root and has a parent 


        bool zIsLC;    // if true then z is left child of its parent 

        if(p->getLeft() == z){
            
            zIsLC = true;

        }else{

            zIsLC = false;

        }

        // Y and P connection
        if(zIsLC){

            p->setLeft(y);

        }else{

            p->setRight(y);

        }
        y->setParent(p);

        // Z and Y connection 
        z->setParent(y);

        AVLNode<Key, Value>* oldYLeftChild = y->getLeft();
        y->setLeft(z);

        z->setRight(oldYLeftChild);

        if(oldYLeftChild != nullptr){
            oldYLeftChild->setParent(z);
        }

        // Update heights 
        updateHeight(z);
        updateHeight(y);


    }else{      // CASE 2: Z is the root and doesnt have a parent 

        this->root_ = y;

        y->setParent(nullptr);

        AVLNode<Key, Value>* oldYLeftChild = y->getLeft();

        y->setLeft(z);

        z->setParent(y);
    
        z->setRight(oldYLeftChild);

        if(oldYLeftChild != nullptr){
            oldYLeftChild->setParent(z);
        }

        updateHeight(z);

        updateHeight(y);
    }

}


template<class Key, class Value>
void AVLTree<Key,Value>::rightRotate(AVLNode<Key, Value>* z)
{

    if(z == nullptr){

    }

    AVLNode<Key, Value>* y = z->getLeft();

    AVLNode<Key, Value>* p = z -> getParent();


    if(p != nullptr){   // CASE 1: z is not the root and has a parent 

        bool zIsLC;    // if true then z is left child of its parent 

        if(p->getLeft() == z){
            
            zIsLC = true;

        }else{

            zIsLC = false;

        }
 

        // Y and P connection
        if(zIsLC){

            p->setLeft(y);

        }else{

            p->setRight(y);

        }
        y->setParent(p);

        // Z and Y connection 
        z->setParent(y);

        AVLNode<Key, Value>* oldYRightChild = y->getRight();
        y->setRight(z);

        z->setLeft(oldYRightChild);

        if(oldYRightChild != nullptr){
            oldYRightChild->setParent(z);
        }

        updateHeight(z);
        updateHeight(y);

    }else{      // CASE 2: Z is the root and doesnt have a parent 

        this->root_ = y;

        y->setParent(nullptr);

        AVLNode<Key, Value>* oldYRightChild = y->getRight();

        y->setRight(z);

        z->setParent(y);
    
        z->setLeft(oldYRightChild);

        if(oldYRightChild != nullptr){
            oldYRightChild->setParent(z);
        }

        updateHeight(z);
        updateHeight(y);

    }
}



template<class Key, class Value>
void AVLTree<Key,Value>::leftRotateForRemove(AVLNode<Key, Value>* z)
{

    if(z == nullptr){

    }

    AVLNode<Key, Value>* y = z->getRight();

    AVLNode<Key, Value>* p = z -> getParent();


    if(p != nullptr){   // CASE 1: z is not the root and has a parent 

        bool zIsLC;    // if true then z is left child of its parent 

        if(p->getLeft() == z){
            
            zIsLC = true;

        }else{

            zIsLC = false;

        }

        // Y and P connection
        if(zIsLC){

            p->setLeft(y);

        }else{

            p->setRight(y);

        }
        y->setParent(p);

        // Z and Y connection 
        z->setParent(y);

        AVLNode<Key, Value>* oldYLeftChild = y->getLeft();
        y->setLeft(z);

        z->setRight(oldYLeftChild);

        if(oldYLeftChild != nullptr){
            oldYLeftChild->setParent(z);
        }

        // Update heights 


    }else{      // CASE 2: Z is the root and doesnt have a parent 

        this->root_ = y;

        y->setParent(nullptr);

        AVLNode<Key, Value>* oldYLeftChild = y->getLeft();

        y->setLeft(z);

        z->setParent(y);
    
        z->setRight(oldYLeftChild);

        if(oldYLeftChild != nullptr){
            oldYLeftChild->setParent(z);
        }
    }

}


template<class Key, class Value>
void AVLTree<Key,Value>::rightRotateForRemove(AVLNode<Key, Value>* z)
{

    if(z == nullptr){


    }

    AVLNode<Key, Value>* y = z->getLeft();

    AVLNode<Key, Value>* p = z -> getParent();


    if(p != nullptr){   // CASE 1: z is not the root and has a parent 

        bool zIsLC;    // if true then z is left child of its parent 

        if(p->getLeft() == z){
            
            zIsLC = true;

        }else{

            zIsLC = false;

        }
 

        // Y and P connection
        if(zIsLC){

            p->setLeft(y);

        }else{

            p->setRight(y);

        }
        y->setParent(p);

        // Z and Y connection 
        z->setParent(y);

        AVLNode<Key, Value>* oldYRightChild = y->getRight();
        y->setRight(z);

        z->setLeft(oldYRightChild);

        if(oldYRightChild != nullptr){
            oldYRightChild->setParent(z);
        }


    }else{      // CASE 2: Z is the root and doesnt have a parent 

        this->root_ = y;

        y->setParent(nullptr);

        AVLNode<Key, Value>* oldYRightChild = y->getRight();

        y->setRight(z);

        z->setParent(y);
    
        z->setLeft(oldYRightChild);

        if(oldYRightChild != nullptr){
            oldYRightChild->setParent(z);
        }


    }
}


// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************

// will a particular node's height need to be updated given the current state of its children
template<class Key, class Value>
bool AVLTree<Key, Value>::willHeightChange( AVLNode<Key,Value>* n)
{
    int oldHeight = n->getHeight();

    int leftHeight; 

    int rightHeight;

    if(n->getLeft() != nullptr){

        leftHeight = n->getLeft()->getHeight();

    }else{

        leftHeight = 0;
    }
    
    if(n->getRight() != nullptr){

        rightHeight = n->getRight()->getHeight();

    }else{

        rightHeight = 0;
    }

    int newHeight = std::max(leftHeight, rightHeight) + 1;

    if(newHeight != oldHeight){

        return true;

    } else{

        return false;
    }
    
}


template<class Key, class Value>
AVLNode<Key,Value>* AVLTree<Key,Value>::getG(AVLNode<Key, Value>* c, AVLNode<Key, Value>* n)
{
    if(c == nullptr){

        return nullptr;

    }

    int leftHeight;

    int rightHeight;

    if(c->getLeft() != nullptr){

        leftHeight = c->getLeft()->getHeight();

    }else{

        leftHeight = 0;
    }
    
    if(c->getRight() != nullptr){

        rightHeight = c->getRight()->getHeight();

    }else{

        rightHeight = 0;

    }

    if(rightHeight == leftHeight){

        // return the one that causes a zig zig
        
        if(n->getLeft() == c){

            return c->getLeft();

        }
        else if(n->getRight() == c){

            return c->getRight();

        }

    }

    // assuming right and left are not null

    if(rightHeight > leftHeight) return c->getRight();

    return c->getLeft();   

}




// RETURNS: void. FUNCTION: look's at node's children and update's height 
template<class Key, class Value>
void AVLTree<Key,Value>::updateHeight(AVLNode<Key, Value>* node)
{   

    if(node == nullptr){
        return;
    }

    int leftHeight; 

    int rightHeight;

    if(node->getLeft() != nullptr){

        leftHeight = node->getLeft()->getHeight();

    }else{

        leftHeight = 0;
    }
    
    if(node->getRight() != nullptr){

        rightHeight = node->getRight()->getHeight();

    }else{

        rightHeight = 0;
    }

    int newHeight= std::max(rightHeight, leftHeight) + 1;

    node->setHeight(newHeight);
}


// RETURN: if node is balanced or not
template<class Key, class Value>
bool AVLTree<Key,Value>::checkIfBalanced(AVLNode<Key, Value>* node)
{

    if(node == nullptr){
        return true;
    }

    int leftHeight; 

    int rightHeight;

    if(node->getLeft() != nullptr){

        leftHeight = node->getLeft()->getHeight();


    }else{

        leftHeight = 0;
    }
    
    if(node->getRight() != nullptr){

        rightHeight = node->getRight()->getHeight();


    }else{

        rightHeight = 0;
    }


    if(abs(rightHeight - leftHeight) <= 1){     // node is balanced acorrding to balance factor avl property

        return true;

    }else{      // node not balanced 

        return false;

    }

}

// RETURN: wheather node's parent's height has chanegd or not 
template<class Key, class Value>
bool AVLTree<Key,Value>::parentHeightChange(AVLNode<Key, Value>* node)
{

    AVLNode<Key, Value>* parent = node->getParent();

    int oldParentHeight = parent->getHeight();

    int leftHeight; 

    int rightHeight;

    if(parent->getLeft() != nullptr){

        leftHeight = parent->getLeft()->getHeight();

    }else{

        leftHeight = 0;
    }
    
    if(parent->getRight() != nullptr){

        rightHeight = parent->getRight()->getHeight();

    }else{

        rightHeight = 0;
    }

    int newParentHeight = std::max(leftHeight, rightHeight) + 1;

    if(newParentHeight != oldParentHeight){

        return true;

    } else{

        return false;
    }
}

// RETURN: child with greater height 
template<class Key, class Value>
AVLNode<Key,Value>* AVLTree<Key,Value>::getGreaterChild(AVLNode<Key, Value>* node)
{
    int leftChildH;

    int rightChildH;

    if(node->getLeft() != nullptr){

        leftChildH = node->getLeft()->getHeight();

    }else{

        leftChildH = 0;
    }
    
    if(node->getRight() != nullptr){

        rightChildH = node->getRight()->getHeight();

    }else{

        rightChildH = 0;
    }

    if(leftChildH > rightChildH){

        return node->getLeft();

    }
    else{

        return node->getRight();
    }
}


// RETURNS: height of a node after checking it's rightchild and leftchild
template<class Key, class Value>
int AVLTree<Key,Value>::caclHeight(AVLNode<Key, Value>* node)
{
    int leftChildH;

    int rightChildH;

    if(node->getLeft() != nullptr){

        leftChildH = node->getLeft()->getHeight();

    }else{

        leftChildH = 0;
    }
    
    if(node->getRight() != nullptr){

        rightChildH = node->getRight()->getHeight();

    }else{

        rightChildH = 0;
    }

    return std::max(leftChildH, rightChildH) + 1;
}


#endif

