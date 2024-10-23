/***************************************************************
 *
 * file: pss.h
 *
 * @Author  Nikolaos Vasilikopoulos (nvasilik@csd.uoc.gr), John Petropoulos (johnpetr@csd.uoc.gr)
 * @Version 30-11-2022
 *
 * @e-mail       hy240-list@csd.uoc.gr
 *
 * @brief   Implementation of the "pss.h" header file for the Public Subscribe System,
 * function definitions
 *
 *
 ***************************************************************
 */

#include <stdio.h>
#include <stdlib.h>

#include "pss.h"
struct Group G[MG]; 
struct SubInfo *T[MG];
struct SubInfo **HashTable;
int sizeofhash;
int Universal;
int A,B;
int copycounter=0;
int *IID; 
int *ITM;
int k11=0;
int pdone=0;
struct Subscription *S=NULL,*starts=NULL;
void  Sl_LookUp(struct SubInfo*p,int stm)
{
    struct SubInfo *p1=p;
    while(p1!=NULL && p1->sId!=stm)
    {
        p1=p1->snext;
    }
    if(p1!=NULL)
    {
       printf("The stm was not found!");   
    }
    else
    {
        printf("The stm was not found!");   
    }
}
void Sl_Delete(struct SubInfo** p, int sid)
{
    struct SubInfo* p1=*p,*bef;
    if(p1!=NULL && p1->sId==sid)
    {
        *p=p1->snext;
        free(p1);
    }
    else
    {
      while(p1!=NULL && p1->sId!=sid)
      {
        bef=p1;
        p1=p1->snext;
      }
      if(p1!=NULL)
      {
        bef->snext=p1->snext;
        free(p1);
      }
    }
}
void Sl_Print(struct SubInfo *p)//Sl_print void function is called to print all the elements of a single sorted list type of subinfo
{
  struct SubInfo *p1=p;//attach the node of the first element into a new pointer type of subInfo
  printf("<");
  while(p1!=NULL)//until the last element
  {
    printf("%d,",p1->sId);//print the current element
    p1=p1->snext;//go to the next
  }
    printf(">");
}
void Sl_Insert( struct SubInfo **p,int stm,int sId) //function void that is passing by refference the list and it insert the stm sid in a sinlge sorted list
{ 
    struct SubInfo* p1,*p2=(*p),*p3;//hold the fist element and make 2 more type of subinfo
    p1=(struct SubInfo *)malloc(sizeof(struct SubInfo));//make space for it
      if(!p1)//if something goes wrong with the memory allocation
    {
        printf("Something went wrong in memory allocation");//inform the user
        exit(1);//exit -1
    }
    p1->stm=stm;//insert the stm to the node we make before
    p1->sId=sId;//insert the sid to the node we  make before
    p1->snext=NULL;//and make the next element of this node to point to nul
    if( (*p)==NULL ||(*p)->stm>=stm)//if the list is empty just make the node p1 head 
    {
        p1->snext=(*p);
        (*p)=p1;
    }
    else//else we make a while until the second last element and until its found so if its found we insert the p1 to p2 that is the element bettween its next number or if its the 2nd last element we make p1 last
    {
      p3=p2;
      p3=p3->snext;
      while(p3!=NULL && p3->stm<=p1->stm)
      {
        p2=p2->snext;
        p3=p3->snext;
      }
        p1->snext=p2->snext;
        p2->snext=p1;
    }
}
void L_insert(struct Subscription **p,int sId)//function void insert (the list called by reffernce),and its the same as sl_insert but this list is not sorted so we dont need to do a while here just put it to the last node of list
{
  struct Subscription *p1,*p2=*p,*p3;
    p1=(struct Subscription *)malloc(sizeof(struct Subscription));
    if(!p1)
    {
        printf("Something went wrong in memory allocation");
        exit(1);
    }
    p1->sId=sId;
    p1->snext=NULL;
    if( (*p)==NULL)
    {
        p1->snext=(*p);
        (*p)=p1;
    }
    else
    {
      p3=p2;
      p3=p3->snext;
      while(p3!=NULL)
      {
        p2=p2->snext;
        p3=p3->snext;
      }
        p1->snext=p2->snext;
        p2->snext=p1;
    }
}
void L_Print(struct Subscription *p)//function called for printing the Group list that is not sorted and its linked (not doubly)
{
  printf("<");
  while(p!=NULL)//until we find the last element
  {
    printf("%d,",p->sId);//print the element in the list
    p=p->snext;//go to the next node in the list
  }
  printf(">");
}
int L_lookup(struct Subscription *p,int sId)//it used for findning an element in a not sorted single linked list 
{
  struct Subscription *p1=p;
  int counter=0;
  while(p1!=NULL && p1->sId!=sId)//until the last element and the element we looking for
  {
    counter++;
    p1=p1->snext;
  }
  if(p1==NULL)//if it is never found just return something that is not coorect as place in the list
  {
    return -1;
  }
  else
  {
    return counter;//else return the place into the list that it is found!
  }
}
void L_Delete(struct Subscription **p,int sId)//function that deletes an element from a single not sorted list (and its void because its called by reference!)
{
     struct Subscription * p1=*p,*bef;//save the start of the list in a new pointer  and make 1 more to hold the element before the one we need to delete so we can link it with the next one
    if(p1!=NULL && p1->sId==sId)// if its found and the list is not empty make hte pointer of the first element to point into the second and delete the first
    {
        *p=p1->snext;
        free(p1);
    }
    else//else make a while until we find or the last element 
    {
      while(p1!=NULL && p1->sId!=sId)
      {
        bef=p1;
        p1=p1->snext;
      }
      if(p1!=NULL)//if p1!=null means it was found so we delete it and we ling the element before with element next of it ,else if p1==null it means it is not found into the list
      {
        bef->snext=p1->snext;
        free(p1);
      }
    }
}
struct Info* Min(struct Info* head)
{
 while(head->ilc!=NULL)
 {
    head=head->ilc;
 }
 return head;
}
struct Info* BST_print(struct Info * head)
{
    if(head==NULL){return NULL;}
     BST_print(head->ilc);
     printf("%d,",head->iId);
     BST_print(head->irc);
}
struct Info* BST_lookup(struct Info *head,int iId,int itm)
{
 struct Info *p = head;
 while (p != NULL && itm != p->itm) {
 if (itm < p->itm) p = p->ilc;
 else p = p->irc;
 }
 if (p != NULL){
 return p ;
 }
 else return NULL;
}
int FindiId(struct Info* head,int itm)
{
 struct Info *p = head;
 while (p != NULL && itm != p->itm) {
 if (itm < p->itm) p = p->ilc;
 else p = p->irc;
 }
 if (p != NULL){
 return p->iId ;
 }
 else return -1;
}
struct Info* NewCell(int itm,int iId)
{
    struct Info* temp=NULL;
    temp=(struct Info*)malloc(sizeof(struct Info));
    temp->itm=itm;
    temp->iId=iId;
    temp->ilc=NULL;
    temp->irc=NULL;
    temp->ip=NULL;
    return temp;
}
struct Info* BST_Insert(int itm,int iId,struct Info* head)
{
 struct Info* p=NULL,*q=NULL,*prev=NULL;
 p=head;
 while(p!=NULL)
 {
    if(p->itm==itm)
    {
        if(prev!=NULL)
        {
            p->ip=prev;
        }
        p->iId=iId;
        return head;
    }
    prev=p;
    if(itm<p->itm){p=p->ilc;}
    else{p=p->irc;}
 }
 q = NewCell(itm,iId);
 if (prev == NULL){ return q;}
 else if (itm < prev->itm) { q->ip=prev; prev->ilc = q;}
 else  {q->ip=prev; prev->irc = q;}
 return head;

}
struct Info* BST_delete(int itm,int iId,struct Info* head)
{//   printf("eerr");
 struct Info  * temp=head;
 if(temp==NULL)
 {
    return NULL;
 }
 else
 {
      if( temp->ilc==NULL && temp->irc==NULL)
      {
        free(temp);
       // temp=NULL;
        head=NULL;
        return head;
      }
      struct Info* tobedeleted;
      tobedeleted=(struct Info *)malloc(sizeof(struct Info));
       tobedeleted=BST_lookup(temp,iId,itm);
     if(tobedeleted->irc==NULL && tobedeleted->ilc==NULL)
      { 
        if(tobedeleted->ip->irc==tobedeleted)
        {
             tobedeleted->ip->irc=NULL;
        }
        else
        {
           tobedeleted->ip->ilc=NULL;
        }
       // printf("\ntobedeletedleaf:%d\n",tobedeleted->ip->itm);
           // tobedeleted=NULL;
           /*tobedeleted->ilc=NULL;
            tobedeleted->irc=NULL;
            tobedeleted->ip=NULL;*/
            free(tobedeleted);
        tobedeleted=NULL;
         return head;
    } 
      else if(tobedeleted->ilc==NULL || tobedeleted->irc==NULL)
      {
        struct Info* parent=tobedeleted->ip;
       if(tobedeleted!=temp)
        {
        if(tobedeleted->ilc!=NULL)
        {
            if(parent->ilc==tobedeleted){
            parent->ilc=tobedeleted->ilc;
            tobedeleted->ilc->ip=parent;
            }
            if(parent->irc==tobedeleted){
             parent->irc=tobedeleted->ilc;
              tobedeleted->ilc->ip=parent;
            }
         }
         if(tobedeleted->irc!=NULL)
          {
            if(parent->ilc==tobedeleted){
              parent->ilc=tobedeleted->irc;
               tobedeleted->irc->ip=parent;
            }
            if(parent->irc==tobedeleted){
              tobedeleted->irc->ip=parent->irc;
               tobedeleted->ilc->ip=parent;
            }
          }
        }
      else
        {
          if(tobedeleted->ilc!=NULL){head=tobedeleted->ilc;} else{head=tobedeleted->irc;}
        }
           /* tobedeleted->ilc=NULL;
            tobedeleted->irc=NULL;
            tobedeleted->ip=NULL;*/
            free(tobedeleted);
           // tobedeleted=NULL;
          //  printf("\nparent:%d\n",parent->itm);
          //  printf("\nparent lc:%d\n",parent->ilc->itm);
            return head;
        }
      if(tobedeleted->ilc!=NULL && tobedeleted->irc!=NULL)
      {    // printf("eerr");
            struct Info* succ=Min(tobedeleted->irc);
            int temp=succ->itm;
            BST_delete(temp,0,head);
            tobedeleted->itm=temp;
            return head;
      }
 }
}
struct TreeInfo* TNewCell(int ttm,int tId)
{
    struct TreeInfo* temp=NULL;
    temp=(struct TreeInfo*)malloc(sizeof(struct TreeInfo));
    temp->ttm=ttm;
    temp->tId=tId;
    temp->tlc=NULL;
    temp->trc=NULL;
    temp->tp=NULL;
    temp->next=NULL;
    temp->prev=NULL;
    return temp;
}
struct TreeInfo* BST_TLOOKUP(struct TreeInfo * head,int ttm,int tId)
{
 struct TreeInfo *p = head;
 while (p != NULL) {
 if (ttm <= p->ttm)
 { p = p->tlc;}
 else{
   p = p->trc;
  }
  if(ttm==p->ttm && p->tlc==NULL &&p->trc==NULL)
  {
    break;
  }
 }
 if (p != NULL){
 printf("found");
 return p ;
 }
 else return NULL;
}
struct TreeInfo* BST_Tprint(struct TreeInfo * head)
{
    if(head==NULL){return NULL;}
     BST_Tprint(head->tlc);
     printf("iId:%ditm:%d\n",head->tId,head->ttm);
     BST_Tprint(head->trc);
}
struct TreeInfo* BST_TInsert(int ttm,int tId,struct TreeInfo* head)
{
 struct TreeInfo* p=NULL,*q=NULL,*prev=NULL,*subq=NULL,*subprev=NULL,*parentprev=NULL;
 p=head;  
 while(p!=NULL)
 {
    if(p->ttm==ttm && p->tlc==NULL && p->trc==NULL )
    {
        if(prev!=NULL)
        {
            p->tp=prev;
        }
        p->tId=tId;
        return head;
    }
    prev=p;
    if(ttm<=p->ttm){p=p->tlc;}
    else{p=p->trc;}
 }
 q = TNewCell(ttm,tId);

