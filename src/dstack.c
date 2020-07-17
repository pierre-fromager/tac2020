
#include "dstack.h"

void dstk_init(DStack *dstack)
{
    Item *content;
    content = (Item *)calloc(DSTACK_INIT_SIZE, sizeof(Item));
    if (content == NULL)
    {
        fprintf(stderr, "Fail to initialize dstack\n");
        exit(EXIT_FAILURE);
    }
    dstack->content = content;
    dstack->size = DSTACK_INIT_SIZE;
    dstack->top = DSTACK_INIT_TOP;
}

void dstk_destroy(DStack *dstack)
{
    free(dstack->content);
    dstack->content = NULL;
    dstack->size = 0;
    dstack->top = DSTACK_INIT_TOP;
}

void dstk_resize(DStack *dstack)
{
    Item *resized_content;
    resized_content = (Item *)calloc(dstack->size * DSTACK_INIT_SIZE, sizeof(Item));
    memcpy(resized_content, dstack->content, sizeof(Item) * dstack->top + 1);
    free(dstack->content);
    dstack->content = resized_content;
    dstack->size = dstack->size * DSTACK_INIT_SIZE;
}

int dstk_isempty(DStack *dstack)
{
    return (dstack->top < 0) ? 1 : 0;
}

int dstk_isfull(DStack *dstack)
{
    return (dstack->top == dstack->size - 1) ? 1 : 0;
}

void dstk_push(DStack *dstack, Item item)
{
    if (dstk_isfull(dstack) == 1)
        dstk_resize(dstack);
    dstack->top++;
    dstack->content[dstack->top] = item;
}

Item dstk_peek(DStack *dstack)
{
    return dstack->content[dstack->top];
}

Item dstk_pop(DStack *dstack)
{
    Item data = dstack->content[dstack->top];
    dstack->top--;
    return data;
}
