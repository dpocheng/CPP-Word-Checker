// AVLSet.hpp
//
// ICS 46 Spring 2015
// Project #3: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector).  Instead, you'll need
// to implement your AVL tree using your own dynamically-allocated nodes,
// with pointers connecting them, and with your own balancing algorithms
// used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include "Set.hpp"
#include <string>



template <typename T>
class AVLSet : public Set<T>
{
public:
    // Initializes an AVLSet to be empty.
    AVLSet();

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet();

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    virtual bool isImplemented() const;

    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    virtual void add(const T& element);

    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    virtual bool contains(const T& element) const;

    // size() returns the number of elements in the set.
    virtual unsigned int size() const;

    // declare a structure for AVL
    struct Node {
        T data;
        Node* left;
        Node* right;
        bool isCurrentNodeAdded;
        //bool isLeftNodeAdded;
        //bool isRightNodeAdded;
    };

private:
    // declare a root Node for AVL class
    Node* root;

    // declare a variable to count the size of AVL
    int numberOfElements;

    // helper function for add()
    void insert(Node*& n, const T& element);

    // balance() function is to balance the AVLSet
    void balance(Node*& n);

    // helper function for balance() to find the height
    int findHeight(Node* n);

    // helper function for balance() to find the difference between two sides
    int heightDifference(Node* n);

    // helper function for contains()
    const bool find(Node* n, const T& element) const;

    // helper function for deallocate memory
    void deallocate(Node* n);
};


template <typename T>
AVLSet<T>::AVLSet()
{
    // initialize root
    root = new Node;
    root->isCurrentNodeAdded = false;
    //root->isLeftNodeAdded = false;
    //root->isRightNodeAdded = false;
    root->left = NULL;
    root->right = NULL;

    // initialize numberOfElements as 0, since the AVL is empty
    numberOfElements = 0;
}


template <typename T>
AVLSet<T>::~AVLSet()
{
    // call helper function to avoid memory leak
    deallocate(root);
}


template <typename T>
AVLSet<T>::AVLSet(const AVLSet& s)
{
    //copy class variables from s
    this->root = new Node(s.root);
    this->numberOfElements = s.numberOfElements;
}


template <typename T>
AVLSet<T>& AVLSet<T>::operator=(const AVLSet& s)
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
bool AVLSet<T>::isImplemented() const
{
    // no comment......
    return true;
}


template <typename T>
void AVLSet<T>::add(const T& element)
{
    // add element to AVL if it doesn't contain in the set and balance
    if (!contains(element))
    {
        // call the helper function to do insert function
        insert(root, element);
        // numberOfElements +1 when a element is added to the set
        numberOfElements++;
    }
}


template <typename T>
bool AVLSet<T>::contains(const T& element) const
{
    // call the helper functioin
    return find(root, element);
}


template <typename T>
unsigned int AVLSet<T>::size() const
{
    // return the size of AVL
    return numberOfElements;
}


template <typename T>
void AVLSet<T>::insert(Node*& n, const T& element)
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
            if (n->left == NULL)
            {
                n->left = new Node;
                n->left->data = element;
                //n->isLeftNodeAdded = true;
                n->left->isCurrentNodeAdded = true;
                //n->left->isLeftNodeAdded = false;
                //n->left->isRightNodeAdded = false;
                n->left->left = NULL;
                n->left->right = NULL;
                balance(n);
            }
            else
            {
                insert(n->left, element);
                balance(n);
            }
        }
        else
        {
            // check if right node is exist or not
            if (n->right == NULL)
            {
                n->right = new Node;
                n->right->data = element;
                //n->isRightNodeAdded = true;
                n->right->isCurrentNodeAdded = true;
                //n->right->isLeftNodeAdded = false;
                //n->right->isRightNodeAdded = false;
                n->right->left = NULL;
                n->right->right = NULL;
                balance(n);
            }
            else
            {
                insert(n->right, element);
                balance(root);
            }
        }
    }
}


template <typename T>
void AVLSet<T>::balance(Node*& n)
{
    int balanceHeight = heightDifference(n);
    if (balanceHeight > 1)
    {
        int leftBalanceHeight = heightDifference(n->left);
        if (leftBalanceHeight > 0)
        {
            // do ll rotation
            Node* temp;
            temp = n->left;
            n->left = temp->right;
            temp->right = n;
            n = temp;
        }
        else
        {
            // do lr rotation
            Node* temp1;
            temp1 = n->left;
            Node* temp2;
            temp2 = temp1->right;
            temp1->right = temp2->left;
            temp2->left = temp1;
            n->left = temp2;
            Node* temp3;
            temp3 = n->left;
            n->left = temp3->right;
            temp3->right = n;
            n = temp3;
        }
    }
    else if (balanceHeight < -1)
    {
        int rightBalanceHeight = heightDifference(n->right);
        if (rightBalanceHeight > 0)
        {
            // do rl rotation
            Node* temp1;
            temp1 = n->right;
            Node* temp2;
            temp2 = temp1->left;
            temp1->left = temp2->right;
            temp2->right = temp1;
            n->right = temp2;
            Node* temp3;
            temp3 = n->right;
            n->right = temp3->left;
            temp3->left = n;
            n = temp3;
        }
        else
        {
            // do rr rotation
            Node* temp;
            temp = n->right;
            n->right = temp->left;
            temp->left = n;
            n = temp;
        }
    }
}


template <typename T>
int AVLSet<T>::findHeight(Node* n)
{
    int height = 0;
    if (n != NULL)
    {
        int leftHeight = findHeight(n->left);
        int rightHeight = findHeight(n->right);
        int max_height = (leftHeight > rightHeight) ? leftHeight : rightHeight;
        height = max_height + 1;
    }
    return height;
}


template <typename T>
int AVLSet<T>::heightDifference(Node* n)
{
    int leftHeight = findHeight(n->left);
    int rightHeight = findHeight(n->right);
    int balanceHeight = leftHeight - rightHeight;
    return balanceHeight;
}


template <typename T>
const bool AVLSet<T>::find(Node* n, const T& element) const
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
            if (n->left != NULL)
            {
                return find(n->left, element);
            }
        }
        else if (n->data.compare(element) < 0)
        {
            if (n->right != NULL)
            {
                return find(n->right, element);
            }
        }
        return false;
    }
}


template <typename T>
void AVLSet<T>::deallocate(Node* n)
{
    // go through every node. If it exists, delete it
    if(n != NULL)
    {
        deallocate(n->left);
        deallocate(n->right);
        delete n;
    }
}



#endif // AVLSET_HPP