 subq=TNewCell(ttm,tId);
 if (prev == NULL ){return q;}
 else if (ttm < prev->ttm) {
    // printf("\n%d %d\n",q->ttm,prev->ttm );
     if(prev->tp!=NULL)
     {
       // printf("bhke\n");
        parentprev=prev->tp;
     }
     if(prev->tlc==NULL && prev->trc==NULL){/*printf("\n LEAF \n"); if(prev->ttm==1000){/*leaff=prev;leaff->next=prev->next;*/}
    q->tp=NULL;
    q->trc=prev;
    q->tlc=subq;
    subq->tp=q;
    prev->tp=q;
    if(prev->prev!=NULL)
    {
    prev->prev->next=subq;   
    subq->prev=prev->prev;
    }
    subq->next=prev;
    prev->prev=subq;
     //printf("\nsubq:%d q:%d prev:%d\n",subq->ttm,q->ttm,prev->ttm );
    if(parentprev!=NULL)
    {
        q->tp=parentprev;
        if(parentprev->ttm>q->ttm)
        {
            parentprev->tlc=q;
        }
        else
        {
              parentprev->trc=q;
        }
     //   printf("\n%d %d %d\n",q->ttm,prev->ttm,parentprev->ttm);
        //printf("bhke\n");
        return head;
    }
    else
    {
        return q;
    }
}
 else  {
  if(prev->tlc==NULL && prev->trc==NULL)
   {

    subprev=TNewCell(prev->ttm,prev->tId);
     prev->tlc=subprev;
     prev->trc=q;
    q->tp=prev;
    subprev->tp=prev;
     if(prev->prev!=NULL)
     {
       subprev->prev=prev->prev;
     prev->prev->next=subprev;
     }
     subprev->next=q;  
     q->prev=subprev;
   }
  prev->trc = q;
  q->tp=prev;
  q->next=NULL;
  q->prev=prev->tlc;
  prev->tlc->next=q;
  }
 return head;

}
/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @param m Size of the hash table.
 * @param p Prime number for the universal hash functions.
 *
 * @return 0 on success
 *         1 on failure
 */
