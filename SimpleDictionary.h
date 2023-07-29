#pragma once
// NOTE: No includes should be added to this file.


class SimpleDictionary
{
public:
    SimpleDictionary(int size);
    ~SimpleDictionary();

    // Disabled move/copy constructors and move/assignment operators.
    // These do not need to be implemented.
    SimpleDictionary(SimpleDictionary&&) = delete;
    SimpleDictionary(const SimpleDictionary&) = delete;
    SimpleDictionary& operator=(SimpleDictionary&&) = delete;
    SimpleDictionary& operator=(const SimpleDictionary&) = delete;

    // Add an entry to the dictionary
    //  pKey:         The dictionary key, a NULL terminated string.
    //  pValue:       The pointer to the dictionary value.
    //  pfnFreeValue: Caller-provided function that the dictionary needs to call (using pValue as
    //                the argument) any time it removes the associated entry.
    // Returns true if the key and value were successfully added; otherwise, false.
    bool Add(const char* pKey, void* pValue, void (*pfnFreeValue)(void*));

    // Returns true if the key was successfully removed; otherwise, false.
    bool Remove(const char* pKey);

    // Attempts to find the value associated with the passed key. If the key is found, the value
    // of the key is stored in pOutValue and true is returned; otherwise, false is returned.
    bool Find(const char* pKey, void** pOutValue) const;

    // Prints current state of hash table to console.
    // Organized by slots with each (printed) slot having one or more Entries.
    void logTable();

private:
    // Initialized in the constructor.
    const int m_tableSize;

    struct Entry {
        ~Entry()
        {
            // Run the user provided callback when Entry is destroyed.
            callback(value);
        }
        const char* key;
        void* value;
        void (*callback)(void *);
        Entry* linkedEntry;
    };

    struct HashTable {
        Entry** slots;
    };
    
    HashTable m_table;
    int hashStr(const char* str) const;
};
