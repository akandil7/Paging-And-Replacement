#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//linked list to read input :
typedef struct{
int value;
struct Node* next;
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
        pages=temp;
    //loop untill last element
    else
    {
       while(currentLooping->next!=NULL)
       {
           currentLooping=currentLooping->next;
       }
       currentLooping->next=temp;
    }
}

//replacement policy methods:
void fifo();
void optimal();
void lru();
void clock();
//checks & helps functions:
int checkExist(int);
int getFarthest();
int getLeastRecent();
int getTurnAndUpdate(int,int []);
//additional:
void printHeader(char[]);
void printFaults(int);

int *frames;
int sizeOfFrame;

void printHeader(char methodName[])
{
    printf("Replacement Policy = %s\n",methodName);
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
}

void printFaults(int number)
{
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n",number);
}

int main()
{
    int num,i,j;
    char type [8];
    pages = NULL;

    //get the number of Frames & initialize it
    scanf("%d",&sizeOfFrame);
    frames = (int*)malloc(sizeOfFrame*sizeof(int));

    for (i=0;i<sizeOfFrame;i++)
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
    optimal();
    else if (strcasecmp(type,"FIFO")==0)
    fifo();
    else if (strcasecmp(type,"LRU")==0)
    lru();
    else
    clock();

    return 0;
}

int checkExist(int searchingVal)
{
    int k;
    for (k=0;k<sizeOfFrame;k++)
    {
        if(searchingVal==frames[k])
        {
            return k;
        }
    }

    return -1;
}

void fifo(){

    int i,num,pageFault=0,lastTurn=0,looping;
    printHeader("FIFO");

    while(pages!=NULL)
    {
        if((checkExist(pages->value))==-1)
        {
            if( frames[(lastTurn) % sizeOfFrame] != -1)
            {
                pageFault++;
                printf("%02d F   ",pages->value);
            }
	    else
	        printf("%02d     ",pages->value);
            frames[(lastTurn++) % sizeOfFrame] = pages->value;
        }
	else
        printf("%02d     ",pages->value);

        for(looping=0;looping<sizeOfFrame;looping++)
        {
            if(frames[looping]==-1)
            printf(" ");
            else
            printf("%02d ",frames[looping]);
        }
        printf("\n");

        pages=pages->next;
    }
    printFaults(pageFault);
}

//get the far-est return index of frame to be replaced
int getFarthest()
{
    int i,current_counter,far=-1,index;
    int tempFar [sizeOfFrame];

    for(i=0;i<sizeOfFrame;i++)
    {
        tempFar[i]=-1;
    }

    Node* loopNode=pages;

    current_counter=0;

    while(loopNode != NULL)
    {
       for(i=0;i<sizeOfFrame;i++)
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
    for(i=0;i<sizeOfFrame;i++)
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

void optimal()
{
    int pageFaults=0,lastTurn=0,replacedPlace,i;
    printHeader("OPTIMAL");

    while(pages!=NULL)
    {
        if(checkExist(pages->value)==-1)
        {
             if( frames[(lastTurn) % sizeOfFrame] != -1)
            {
                pageFaults++;
                replacedPlace=getFarthest();
                frames[(replacedPlace %sizeOfFrame)] = pages->value;
                printf("%02d F   ",pages->value);
            }
            else
            {
                frames[(lastTurn++) % sizeOfFrame] = pages->value;
                printf("%02d     ",pages->value);
            }
        }
        else
        printf("%02d     ",pages->value);

        for(i=0;i<sizeOfFrame;i++)
        {
            if(frames[i]==-1)
            printf(" ");
            else
            printf("%02d ",frames[i]);
        }
        printf("\n");

        pages=pages->next;
    }
    printFaults(pageFaults);
}

int getLeastRecent(int time[])
{
    int index=0,smallest=time[0],i;

    for(i=1;i<sizeOfFrame;i++)
    {
        if(time[i]<smallest)
        {
            smallest=time[i];
            index=i;
        }
    }
    return index;
}

void lru()
{
    int pageFaults=0,valExist,lastTurn=0,lruPlace,i,counter=0,flag;
    int* timeArray;
    timeArray= (int*)malloc(sizeOfFrame*sizeof(int));
    printHeader("LRU");

    while(pages!=NULL)
    {
        flag=0;
        valExist= checkExist(pages->value);
        if(valExist==-1)
        {
             if( frames[(lastTurn) % sizeOfFrame] != -1)
            {
                pageFaults++;
                flag=1;
                lruPlace=getLeastRecent(timeArray);
                frames[(lruPlace %sizeOfFrame)] = pages->value;
                timeArray[(lruPlace %sizeOfFrame)]= counter;
                printf("%02d F   ",pages->value);
            }
            else
            {
                frames[(lastTurn) % sizeOfFrame] = pages->value;
                timeArray[(lastTurn %sizeOfFrame)]= counter;
                lastTurn++;
            }
        }
        else
        {
            timeArray[valExist%sizeOfFrame] = counter;
        }

        if(flag==0)
        {
            printf("%02d     ",pages->value);
        }

        for(i=0;i<sizeOfFrame;i++)
        {
            if(frames[i]==-1)
            printf(" ");
            else
            printf("%02d ",frames[i]);
        }
        printf("\n");

        pages=pages->next;
        counter++;
    }
    printFaults(pageFaults);
}

/*int getTurnAndUpdate(int start,int used[])
{
    int i=start;

    while(used[i%sizeOfFrame]==1)
    {
        used[i%sizeOfFrame]=0;
        i++;
    }

    return(i%sizeOfFrame);
}*/

void clock()
{
    int used[sizeOfFrame];
    int turn=0,pageFaults=0,check;
    int i;

    printHeader("CLOCK");

    while(pages!=NULL)
    {
	check = checkExist(pages->value);
        if(check == -1)
        {
            if(frames[turn%sizeOfFrame]==-1)
            {
                frames[turn%sizeOfFrame]=pages->value;
                used[turn%sizeOfFrame]=1;
                turn++;
                printf("%02d     ",pages->value);
            }
            else
            {
                    pageFaults++;
                    while(used[turn%sizeOfFrame]==1)
    		    {
                          used[turn%sizeOfFrame]=0;
                          turn++;
                    }
                    frames[turn%sizeOfFrame]=pages->value;
                    used[turn%sizeOfFrame]=1;
		    turn++;
               	    printf("%02d F   ",pages->value);
            }
        }
        else
        {
            //if lru set to 1 do nothing else:
            if(used[check%sizeOfFrame]==0)
            {
                used[check%sizeOfFrame]=1;
            }
            printf("%02d     ",pages->value);
        }

        for(i=0;i<sizeOfFrame;i++)
        {
            if(frames[i]==-1)
            printf(" ");
            else
            printf("%02d ",frames[i]);
        }
        printf("\n");

        pages=pages->next;
    }
    printFaults(pageFaults);
}