struct TreeInfo* CopyToTreeInfo(struct Info* p,int gid,int hcode)
{
  if(p==NULL){return NULL;}
  HashTable[hcode]->tgp[gid]=BST_TInsert(p->itm,p->iId,HashTable[hcode]->tgp[gid]);
 CopyToTreeInfo(p->ilc,gid,hcode);
 CopyToTreeInfo(p->irc,gid,hcode);
 if(copycounter==0)
 {
 HashTable[hcode]->sgp[gid]=HashTable[hcode]->tgp[gid];
 }
 copycounter++;
}
struct TreeInfo* MinLo(struct TreeInfo* head)
{
 while(head->tlc!=NULL)
 {
    head=head->tlc;
 }
 return head;
}
int hasf(int key)
{
  return (A*key+B)%Universal%sizeofhash;
}
void HashInsert(int sId,int sTM )
{
  int hcode=hasf(sId);
   Sl_Insert(&HashTable[hcode],sTM,sId);
   Sl_Delete(&HashTable[hcode],-1);
 }
 void hashDelete(int sId,int p,int a,int b)
 {
  int hcode=hasf(sId);
  Sl_Delete(&HashTable[hcode],sId);
 }
 void hashLookUp(int sId,int p,int a,int b)
 {
  int hcode=hasf(sId);
  Sl_LookUp(HashTable[hcode],sId);
 }
