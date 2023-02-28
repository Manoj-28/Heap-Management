#include<stdio.h>
#include<stdlib.h>

typedef enum {free_node,allocated} status;

int count=1;						//keep track of tag no.s


typedef struct allocate_tag
{
	int size;
	status sc;
	int tag;
	struct allocate_tag* next;
	struct allocate_tag* prev;
}allocate_node;

allocate_node* head=NULL;


void create_heap()							//function to create a heap(node) of any size
{
	int heap_size;
	printf("Enter heap size\n");
	scanf("%d",&heap_size);
	allocate_node* f;
	f = (allocate_node*)malloc(sizeof(allocate_node));
	f->size = heap_size;
	f->sc= free_node;
	f->tag = -1;
	head=f;
	f->next=NULL;
	f->prev=NULL;		
}

//free_ptr search and points to free node of required size
allocate_node* check_free(allocate_node* free_ptr,int n)					//function to check free memory in heap
{
	int flag=1;
	while(free_ptr != NULL && (flag==1))
	{
		if(free_ptr->sc == free_node)
		{
			if(free_ptr->size >= n)
			{
				flag=0;
			}
			else
			{
				free_ptr= free_ptr->next;
			}
		}
		else
		{
			free_ptr= free_ptr->next;
		}
	}
	
	return free_ptr;
}

void allocate(int n)									//function allocate memory on heap using first fit algorithm
{
	allocate_node* M;
	allocate_node* temp;
	allocate_node* free_ptr = head;
	
	free_ptr = check_free(free_ptr, n);
	if(free_ptr == NULL)						// no new node is created as there is not enough space
	{
		printf("No free space to allocate\n");
	}
	else if(free_ptr->size == n)				//in this condition no need to create new node
	{
		temp= free_ptr;
		free_ptr->size = n;
		free_ptr->sc = allocated;
		free_ptr->tag = count;
		count++;
	}
	else										//whereas here we have to create a new node 
	{
		temp= free_ptr;
		M = (allocate_node*)malloc(sizeof(allocate_node));
		M->size =n;
		free_ptr->size = free_ptr->size - n;
		M->sc= allocated;
		if(temp->prev != NULL)
		{
			temp= temp->prev;
			temp->next = M;
			M->prev = temp;
			M->next = free_ptr;
			free_ptr->prev= M;
		}
		else
		{
			M->prev= NULL;
			M->next = free_ptr;
			free_ptr->prev= M;
			head = M;
		}
		M->tag = count;
		count++; 		
	}

}


void free_heap(int num)										//function to free memory on heap
{
	allocate_node* ptr=head;
	allocate_node *front,*back, *dptr;
	while(ptr->tag != num && (ptr->next!= NULL))
	{
		ptr= ptr->next;
	}
	if(ptr->tag == num)
	{
		front = ptr->next;
		back = ptr->prev;
		if(front != NULL)							
		{
			if(front->sc == free_node)					//if the next node is also free then we merge them  
			{
				ptr->size = ptr->size + front->size;
				dptr = front;
				ptr->next = front->next;
				if(front->next != NULL)
				{
					front = front->next;
					front->prev = ptr;
					free(dptr);
					dptr = NULL;
					
				}
			}
			ptr->sc = free_node;
			ptr->tag = -1;
		}
		else
		{
			ptr->sc = free_node;
			ptr->tag = -1;
		}
		
		if(back != NULL)
		{
			if(back->sc == free_node)								//if the previous node is also free then we merge them
			{
				back->size = back->size + ptr->size;
				dptr = ptr;
				back->next = ptr->next;
				if(ptr->next != NULL)
				{
					ptr = ptr->next;
					ptr->prev = back;
					free(dptr);
					dptr = NULL; 
				}
			}
			
		}				
	}
	else
	{
		printf("No memory available with tag %d\n",num);
	}
}

void print_heap()												//function to print heap
{
	allocate_node* print_ptr= head;
	while(print_ptr != NULL)
	{
		printf("%d(%d)<->",print_ptr->size, print_ptr->tag);	//it prints size(tag),if tag is -1,which represents that block is free
		print_ptr = print_ptr->next;
	}
	printf("\n");
}
 
 
 
int main()
{
	int i,n,j,flag=1;
	create_heap();
	
	printf("Enter 1 to allocate memory\n");
	printf("Enter 2 to free memory\n");
	printf("Enter 3 to print heap\n");
	printf("Enter 0 to exit the program\n");
	
	while(flag==1)
	{
		scanf("%d",&i);
		switch(i)
		{
			case 0:
				{
					flag=0;
					break;
				}
			case 1:
				{
					printf("Enter size to allocate\n");
					scanf("%d",&n);
					allocate(n);
					break;
				}
			case 2:
				{
					printf("Enter tag no to free\n");
					scanf("%d",&j);
					free_heap(j);
					break;
				}
			case 3:
				{
					print_heap();
					break;	
				}
			default:
				{
					printf("Please enter correct input\n");
					break;	
				}	
		}
	}
}