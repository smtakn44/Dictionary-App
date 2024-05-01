/*
Define a structure 'node' with two fields:
  - turkish (array of 20 characters)
  - english (array of 20 characters)

Main Program:
  Initialize startIndex, updateCount, and size to -1, 0, and 0 respectively
  Declare an array 'dictArr' of 'node' structures of size MaxSize
  Declare an array 'nextArr' of integers of size MaxSize
  Declare arrays 'turWord' and 'engWord' to store Turkish and English words
  Declare 'turWordtoUpdate' to store the Turkish word to be updated
  Declare 'option' to store the user's menu choice

  Repeat until the user chooses to quit:
    Display the main menu options
    Get the user's choice
    Perform an action based on the user's choice:
      - If option 1: Prompt for a Turkish and English word and add the entry
      - If option 2: Prompt for a Turkish word and delete the entry
      - If option 3: Prompt for the current Turkish word, new Turkish and English words, and update the entry
      - If option 4: Print all dictionary entries in order
      - If option 5: Exit the program
      - Otherwise: Display an invalid choice message

Function turkish_strcmp:
    Initialize a string representing the Turkish alphabet order.
    Compare characters of two strings based on their order in the Turkish alphabet.
    If characters differ, return the difference in their order positions.
    Continue comparison until the end of either string is reached.
    If one string ends, determine which string is greater based on remaining characters.
    Return 0 if both strings are identical in Turkish alphabetical order.

Function addAnEntry:
  If the dictionary is full, display a message and return
  Otherwise, add the new entry to 'dictArr' and 'nextArr'
  If the dictionary is empty, set the first entry
  Otherwise, find the correct position and insert the new entry

Function deleteAnEntry:
  If the dictionary is empty, display a message and return
  Otherwise, find the entry to delete
  If found, rearrange 'nextArr' and replace the deleted entry with the last entry
  Decrease the size of the dictionary

Function updateAnEntry:
  If the dictionary is empty, display a message and return
  Otherwise, find the entry to update
  If found, rearrange 'nextArr' and update the entry with new words
  Reinsert the updated entry in the correct position
  Increment the update count

Function printEntriesInOrder:
  If the dictionary is empty, display a message and return
  Otherwise, iterate through 'nextArr' and print each entry

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MaxSize 9

struct node
{
    char turkish[20];
    char english[20];
};

int turkish_strcmp(const char *str1, const char *str2);
void addAnEntry(char turWord[], char engWord[], struct node dictArr[], int nextArr[], int *size, int *startIndex);
void deleteAnEntry(char turWord[], struct node dictArr[], int nextArr[], int *size, int *startIndex);
void updateAnEntry(char turWordtoUpdate[], char turWord[], char engWord[], struct node dictArr[], int nextArr[], int *size, int *startIndex, int *updateCount);
void printEntriesInOrder(struct node dictArr[], int nextArr[], int size, int startIndex, int updateCount);

int main()
{
    int startIndex = -1;
    int updateCount = 0;
    int size = 0;

    struct node dictArr[MaxSize];
    int nextArr[MaxSize];

    char turWord[20];
    char engWord[20];

    char turWordtoUpdate[20];

    int option;
    do
    {
        printf("\n\n *****MAIN MENU *****");
        printf("\n 1: Add an entry");
        printf("\n 2: Delete an entry");
        printf("\n 3: Update the entry");
        printf("\n 4: Print the entries in order");
        printf("\n 5: Quit\n");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            printf("Enter Turkish word:\n");
            scanf("%s", turWord);
            printf("Enter English word:\n");
            scanf("%s", engWord);
            addAnEntry(turWord, engWord, dictArr, nextArr, &size, &startIndex);
            break;
        case 2:
            printf("Enter Turkish word to delete:\n");
            scanf("%s", turWord);
            deleteAnEntry(turWord, dictArr, nextArr, &size, &startIndex);
            break;
        case 3:
            printf("Enter Turkish word that you want to update :\n");
            scanf("%s", turWordtoUpdate);
            printf("Enter new Turkish word :\n");
            scanf("%s", turWord);
            printf("Enter new English word:\n");
            scanf("%s", engWord);
            updateAnEntry(turWordtoUpdate, turWord, engWord, dictArr, nextArr, &size, &startIndex, &updateCount);
            break;
        case 4:
            printEntriesInOrder(dictArr, nextArr, size, startIndex, updateCount);
            break;
        case 5:
            break;

        default:
            puts("Invalid Choice");
            break;
        }

    } while (option != 5);

    return 0;
}

int turkish_strcmp(const char *str1, const char *str2)
{
    const char *turkishAlphabet = "aAbBcCçÇdDeEfFgGğĞhHıIiİjJkKlLmMnNoOöÖpPrRsSşŞtTuUüÜvVyYzZ";

    int index1, index2;
    while (*str1 && *str2)
    {
        // Find the index of the characters in the Turkish order string
        char *ptr1 = strchr(turkishAlphabet, *str1);
        char *ptr2 = strchr(turkishAlphabet, *str2);
        index1 = (ptr1 == NULL) ? -1 : ptr1 - turkishAlphabet;
        index2 = (ptr2 == NULL) ? -1 : ptr2 - turkishAlphabet;

        // If indexes are different, return the difference, this means they are different letters
        if (index1 != index2)
            return index1 - index2;

        // Move to the next character (case of same letters)
        str1++;
        str2++;
    }

    // If both strings ended, they are equal
    if (*str1 == '\0' && *str2 == '\0')
        return 0;

    // If only str1 ended, str2 is greater
    if (*str1 == '\0')
        return -1;

    // If only str2 ended, str1 is greater
    return 1;
}

void addAnEntry(char turWord[], char engWord[], struct node dictArr[], int nextArr[], int *size, int *startIndex)
{
    if (*size >= MaxSize)
    {
        // if the array is full
        puts("Array is full! You cannot add\n");
        return;
    }

    // we add to dictArr straightly
    strcpy(dictArr[*size].turkish, turWord);
    strcpy(dictArr[*size].english, engWord);

    // If the dictionary is empty, it will be the first entry
    if (*startIndex == -1)
    {
        *startIndex = 0;
        nextArr[*size] = -1; // End of the list
    }
    else
    {
        // Find the correct position to insert the new entry
        int current = *startIndex;
        int previous = -1;
        while (current != -1 && turkish_strcmp(turWord, dictArr[current].turkish) <= 0)
        // if you find any strcmp(turWord, dictArr[current].turkish) > 0, then you insert between previous and current
        {
            previous = current;
            current = nextArr[current];
        }

        // Inserting a new entry
        if (previous == -1) // Insert at the beginning
        {
            nextArr[*size] = *startIndex;
            *startIndex = *size;
        }
        else // Insert in the middle or at the end
        {
            nextArr[previous] = *size;
            nextArr[*size] = current;
        }
    }

    (*size)++;
}

void deleteAnEntry(char turWord[], struct node dictArr[], int nextArr[], int *size, int *startIndex)
{
    if (*size == 0)
    {
        printf("Array is empty! You cannot delete \n");
        return;
    }

    int current = *startIndex;
    int previous = -1;
    int var = 0;

    // Find the entry to delete
    while (current != -1 && turkish_strcmp(dictArr[current].turkish, turWord) != 0)
    {
        previous = current;
        current = nextArr[current];
    }

    // If the word is found
    if (current != -1)
    {
        var = 1;
        // start index and nextArr connections arrangements
        if (previous == -1)
        {
            *startIndex = nextArr[current]; // If the word is at the start just change the starting index
        }
        else
        {
            nextArr[previous] = nextArr[current]; // else, make prev->next = prev->next->next
        }

        // If the node we delete is not the last element, move the last element to this deleted location
        if (current != *size - 1)
        {
            strcpy(dictArr[current].turkish, dictArr[*size - 1].turkish);
            strcpy(dictArr[current].english, dictArr[*size - 1].english);
            nextArr[current] = nextArr[*size - 1];

            // last element was moved so the node whose next is last element, will point to moved location
            for (int i = 0; i < *size; i++)
            {
                if (nextArr[i] == *size - 1)
                {
                    nextArr[i] = current;
                    break;
                }
            }
        }

        (*size)--;
    }

    if (!var)
    {
        printf("Word is not in the dictionary.\n");
    }
}

void printEntriesInOrder(struct node dictArr[], int nextArr[], int size, int startIndex, int updateCount)
{
    if (startIndex == -1)
    {
        printf("Dictionary is empty.\n");
        printf("Update Count: %d\n", updateCount);
        return;
    }

    int i = startIndex;
    while (i != -1)
    {
        printf("Turkish: %s - English: %s\n", dictArr[i].turkish, dictArr[i].english);
        i = nextArr[i];
    }
    printf("Update Count: %d\n", updateCount);
}

void updateAnEntry(char turWordtoUpdate[], char turWord[], char engWord[], struct node dictArr[], int nextArr[], int *size, int *startIndex, int *updateCount)
{
    if (*size == 0)
    {
        printf("Array is empty! You cannot update \n");
        return;
    }

    int upd_idx = *startIndex; // current location of element that we want to update
    int upd_idx_prev = -1;

    // Find the entry to update
    while (upd_idx != -1 && turkish_strcmp(dictArr[upd_idx].turkish, turWordtoUpdate) != 0)
    {
        upd_idx_prev = upd_idx;
        upd_idx = nextArr[upd_idx];
    }

    // If the word is found
    if (upd_idx != -1)
    {
        // start index and nextArr connections arrangements
        if (upd_idx_prev == -1)
        {
            *startIndex = nextArr[upd_idx]; // If the word is at the start just change the starting index
        }
        else
        {
            nextArr[upd_idx_prev] = nextArr[upd_idx]; // else, make prev->next = prev->next->next
        }

        // update words
        strcpy(dictArr[upd_idx].turkish, turWord);
        strcpy(dictArr[upd_idx].english, engWord);

        // update next
        int current = *startIndex;
        int previous = -1;
        while (current != -1 && turkish_strcmp(turWord, dictArr[current].turkish) <= 0)
        // if you find any turkish_strcmp(turWord, dictArr[current].turkish) > 0, then you insert between previous and current
        {
            previous = current;
            current = nextArr[current];
        }

        if (previous == -1) // Insert at the beginning
        {
            nextArr[upd_idx] = *startIndex;
            *startIndex = upd_idx;
        }
        else // Insert in the middle or at the end
        {
            nextArr[previous] = upd_idx;
            nextArr[upd_idx] = current;
        }

        (*updateCount)++;
    }
    else
    {
        printf("Word is not in the dictionary.\n");
    }
}