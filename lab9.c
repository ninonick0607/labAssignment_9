#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Node for linked list
struct Node
{
    struct RecordType record;
    struct Node* next;
};

// HashType
struct HashType
{
    int size;
    struct Node** pLists; // Array of linked lists
};

// Compute the hash function
int hash(int x, int size)
{
    return x % size;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    int i;

    for (i = 0; i < hashSz; ++i)
    {
        printf("index %d -> ", i);
        struct Node* currentNode = pHashArray->pLists[i];
        while (currentNode != NULL)
        {
            printf("%d %c %d", currentNode->record.id, currentNode->record.name, currentNode->record.order);
            currentNode = currentNode->next;
            if (currentNode != NULL) printf(" -> ");
        }
        printf("\n");
    }
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);
    // Your hash implementation
    struct HashType hashArray;
    hashArray.size = 10;
    hashArray.pLists = (struct Node**)calloc(hashArray.size, sizeof(struct Node*));

    // Insert records into the hash table
    int i;
    for (i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id, hashArray.size);
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->record = pRecords[i];
        newNode->next = hashArray.pLists[index];
        hashArray.pLists[index] = newNode;
    }
    displayRecordsInHash(&hashArray, hashArray.size);
    return 0;
}
