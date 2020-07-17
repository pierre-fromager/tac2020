/**
 * @brief dynamic stack
 * 
 */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef DDStack_H
#define DDStack_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DSTACK_INIT_SIZE 2
#define DSTACK_INIT_TOP -1

    //typedef char String50 { char x[DSTACK_INIT_SIZE]; } String50;
    //typedef char Item[50];
    typedef int Item;

    typedef struct
    {
        Item *content;
        int size;
        int top;
    } DStack;

    void dstk_init(DStack *DStack);
    void dstk_destroy(DStack *DStack);
    void dstk_resize(DStack *DStack);
    int dstk_isempty(DStack *DStack);
    int dstk_isfull(DStack *DStack);
    void dstk_push(DStack *DStack, Item data);
    Item dstk_peek(DStack *DStack);
    Item dstk_pop(DStack *DStack);

#endif // ifndef DDStack_H

#ifdef __cplusplus
}
#endif