// BSTSet.hpp
//
// ICS 46 Spring 2015
// Project #3: Set the Controls for the Heart of the Sun
//
// A BSTSet is an implementation of a Set that is a binary search tree,
// albeit one that makes no attempt to remain balanced.  (You explicitly
// should not do any balancing here; do that in AVLSet instead, if you
// choose to work on that one.)
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector).  Instead, you'll need
// to implement your binary search tree using your own dynamically-allocated
// nodes, with pointers connecting them.

#ifndef BSTSET_HPP
#define BSTSET_HPP

#include "Set.hpp"
#include <string>



template <typename T>
class BSTSet : public Set<T>
{
public:
    // Initializes a BSTSet to be empty.
    BSTSet();

    // Cleans up the BSTSet so that it leaks no memory.
    virtual ~BSTSet();

    // Initializes a new BSTSet to be a copy of an existing one.
    BSTSet(const BSTSet& s);

    // Assigns an existing BSTSet into another.
    BSTSet& operator=(const BSTSet& s);


    // isImplemented() should be modified to return true if you've
    // decided to implement a BSTSet, false otherwise.
    virtual bool isImplemented() const;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function runs in O(n) time when there
    // are n elements in the binary search tree, and is sometimes as fast as
    // O(log n) (when the tree is relatively balanced).
    virtual void add(const T& element);


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in O(n) time when there
    // are n elements in the binary search tree, and is sometimes as fast as
    // O(log n) (when the tree is relatively balanced).
    virtual bool contains(const T& element) const;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const;

    // declare a structure for BST
    struct Node {
        T data;
        Node* left;
        Node* right;
        bool isCurrentNodeAdded;
        bool isLeftNodeAdded;
        bool isRightNodeAdded;
    };


private:
    // declare a root Node for BST class
    Node* root;

    // declare a variable to count the size of BST
    int numberOfElements;

    // helper function for add()
    void insert(Node*& n, const T& element);

    // helper function for contains()
    const bool find(Node* n, const T& element) const;

    // helper function for deallocate memory
    void deallocate(Node* n);
};


template <typename T>
BSTSet<T>::BSTSet()
{
    // initialize root
    root = new Node;
    root->isCurrentNodeAdded = false;
    root->isLeftNodeAdded = false;
    root->isRightNodeAdded = false;

    // initialize numberOfElements as 0, since the BST is empty
    numberOfElements = 0;
}


template <typename T>
BSTSet<T>::~BSTSet()
{
    // call helper function to avoid memory leak
    deallocate(root);
}


template <typename T>
BSTSet<T>::BSTSet(const BSTSet& s)
{
    // copy class variables from s
    this->root = new Node(s.root);
    this->numberOfElements = s.numberOfElements;
}


template <typename T>
BSTSet<T>& BSTSet<T>::operator=(const BSTSet& s)
{
    // reallocate to s class variable
    if (this != &s)
    {
        this->root = s.root;
        this->numberOfElements = s.numberOfElements;
    }
    return *this;
}


template <typename T>
bool BSTSet<T>::isImplemented() const
{
    // no comment......
    return true;
}


