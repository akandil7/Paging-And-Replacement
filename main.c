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
void fifo();
void optimal();
void lru();
void clock();
//check if a ceratin page exist or NOT
int checkExist(int);
int getFarthest();
int getLeastRecent();
int getTurnAndUpdate(int,int []);

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
    int sizeOfFrame= (sizeof(frames)/sizeof(*frames))+1;
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

    int i,num,pageFault=0,lastTurn=0,looping,flag=0;

    int sizeOfFrame= (sizeof(frames)/sizeof(*frames))+1;

    printf("Replacement Policy = FIFO\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");

    while(pages!=NULL)
    {
        if((checkExist(pages->value))==-1)
        {
            //flag=1;
            if( frames[(lastTurn) % sizeOfFrame] != -1)
            {
                pageFault++;
                printf("%d F    ",pages->value);
            }
            else
                printf("%d      ",pages->value);

            frames[(lastTurn++) % sizeOfFrame] = pages->value;
        }
        else
        {
            printf("%d      ",pages->value);
        }


        for(looping=0;looping<sizeOfFrame;looping++)
        {
            printf("%d ",frames[looping]);
        }
        printf("\n");

        pages=pages->next;
    }

    printf("-------------------------------------\n");
    printf("Number of page faults = %d",pageFault);
}

//get the far-est return index of frame to be replaced
int getFarthest()
{
    int i,current_counter,far=-1,index;
    int sizeOfFrame= (sizeof(frames)/sizeof(*frames))+1;
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
    int sizeOfFrame= (sizeof(frames)/sizeof(*frames))+1;

    printf("Replacement Policy = OPTIMAL\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");

    while(pages!=NULL)
    {
        if(checkExist(pages->value)==-1)
        {
             if( frames[(lastTurn) % sizeOfFrame] != -1)
            {
                pageFaults++;
                replacedPlace=getFarthest();
                frames[(replacedPlace %sizeOfFrame)] = pages->value;
                printf("%d F    ",pages->value);
            }
            else
            {
                frames[(lastTurn++) % sizeOfFrame] = pages->value;
                printf("%d      ",pages->value);
            }
        }
        else
        {
            printf("%d      ",pages->value);
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

int getLeastRecent(int time[])
{
    int index=0,smallest=time[0],i;
    int sizeOfFrame= (sizeof(frames)/sizeof(*frames))+1;

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
    int pageFaults=0,valExist,lastTurn=0,lruPlace,i,counter=0;
    int sizeOfFrame= (sizeof(frames)/sizeof(*frames))+1;
    int* timeArray;
    timeArray= (int*)malloc(sizeOfFrame*sizeof(int));

    printf("Replacement Policy = LRU\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");

    while(pages!=NULL)
    {
        valExist= checkExist(pages->value);
        if(valExist==-1)
        {
             if( frames[(lastTurn) % sizeOfFrame] != -1)
            {
                pageFaults++;
                lruPlace=getLeastRecent(timeArray);
                frames[(lruPlace %sizeOfFrame)] = pages->value;
                timeArray[(lruPlace %sizeOfFrame)]= counter;
                printf("%d F    ",pages->value);
            }
            else
            {
                frames[(lastTurn) % sizeOfFrame] = pages->value;
                timeArray[(lastTurn %sizeOfFrame)]= counter;
                lastTurn++;
                printf("%d      ",pages->value);
            }
        }
        else
        {
            timeArray[valExist%sizeOfFrame] = counter;
            printf("%d      ",pages->value);
        }

        pages=pages->next;
        counter++;

        for(i=0;i<sizeOfFrame;i++)
        {
           printf("%d ",frames[i]);
        }
        printf("\n");

    }

    printf("-------------------------------------\n");
    printf("Number of page faults = %d",pageFaults);
}

int getTurnAndUpdate(int start,int used[])
{
    int i=start;
    int sizeOfFrame= (sizeof(frames)/sizeof(*frames))+1;

    while(used[i%sizeOfFrame]==1)
    {
        used[i%sizeOfFrame]=0;
        i++;
    }

    return(i%sizeOfFrame);

}

void clock()
{
    int sizeofFrame= (sizeof(frames)/sizeof(*frames))+1;
    int used[sizeofFrame];
    int turn=0,pageFaults=0;
    int i;

    printf("Replacement Policy = CLOCK\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");

    while(pages!=NULL)
    {

        if(checkExist(pages->value) == -1)
        {
            if(frames[turn%sizeofFrame]==-1)
            {
                frames[turn%sizeofFrame]=pages->value;
                used[turn%sizeofFrame]=1;
                turn++;
                printf("%d      ",pages->value);
            }
            else
            {
                pageFaults++;
                //if recently used=0 ->replace
                if(used[turn%sizeofFrame]==0)
                {
                    frames[turn%sizeofFrame]=pages->value;
                    used[turn%sizeofFrame]=1;
                    turn++;
                    printf("%d F    ",pages->value);
                }
                //else check for replacement
                else
                {
                    turn= getTurnAndUpdate(turn,used);
                    frames[turn%sizeofFrame]=pages->value;
                    used[turn%sizeofFrame]=1;
                    turn++;
                    printf("%d F    ",pages->value);
                }
            }
        }
        else
        {
            //if lru set to 1 do nothing else:
            if(used[turn%sizeofFrame]==0)
            {
                used[turn%sizeofFrame]=1;
            }
            printf("%d      ",pages->value);
        }

        pages=pages->next;
        for(i=0;i<sizeofFrame;i++)
        {
            printf("%d ",frames[i]);
        }
        printf("\n");
    }

    printf("-------------------------------------\n");
    printf("Number of page faults = %d",pageFaults);

}
