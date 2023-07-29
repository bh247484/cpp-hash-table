#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "SimpleDictionary.h"
// NOTE: No additional includes should be added to this file besides the ones above.

SimpleDictionary::SimpleDictionary(int size)
    : m_tableSize(size)
{
    // Dynamically allocate memory for Entry pointer array.
    Entry** initEntries = new Entry*[m_tableSize] { nullptr };
    m_table.slots = initEntries;
}

SimpleDictionary::~SimpleDictionary()
{
    // Cleanup heap allocations.
    for (int i = 0; i < m_tableSize; ++i) {
        Entry* targetEntry = m_table.slots[i];
        
        while (targetEntry != NULL) {
            // Capture linkedEntry before we remove current Entry.
            Entry* nextEntry = targetEntry->linkedEntry;
            // Free Entry memory, using delete so Entry deconstructor is called.
            delete targetEntry;
            // Recurse through linked list where applicable.
            targetEntry = nextEntry;
        }
    }
    // We dynamically allocated this mem in the constructor, we need to cleanup here.
    free(m_table.slots);
}

bool SimpleDictionary::Add(const char* pKey, void* pValue, void (*pfnFreeValue)(void*))
{
    int hashedKey = hashStr(pKey);
    
    Entry* targetEntry = m_table.slots[hashedKey];
    
    Entry* newEntry = new Entry {
        pKey,
        pValue,
        pfnFreeValue,
        nullptr,
    };
    
    // Entry not yet initialized, no collisions/chains possible.
    // Add new Entry.
    if (targetEntry == NULL) {
        m_table.slots[hashedKey] = newEntry;
        printf("%s %s => %d\n", "New:", pKey, *(int*)pValue);
        return true;
    }
    
    // Entry with this hash already registered.
    // Traverse linkedEntries linked list.
    Entry* previousEntry;
    while (targetEntry != NULL) {
        // If pKey matches key of Entry, we're updating an existing Entry.
        if (strcmp(targetEntry->key, pKey) == 0) {
            targetEntry->value = pValue;
            targetEntry->callback = pfnFreeValue;
            printf("%s %s => %d\n\n", "Update:", pKey, *(int*)pValue);
            return true;
        }
        
        // If no match, continue traversing list.
        previousEntry = targetEntry;
        targetEntry = previousEntry->linkedEntry;
    }
    
    // No existing Entry found in linked list.
    // Add new Entry as linkedEntry to last Entry in list.
    previousEntry->linkedEntry = newEntry;
    printf("%s %s => %d\n", "New via linked list:", pKey, *(int*)pValue);
    
    return true;
}

bool SimpleDictionary::Remove(const char* pKey)
{
    int hashedKey = hashStr(pKey);
    Entry* targetEntry = m_table.slots[hashedKey];
    
    if (targetEntry == NULL) {
        printf("%s\n", "Entry does not exist, it cannot be removed.");
        return false;
    }
    
    // Traverse linked list and remove target Entry node.
    Entry* previousEntry = nullptr;
    while (targetEntry != NULL) {
        // Check current Entry against pKey.
        if (strcmp(targetEntry->key, pKey) == 0) {
            // First list item with no linked children, nullify entire slot.
            if (previousEntry == NULL && targetEntry->linkedEntry == NULL) {
                m_table.slots[hashedKey] = nullptr;
            }
            
            // First list item with linked child.
            if (previousEntry == NULL && targetEntry->linkedEntry) {
                m_table.slots[hashedKey] = targetEntry->linkedEntry;
            }
            
            // If list item has parent and child, remove it and link its parent + child.
            if (previousEntry && targetEntry->linkedEntry) {
                previousEntry->linkedEntry = targetEntry->linkedEntry;
            }
            
            // Last list item, nullify previousEntry's link.
            if (previousEntry && targetEntry->linkedEntry == NULL) {
                previousEntry->linkedEntry = nullptr;
            }
            
            // Finally, free Entry memory.
            // Using `delete` so that Entry deconstructor is called.
            delete targetEntry;

            printf("%s\n", "Entry removed.");
            return true;
        }
        
        // If no match, continue traversing list.
        previousEntry = targetEntry;
        targetEntry = previousEntry->linkedEntry;
    }
    
    // Cleanup Entry.
    delete targetEntry;
    m_table.slots[hashedKey] = nullptr;
    
    printf("%s\n", "Entry removed.");
    return true;
}

bool SimpleDictionary::Find(const char* pKey, void** pOutValue) const
{
    int hashedKey = hashStr(pKey);
    Entry* targetEntry = m_table.slots[hashedKey];
    
    if (targetEntry == NULL) {
        printf("%s\n", "Entry does not exist.");
        return false;
    }
    
    while (targetEntry != NULL) {
        if (strcmp(targetEntry->key, pKey) == 0) {
            printf("%s: %s => %d\n", "Found", pKey, *(int*)targetEntry->value);
            *pOutValue = targetEntry->value;
            return true;
        }
        
        // Traverse linked list where applicable.
        targetEntry = targetEntry->linkedEntry;
    }
    
    printf("Entry: '%s' not found.\n", pKey);
    return false;
}

void SimpleDictionary::logTable()
{
    printf("\n\n");
    printf("---- Log Table Start ----\n");
    printf("\n");
    // Loop through all Entries in table
    for (int i = 0; i < m_tableSize; ++i) {
        Entry* targetEntry = m_table.slots[i];
        
        // Don't print null Entries.
        if (targetEntry == NULL) {
            continue;
        }
        
        printf("Slot %d: | ", i);
        
        while (targetEntry != NULL) {
            printf("%s => %d | ", targetEntry->key, *(int*)targetEntry->value);
            targetEntry = targetEntry->linkedEntry;
        }
        
        printf("\n");
    }
    printf("\n");
    printf("---- Log Table End ----\n");
    printf("\n");
}

int SimpleDictionary::hashStr(const char* inputStr ) const {
    // Encountered edge case negative hashVals so coercing positive integers here with unsigned.
    unsigned int hashVal = 0;
    
    for (int i = 0; i < strlen(inputStr); ++i) {
        hashVal = hashVal * 37 + inputStr[i];
    }
    
    // Scale by number of slots in hash m_table.
    return hashVal % m_tableSize;
}