template <typename T>
void BSTSet<T>::add(const T& element)
{
    // add element to BST if it doesn't contain in the set
    if (!contains(element))
    {
        // call the helper function to do insert function
        insert(root, element);
        // numberOfElements +1 when a element is added to the set
        numberOfElements++;
    }

/*
    // assign element to root if the size of BST is 0
    if (this->size() == 0) {
        root->data = element;
        root->isCurrentNodeAdded = true;
        numberOfElements++;
    }
    // check if element is in the BST
    else if (!this->contains(element)) {
        // create a temp node
        Node* temp = root;
        // a control for while loop
        bool isAdded = false;
        while (!isAdded) {
            // iterate to left node if it exists
            if ((temp->data.compare(element) > 0) && (temp->isLeftNodeAdded)) {
                temp = temp->left;
            }
            // create a left node if it doesn't exist
            else if ((temp->data.compare(element) > 0) && (!temp->isLeftNodeAdded)) {
                temp->left = new Node;
                temp->left->data = element;
                temp->isLeftNodeAdded = true;
                temp->left->isCurrentNodeAdded = true;
                temp->left->isLeftNodeAdded = false;
                temp->left->isRightNodeAdded = false;
                isAdded = true;
            }
            // iterate to right node if it exists
            else if ((temp->data.compare(element) < 0) && (temp->isRightNodeAdded)) {
                temp = temp->right;
            }
            // create a right node if it doesn't exist
            else if ((temp->data.compare(element) < 0) && (!temp->isRightNodeAdded)) {
                temp->right = new Node;
                temp->right->data = element;
                temp->isRightNodeAdded = true;
                temp->right->isCurrentNodeAdded = true;
                temp->right->isLeftNodeAdded = false;
                temp->right->isRightNodeAdded = false;
                isAdded = true;
            }
        }
        // numberOfElements +1 every time when a node is added to BST
        numberOfElements++;
    }
*/
}


template <typename T>
bool BSTSet<T>::contains(const T& element) const
{
    // call the helper function
    return find(root, element);

/*
    // create and copy a node from root
    Node* temp = root;
    // a control for while loop
    bool isNull = false;
    while (!isNull)
    {
        // if it is equal, return true
        if (temp->data == element)
        {
            return true;
        }
        // go to left node if less than 0
        else if (temp->data.compare(element) > 0)
        {
            // check if left node exists
            if (temp->isLeftNodeAdded) {
                temp = temp->left;
            }
            // if left node does not exist, then quit the while loop
            else {
                isNull = true;
            }
        }
        // go to right node if larger than 0
        else
        {
            // check if right node exists
            if (temp->isRightNodeAdded) {
                temp = temp->right;
            }
            // if right node does not exist, then quit the while loop
            else {
                isNull = true;
            }
        }
    }
    return false;
*/
}


template <typename T>
unsigned int BSTSet<T>::size() const
{
    // return the size of BST
    return numberOfElements;
}


template <typename T>
void BSTSet<T>::insert(Node*& n, const T& element)
{
    // if the root node is empty, add element to root
    if (!n->isCurrentNodeAdded)
    {
        n->data = element;
        n->isCurrentNodeAdded = true;
    }
    else
    {
        // check if element should be on left node or not
        if (n->data.compare(element) > 0)
        {
            // check is left node is exist or not
            if (!n->isLeftNodeAdded)
            {
                n->left = new Node;
                n->left->data = element;
                n->isLeftNodeAdded = true;
                n->left->isCurrentNodeAdded = true;
                n->left->isLeftNodeAdded = false;
                n->left->isRightNodeAdded = false;
            }
            else
            {
                insert(n->left, element);
            }
        }
        else
        {
            // check if right node is exist or not
            if (!n->isRightNodeAdded)
            {
                n->right = new Node;
                n->right->data = element;
                n->isRightNodeAdded = true;
                n->right->isCurrentNodeAdded = true;
                n->right->isLeftNodeAdded = false;
                n->right->isRightNodeAdded = false;
            }
            else
            {
                insert(n->right, element);
            }
        }
    }
}


template <typename T>
const bool BSTSet<T>::find(Node* n, const T& element) const
{
    // return true if data match element
    if (n->data == element)
    {
        return true;
    }
    else
    {
        // check if it is on left tree or right tree
        if (n->data.compare(element) > 0)
        {
            if (n->isLeftNodeAdded)
            {
                return find(n->left, element);
            }
        }
        else if (n->data.compare(element) < 0)
        {
            if (n->isRightNodeAdded)
            {
                return find(n->right, element);
            }
        }
        return false;
    }
}


template <typename T>
void BSTSet<T>::deallocate(Node* n)
{
    // go through every node. If it exists, delete it
    if(n != NULL)
    {
        deallocate(n->left);
        deallocate(n->right);
        delete n;
    }
}



#endif // BSTSET_HPP

