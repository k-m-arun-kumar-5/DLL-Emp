/* ======================================================================
  File Name     : Double LL - EmpID.C
  Description   : Maintaining Employee's Record using EmpID based Double Linked List.
  Author        : K.M. Arun Kumar alias Arunkumar Murugeswaran
  Date          : 27th April 2007.
  Remarks     1 : Double Link List is dedicated designed for employee record.
              2 : employee record is arranged based on Employee ID (key) in Double Linked List.
              3 : assumes that user input are valid, in order and within the range.
              4 : For input string, number of bytes is within its allocated memory size.
              5 : supports only specfic emp data or whole employee records.
              6 : by default, global execution flow flag is enabled for debugging.
  Known Bugs  1 : giving non numeric input for numeric data.
  Modification
       History  :
  Others        : Compiled and Tested in Turbo-C++ on Windows 7 (32 bit) Desktop OS.
====================================================================== */
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "time.h"
#include "stdlib.h"


#define NAME_SIZE      15
#define PHONE_SIZE     20
#define MAX_EMP_ID    (1000UL)
#define MIN_EMP_ID    (1UL)
#define MAX_OPER       05  /* add, delete, print, quit, retrieve */
#define OPER_SIZE      20  /* temp string size for input validation */
#define QUIT_OPER         00
#define ADD_OPER          01  /* insert node oper */
#define DELETE_OPER       02
#define PRINT_OPER        03
#define RETRIEVE_OPER     04
#define EMPID_TARGET    01 /* EMPID linked(single/double/circular) List */
#define DEL_WHOLE_LIST  02 /* delete whole list irrespect of keys */
#define EMPID_RANGE     04 /* range of empid for oper */
#define EMPPHONE_RANGE  05 /* range of emp phone for oper */
#define EMPNAME_TARGET  06 /* search using emp name, which is unordered & non unique */
#define TRUE            01 /* true state of  oper */
#define FALSE           00 /* false state of oper */
#define FREE_ACCESS     00 /* allow access to global mega_head's curptr */
#define EMPID_ACCESS    01 /* only empid list is allowed access to global mega_head's curptr */
#define EMPPHONE_ACCESS 02
#define NO_ACCESS       03
#define MODIFY_RECORD    04 /* Emp record to be modified */
#define NO_MODIFY_RECORD 05
#define NO_EXIST_RECORD  02 /* unique record */
#define EXIST_RECORD     03
#define TRACE_ON         1
#define TRACE_OFF        0
#define MEGA_DATA_PRINT 02  /* display mega_data info for debug */
#define INVALID_DATA    -1
#define ACCESS_DENIED   -1  /* access_curptr does not allow to access */
#define EMP_ID_FWD       6
#define EMP_ID_BACK      7

struct employee
{
  long int emp_id;                        /* Emp Id as the one key for emp linked list  */
  char emp_name[NAME_SIZE];
  char emp_phone[PHONE_SIZE];             /* phone as the other key for emp linked list  */
};

struct employee_list                      /* data struct */
{
  struct employee emp_data;
  struct employee_list *idforeptr;        /* emp_id front ptr */
  struct employee_list *idbackptr;        /* emp_id back ptr */
};
typedef struct employee_list emp_record;

struct megadata                            /* link control */
{
  emp_record *idheadptr;                   /*  contains emp_id head's node */
  emp_record *idtailptr;                   /*  contains emp_id tail's node */
  emp_record *curptr;                      /*  temp ptr contains current node */
  emp_record *preptr;                      /*  temp ptr contains predecessor's node */
  int count;                               /*  number of nodes */
  char access_curptr;                      /*  acts as control flag to access curptr */
  char id_link_access_flag;

};

/* global variable defination */
struct megadata mega_head;                 /* contains head link for emp linked list */
int trace_flag = TRACE_OFF;                 /* global trace flow for debugging */

/* function declaration */
extern int Initialize_MegaEmpList(void );
extern int Link_Menu(void);
extern int Get_EmpData(void);
extern int Add_EmpNode(int target_mode, struct employee empdata, int exist );
extern int Delete_EmpData(int target_mode, void *from_del, void *to_del, struct employee *get_delnodedata, unsigned int *node_nos_ptr );
extern int Delete_EmpNode(int target_mode);
extern int Get_DelEmpData(int del_mode);
extern int Print_EmpList( int target_mode);
extern int Search_EmpNode(int target_mode, void *dataptr, void *target, unsigned int *data_nos_ptr);
extern int Retrieve_EmpData(int retrieve_mode);
extern int Trace_flag(void);
extern int Print_EmpList(int target_mode);
extern int Access_Curptr(int access_mode);


