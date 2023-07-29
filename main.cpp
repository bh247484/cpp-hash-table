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

void freeMemCallback(void* value)
{
    printf("Callback - [ Addr: %p, Val: %d ]\n", value, *(int*)value);
}

int main() {
    
    /**
     * Test variable declarations. They'll be iterated over and added to the dictionary.
     * We're using integers as test values here so you'll see some *(int*) dereferencing/casting for log outputs.
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
    
    // Dynamically allocate dictionary size based on user input.
    int size;
    printf("Enter a dictionary size (integer).\n\n");
    printf("Try a constrained size like 4 to test the hash table's linked list collision handling.\n");
    printf("Or try a larger value (>= 10) to see the hash keys distributed more evenly across slots.\n\n");
    printf(">");
    // Some compilers complain about `scanf()` security vulnerabilities so used std::cin instead.
    std::cin >> size;
    printf(" ^ Dictionary size\n");
    
    // Heap allocating here so that we have finer control of obj lifecycle.
    // This way we can manually trigger the freeMemCallbacks when `dictionary`, along with its Entries, is deleted.
    SimpleDictionary* dictionary = new SimpleDictionary(size);

    printf("\n> Initialize dictionary entries.\n");
    for (int i = 0; i < testKeys.size(); ++i) {
        dictionary->Add(testKeys[i].c_str(), &testVals[i], &freeMemCallback);
    }
    
    printf("\n\n> Initial hash table state.");
    dictionary->logTable();

    // Begin Test Suite
    
    // Test 1, Find.
    printf("\n> Test 1 - Finding an Entry.\n");
    void* voidAllocation = nullptr;
    void** outDubPtr = &voidAllocation;
    bool foundEntry = dictionary->Find("key5", outDubPtr);
    printf(".Find()'s boolean return value: %s\n", foundEntry ? "'true'" : "'false'");

    printf("\n>> Compare Values.\n");
    printf("Local: %d\nDictionary: %d\n", testVals[5], *(int*)*outDubPtr);
    
    printf("\n>> Compare Memory Addresses.\n");
    printf("Local: %p\nDictionary: %p\n", &testVals[5], *outDubPtr);
    
    // Test 2, Find error handling.
    printf("\n> Test 2 - Accessing an Entry that doesn't exist.\n");
    bool unfoundEntry = dictionary->Find("not-a-key", outDubPtr);
    printf(".Find()'s boolean return value: %s\n", unfoundEntry ? "'true'" : "'false'");
    
    // Test 3, Update.
    printf("\n> Test 3 - Updating an Entry.\n");
    int newVal = 11;
    dictionary->Add("key1", &newVal, &freeMemCallback);
    
    printf("\n>> Confirm key was updated.\n");
    dictionary->Find("key1", outDubPtr);
    
    // Test 4, Remove.
    printf("\n> Test 4 - Removing an Entry.\n");
    dictionary->Remove("key5");

    printf("\n>> Confirm key no longer exists.\n");
    bool removedEntry = dictionary->Find("key5", outDubPtr);
    printf(".Find()'s boolean return value: %s", removedEntry ? "'true'" : "'false'");
    
    printf("\n\n> Final hash table state.");
    dictionary->logTable();
    
    // Deleting dictionary here will trigger freeMemCallback's for each Entry.
    // The callbacks print the memory address and dereferenced int val of each Entry's value field.
    delete dictionary;
    
    printf("\n\n^^ Scroll up to view program's test suite output. ^^\n\n");
    
    return 0;
}
