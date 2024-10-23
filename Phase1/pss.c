/***************************************************************
 *
 * file: pss.h
 *
 * @Author  Nikolaos Vasilikopoulos (nvasilik@csd.uoc.gr)
 * @Version 20-10-2020
 *
 * @e-mail       hy240-list@csd.uoc.gr
 *
 * @brief   Implementation of the "pss.h" header file for the Public Subscribe System,
 * function definitions
 *
 *
 *************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include "pss.h"
struct Group G[MG];//Group
struct SubInfo *s[MG],t[MG];//subInfo 
struct Subscription *S=NULL,*starts=NULL,*starts1[MG];//used for subinfo for each group,for holding the first element of the subcribers
struct Info *First[MG],*startI[MG];//holding the first element of Info
int carr[MG],ccounter=0;//used for checking if consume with the same sid is given already,the counter of the array
int mg1now=0,mgsnow=0;//counters attached to arrays that used for checking for wrong gid or Iid or sId
int k11=0,arrayid[MG],arraysid[MG],arrayi[MG],arrays[MG],*gdsa=NULL;//the arrays
void printReverse(struct Info* head)//recursive function that is used for printing backwards(like a stack ) for the consume purpose!
{
    if (head == NULL)
    {
       return;
    }
    printReverse(head->inext);//recursive call
    printf("%d->", head->iId);//print the item
}
/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 0 on success
 *         1 on failure
 */
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
    p1->snext=NULL;//and make the next element of this node to point to null
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
void Dl_Print(struct Info *p)//Dl_printf void function that its funtionallity is the same as Sl_Print but its used for Doubly linked sorted list type ofInfo
{
    struct Info *p1=p;
    printf("<");
    while(p1!=NULL)
    {
        printf("%d,",p1->iId);
        p1=p1->inext;
    }
    printf(">");
}
int  I_Print(int gids_arr[],int num)//I print used for printing the gid that we take from each line same functionallity as the other prints but here we print a static array of structs 
{
    for(int i=0; i<num-1; i++)
    {
    printf("GROUPID<%d>,",gids_arr[i]);
    printf("INFOLIST:");
    Dl_Print(G[gids_arr[i]].gfirst);
    printf("\n");
    }
     return EXIT_SUCCESS;
}
int initialize(void){//initialize funtcion
 for(int i=0; i<MG; i++)//until MG
  {
    carr[i]=-1;//used for the consume purpose
    starts1[i]=NULL;
    First[i]=NULL;
    s[i]=NULL;//the subinfo used for each group
    for(int j=0; j<MG; j++)
     {
    t[i].sgp[j]=(struct Info*)1;//the sgp pointers of each subscriber!
     }
    G[i].gId=i;// the gid of each group
    G[i].gfirst=NULL;//the first element of each group
    G[i].glast=NULL;//the last element of the the info list of each group
    G[i].ggsub=NULL;//for subcriber of each group
  }
    return EXIT_SUCCESS;
}
void DL_Insert(struct Info **p,int itm,int iId)// same as sl_insert but this is used for inserting elements to a doubly linked list type of Info
{
    struct Info *p1,*p2=(*p),*p3;
     p1=(struct Info *)malloc(sizeof(struct Info));
    if(!p1)
    {
        printf("Something went wrong in memory allocation");
        exit(1);
    }
    p1->itm=itm;
    p1->iId=iId;
    p1->inext=NULL;//make the next NULL
    p1->iprev=NULL;//make the prev NULL
    if((*p)==NULL)
    {
     (*p)=p1;
    }
    else{
    if(p1->itm<(*p)->itm)//if its value is less than the value of the head make thisnode the head
    {
         p1->inext=(*p);//the next pointer of this node points to the previous head
        (*p)=p1;//make head this node
        p1->inext->iprev=p1;//and make the previous pointer of the next element point to this node
    }
    else
    {
      p3=p2;
      p3=p3->inext;
      while(p3!=NULL && p3->itm<=p1->itm)
      {
        p2=p2->inext;
        p3=p3->inext;
      }
        p1->inext=p2->inext;
        p2->inext=p1;
        p1->iprev=p2;
        if(p2->inext!=NULL)// if we are not talking about the last element
        {
        p2->inext->iprev=p2;
        }
    }
    }
}
int  Dl_LookUp(struct Info *p,int iId)
{
 int i=0; 
 struct Info *p1=p;
 while(p1!=NULL && p1->iId!=iId)
 { 
    i++;
    p1=p1->inext;
 }
 if(p1!=NULL)
 {
    return 0;
 }
 else
 {
    return -1;
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
int  findSid(int cc[MG],int sId)//used for trying to find multiply consumes with the same sId
{
  for(int i=0; i<MG; i++)
  {
     if(carr[i]==sId && i!=ccounter)
     {
      return 0;//if its found return 0
     }
  }
  return 1;//if its not reutrn 1
}
int check(int r[],int mgnow)//this function used for multipli gids in the same row or multiply sid iid or stms itms it the same file
{
  for(int i=0; i<mgnow-1; i++)//this while ends -1 before
  {
    for(int j=i+1; j<mgnow; j++)//starts from i+1 beacuse we dont want to compare the same item 
    {
        if(r[i]==r[j])//if we have duplicates
        {
          return 1;//return 1
        }
    }
  }
  return 0;//if not return 0
}
/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */
int free_all(void){
  for(int i=0; i<MG; i++)
  {
     free(s[i]);
  }
  free(S);
  free(starts);
 // free(starts1);
 // free(First);
 // free(startI);
  free(gdsa);
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
int  Insert_Info(int iTM,int iId,int* gids_arr,int size_of_gids_arr)
{
  gdsa=(int *)malloc(sizeof(int)*size_of_gids_arr);//used for checking same gids in the line 
  for(int i=0; i<size_of_gids_arr-1; i++)
  {
    gdsa[i]=gids_arr[i];//insert the gids
  }
  arrayi[mg1now]=iTM;//insert the itm because we need to check if this itm already found in this line
   mg1now++;
  if(check(arrayi,mg1now)==0)//if the itm is not found in this file the gids are different bettwen them in this line
  {
  if(iId<MG)//if iId<64
  {
  for(int i=0; i<size_of_gids_arr-1; i++)//until size-1 because we dont want -1
  {
    if(gids_arr[i]<MG)//if gids are not bigger than mg that is 64
    {
    
    if(Dl_LookUp(G[gids_arr[i]].gfirst,iId)==-1)
     {
    //  printf("%d",Dl_LookUp(G[gids_arr[i]].gfirst,iId));
    DL_Insert(&G[gids_arr[i]].gfirst,iTM,iId);//insert the itm and iId into the gfirst pointer that is type of info
    printf("GROUPID<%d>,",gids_arr[i]);//output to the user
    printf("INFOLIST:");//output to the user
    for(int k=0; k<MG; k++)//for each element of this line
    {
      if(G[gids_arr[i]].gfirst==NULL)//if the infolist is null of this group
       {
       t[k].sgp[gids_arr[i]]=NULL;//then make the pointer sgp of the subscriber k =null cause there is nothing in this infolist
      }
      else//else
       {
       t[k].sgp[gids_arr[i]]=G[gids_arr[i]].gfirst;//make it point to gfirst
      if( First[gids_arr[i]]==NULL ||  First[gids_arr[i]]->itm>G[gids_arr[i]].gfirst->itm)//if the first is null or the first changed 
       {
       First[gids_arr[i]]=t[k].sgp[gids_arr[i]];//make it point again or for the first time to the first element of the sgp pointer of the k subscriber
       }
      t[k].sgp[gids_arr[i]]=t[k].sgp[gids_arr[i]]->inext;//make the sgp pointer forward
     }
       }
        Dl_Print(G[gids_arr[i]].gfirst);//output to the user
    printf("\n");
    }
    }
    }
  }
  }
  else//if something went wrong
  {
   for(int i=0; i<size_of_gids_arr-1; i++)
  {
    gdsa[i]=-1;
  }
    mg1now--;
   arrayi[mg1now]=-1;//take the itm  out of the array beacuse we didnt insert its inforamtion to the groups 
   mg1now++;
  }
return EXIT_SUCCESS;
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
    arrays[mgsnow]=sTM;//insert the stm to this array
    arraysid[mgsnow]=sId;
    mgsnow++;
    gdsa=(int *)malloc(sizeof(int)*size_of_gids_arr);//allocate memory for the pointer that is used for to check if there is any duplicates to this line
   for(int i=0; i<size_of_gids_arr-1; i++)
  {
    gdsa[i]=gids_arr[i];//insert hte gids

  }
  if(check(arrays,mgsnow)==0 )//same as insertifo
  {
  if(sId<MG)
  {
  for(int i=0; i<size_of_gids_arr-1; i++)
  {
    if(gids_arr[i]<MG)
    {
    if(L_lookup(G[gids_arr[i]].ggsub,sId)==-1)
    {
    struct Subscription *p=NULL;
    if(L_lookup(S,sId)==-1)//if the subscriber is not already to this list that is used to hold all the subscriber that attached to all groups
    { 
      L_insert(&S,sId);//insert the new sub
    }
    if(starts==NULL)//if the pointer that points the first element of the list up
    {
      starts=S;//insert if its tghe first element
    }
    if(G[gids_arr[i]].gfirst==NULL)//if the gorup that the sub with number sid has no info that means null
     {
       t[sId].sgp[gids_arr[i]]=NULL;//make its sgp null
     }
   else
    {
      if(G[gids_arr[i]].gfirst!=NULL)//if its not null
      {
      t[sId].sgp[gids_arr[i]]=G[gids_arr[i]].gfirst;//insert the gfirst pointer to sgp(gid) of this sub
      }
      if( First[gids_arr[i]]==NULL ||  First[gids_arr[i]]->itm> t[sId].sgp[gids_arr[i]]->itm)//same as insert
       {
       First[gids_arr[i]]=t[sId].sgp[gids_arr[i]];
      }
     t[sId].sgp[gids_arr[i]]=t[sId].sgp[gids_arr[i]]->inext;
      }
     Sl_Insert(&s[gids_arr[i]],sTM,sId);//insert the subscriber to the subinfo array of lists
    if(G[gids_arr[i]].ggsub==NULL)//if the sub is not already been in the group struct
    {
      L_insert(&G[gids_arr[i]].ggsub,sId);//insert it
      starts1[gids_arr[i]]=G[gids_arr[i]].ggsub;//and hold its first element
    }
    else{
      L_insert(&G[gids_arr[i]].ggsub,sId);
    }
    printf("GROUPID<%d>,",gids_arr[i]);  
    printf("SUBLIST:");
    Sl_Print(s[gids_arr[i]]);
    printf("\n");
   }
   }
  }
   }
  }
  else
  {
  for(int i=0; i<size_of_gids_arr-1; i++)//this block same as inserinfo!
  {
    gdsa[i]=-1;
  }
    mgsnow--;
    arrays[mgsnow]=-1;
    mgsnow++;
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
  carr[ccounter]=sId;//used to check if an sid like this already consumed 
  S=starts;//the pointer starts holds the first node of the list S and beacuse i make function that called by refference and they move S i point again the first element to this list 
  for(int i=0; i<MG; i++)
  {
      if(L_lookup(G[i].ggsub,sId)!=-1)//if the sid given is valid
       {
         printf("GROUPID<%d>,",G[i].gId);//output to the user
         printf("INFOLIST:");//output to the user
         if(First[i]!=NULL && t[sId].sgp[i]!=(struct Info*)1)//if the the goruplist has infos or the sgp is used
          {
            if(findSid(carr,sId)!=0)//the function that is told before
            {
              t[G[i].gId].sgp[i]=First[i];//insert the newsgp to the sgp
               printReverse(First[i]);//printbackwards the list
               First[i]=t[G[i].gId].sgp[i];
               printf("NEWSGP:%d",t[G[i].gId].sgp[i]->iId);//out the new sgp to the user
            }
          }
          printf("\n");
     }
  }
   ccounter++;//increase the elements of the sid given for consume
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
  L_Delete(&S,sId);//delete it from the list with all subs
  S=starts;
  L_Print(S);//print all subs
  printf(">\n");
  for(int i=0; i<MG; i++)
  {
    if(G[i].ggsub!=NULL)//if the group id has any sub
    {
     if(L_lookup(G[i].ggsub,sId)!=-1)//if it has this sub
     {
         printf("GROUPID<%d>,",G[i].gId);  //output to the user
         printf("SUBLIST=<");//output to ther user
       k11=1;//for output purpose
     }
     L_Delete(&G[i].ggsub,sId);//delete it from the subscription list of this group
     Sl_Delete(&s[i],sId);//delete it from the subinfo list
     if(k11==1)
     {
     L_Print(G[i].ggsub);
     printf(">\n");
     }
     k11=0;
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
  int NO_GROUPS=0;//number of groups
  int NO_SUBSCRIBERS=0;// number of subs
  for(int i=0; i<MG; i++)//for all groups
  {
    if(G[i].gfirst!=NULL || G[i].ggsub!=NULL )//if there is either a sub list or infolist for this group
    {
       printf("GROUPID<%d>,",G[i].gId); //output the gorup number to the user
        printf("INFOLIST=");//infolist
         Dl_Print(G[i].gfirst);//infolist
         printf(",");
        printf("SUBLIST=");//sublist
        L_Print(G[i].ggsub);//sublist
        printf("\n"); 
        NO_GROUPS++;//increase the number of groups by 1
    }
  }
    S=starts;//already told fo r this
    printf("SUBSCRIBERLIST=");//output to the user
    L_Print(S);//print the list with all subs
      S=starts;
    printf("\n");
    for(int i=0; i<MG; i++)//for all gorups
    {
      if(S!=NULL)//if the is any subleft
      {
        printf("SUBSCRIBERID=%d ",S->sId);//print the sub
        printf("GROUPLIST=<");
        for(int j=0; j<MG; j++)
        {
          if(L_lookup(G[j].ggsub,S->sId)!=-1)//check if the sub exists for this gid
          {
              printf("%d,",G[j].gId);//print the gorup
          }
        }
         S=S->snext;//make forward the list
        printf(">\n");
        NO_SUBSCRIBERS++;//increase the number of subs
      }
    }
    printf("NO_GROUPS=<%d>",NO_GROUPS);//let the user know about the number of groups
    printf(" NO_SUBSCRIBERS=<%d>\n",NO_SUBSCRIBERS);//let the user know abou the number of subs
    return EXIT_SUCCESS;
}