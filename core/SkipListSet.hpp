// SkipListSet.hpp
//
// ICS 46 Spring 2015
// Project #3: Set the Controls for the Heart of the Sun
//
// A SkipListSet is an implementation of a Set that is a skip list, implemented
// as we discussed in lecture.  A skip list is a sequence of levels
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector), *except* that you can use
// std::vector to store your levels (e.g., pointers to the head node of each
// level) if you'd like.  Beyond that, you'll need to implement your own
// dynamically-allocated nodes with pointers connecting them.
//
// Each node should contain only two pointers: one to the node that follows it
// on the same level and another to the equivalent node on the level below it.
// Additional pointers use more memory but don't enable any techniques not
// enabled by the other two.
//
// A couple of utilities are included here: SkipListKind and SkipListKey.
// You can feel free to use these as-is and probably will not need to
// modify them.

#ifndef SKIPLISTSET_HPP
#define SKIPLISTSET_HPP

#include "Set.hpp"
#include <random>
#include <string>
// for storing skip list levels
#include <vector>




// SkipListKind indicates a kind of key: a normal one, the special key
// -INF, or the special key +INF.  It's necessary for us to implement
// the notion of -INF and +INF separately, since we're building a class
// template and not all types of keys would have a reasonable notion of
// -INF and +INF.

enum class SkipListKind
{
    Normal,
    NegInf,
    PosInf
};




// A SkipListKey represents a single key in a skip list.  It is possible
// to compare these keys using < or == operators (which are overloaded here)
// and those comparisons respect the notion of whether each key is normal,
// -INF, or +INF.

template <typename T>
class SkipListKey
{
public:
    SkipListKey(SkipListKind kind, const T& key);

    bool operator==(const SkipListKey& other) const;
    bool operator<(const SkipListKey& other) const;

private:
    SkipListKind kind;
    T key;
};


template <typename T>
SkipListKey<T>::SkipListKey(SkipListKind kind, const T& key)
    : kind{kind}, key{key}
{
}


template <typename T>
bool SkipListKey<T>::operator==(const SkipListKey& other) const
{
    return kind == other.kind
        && (kind != SkipListKind::Normal || key == other.key);
}


template <typename T>
bool SkipListKey<T>::operator<(const SkipListKey& other) const
{
    switch (kind)
    {
    case SkipListKind::NegInf:
        return other.kind != SkipListKind::NegInf;

    case SkipListKind::PosInf:
        return false;

    default: // SkipListKind::Normal
        return other.kind == SkipListKind::PosInf
            || (other.kind == SkipListKind::Normal && key < other.key);
    }
}



template <typename T>
class SkipListSet : public Set<T>
{
public:
    // Initializes an SkipListSet to be empty.
    SkipListSet();

    // Cleans up the SkipListSet so that it leaks no memory.
    virtual ~SkipListSet();

    // Initializes a new SkipListSet to be a copy of an existing one.
    SkipListSet(const SkipListSet& s);

    // Assigns an existing SkipListSet into another.
    SkipListSet& operator=(const SkipListSet& s);

    // isImplemented() should be modified to return true if you've
    // decided to implement a SkipListSet, false otherwise.
    virtual bool isImplemented() const;

    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function runs in an expected time
    // of O(log n) (i.e., over the long run, we expect the average to be
    // O(log n)) with very high probability.
    virtual void add(const T& element);

    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in an expected time of O(log n)
    // (i.e., over the long run, we expect the average to be O(log n))
    // with very high probability.
    virtual bool contains(const T& element) const;

    // size() returns the number of elements in the set.
    virtual unsigned int size() const;

    // structure for skip list
    struct Node
    {
        SkipListKind kind;
        T key;
        Node* folow;
        Node* below;
    };


private:
    // create a vector for storing head pointers
    std::vector<Node*> headPointers;

    // store the height of the skip list
    int height;

    // store the size of the skip list
    int numberOfElements;

    // coinFlip() function for deciding promote a key or not
    bool coinFlip();

    // helper function for destructor
    void deallocate(Node* n);
};


template <typename T>
SkipListSet<T>::SkipListSet()
{
    // creating head and tail node for the level
    Node* head = new Node;
    Node* tail = new Node;
    // assign SkipListKind to head and tail nodes
    head->kind = SkipListKind::NegInf;
    tail->kind = SkipListKind::PosInf;
    // connect tail to head
    head->folow = tail;
    head->below = NULL;
    tail->folow = NULL;
    tail->below = NULL;
    // add the head pointer to vector
    headPointers.push_back(head);
    // initialize height and the size
    height = 0;
    numberOfElements = 0;
}


template <typename T>
SkipListSet<T>::~SkipListSet()
{
    // passing one level per time to destructor helper function
    for (int i = 0; i < headPointers.size(); i++)
    {
        deallocate(headPointers[i]);
    }
}


template <typename T>
SkipListSet<T>::SkipListSet(const SkipListSet& s)
{
    // copy constructor
    this->headPointers = s.headPointers;
    this->height = s.height;
    this->numberOfElements = s.numberOfElements;
}


template <typename T>
SkipListSet<T>& SkipListSet<T>::operator=(const SkipListSet& s)
{
    // copy assignment
    if (this != &s)
    {
        this->headPointers = s.headPointers;
        this->height = s.height;
        this->numberOfElements = s.numberOfElements;
    }
    return *this;
}


template <typename T>
bool SkipListSet<T>::isImplemented() const
{
    // implemented...
    return true;
}