/* =========================================================
   Function name : main
   Description   : operation on Single Linked list
   Remarks       : assume data input are valid
   Func ID       : 1
=========================================================== */
int main()
{
  char control ;
  int print_flag;
  struct employee temp_emp;        /* Temp Emp data before processing */
  int del_mode;
  int retrieve_mode;

  /* initialise the head link for employees record */
  if(!Initialize_MegaEmpList())
  {
      printf("\n ERR[1.1]: Unable to create a list");
      exit(FALSE);
  }
   /* Some Basic operation on Emp Single Linked list */
  do
  {
    control = Link_Menu();
   /* Trace_flag(); */       /* for debugging */
    switch( control)
    {
       case ADD_OPER:
	     if(!Get_EmpData())
	       continue;
	     break;
       case DELETE_OPER:
         printf("\n 1 - Emp Id, 2 - whole records: Enter kind: ");
         scanf("%d",&del_mode);
         if(!Get_DelEmpData(del_mode))
	        continue;
	 	 break;
	   case PRINT_OPER:
         printf("\n Print Emp records based on: 1 - ID, 2 - Mega Node: Enter : ");
         scanf("%d", &print_flag);
         if(!Print_EmpList(print_flag))
	       continue;
	     break;
	   case RETRIEVE_OPER:
	    /* retrieve given a phone, name or empid */
	    printf("\n Retrieve Emp records based on: 1 - ID : Enter : ");
		scanf("%d", &retrieve_mode);
		switch(retrieve_mode)
		{
		   /* maps user input name search to search mode, to avoid conflict with DEL_WHOLE_LIST macro, used in search func */
			case EMPID_TARGET    :
			    if(!Retrieve_EmpData(retrieve_mode))
                    continue;
			break;
            default:
                printf("\n ERR : Invalid retrieve mode ");
                continue;
	    }


	   break;
       case QUIT_OPER:
         if(!Get_DelEmpData(DEL_WHOLE_LIST))
         ;
	 	 break;
      default:
	     printf("\n ERR[1.2]: Illegal Linked list operation: %d", control);
     }
  } while(control != QUIT_OPER );
 return TRUE;
}

/* ==================================================================
   Function Name : Initialize_MegaEmpList(struct megadata *megaptr)
   Description   : creates a empty linked list
   Remarks       :
   Func_ID       : 2
=================================================================== */
int Initialize_MegaEmpList()
{
   /* Empty Linked list */
   mega_head.count = 0;
   mega_head.idheadptr = NULL;
   mega_head.idtailptr = NULL;
   mega_head.id_link_access_flag = FALSE;
   Access_Curptr(FREE_ACCESS);
   if(trace_flag)
        printf("\n TRACE[2.1]: A DLL Initialised");
   return TRUE;
}

/* ==================================================================
   Function Name : Link_Menu()
   Description   : get & valid input code for single linked list operation
   Remarks       : assume that input data are valid
   fUNC_ID       : 3
=================================================================== */
int Link_Menu(void)
{
  char get_oper[OPER_SIZE];
  char check_oper[OPER_SIZE];
  int get_ctrl;

  printf("\n MENU for Single Linked List Operation in Emp record :- ");
  printf("\n =======================================================");
  printf("\n 1 - Add a Emp  2 - Delete a Emp  3 - Display Emp  4 - Retrieve Emp  0 - Quit : Enter : ");
  scanf("%d",&get_ctrl);
  return get_ctrl;

}

int Trace_flag(void)
{
   printf("\n 1: Use to Enable Global Trace : ");
   scanf("%d", &trace_flag);
   return TRUE;
}

