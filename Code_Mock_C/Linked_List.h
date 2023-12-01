#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include "ParseFat.h"

typedef struct NodeAddr
{
	uint32_t Address;
	struct NodeAddr * pNext;
} Node_Address_t;

void List_Address_Add_Node(uint32_t Address);
void List_Address_DelNode();
uint32_t List_Address_GetAddr();
void List_free();

#endif	/* _LINKED_LIST_H_ */
