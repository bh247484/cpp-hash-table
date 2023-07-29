//
//  main.cpp
//  CppExercise
//
//  Created by Brendan Huffman on 7/28/23.
//

#include <iostream>
#include <string>
#include <vector>
#include "SimpleDictionary.h"

void testDealloc(void* value)
{
    printf("Mem callback: %d\n", *(int*)value);
}

int main() {
    
    /**
     * Test variable declarations. They'll be iterated over and added to the dictionary.
     * I'm using integers as values so you'll see some *(int*) dereferencing/casting for log outputs.
     * The value entries in the dictionary are still void pointers though so the logging could be reworked for other data types.
     */
    int testVals[] = {0,1,2,3,4,5,6,7,8,9};
    std::vector<std::string> testKeys = {
        "key0",
        "key1",
        "key2",
        "key3",
        "key4",
        "key5",
        "key6",
        "key7",
        "key8",
        "key9",
    };
    
    /**
     * Dictionary is initialized with a constrained size of just 4 slots.
     * This is to demostrate the linked list style collision handling.
     * After running the program and checking the console output for a dictionary of size 4, try changing the size contructor arg from 4 to 10.
     * You'll notice the hash keys distribute more evenly at values closer to (or above) the total number of entries.
     */
    SimpleDictionary dictionary(4);
    
    printf("\n> Initialize dictionary entries.\n");
    for (auto i = 0; i < testKeys.size(); ++i) {
        dictionary.Add(testKeys[i].c_str(), &testVals[i], &testDealloc);
    }
    
    printf("\n\n> Initial hash table state.");
    dictionary.logTable();
    
    printf("\n> Test updating an existing value.\n");
    int newVal = 11;
    dictionary.Add("key1", &newVal, &testDealloc);


    printf("\n> Test finding a key.\n");
    void* voidAllocation = nullptr;
    void** outDubPtr = &voidAllocation;
    bool foundEntry = dictionary.Find("key5", outDubPtr);
    printf(".Find()'s boolean return value: %s\n", foundEntry ? "'true'" : "'false'");

    printf("\n>> 1. Compare Values.\n");
    printf("Local: %d\nDictionary: %d\n", testVals[5], *(int*)*outDubPtr);
    
    printf("\n>> 2. Compare Memory Addresses.\n");
    printf("Local: %p\nDictionary: %p\n\n", &testVals[5], *outDubPtr);
    
    printf("\n> Test accessing key that doesn't exist.\n");
    bool unfoundEntry = dictionary.Find("not-a-key", outDubPtr);
    printf(".Find()'s boolean return value: %s", unfoundEntry ? "'true'" : "'false'");
    
    printf("\n> Test key removal.\n");
    dictionary.Remove("key5");
    printf("\n>> Confirm key no longer exists.\n");
    bool removedEntry = dictionary.Find("key5", outDubPtr);
    printf(".Find()'s boolean return value: %s", removedEntry ? "'true'" : "'false'");
    
    printf("\n\n> Final hash table state.");
    dictionary.logTable();
    
    return 0;
}
