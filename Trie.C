#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Trie structure
struct Trie
{
    struct Trie *children[26];
    int C;
};

// Inserts the word into the trie structure
struct Trie *createTrie();
void insert(struct Trie *pTrie, char *word)
{
    struct Trie *Curr = pTrie;
    int L = strlen(word);
    for (int i = 0; i < L; i++)
    {
        int IDX = word[i] - 'a';
        if (!Curr->children[IDX]){

            Curr->children[IDX] = createTrie(); 
        }
        Curr = Curr->children[IDX];
    }
    Curr->C++;
}



// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct Trie *Curr = pTrie;
    int L = strlen(word);
    for (int i = 0; i < L; i++)
    {
        int IDX = word[i] - 'a';
        if (!Curr->children[IDX])
        {
            return 0;
        }
        Curr = Curr->children[IDX];
    }
    if (Curr)
    {
        return Curr->C;
    }
    else
    {
        return 0;
    }
}

// Deallocates the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie)
    {
        for (int i = 0; i < 26; i++)
        {
            deallocateTrie(pTrie->children[i]);
        }
        free(pTrie);
    }
    return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *T = (struct Trie *)malloc(sizeof(struct Trie));
    if (T)
    {
        for (int i = 0; i < 26; i++)
        {
            T->children[i] = NULL;
        }
        T->C = 0; // Initialize count to zero
    }
    return T;
}

// This function will return the number of words in the dictionary
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Unable to open file\n");
        exit(1);
    }

    int count = 0;
    char word[4095]; 

    while (fscanf(fp, "%s", word) != EOF)
    {
        int L = strlen(word);
        int Beg = 0;
        while (Beg < L && (word[Beg] < 'a' || word[Beg] > 'z'))
        {
            Beg++;
        }
        if (Beg < L)
        {
            pInWords[count] = strdup(word + Beg);
            printf("Read word: %s\n", pInWords[count]); // Debug print
            count++;
        }
    }

    fclose(fp);
    return count;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}