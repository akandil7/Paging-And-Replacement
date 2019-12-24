#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//linked list to read input :
typedef struct{
int value;
struct Node* next;
int size;
}Node;

Node* pages;

void insert(int val)
{
    Node* currentLooping = pages;

    //initialize node:
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->value = val;
    temp->next= NULL;

    //if first node->head
    if(pages==NULL)
    {
        pages=temp;
        pages->size=1;
    }
    //loop untill last element
    else
    {
        temp->size=pages->size++;
        while(currentLooping->next!=NULL)
        {
            currentLooping=currentLooping->next;
        }
        currentLooping->next=temp;
    }
}


//replacement policy methods:
void fifo(int);

//check if a ceratin page exist or NOT
int checkExist(int,int);
int *frames;

int main()
{
    int numFrames,num,i,j;
    char type [8];
    pages = NULL;


    //get the number of Frames & initialize it
    scanf("%d",&numFrames);
    frames = (int*)malloc(numFrames*sizeof(int));
    for (i=0;i<numFrames;i++)
    {
        frames[i]=-1;
    }

    //get method:
    do {
        scanf("%s",&type);
    } while(strlen(type)<0);

    //get pages:
    scanf("%d",&num);
    while(num!=-1){
        insert(num);
        scanf("%d",&num);
    }

    if(strcasecmp(type,"OPTIMAL")==0)
    fifo(numFrames);
    else if (strcasecmp(type,"FIFO")==0)
    fifo(numFrames);
    else if (strcasecmp(type,"LRU")==0)
    fifo(numFrames);
    else
    fifo(numFrames);

    return 0;
}

int checkExist(int searchingVal,int sizeOfFrames)
{
    int k;
    for (k=0;k<sizeOfFrames;k++)
    {
        if(searchingVal==frames[k])
        {
            return k;
        }
    }

    return -1;
}

void fifo(int sizeOfFrame){

    int i,num,pageFault=0,lastTurn=0,looping,flag=0;

    printf("Replacement Policy = FIFO\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");

    while(pages!=NULL)
    {
        if((checkExist(pages->value,sizeOfFrame))==-1)
        {
            flag=1;
            if( frames[(lastTurn) % sizeOfFrame] != -1)
            {
                pageFault++;
                printf("%d F   ",pages->value);
            }
            else
                printf("%d    ",pages->value);

            frames[(lastTurn++) % sizeOfFrame] = pages->value;

            for(looping=0;looping<sizeOfFrame;looping++)
            {
                printf("%d ",frames[looping]);
            }
            printf("\n");
        }

        if(flag!=1)
        {
            printf("%d    ",pages->value);

            for(looping=0;looping<sizeOfFrame;looping++)
            {
                printf("%d ",frames[looping]);
            }
                printf("\n");
        }

        flag=0;
        pages=pages->next;
    }

    printf("-------------------------------------\n");
    printf("Number of page faults = %d",pageFault);
}
