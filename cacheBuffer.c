#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WEBLEN 32
#define FILELEN 16

typedef struct cacheBuffer
{
	char webpage[WEBLEN];			//keeps the address of webpage
	int counter;				//keeps the total number of requests
	struct cacheBuffer *next, *prev;	//links the element to next and previous nodes
} BUFFER;

BUFFER * inBuffer(BUFFER*, char*);
BUFFER * createNode(char*);
void add2List(BUFFER**, BUFFER*);
void printList(BUFFER*);
int size(BUFFER*);
void resize(BUFFER**);
void assignHead(BUFFER**, BUFFER*);

int main(void)
{
	BUFFER *cache = NULL;	//cache buffer list
	BUFFER *newNode;
	int option;
	int length;
	int threshold;
	char isClear;
	char page[WEBLEN];
	char fileName[FILELEN];
	FILE *fp;

	printf("The length of the cache : ");
	scanf("%d", &length);

	printf("Threshold of cache : ");
	scanf(" %d", &threshold);

	do
	{
		printf("Read from text -> 1\nRead from user -> 2\n");
		scanf("%d", &option);
	}while(option != 1 && option != 2);

	if(option == 1)
	{
		printf("Enter a text file : ");
		scanf("%s", fileName);
		fp = fopen(fileName, "r");
	}

	if(option == 2)
	{
		printf("Write a webpage per line :\n");
		fp = stdin;
	}
	
	while( !feof(fp) )
	{
		if(option == 2)
			printf("Webpage : ");

		fscanf(fp, "%s", page);

		newNode = inBuffer(cache, page);

		if( !newNode )	//if page is not in cache buffer
		{
			//create new node
			newNode = createNode(page);

			//add new node to cache
			add2List(&cache, newNode);
		}

		else
		{
			//increase the counter
			newNode -> counter++;

			//check the threshold is crossed
			if(newNode -> counter == threshold)
	                	assignHead(&cache, newNode);
		}
		
		if(option == 2)
		{	
			printf("Clear cache? [y/n]");
	
			do
			{
				scanf(" %c", &isClear); 
			}while(isClear != 'y' && isClear != 'n');

			if(isClear == 'y')	//if y is given
				cache = NULL;	//clear cache
		}

		//check if the capacity is over
		if( size(cache) > length )
			resize(&cache);		

		//print cache buffer
		printList(cache);
	}
}

BUFFER * inBuffer(BUFFER *buff, char *page)
{
	/* 
	 * Checks if a node in the buffer (linked list)
	 * If so, return node;
	 * else, return NULL
	 *
	 */

	BUFFER *curr = buff;	//keep the head node of buffer unchanged

	while( curr && 						//while current node is not NULL, and 
		strcmp(curr -> webpage, page) != 0)		//webpage is not find
		curr = curr -> next;				//try next node
	
	return curr;
}

BUFFER * createNode(char *page)
{
	/*
	 * Creates a new BUFFER node
	 *
	 * as default:
	 * 	node -> wepage = page
	 * 	node -> counter = 1
	 * 	node -> next/prev = NULL
	 */
	
	BUFFER *node = (BUFFER*) malloc(sizeof(BUFFER));

	strcpy(node -> webpage, page);
	node -> counter = 1;
	node -> next = NULL;
	node -> prev = NULL;
	
	return node;
}

void add2List(BUFFER **head, BUFFER *add)
{
	// Adds node add to the head

	if( !(*head) ) 		//if head is NULL, so is the list empty
	{
		*head = add;	//add node is the head node
		return;
	}

	(*head) -> prev = add;		//add node is before current node, so is new node		
	add -> next = *head;		//curr node is next node of curr
	(*head) = add;
}

void printList(BUFFER *head)
{
	BUFFER *curr = head;

	while( curr )
	{
		printf("%s , %d -> ", curr -> webpage, curr -> counter);
		curr = curr -> next;
	}
	printf("NULL\n");
}

int size(BUFFER *head)
{
	//Finds the size of the buffer
	
	BUFFER *curr = head;
	int length = 0;		//count the length
	
	while( curr )
	{
		curr = curr -> next;		//go to tail node
		length++;			//increase the counter
	}
	return length;
}

void resize(BUFFER **head)
{
	BUFFER *curr = *head;

	while( curr -> next )		//go to tail node
		curr = curr -> next;

	curr -> prev -> next = NULL;	//new tail node
	free(curr);			//free old tail node
}

void assignHead(BUFFER **head, BUFFER *curr)
{
	if( curr == *head )
		return;

	curr -> prev -> next = curr -> next;
	if(curr -> next)
		curr -> next -> prev = curr -> prev;

	curr -> next = *head;
	curr -> prev = NULL;

	(*head) -> prev = curr;
	*head = curr;
}