int GetSizeOfTree(struct Info* head)
{
 
  if (head==NULL)
    return 0;
  else    
    return( GetSizeOfTree(head->ilc) + 1 + GetSizeOfTree(head->irc)); 

}
void DestroyTree(struct TreeInfo* head)
{
  if(head==NULL){return ;}
  DestroyTree(head->tlc);
  DestroyTree(head->trc);
  printf("bhke");
  free(head);
}
int initialize(int m, int p){
    A=rand()%p+1;
    B=rand()%p;
    printf("A:%d B:%d",A,B);
    sizeofhash=m;
    Universal=p;
    HashTable=(struct SubInfo**)malloc(sizeof(struct SubInfo)*m);
    for(int i=0; i<MG; i++)
    {  
        G[i].gId=i;
        G[i].gr=NULL;
        G[i].gsub=NULL;
     }
     for(int i=0; i<m; i++)
     {
      HashTable[i]=NULL;
      Sl_Insert(&HashTable[i],-1,-1);
      for(int j=0; j<64; j++)
        {
        HashTable[i]->tgp[j]=(struct TreeInfo*)malloc(sizeof(struct TreeInfo));
        HashTable[i]->tgp[j]=(struct TreeInfo*)1;
          HashTable[i]->sgp[j]=(struct TreeInfo*)malloc(sizeof(struct TreeInfo));
        HashTable[i]->sgp[j]=(struct TreeInfo*)1;
       }
      }
    return EXIT_SUCCESS;
}
 /*void HashTreeInfo(int sId,int gid)
 {
    int hcode=hasf(sId);
    printf("hcode:%d gid:%d",hcode, gid);
    if( HashTable[hcode]->tgp[gid]==NULL)
    {
      printf("\nEdo re");
    BST_TInsert(sId,gid,HashTable[hcode]->tgp[gid]);
    }
    else
    {
       printf("\nOxi Edo");
     BST_TInsert(sId,gid,HashTable[hcode]->tgp[gid]);
    }
 }*/