/* ==================================================================
   Function Name : Get_EmpData(void)
   Description   : get emp id & emp name
   Remarks       : assume that input data are valid & phone is not a key
   fUNC_ID       : 5
=================================================================== */
int Get_EmpData(void)
{
   struct employee get_empdata;
   struct employee retrieve_empdata;
   long int emp_id;
   char emp_phone[PHONE_SIZE];
   unsigned int emp_id_nodes;
   int modify_flag = NO_EXIST_RECORD;
   char modify;

   printf("\n Enter Employee's ID : ");
   scanf("%ld", &emp_id);
   modify_flag = Search_EmpNode(EMPID_TARGET, &retrieve_empdata, &emp_id, &emp_id_nodes);
   /* unable to access mega emp head, give up */
   if(modify_flag == ACCESS_DENIED)
   {
	   return FALSE;
   }
   if(modify_flag == TRUE )
   {   /* retrieve_empdata contains emp record for existing empid */
	   printf("\n WARN[5.1]: emp_id: %ld already exist",emp_id );
	   Access_Curptr(FREE_ACCESS);
	   if(trace_flag)
	     printf("\n TRACE[5.2]: Exist Emp record: ID: %ld, name: %s, phone: %s",retrieve_empdata.emp_id, retrieve_empdata.emp_name, retrieve_empdata.emp_phone );
	  /* printf("\n Do you want to modify its record(y/n) :");
	   scanf(" %c", &modify);
	   if(modify != 'y')
	   {
		   Access_Curptr(FREE_ACCESS);
	   	   return FALSE;
       }
       // modify existing emp record
       modify_flag = MODIFY_RECORD; */
       return EXIST_RECORD;
   }
   get_empdata.emp_id = emp_id;
   printf("\n Enter Employee's name having emp_id: %ld: ", get_empdata.emp_id);
   scanf(" %[^\n]", get_empdata.emp_name);
   printf("\n Enter Employee's phone having emp_id: %ld: ", get_empdata.emp_id);
   scanf(" %s", emp_phone);
   /* copies valid & entered phone data to its record later to be added in list */
   memcpy(get_empdata.emp_phone, emp_phone, sizeof(emp_phone));
   if(!Add_EmpNode(EMPID_TARGET, get_empdata, modify_flag ))
   	   return FALSE;

   return TRUE;
}

/* ==================================================================
   Function Name : Search_EmpNode( int target_mode, void *dataptr, void *target, int phone_size)
   Description   : search for given key
   Remarks       : assumes unique & ordered emp id & phone. doesnt support phone as key.
                   headptr, phone input data are reserved for futhur use
   fUNC_ID       : 6
=================================================================== */
int Search_EmpNode(int target_mode, void *dataptr, void *target, unsigned int *data_nos_ptr)
{
    int ret_state = FALSE;
    long int target_empid;
    char target_phone[PHONE_SIZE];
    char target_name[NAME_SIZE];
    emp_record *temp_empnode;

    mega_head.id_link_access_flag = EMP_ID_FWD;
	*data_nos_ptr = 0;
    if(mega_head.access_curptr != FREE_ACCESS)
    {
        printf("\n ERR[6.3]: Unable to access head control data: %d",mega_head.access_curptr);
		return ACCESS_DENIED;
    }
    mega_head.preptr = NULL;
    switch(target_mode)
    {
	   case EMPID_TARGET:
	      target_empid = *(long int *) target;
	      Access_Curptr(EMPID_ACCESS);
              if(target_empid < MIN_EMP_ID || target_empid > MAX_EMP_ID)
              {
                  printf("\n ERR[6.1]: Invalid Emp ID: %ld valid range[%ld, %ld]", target_empid, MIN_EMP_ID, MAX_EMP_ID);
                  return ret_state ;
              }
              /* check for emp id, which is ordered and unique */
             if(target_empid <= ((unsigned long int)(MAX_EMP_ID / 2)))
             {
                   mega_head.curptr = mega_head.idheadptr;
                   mega_head.id_link_access_flag = EMP_ID_FWD;
                   while( mega_head.curptr != NULL && mega_head.curptr->emp_data.emp_id < target_empid)
                   {

					    /* move to next node of emp id list to check again */
                        mega_head.preptr = mega_head.curptr;
		   	            mega_head.curptr = mega_head.curptr->idforeptr;
		   	            if(trace_flag)
                        {
                             printf("\n TRACE[6.2]: FWD search preptr: 0X%X, curptr: 0X%X", mega_head.preptr, mega_head.curptr);
                        }
                   }
			 }
              else
              {
                  mega_head.curptr = mega_head.idtailptr;
                  mega_head.id_link_access_flag = EMP_ID_BACK;
                  while(mega_head.curptr != NULL && mega_head.curptr->emp_data.emp_id > target_empid)
                  {
					   /* move to next node of emp id list to check again */
		   	           mega_head.preptr = mega_head.curptr;
		   	           mega_head.curptr = mega_head.curptr->idbackptr;
					   if(trace_flag)
                       {
                            printf("\n TRACE[6.3]: BACK search preptr: 0X%X, curptr: 0X%X", mega_head.preptr, mega_head.curptr);
                       }
                  }
              }
			  if(mega_head.curptr != NULL && mega_head.curptr->emp_data.emp_id == target_empid)
			  {
				   temp_empnode = mega_head.curptr;
				   ret_state = TRUE;
			       memcpy(dataptr, &mega_head.curptr->emp_data, sizeof(struct employee));
				   (*data_nos_ptr)++;
                   if(trace_flag)
				   {
					   printf("\n TRACE[6.5]: Found Emp Id: %ld, name: %s, phone: %s",
					        temp_empnode->emp_data.emp_id, temp_empnode->emp_data.emp_name, temp_empnode->emp_data.emp_phone);
					   printf("\n TRACE[6.6]: Found Emp Id: %ld, Node: 0X%X, foreptr: 0X%X, backptr: 0X%X",
					      temp_empnode->emp_data.emp_id, temp_empnode, temp_empnode->idforeptr, temp_empnode->idbackptr );
                       printf("\n TRACE[6.7]: curptr: 0X%X, head: 0X%X, tail: 0X%X, access: %d",
					        temp_empnode, mega_head.idheadptr, mega_head.idtailptr, mega_head.id_link_access_flag );

				   }
		      }
			  else
			  {
				 if(trace_flag)
					  printf("\n TRACE[6.4]: Emp Id: %ld not found ", target_empid);
			  }
		  break;
		  case DEL_WHOLE_LIST:
	      Access_Curptr(EMPID_ACCESS);
		  if(mega_head.idheadptr)
	      {
			 mega_head.id_link_access_flag = EMP_ID_FWD;
	         mega_head.curptr = mega_head.idheadptr;
	         while(mega_head.curptr)
             {
				  /* retrieves whole emp records from emp list before destroying it */
				  if((*data_nos_ptr) < OPER_SIZE)
                     *((struct employee *)dataptr + (*data_nos_ptr)++)= mega_head.curptr->emp_data;
				  else
				  {
					  printf("\n ERR[6.5]: deleted emp nos exceeds max size: %u", OPER_SIZE);
				  }
				temp_empnode = mega_head.curptr;
				free(mega_head.curptr);
                mega_head.curptr = temp_empnode->idforeptr;
	         }
	      }
	       Initialize_MegaEmpList();
	       ret_state = TRUE;
          break;
         default:
          printf("\n ERR[6.8]: doesn't support target search: %d",target_mode );
    }
    return ret_state;
}

