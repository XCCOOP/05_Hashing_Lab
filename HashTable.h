//DO NOT CHANGE THIS FILE
//Author: Bo Brinkman
//Date: 2013/07/24
#include "USet.h"

/*
 * Note: Just above your template declaration when you use this class, you
 * must define method called "hash" that takes a Key as input, and returns
 * an unsigned long (which is the hash value)
 *
 * For example, you might do:
 * unsigned long hash(char c){ return 10*((unsigned long)c)%backingArraySize; }
 * HashTable<char,int> mySillyTable;
 *
 * If you don't define an appropriate hash function, the class won't compile.
 */

template <class Key, class T>
class HashTable : public USet <Key, T> {
 private:
  class HashRecord {
  public:
    Key k;
    T x;

    //If the slot in the hash table is totally empty, set this to true.
    bool isNull;

    //If the slot used to have something in it, but doesn't now, set
    // isDel to true, and isNull to false. isNull is only for slots
    // that have never been used
    bool isDel;

    HashRecord() { isNull = true; isDel = false; };
  };

 public:
  //See USet.h for documentation of these methods
  virtual unsigned long size();
  virtual void add(Key k, T x);
  virtual void remove(Key k);
  virtual T find(Key k);
  virtual bool keyExists(Key k);

  //Initialize all private member variables.
  HashTable();
  //Delete any dynamically allocated memory.
  virtual ~HashTable();

private:
  //A pointer to the array that holds the hash table data
  HashRecord* backingArray;

  //Whenever numItems + numRemoved >= backingArraySize/2, call
  // grow(). grow() should make a new backing array that is twice the
  // size of the old one, similar to what we did in the ArrayQueue
  // lab.
  //Note: You cannot just loop through the old array and copy it to the
  // new one! Since the backing array size has changed, each item will likely
  // map to a different slot in the array. You may just want to use add()
  // after initializing the new array.
  void grow();

  //This helper method should take a key, and return the index for that
  // item within the hash table. If the item already exists, return the
  // index of the existing item. If the item doesn't exist, return the index
  // where it OUGHT to be. This function can then be used as a helper method in
  // your other methods.
  unsigned long calcIndex(Key k);

  unsigned long numItems; //Number of items in the hash table

  //Note: Ordinarily, these OUGHT to be private. In this case I have
  // made them public for easy of testing.
 public:
  unsigned long numRemoved; //Number of slots that have been removed but not re-used. Those that have isDel == true
  unsigned long backingArraySize;
};

//You will need this so you can make a string to throw in
// remove
#include <string>


//Author: Nick Cooperrider
//Date: 10/7/2014
//All method implementations from here down are all mine with help from in class and the online book.


template <class Key, class T>
HashTable<Key,T>::HashTable(){
	backingArraySize = hashPrimes[0];
	backingArray = new HashRecord[backingArraySize];
	numItems = 0;
	numRemoved = 0;
}

template <class Key, class T>
HashTable<Key,T>::~HashTable() {
  delete[] backingArray;
}

template <class Key, class T>
unsigned long HashTable<Key,T>::calcIndex(Key k){
	int startPoint = hash(k)%backingArraySize;
	int newStart = startPoint;

	if(k == backingArray[startPoint].k) {
		return startPoint;
	}

	while(k != backingArray[newStart].k && backingArray[newStart].isNull == false) {
		newStart = (newStart++ % backingArraySize);
	}

	if(numRemoved == 0) {
		return newStart;
	} else {
		while(backingArray[startPoint].isDel == false) {
			startPoint = (startPoint++ % backingArraySize);
		}
		return startPoint;
	}
}

template <class Key, class T>
void HashTable<Key,T>::add(Key k, T x){
	if(keyExists(k) == false) {
		numItems++;
		if(numItems + numRemoved >= backingArraySize/2) {
			grow();
		}
		int i = calcIndex(k);
		backingArray[i].k = k;
		backingArray[i].x = x;
		backingArray[i].isNull = false;
		backingArray[i].isDel = false;

	}
}

template <class Key, class T>
void HashTable<Key,T>::remove(Key k){
	numItems--;
	numRemoved++;
	backingArray[calcIndex(k)].isDel = true;
}

template <class Key, class T>
T HashTable<Key,T>::find(Key k){
	
	int i = calcIndex(k);
	while(backingArray[i].isNull == false) {
		if(backingArray[i].isDel == false && backingArray[i].k == k) {
			return backingArray[i].x;
		}
		i = (i == backingArraySize-1) ? 0 : i + 1;
	}
	throw std::string("Key does not exist");
}

template <class Key, class T>
bool HashTable<Key,T>::keyExists(Key k){
	return (k == backingArray[calcIndex(k)].k && backingArray[calcIndex(k)].isDel == false);
}

template <class Key, class T>
unsigned long HashTable<Key,T>::size(){
  return numItems;
}

template <class Key, class T>
void HashTable<Key,T>::grow(){
  numItems = 0;
  numRemoved = 0;

  
  unsigned long tempArraySize = backingArraySize;
  unsigned long i = 0;
  while(hashPrimes[i] <= tempArraySize) {
	  i++;
  }
  tempArraySize = hashPrimes[i];
  HashRecord* oldBackingArray = new HashRecord[backingArraySize];
  unsigned long oldBackingArraySize = backingArraySize;

  for(unsigned int i = 0; i<oldBackingArraySize; i++){
		oldBackingArray[i] = backingArray[i];
	}

  delete[] backingArray;

  HashRecord* tempArray = new HashRecord[tempArraySize];

  backingArray = tempArray;
  backingArraySize = tempArraySize;

  for(unsigned int i = 0; i < backingArraySize; i++) {
	  if(oldBackingArray[i].isNull == false && oldBackingArray[i].isDel == false) {
		  add(oldBackingArray[i].k,oldBackingArray[i].x);
	  }
  }

  delete[] oldBackingArray;
  delete[] tempArray;
}

