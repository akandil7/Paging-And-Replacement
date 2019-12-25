#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//linked list to read input :
typedef struct{
int value;
struct Node* next;
//int size;
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
        //pages->size=1;
    }
    //loop untill last element
    else
    {
        //temp->size=pages->size++;
        while(currentLooping->next!=NULL)
        {
            currentLooping=currentLooping->next;
        }
        currentLooping->next=temp;
    }
}


//replacement policy methods:
void fifo(int);
void optimal();

//check if a ceratin page exist or NOT
int checkExist(int,int);
int farestOpt(int);

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
    optimal(numFrames);
    else if (strcasecmp(type,"FIFO")==0)
    fifo(numFrames);
    else if (strcasecmp(type,"LRU")==0)
    fifo(numFrames);
    else
    fifo(numFrames);

    return 0;
}

//for fifo
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

//get the far-est return index of frame to be replaced
int farestOpt(int sizeOfFrames)
{
    int i,current_counter,far=-1,index;
    int tempFar [sizeOfFrames];

    for(i=0;i<sizeOfFrames;i++)
    {
        tempFar[i]=-1;
    }

    Node* loopNode=pages;

    current_counter=0;

    while(loopNode != NULL)
    {
       for(i=0;i<sizeOfFrames;i++)
        {
            if(loopNode->value== frames[i])
            {
                if(tempFar[i]==-1)
                tempFar[i]= current_counter;
                break;
            }
        }

        current_counter++;
        loopNode= loopNode->next;
    }

    //find far-est, if not assigned in future -> replace
    for(i=0;i<sizeOfFrames;i++)
    {
        if(tempFar[i]==-1)
        return i;
        else
        {
            if(tempFar[i]>far)
            {
                far=tempFar[i];
                index=i;
            }
        }
    }

    return index;

}

void optimal(int sizeOfFrame)
{
    int pageFaults=0,lastTurn=0,replacedPlace,i;

    printf("Replacement Policy = FIFO\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");

    while(pages!=NULL)
    {
        if(checkExist(pages->value,sizeOfFrame)==-1)
        {
             if( frames[(lastTurn) % sizeOfFrame] != -1)
            {
                pageFaults++;
                replacedPlace=farestOpt(sizeOfFrame);
                frames[(replacedPlace %sizeOfFrame)] = pages->value;
                printf("%d F   ",pages->value);
            }
            else
            {
                frames[(lastTurn++) % sizeOfFrame] = pages->value;
                printf("%d     ",pages->value);
            }
        }

        for(i=0;i<sizeOfFrame;i++)
        {
           printf("%d ",frames[i]);
        }
        printf("\n");

        pages=pages->next;

    }

    printf("-------------------------------------\n");
    printf("Number of page faults = %d",pageFaults);
}