/* ==================================================================
   Function Name : Access_Curptr(int access_mode)
   Description   : control access to head's curptr
   Remarks       :
   fUNC_ID       : 7
=================================================================== */
int Access_Curptr(int access_mode)
{
	int istrue;
	switch(access_mode)
	{
		case FREE_ACCESS:
		case EMPID_ACCESS:
		case EMPPHONE_ACCESS:
		case NO_ACCESS:
		  mega_head.access_curptr = access_mode;
		  istrue = TRUE;
		  break;
		default:
		  printf("\n ERR[7.1]: Invalid access_mode : %d",access_mode );
		  istrue = FALSE;
     }
  return istrue;
 }

/* ==================================================================
   Function Name : Access_Curptr(int access_mode)
   Description   : control access to head's curptr
   Remarks       : consider only single link list of empid
   fUNC_ID       : 8
=================================================================== */
int Add_EmpNode(int target_mode, struct employee emp_data, int modify)
{
   emp_record *new_emprecord;
   int ret_state = FALSE;

   switch(target_mode)
   {
	   case EMPID_TARGET:
	      /* make sure u know where to insert node in empid list */
	      if(mega_head.access_curptr != EMPID_ACCESS)
	      {
	      	  printf("\n ERR[8.1]: Invalid head access: %d",mega_head.access_curptr );
	      	  break;
	      }
		  if((new_emprecord = (emp_record *)calloc(1, sizeof( emp_record))) == NULL)
	      {
			 printf("\n ERR[8.2]: No new Memory to allocate %d elemnent of %d", 1, sizeof(emp_record) );
			 break;
		  }
		  new_emprecord->emp_data = emp_data;
		  if(trace_flag)
          {
               printf("\n TRACE[8.3]: Emp Record: id %ld, name: %s, phone: %s, Node: 0X%X ",
                    new_emprecord->emp_data.emp_id,new_emprecord->emp_data.emp_name, new_emprecord->emp_data.emp_phone, new_emprecord);
               printf("\n TRACE[8.4]: preptr: 0X%X, curptr: 0X%X, headptr: 0X%X, tailptr: 0X%X", mega_head.preptr, mega_head.curptr, mega_head.idheadptr, mega_head.idtailptr);
           }
          switch(mega_head.id_link_access_flag)
          {
              case EMP_ID_FWD:
                  /* updata  pointer of empid list to add as end or begin node , update mega data */
	             if(mega_head.curptr == NULL)
                  {
                     new_emprecord->idforeptr = NULL;
                     new_emprecord->idbackptr = mega_head.preptr;
                     mega_head.idtailptr = new_emprecord;
                     //only node
                     if(mega_head.preptr == NULL)
                     {
                         mega_head.idheadptr = new_emprecord;

                     }
                     else
                     {
                        mega_head.preptr->idforeptr = new_emprecord;
                     }
                  }
                  else
                  {
                      /* updata  pointer of empid list to add nodes at middle */
                      new_emprecord->idforeptr = mega_head.curptr;
                      new_emprecord->idbackptr = mega_head.preptr;
                      if(mega_head.preptr == NULL)
                      {
                          mega_head.idheadptr = new_emprecord;
                          mega_head.curptr->idbackptr = new_emprecord;
                      }
                      else
                      {
                           mega_head.preptr->idforeptr = new_emprecord;
                           mega_head.curptr->idbackptr = new_emprecord;
                      }
                  }
                 ++mega_head.count;
	             ret_state = TRUE;
                 break;
              case EMP_ID_BACK:
                 if(mega_head.curptr == NULL)
                 {
                     new_emprecord->idbackptr = NULL;
                     new_emprecord->idforeptr = mega_head.preptr;
                     mega_head.idheadptr = new_emprecord;
                     if(mega_head.preptr == NULL)
                     {
                         mega_head.idtailptr = new_emprecord;
                     }
                      else
                         mega_head.preptr->idbackptr = new_emprecord;
                  }
                  else
                  {
                      /* updata  pointer of empid list to add nodes either at middle or end */
                      new_emprecord->idbackptr = mega_head.curptr;
                      new_emprecord->idforeptr = mega_head.preptr;
                      if(mega_head.preptr == NULL)
                      {
                          mega_head.idtailptr = new_emprecord;
                          mega_head.curptr->idforeptr = new_emprecord;

                      }
                      else
                      {
                          mega_head.preptr->idbackptr = new_emprecord;
                          mega_head.curptr->idforeptr = new_emprecord;
                      }
                  }
                 ++mega_head.count;
	             ret_state = TRUE;
              break;
              case MODIFY_RECORD:
                  mega_head.curptr->emp_data = emp_data;
                  Access_Curptr(FREE_ACCESS);
	              /* TO DO: check for emp id, which is ordered and unique */
                  ret_state = TRUE;
              break;
			  default:
			     printf("\n ERR[8.5]: doesn't support access link search: %d", mega_head.id_link_access_flag);
          }
          if(trace_flag)
          {
               printf("\n TRACE[8.6]: Emp Record: id %ld, Node: 0X%X, foreptr: 0X%X, backptr: 0X%X ",
			        new_emprecord->emp_data.emp_id, new_emprecord, new_emprecord->idforeptr, new_emprecord->idbackptr);
               printf("\n TRACE[8.7]: preptr: 0X%X, curptr: 0X%X, headptr: 0X%X, tailptr: 0X%X",
			        mega_head.preptr, mega_head.curptr, mega_head.idheadptr, mega_head.idtailptr);
           }
          break;
          default:
             printf("\n ERR[8.8]: doesn't support target search: %d",target_mode );
   }
   Access_Curptr(FREE_ACCESS);
   return ret_state;
}