/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */
struct TreeInfo* MakeNull(struct TreeInfo* P)
{
  P=NULL;
  return P;
}

int free_all(void){
    return EXIT_SUCCESS;
}

/**
 * @brief Insert info
 *
 * @param iTM Timestamp of arrival
 * @param iId Identifier of information
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Insert_Info(int iTM,int iId,int* gids_arr,int size_of_gids_arr)
{
  if(iId<64)
  {
  for(int i=0; i<size_of_gids_arr-1; i++)
  {
    if(gids_arr[i]<64)
    {
     G[gids_arr[i]].gr=BST_Insert(iTM,iId,G[gids_arr[i]].gr);
     printf("GROUPID<%d>,",gids_arr[i]);//output to the user
     printf("INFOLIST:");//output to the user
     BST_print(G[gids_arr[i]].gr);
     printf("\n");
    }
  }
   return EXIT_SUCCESS;
  }
}
/**
 * @brief Subsriber Registration
 *
 * @param sTM Timestamp of arrival
 * @param sId Identifier of subscriber
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Subscriber_Registration(int sTM,int sId,int* gids_arr,int size_of_gids_arr){
  if(sId<64)
  {
  for(int i=0; i<size_of_gids_arr-1; i++)
  {
   if(gids_arr[i]<64)
   {
   if(L_lookup(S,sId)==-1)//if the subscriber is not already to this list that is used to hold all the subscriber that attached to all groups
    { 
      L_insert(&S,sId);//insert the new sub
    }
    if(starts==NULL)//if the pointer that points the first element of the list up
    {
      starts=S;//insert if its tghe first element
    }
    int flag=0;
     L_insert(&G[gids_arr[i]].gsub,sId);
    HashInsert(sId,sTM);
     printf("GROUPID<%d>,",gids_arr[i]);//output to the user
     printf("SUBLIST:");//output to the user
     L_Print(G[gids_arr[i]].gsub);
     HashTable[hasf(sId)]->tgp[gids_arr[i]]=NULL;
     HashTable[hasf(sId)]->sgp[gids_arr[i]]=NULL;
     if(HashTable[hasf(3)]->tgp[1]==NULL)
     {
      printf("\n NULL \n");
     }
     printf("\n");
  }
  }
  }
    return EXIT_SUCCESS;
}
/**
 * @brief Prune Information from server and forward it to client
 *
 * @param tm Information timestamp of arrival
 * @return 0 on success
 *          1 on failure
 */
