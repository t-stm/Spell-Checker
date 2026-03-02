// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int length_categories = 9;
const unsigned int ascii_categories = 9;
const unsigned int N = length_categories * ascii_categories;

// Hash table
node *table[N];

// Declare counter for words in dictionary
unsigned long dictionary_size = 0;

// Set variable to true if dictionary has been loaded
bool dictionary_loaded = false;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash the word
    unsigned int hash_index = hash(word);
    node *ptr = table[hash_index];

    while (ptr != NULL)
    {
        if (strcasecmp(word, ptr->word) == 0)
        {
            return true;
        }

        else if (ptr->next == NULL)
        {
            return false;
        }

        else
        {
            ptr = ptr->next;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int length_index = strlen(word) % length_categories;

    int ascii_sum = 0;

    for (int i = 0; word[i] != '\0'; i++)
    {
        ascii_sum += tolower(word[i]);
    }

    int ascii_index = ascii_sum % ascii_categories;

    return length_index * length_categories + ascii_index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{

    // Open dictionary file
    FILE *source_file = fopen(dictionary, "r");

    // Check that file was opened successfully
    if (source_file == NULL)
    {
        printf("Dictionary couldn't be opened!\n");
        return false;
    }

    // Read strings from dictionary file into buffer

    char *buffer_word = malloc((LENGTH + 1) * sizeof(char));

    while (fscanf(source_file, "%s", buffer_word) != EOF)
    {
        // Allocate memory for new node
        node *n = malloc(sizeof(node));

        // Check that memory has been allocated correctly
        if (n == NULL)
        {
            printf("Incorrect memory allocation!\n");
            return false;
        }

        // Populate node (Word itself)
        strcpy(n->word, buffer_word); // Have to use strcopy() here because using the assignment operator would only copy a pointer
                                      // (since strings are actually char*)

        // Populate node (pointer to next node)
        n->next = NULL;

        // Hash the word to obtain hash value
        unsigned int hash_value = hash(n->word);

        // Find pointer in hash table corresponding to the hash value

        if (table[hash_value] != NULL)
        {
            // If the pointer != NULL, make the pointer in the new node (n->next) point to the head of the list
            n->next = table[hash_value];
        }

        // Make the pointer in the hash table point to the new node
        table[hash_value] = n;

        // Increase dictionary_size counter
        dictionary_size++;
    }

    // Close the dictionary file
    fclose(source_file);

    // Free memory allocated for buffer_word
    free(buffer_word);

    dictionary_loaded = true;
    return true; // check when to return true/false!!
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (dictionary_loaded == true)
    {
        return dictionary_size;
    }

    else
    {
        return 1;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // For all elemtns in the hash table
    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            continue;
        }

        else
        {
            node *ptr = NULL;
            // Travel to the end of the list
            do
            {
                ptr = table[i];
                table[i] = ptr->next;
                free(ptr);
            }
            while (table[i] != NULL);
        }
    }

    return true;
}