/* ==================================================================
   Function Name : Print_EmpList(int target_mode)
   Description   : display req data for results and for debugging
   Remarks       : consider only single link list of empid
   fUNC_ID       : 9
=================================================================== */
int Print_EmpList(int target_mode)
{
   emp_record *curptr = NULL;       /* local temp ptr contains current node */
   int ret_state = TRUE;
   int i = 0;

   if(mega_head.access_curptr != FREE_ACCESS)
   {
	   printf("\n ERR[9.1]: Unable to access head control data: %d",mega_head.access_curptr);
	   return FALSE;
   }
    switch(target_mode)
    {
		case EMPID_TARGET:
		  curptr = mega_head.idheadptr;
    	  if(curptr == NULL)
		  {
			  printf("\n Emp records does not exist");
			  return TRUE;
		  }		  
		  mega_head.id_link_access_flag = EMP_ID_FWD;
		  printf("\n Display Employee's record based on EMP_ID's list:");
		  printf("\n =================================================");
		  while(curptr != NULL)
		  {
			  ++i;
		      printf("\n %03d Node Data:: ID: %10ld, name: %10s, phone: %10s",\
		         i,curptr->emp_data.emp_id,curptr->emp_data.emp_name, curptr->emp_data.emp_phone);
			  if(trace_flag)
		        printf("\n %03d: Addr Node: %#X, ID Front: %#X, ID Back: %#X",\
		             i, curptr, curptr->idforeptr,curptr->idbackptr);
		      curptr = curptr->idforeptr;
	      }
	      printf("\n =================================================");
          break;
        case MEGA_DATA_PRINT:
           printf("\n Display Emp's Mega Node's Control Info:");
           printf("\n ==================================================");
           printf("\n No of nodes: %d, access_curptr: %d, curptr: 0X%X, preptr: 0X%X", \
            mega_head.count, mega_head.access_curptr, mega_head.curptr, mega_head.preptr);
           printf("\n idheadptr: 0X%X, idtailptr: 0X%X, id_access_flag: %d", \
            mega_head.idheadptr, mega_head.idtailptr, mega_head.id_link_access_flag);
           printf("\n =================================================="); 
          break;
        default:
           printf("\n ERR[9.1]: Invalid display mode: %d",target_mode );
           ret_state = FALSE;
    }
    return ret_state;
}

