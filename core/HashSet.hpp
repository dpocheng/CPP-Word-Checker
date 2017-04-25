// HashSet.hpp
//
// ICS 46 Spring 2015
// Project #3: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::vector, std::list, or std::array).  Instead, you'll need
// to use a dynamically-allocated array and your own linked list
// implementation; the linked list doesn't have to be its own class,
// though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename T>
class HashSet : public Set<T>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction
    typedef std::function<unsigned int(const T&)> HashFunction;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet();

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.  In the case
    // where the array is resized, this function runs in linear time (with
    // respect to the number of elements, assuming a good hash function);
    // otherwise, it runs in constant time (again, assuming a good hash
    // function).
    virtual void add(const T& element);


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    virtual bool contains(const T& element) const;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const;

    struct Node
    {
        T data;
        Node* next;
    };


private:
    HashFunction hashFunction;

    // store the current capacity
    int expandableCapacity;

    // store the size of hashset
    int numberOfElements;

    // a Node array for storing element
    Node** hashNode;

    // helper function for destructor
    void deallocate(Node* n);
};



template <typename T>
HashSet<T>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{
    hashNode = new Node*[10];
    expandableCapacity = 10;
    numberOfElements = 0;
    for (int i = 0; i < expandableCapacity; i++)
    {
        hashNode[i] = NULL;
    }
}


template <typename T>
HashSet<T>::~HashSet()
{
    for (int i = 0; i < expandableCapacity; i++)
    {
        deallocate(hashNode[i]);
    }
    delete[] hashNode;
}


template <typename T>
HashSet<T>::HashSet(const HashSet& s)
    : hashFunction{s.hashFunction}
{
    this->expandableCapacity = s.expandableCapacity;
    this->hashNode = s.hashNode;
}


template <typename T>
HashSet<T>& HashSet<T>::operator=(const HashSet& s)
{
    if (this != &s)
    {
        this->hashFunction = s.hashFunction;
        this->expandableCapacity = s.expandableCapacity;
        this->hashNode = s.hashNode;
    }
    return *this;
}


template <typename T>
bool HashSet<T>::isImplemented() const
{
    return true;
}


template <typename T>
void HashSet<T>::add(const T& element)
{
    if (!contains(element))
    {
        numberOfElements++;
        unsigned int hashCode = hashFunction(element);
        double exceedCapacity = numberOfElements/expandableCapacity;
        if (exceedCapacity >= 0.8)
        {
            expandableCapacity = expandableCapacity*2;
            Node** newHashNode = new Node*[expandableCapacity];
            for (int i = 0; i < expandableCapacity; i++)
            {
                newHashNode[i] = NULL;
            }
            int index = 0;
            while (index < expandableCapacity/2)
            {
                newHashNode[index] = hashNode[index];
                index++;
            }
            delete[] hashNode;
            hashNode = newHashNode;
         }
        Node* prev = NULL;
        Node* entry = hashNode[hashCode];
        while (entry != NULL)
        {
            prev = entry;
            entry = entry->next;
        }
        if (entry == NULL)
        {
            entry = new Node;
            if (prev == NULL)
            {
                hashNode[hashCode] = entry;
            }
            else
            {
                prev->next = entry;
            }
        }
        else
        {
            entry->data = element;
            entry->next = NULL;
        }
    }
}


template <typename T>
bool HashSet<T>::contains(const T& element) const
{
    bool isContained = false;
    /*bool check = true;
    unsigned int hashCode = hashFunction(element);
    Node* entry = hashNode[hashCode];
    while (entry != NULL && check)
    {
        if (entry->data == element)
        {
            isContained = true;
            check = false;
        }
        entry = entry->next;
    }*/
    return isContained;
}


template <typename T>
unsigned int HashSet<T>::size() const
{
    return numberOfElements;
}


template <typename T>
void HashSet<T>::deallocate(Node* n)
{
    if (n != NULL)
    {
        deallocate(n->next);
        delete n;
    }
}



#endif // HASHSET_HPP