struct Info* delete_lower_than_tm(int tm,struct Info* head)
{
  struct Subscription *start=NULL;
  int j=0;
   for(int i=Min(head)->itm; i<tm; i++ )
    {
      if(BST_lookup(head,0,i)!=NULL)
      {
      ITM[j]=i;
      IID[j]=FindiId(head,i);
       head=BST_delete(i,0,head);
       j++;
      }
    }
  return head;
} 
int Prune(int tm)
{
  pdone=1;
  struct Subscription *start=NULL;
 for(int i=0; i<MG; i++)
    {
      if(G[i].gr!=NULL)
      {
        if(Min(G[i].gr)->itm<tm)
        {
          int GetSizeTree=GetSizeOfTree(G[i].gr);
            ITM=(int*)malloc(sizeof(int)*GetSizeOfTree(G[i].gr));
            IID=(int*)malloc(sizeof(int)*GetSizeOfTree(G[i].gr));
           G[i].gr=delete_lower_than_tm(tm,G[i].gr);
          if(G[i].gsub!=NULL)
             {
              start=G[i].gsub;
              }   G[i].gsub=start;
             while(G[i].gsub!=NULL)
             {
              for(int k=0; k<GetSizeTree; k++)
              {
                HashTable[hasf(G[i].gsub->sId)]->tgp[G[i].gId]=BST_TInsert(ITM[k],IID[k],HashTable[hasf(G[i].gsub->sId)]->tgp[G[i].gId]);
                } 
                if(GetSizeTree==0)
                {
                    HashTable[hasf(G[i].gsub->sId)]->tgp[G[i].gId]=BST_TInsert(ITM[0],IID[0],HashTable[hasf(G[i].gsub->sId)]->tgp[G[i].gId]);
                }
                if(HashTable[hasf(G[i].gsub->sId)]->sgp[G[i].gId]!=MinLo(HashTable[hasf(G[i].gsub->sId)]->tgp[G[i].gId]))
                {
                  HashTable[hasf(G[i].gsub->sId)]->sgp[G[i].gId]=MinLo(HashTable[hasf(G[i].gsub->sId)]->tgp[G[i].gId]);
                }
              G[i].gsub=G[i].gsub->snext;
             }
              G[i].gsub=start;
             printf("GROUPID=<%d>,INFOLIST=<",G[i].gId);
             if(G[i].gr!=NULL)
              {
            BST_print(G[i].gr);
            }
            printf(">,SUBLIST:");
            if(G[i].gsub!=NULL)
            {
              start=G[i].gsub;
            L_Print(G[i].gsub);
            }
            printf("\n");
             while(G[i].gsub!=NULL)
             {
             printf("SubscriberId:%d,GROUPLIST:%d,TREELIST\n",G[i].gsub->sId,G[i].gId);
             if(MinLo(HashTable[hasf(G[i].gsub->sId)]->tgp[G[i].gId])->ttm<tm)
             {
             BST_Tprint(HashTable[hasf(G[i].gsub->sId)]->tgp[G[i].gId]);
             }
             G[i].gsub=G[i].gsub->snext;
             } 
            G[i].gsub=start;
            free(ITM);
            free(IID);
      }
      }
    }
    return EXIT_SUCCESS;
}
/**
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(int sId){
  if(pdone!=0)
  {
  int hcode=hasf(sId);
  struct TreeInfo* last=NULL;
  for(int i=0; i<MG; i++)
  {
     if(L_lookup(G[i].gsub,sId)!=-1)
     {
      if(HashTable[hasf(sId)]->sgp[G[i].gId]!=NULL)
      {
        printf("GROUPID:=<%d>,TREELIST=<",G[i].gId);
      while(HashTable[hasf(sId)]->sgp[G[i].gId]!=NULL)
      {
        printf("%d,",HashTable[hasf(sId)]->sgp[G[i].gId]->tId);
        if(HashTable[hasf(sId)]->sgp[G[i].gId]->next==NULL)
        {
          last=HashTable[hasf(sId)]->sgp[G[i].gId];
        }
        HashTable[hasf(sId)]->sgp[G[i].gId]=HashTable[hasf(sId)]->sgp[G[i].gId]->next;
      }
      printf(">,NEWSGP=<%d>\n",last->tId);
      HashTable[hasf(sId)]->sgp[G[i].gId]=last;
      }
     }
  }
  }
    return EXIT_SUCCESS;
}
/**
 * @brief Delete subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Delete_Subscriber(int sId){ 
   printf("SUBSCRIBERLIST=<");//output to the user
  S=starts;//same as consume
  if(starts->sId==sId)//if the pointer that we use to hold the first node of the list is the element we want to delete just make it to te second element so it is first again after we dlete it
  {
   starts=starts->snext;
  }
  if(L_lookup(S,sId)!=-1)
  {
  L_Delete(&S,sId);//delete it from the list with all subs
  S=starts;
  L_Print(S);//print all subs
  printf(">\n");
  for(int i=0; i<MG; i++)
  {
    if(G[i].gsub!=NULL)//if the group id has any sub
    {
     if(L_lookup(G[i].gsub,sId)!=-1)//if it has this sub
     {
         printf("GROUPID<%d>,",G[i].gId);  //output to the user
         printf("SUBLIST=<");//output to ther user
           k11=1;//for output purpose
            L_Delete(&G[i].gsub,sId);//delete it from the subscription list of this group
     }
    if(k11==1)
     {
     L_Print(G[i].gsub);
     printf(">\n");
     }
     k11=0;
  }
  }
    Sl_Delete(&HashTable[hasf(sId)],sId);
   for(int j=0; j<MG; j++)
    {
       if(HashTable[hasf(sId)]->tgp[j]!=(struct TreeInfo*)1)
       {       
        HashTable[hasf(sId)]->sgp[j]=NULL;
        DestroyTree(HashTable[hasf(sId)]->tgp[j]);
       }
     }
  }
    return EXIT_SUCCESS;
}
/**
 * @brief Print Data Structures of the system
 *
 * @return 0 on success
 *          1 on failure
 */