/* ==================================================================
   Function Name : Delete_EmpData(int target_mode, void *from_del, void *to_iddel, struct employee *get_delnodedata, unsigned int *emp_nodes_ptr)
   Description   : searches specific node based on target key, and deletes it from list
   Remarks       : data retrived from deleted node is retrived in its pointer.
   fUNC_ID       : 10
=================================================================== */
int Delete_EmpData(int target_mode, void *from_del, void *to_del, struct employee *get_delnodedata, unsigned int *emp_nodes_ptr)
{
	int to_nodepos =0, from_node_pos;
    int ret_state = FALSE;
    long int from_iddel, to_iddel;

	switch(target_mode)
	{
       case EMPID_TARGET:
	     ret_state = Search_EmpNode(target_mode, get_delnodedata, from_del, emp_nodes_ptr);
         switch(ret_state)
         {
			 case FALSE:
			 	 printf("\n ERR[10.1]: emp_id: %ld does not exist to delete",*(long int *)from_del );
			     /* lost address passed to retrieve data to be deleted */
			     get_delnodedata = NULL;
		     break;
			 case ACCESS_DENIED:
			     get_delnodedata = NULL;
			 break;
             case TRUE:
			     if(trace_flag)
                    printf("\n TRACE[10.1]: Emp ID: %ld, name: %s, phone: %s",
                       get_delnodedata->emp_id, get_delnodedata->emp_name, get_delnodedata->emp_phone);
		          ret_state = Delete_EmpNode(target_mode);
             break;
             default:
                get_delnodedata = NULL;
	     }

        break;
        case DEL_WHOLE_LIST:
	     ret_state = Search_EmpNode(target_mode, get_delnodedata , NULL, emp_nodes_ptr );
         switch(ret_state)
         {
			case FALSE:
			 	 printf("\n ERR[10.1]: emp_id: %ld does not exist to delete",*(long int *)from_del );
			     get_delnodedata = NULL;
		     break;
			 case ACCESS_DENIED:
			    get_delnodedata = NULL;
			 break;
             case TRUE:
             break;
             default:
                get_delnodedata = NULL;
	     }
         break;
      /* case EMPID_RANGE:
         from_iddel = *(long int *) from_del;
         to_iddel = *(long int *) to_del;
         from_nodepos = Search_EmpNode(target_mode, get_delnodedata, from_del, emp_nodes_ptr );
         if(!nodepos)
         {
			 printf("\n ERR[10.4]: Try to delete already non existing emp id's from: %ld to: %ld",from_iddel, to_iddel);
			 break;
	     }
	     return number of nodes lying within range of emp id to be deleted
	     ret_state = to_nodepos - from_nodepos;
	     break; */
       default:
         printf("\n ERR[10.2]: doesn't support target search: %d",target_mode );
    }
    /* if ret_state is FALSE, then it will not delete the emp nodes */
     /* null statement for if() */
    Access_Curptr(FREE_ACCESS);
    return ret_state;
}

