/******************************************************************************
*                                                                             *
* -------------------------------- stack.h ---------------------------------- *
*                                                                             *
******************************************************************************/

#ifndef STACK_H
#define STACK_H

#include "list.h"

/******************************************************************************
*                                                                             *
* A stack is a singly-linked list restricted to LIFO access.  The List       *
* struct is reused directly — no additional storage is needed.               *
*                                                                             *
******************************************************************************/

typedef List Stack;

/******************************************************************************
*                                                                             *
* __________________________ Public Interface __________________________      *
*                                                                             *
******************************************************************************/

#define stack_init(stack, destroy)    list_init(stack, destroy)

#define stack_destroy(stack)          list_destroy(stack)

int stack_push(Stack *stack, const void *data);

int stack_pop(Stack *stack, void **data);

#define stack_peek(stack)             list_data(list_head(stack))

#define stack_size(stack)             list_size(stack)

#define stack_is_empty(stack)         (list_size(stack) == 0)

#endif /* STACK_H */
