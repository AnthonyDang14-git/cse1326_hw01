#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h>

#define CONTRY_CODE 0
#define REGION_NAME 1
#define IATA 2
#define ICAO 3
#define AIRPORT_NAME 4
#define LAT 5
#define LON 6
#define FIELD_COUNT 7
#define FIELD_MAX 128
#define LINE_MAX 4096 

typedef struct Node	
{
	char data[FIELD_COUNT][FIELD_MAX];
	int latInt, lonInt;
	struct Node *next;
} Node;

static Node *head = NULL;
static Node *tail = NULL;

Node *makeNode(void)
{
	Node *node = (Node*)malloc(sizeof(Node));
	for(int i =0; i < FIELD_COUNT; i++)
	{
		node->data[i][0] = '\0';
	}
	node->latInt = 0;
	node->lonInt = 0;
	node->next = NULL;
	return node;
}

void appendNode(Node *node)
{
	if(!head)
	{
		head = tail = node;
	} else {
		tail->next = node;
		tail = node;
	}
}

void stripQuotes(char *token)
{
	size_t n = strlen(token);
	if(n > 0 && token[0] == '"')
	{
		memmove(token, token + 1, n);
		n = strlen(token);
	}
	if(n > 0 && token[n - 1] == '"')
	{
		token[n - 1] = '\0';
	}
}

int parseLine(const char *lineRaw)
{
	char buffer[LINE_MAX];
	strncpy(buffer, lineRaw, sizeof(buffer)-1);
	buffer[sizeof(buffer)-1] = '\0';

	char *token = strtok(buffer, ",");
	char *fields[FIELD_COUNT] = {0};
	int count = 0;

	while(count < FIELD_COUNT)
	{
		stripQuotes(token);
		fields[count++] = token;
		token = strtok(NULL, ",");
	}

	Node *node = makeNode();
	for(int i = 0; i < FIELD_COUNT; i++)
	{
		strncpy(node->data[i], fields[i], FIELD_MAX -1);
		node->data[i][FIELD_MAX - 1] = '\0';
	}
	node->latInt = atoi(node->data[LAT]);
	node->lonInt = atoi(node->data[LON]);
	appendNode(node);
	return 1;
}

void saveAirportData(FILE *airports)
{
	char line[LINE_MAX];
	int count = 1;
	while(fgets(line, sizeof(line), airports) != NULL)
	{
		parseLine(line);
		count++;
	}
	printf("There are %d airports in the file\n", count);
}

void freeAirportData(Node *head)
{
	while(head)
	{
		Node *next = head->next;
		free(head);
		head = next;
	}
}

void findAirports()
{
	int findLat, findLon;
	char str1[16], str2[16];
	while(1)
	{
		Node *p = head;
		int count = 1;
		if(scanf("%d %d", &findLat, &findLon))
		{
			while(p != NULL){
				if((findLat == p->latInt) && (findLon == p->lonInt)){
					printf("[%d] %s, %s, %s\n", count++, p->data[AIRPORT_NAME], p->data[LAT], p->data[LON]);
				}
				p = p->next;	
			}
		} else if(scanf("%s %s", str1, str2)){
			
			if(strcmp(str1, "stop") == 0){
				break;
			}

		}
	}
}

int main(int argc, char *argv[])
{
	FILE* airports = fopen("iata-icao.csv", "r");
	if(airports == NULL){
		printf("Error opening file\n");
		return 1;
	}
	
	saveAirportData(airports);
	fclose(airports);

	findAirports();

	freeAirportData(head);
	head = tail = NULL;

	return 0;
}