/* ==================================================================
   Function Name : Delete_EmpNode(int target_mode, struct employee *emp_deldata)
   Description   : deletes specific data a node based on target key, also has deleted info in pointer
   Remarks       : for deleting whole list, nodepos is number of nodes in the list
   fUNC_ID       : 11
=================================================================== */
int Delete_EmpNode(int target_mode)
{
	emp_record *temp_ptr = NULL;
	int ret_state = FALSE;

	switch(target_mode)
	{
       case EMPID_TARGET:
	     temp_ptr = mega_head.curptr;
		 if(trace_flag)
		 {
			 printf("\n TRACE[11.1]: to deleted - ID: %ld, name: %s, phone: %s", temp_ptr->emp_data.emp_id, temp_ptr->emp_data.emp_name, temp_ptr->emp_data.emp_phone);
             printf("\n TRACE[11.2]: ID: %ld, Node:0X%X, foreptr: 0X%X, backptr: 0X%X", temp_ptr->emp_data.emp_id, temp_ptr, temp_ptr->idforeptr, temp_ptr->idbackptr);
			 printf("\n TRACE[11.3]: preptr: 0X%X, curptr: 0X%X, headptr: 0X%X, tailptr: 0X%X, access flag: %d",
			        mega_head.preptr, mega_head.curptr, mega_head.idheadptr, mega_head.idtailptr, mega_head.id_link_access_flag);
		 }
		 switch(mega_head.id_link_access_flag)
		 {
			 case EMP_ID_FWD:
			     	 //delete only node
					 if(mega_head.idheadptr == mega_head.curptr && mega_head.idheadptr == mega_head.idtailptr)
					 {
						 mega_head.idheadptr = NULL;
						 mega_head.idtailptr = NULL;
					 }
					 else
					 {
						 if(mega_head.curptr->idbackptr == NULL)
						 {
						     // delete begin node
					        mega_head.idheadptr = mega_head.curptr->idforeptr;
						    mega_head.curptr->idforeptr->idbackptr = NULL;
						 }
						 else
						 {
						    // delete end node
							if(mega_head.curptr->idforeptr == NULL)
					        {
								temp_ptr = mega_head.preptr;
								mega_head.idtailptr = mega_head.preptr;
								mega_head.preptr->idforeptr = NULL;
								temp_ptr = mega_head.curptr;
					        }
					        else
					        {
						       //delete middle node
					    	    mega_head.preptr->idforeptr = mega_head.curptr->idforeptr;
						        temp_ptr->idforeptr->idbackptr = mega_head.preptr;
					        }
					    }
					 }
			 break;
			 case EMP_ID_BACK:
			   //delete only node
					 if(mega_head.idheadptr == mega_head.curptr && mega_head.idheadptr == mega_head.idtailptr)
					 {
						 mega_head.idheadptr = NULL;
						 mega_head.idtailptr = NULL;
					 }
					 else
					 {
						 if(mega_head.curptr->idbackptr == NULL)
						 {
						     // delete begin node
					        mega_head.idheadptr = mega_head.preptr;
						    mega_head.preptr->idbackptr = NULL;
						 }
						 else
						 {
						    // delete end node
							if(mega_head.curptr->idforeptr == NULL)
					        {
								temp_ptr = mega_head.curptr;
								mega_head.idtailptr = mega_head.curptr->idbackptr;
								mega_head.curptr->idbackptr->idforeptr = NULL;
					        }
					        else
					        {
						       //delete middle node
							    mega_head.preptr->idbackptr = mega_head.curptr->idbackptr;
						        mega_head.curptr->idbackptr->idforeptr = mega_head.preptr;
					        }
					    }
					 }
			 break;
		 }
		 if(trace_flag)
		 {
			 printf("\n TRACE[11.4]: deleted - ID: %ld, name: %s, phone: %s", temp_ptr->emp_data.emp_id, temp_ptr->emp_data.emp_name, temp_ptr->emp_data.emp_phone);
			 printf("\n TRACE[11.5]: ID: %ld, Node:0X%X, foreptr: 0X%X, backptr: 0X%X", temp_ptr->emp_data.emp_id, temp_ptr, temp_ptr->idforeptr, temp_ptr->idbackptr);
			 printf("\n TRACE[11.6]: preptr: 0X%X, curptr: 0X%X, headptr: 0X%X, tailptr: 0X%X, access flag: %d",
			        mega_head.preptr, mega_head.curptr, mega_head.idheadptr, mega_head.idtailptr, mega_head.id_link_access_flag);
		 }
		 if(temp_ptr)
		 {
			free(temp_ptr);
			--mega_head.count;
            ret_state = TRUE;
		 }
        break;
	  default:
         printf("\n ERR[11.2]: doesn't support target search: %d",target_mode );
    }
    return ret_state;
}