template <typename T>
void SkipListSet<T>::add(const T& element)
{
    // check if the element is existed in the skip list or not
    if(!contains(element))
    {
        numberOfElements++;
        // add the first element to the skip list
        if (height == 0)
        {
            Node* head = new Node;
            head->kind = headPointers[0]->kind;
            head->key = headPointers[0]->key;
            Node* tail = new Node;
            tail->kind = headPointers[0]->folow->kind;
            tail->key = headPointers[0]->folow->key;
            headPointers[0]->below = head;
            headPointers[0]->folow->below = tail;
            Node* newAddedNode = new Node;
            newAddedNode->key = element;
            newAddedNode->folow = head->folow;
            head->folow = newAddedNode;
            height++;
            headPointers.push_back(head);
            // call the coinFlip() function to see if the element is needed to promote
            while (coinFlip())
            {
                Node* newHead = new Node;
                Node* newMiddle = new Node;
                Node* newTail = new Node;
                newHead->kind = headPointers[height]->kind;
                newHead->key = headPointers[height]->key;
                newHead->folow = newMiddle;
                newHead->below = NULL;
                newMiddle->kind = headPointers[height]->folow->kind;
                newMiddle->key = headPointers[height]->folow->key;
                newMiddle->folow = newTail;
                newMiddle->below = NULL;
                newTail->kind = headPointers[height]->folow->folow->kind;
                newTail->key = headPointers[height]->folow->folow->key;
                newTail->folow = NULL;
                newTail->below = NULL;
                headPointers[height]->below = newHead;
                headPointers[height]->folow->below = newMiddle;
                headPointers[height]->folow->folow->below = newTail;
                height++;
                headPointers.push_back(newHead);
            }
        }
        else
        {
            int tempHeight = 1;
            int currentHeight = 0;
            int position = 0;
            Node* head = headPointers.front();
            bool checkingLevel = true;
            // compare strings and nevigate to cooresponging position
            while (checkingLevel)
            {
                if (head->below == NULL)
                {
                    checkingLevel = false;
                }
                else
                {
                    head = head->below;
                    currentHeight++;
                    bool checkingNode = true;
                    while (checkingNode)
                    {
                        // check if the next one is tail
                        if (head->folow->kind == SkipListKind::PosInf)
                        {
                            checkingNode = false;
                        }
                        // check if the next one is behind the element
                        else if (head->folow->key.compare(element) > 0)
                        {
                            while (currentHeight != height)
                            {
                                head = head->below;
                                currentHeight++;
                            }
                            Node* newAddedNode = new Node;
                            newAddedNode->kind = SkipListKind::Normal;
                            newAddedNode->key = element;
                            newAddedNode->folow = head->folow;
                            head->folow = newAddedNode;
                            // same thing as above, but will reorder the vector if the number of same key is over the height and add a level
                           /* while (coinFlip())
                            {
                                Node* thirdTop = headPointers[1];
                                Node* temp = headPointers[height-tempHeight+1];
                                Node* newHead = headPointers[height-tempHeight];
                                for (int i = 0; i < position; i++)
                                {
                                    newHead = newHead->folow;
                                    temp = temp->folow;
                                    thirdTop = thirdTop->folow;
                                }
                                Node* tempLower = temp->folow;
                                tempLower->folow = newHead->folow;
                                tempLower->below = temp->folow;
                                newHead->folow = tempLower;
                                if (tempHeight > height)
                                {
                                    Node* secondTop = headPointers[0];
                                    Node* tempNew = newAddedNode;
                                    tempNew->folow = secondTop->folow;
                                    tempNew->below = thirdTop->folow;
                                    secondTop->folow = tempNew;
                                    std::vector<Node*> tempVec;
                                    for (int i = 0; i < headPointers.size(); i++)
                                    {
                                        if (i == 1)
                                        {
                                            tempVec.push_back(secondTop);
                                        }
                                        tempVec.push_back(headPointers[i]);
                                    }
                                    headPointers = tempVec;
                                }
                                tempHeight++;
                            }*/
                            checkingNode = false;
                            checkingLevel = false;
                        }
                        else
                        {
                            head = head->folow;
                            position++;
                        }
                    }
                }
            }
        }
    }
}


// a function to return random 0 and 1 (false and true) to see if the element is needed to promote
template <typename T>
bool SkipListSet<T>::coinFlip()
{
    std::random_device device;
    std::default_random_engine engine{device()};
    std::uniform_int_distribution<int> distribution{0, 1};
    return distribution(engine);
}


template <typename T>
bool SkipListSet<T>::contains(const T& element) const
{
    bool isContained = false;
    /*bool checkingLevel = true;
    Node* search = headPointers.front();
    while (checkingLevel)
    {
        if (search->below == NULL)
        {
            checkingLevel = false;
        }
        else
        {
            search = search->below;
            bool checkingNode = true;
            while (checkingNode)
            {
                // check if next node is tail
                if (search->folow->kind == SkipListKind::PosInf)
                {
                    checkingNode = false;
                }
                // check if next node is greater than the element
                else if (search->folow->key.compare(element) > 0)
                {
                    checkingNode = false;
                }
                else
                {
                    search = search->folow;
                    // if find matched element, return true
                    if (search->key == element)
                    {
                        checkingNode = false;
                        checkingLevel = false;
                        isContained = true;
                    }
                }
            }
        }
    }*/
    return isContained;
}


template <typename T>
unsigned int SkipListSet<T>::size() const
{
    // return the size of skip list
    return numberOfElements;
}


//helper function for destructor
template <typename T>
void SkipListSet<T>::deallocate(Node* n)
{
    if (n != NULL)
    {
        deallocate(n->folow);
        delete n->below;
        delete n->folow;
        delete n;
    }
}



#endif // SKIPLISTSET_HPP

