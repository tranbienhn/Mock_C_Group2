#include "Linked_List.h"

static Node_Address_t * HEAD = NULL;

void List_Address_Add_Node(uint32_t Address)
{
	Node_Address_t* ptr;
	ptr = (Node_Address_t*)malloc(sizeof(Node_Address_t));
	ptr->Address = Address;
	ptr->pNext = HEAD;
	HEAD = ptr;
}

void List_Address_DelNode()
{
	Node_Address_t* ptr;
	ptr = HEAD;
	HEAD = HEAD->pNext;
	free(ptr);
}

uint32_t List_Address_GetAddr()
{
	return HEAD->Address;
}

void List_free()
{
	Node_Address_t* freeNode = HEAD;
	
	while (HEAD != NULL)
	{		
		HEAD = HEAD->pNext;
		free(freeNode);
		freeNode = HEAD;
	}
}