/* ==================================================================
   Function Name : Get_DelEmpData(int del_mode)
   Description   : get either empid or ph specific or range to be deleted
   Remarks       : assume that correcponding data is valid.
   fUNC_ID       : 12
=================================================================== */
int Get_DelEmpData(int del_mode)
{
	long int from_iddel, to_iddel;
    char from_phdel[PHONE_SIZE], to_phdel[PHONE_SIZE];
    struct employee get_delempdata[OPER_SIZE];
    int ret_state = FALSE;
    unsigned int node_nos = 0, node_count = 0;
    int node_num = 0;

    switch(del_mode)
    {
		case EMPID_TARGET:
          printf("\n Enter Emp ID to be deleted: ");
          scanf("%ld", &from_iddel);
          /* here: add codes to if u want to validate is numeric one, etc */
          /* to delete specfic empid  */
		    to_iddel = from_iddel;
           if(from_iddel == to_iddel)
           {
		      if((Delete_EmpData(del_mode, &from_iddel, NULL, get_delempdata, &node_nos)) == TRUE)
			  {
			      printf("\n Deleted Data: Emp ID: %ld, name: %s, phone: %s ", \
                    get_delempdata[node_nos - 1].emp_id, get_delempdata[node_nos - 1].emp_name, get_delempdata[node_nos - 1].emp_phone);
			      ret_state = TRUE;
		      }
			  break;
	       }
		   /* range of empid to be deleted */
		   /* if(!Delete_EmpData(EMPID_RANGE, &from_iddel, &to_iddel, get_delempdata, &node_nos ))
		   	  break;
		   printf("\n ERR[12.2]: Support only single empid based deletion"); */
		   ret_state = TRUE;
		   break;
		 case DEL_WHOLE_LIST:
           if((Delete_EmpData(del_mode, NULL, NULL, get_delempdata, &node_nos )) == TRUE)
		   {
          	  while(node_count < node_nos)
			  {
				  printf("\n Whole list been Deleted Data: ID: %ld, name: %s, phone: %s", \
				   get_delempdata[node_count].emp_id, get_delempdata[node_count].emp_name, get_delempdata[node_count].emp_phone);
				  ++node_count;
		      }
			  ret_state = TRUE;
	       }
		   break;
		 default:
		   printf("\n ERR[12.1]: doesn't support target delete: %d",del_mode );
     }
     return ret_state;
 }

/* ==================================================================
   Function Name : Retrieve_EmpData(int retrieve_mode)
   Description   : get retrieve emp record based on empid, phone and name
   Remarks       : assume that correcponding input data is valid.
   fUNC_ID       : 13
=================================================================== */
int Retrieve_EmpData(int retrieve_mode)
{
	int ret_state = FALSE;
    int nodepos;
    struct employee retrieve_empdata[1];
    long int retrieve_empid;
    unsigned int node_nos =0, node_count = 0;

    switch(retrieve_mode)
    {
		case EMPID_TARGET:
		  printf("\n Enter Emp ID to retrieve its record: ");
		  scanf("%ld", &retrieve_empid);
	      if((Search_EmpNode(retrieve_mode, retrieve_empdata, &retrieve_empid, &node_nos)) == TRUE)
          {
            /* retrieve_empdata contains emp record for existing empid */
		     printf("\n Emp Record: ID: %ld, name: %s, phone: %s",
                retrieve_empdata[0].emp_id, retrieve_empdata[0].emp_name, retrieve_empdata[0].emp_phone );
	         ret_state = TRUE;
	      }
	      else
	        printf("\n ERR[13.1]: Emp ID: %ld does not exist",retrieve_empid);
	     break;
		default:
		  printf("\n ERR[13.2]: doesn't support retrievemode: %d",retrieve_mode );
    }
    Access_Curptr(FREE_ACCESS);
  return ret_state;
}
