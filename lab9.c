#include <stdio.h>
#include <stdlib.h>
#define TABLE_SIZE 31

// RecordType
struct RecordType {
  int id;
  char name;
  int order;
};

// HashType
struct HashType {
  struct RecordType* table[TABLE_SIZE]; 
};

// Compute hash function  
int hash(int key) {
  return key % TABLE_SIZE;
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
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
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
void displayRecordsInHash(struct HashType* pHashArray, int hashSz) {
  for (int i = 0; i < hashSz; i++) {
    if (pHashArray->table[i] != NULL) {
      printf("index %d -> ", i);
      for (int j = 0; pHashArray->table[i][j].id != 0; j++) {
        printf("%d %c %d -> ", pHashArray->table[i][j].id, 
               pHashArray->table[i][j].name, pHashArray->table[i][j].order);
      }
      printf("\n");
    }
  }
}

int main(void) {
  struct RecordType* pRecords;
  int recordSz = 0;

  recordSz = parseData("input.txt", &pRecords);
  printRecords(pRecords, recordSz);

  struct HashType hashTable;
  for (int i = 0; i < TABLE_SIZE; i++) {
    hashTable.table[i] = NULL;
  }

  for (int i = 0; i < recordSz; i++) {
    int idx = hash(pRecords[i].id);
    if (hashTable.table[idx] == NULL) {
      hashTable.table[idx] = malloc(sizeof(struct RecordType)); 
    } else {
      hashTable.table[idx] = realloc(hashTable.table[idx], 
                                     (i+1)*sizeof(struct RecordType));
    }
    hashTable.table[idx][i] = pRecords[i];
  }

  displayRecordsInHash(&hashTable, TABLE_SIZE);
  
  // Free memory
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (hashTable.table[i] != NULL) {
      free(hashTable.table[i]); 
    }
  }

  return 0;
}