int Print_all(void){
 int NO_GROUPS=0,NO_SUBSCRIBERS=0;
  for(int i=0; i<MG; i++)
  {
    if(G[i].gr!=NULL ||G[i].gsub!=NULL)
    {
       printf("GROUPID<%d>,",G[i].gId); //output the gorup number to the user
        printf("INFOLIST=");//infolist
         BST_print(G[i].gr);//infolist
         printf(",");
        printf("SUBLIST=");//sublist
        L_Print(G[i].gsub);//sublist
        printf("\n"); 
        NO_GROUPS++;//increase the number of groups by 1
    }
  }
  if(starts!=NULL && S!=NULL)
  {
printf("%d",starts->sId);
  S=starts;
  printf("SubscriberList:");
  L_Print(S);
   S=starts;
   printf("\n");
    for(int i=0; i<MG; i++)//for all gorups
    {
      if(S!=NULL)
      {
        printf("SUBSCRIBERID=%d ",S->sId);//print the sub
         printf("GROUPLISTS=<\n");
        for(int j=0; j<MG; j++)
        {
          if(L_lookup(G[j].gsub,S->sId)!=-1)//check if the sub exists for this gid
          {
              printf("GID:");
              printf("%d,",G[j].gId);//print the gorup
             
              printf("TREELIST:\n");
              BST_Tprint(HashTable[hasf(S->sId)]->tgp[G[j].gId]);
          }
        }
         S=S->snext;//make forward the list
        printf(">\n");
        NO_SUBSCRIBERS++;//increase the number of subs
      }
    }
  }
    printf("NO_GROUPS=<%d>",NO_GROUPS);//let the user know about the number of groups
    printf(" NO_SUBSCRIBERS=<%d>\n",NO_SUBSCRIBERS);//let the user know abou the number of subs*/
    return EXIT_SUCCESS;
}
