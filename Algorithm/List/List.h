#ifndef  __LIST_H
#define  __LIST_H


#ifdef __CC_ARM
#define INLINE  __inline
#else
#define INLINE inline
#endif


typedef struct ListNode
{
    struct ListNode *next;                          /**< point to next node. */
    struct ListNode *prev;                          /**< point to prev node. */
    
    unsigned char Buf[];
    
}LIST_T;                                            /**< Type for lists. */
           


#ifdef __cplusplus
extern "C" {
#endif
  
/**
 * @addtogroup KernelService
 */

/*@{*/


/**
 * @brief initialize a list
 *
 * @param l list to be initialized
 */
static INLINE void List_Init(LIST_T *l)
{
    l->next = l->prev = l;
}

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
static INLINE void List_InsertAfter(LIST_T *l, LIST_T *n)
{
    l->next->prev = n;
    n->next = l->next;

    l->next = n;
    n->prev = l;
}

/**
 * @brief insert a node before a list
 *
 * @param n new node to be inserted
 * @param l list to insert it
 */
static INLINE void List_InsertBefore(LIST_T *l, LIST_T *n)
{
    l->prev->next = n;
    n->prev = l->prev;

    l->prev = n;
    n->next = l;
}

/**
 * @brief remove node from list.
 * @param n the node to remove from the list.
 */
static INLINE void List_Remove(LIST_T *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
static INLINE int List_IsEmpty(const LIST_T *l)
{
    return l->next == l;
}


#if 0
/**
 * @brief get the struct for this entry
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define List_Entry(node, type, member) \
    ((type *)((char *)(node) - (unsigned long)(&((type *)0)->member)))

/**
 * List_ListForEachEntry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define List_ListForEachEntry(pos, head, member)              \
    for (pos = List_Entry((head)->next, typeof(*pos), member);  \
         &pos->member != (head);    \
         pos = List_Entry(pos->member.next, typeof(*pos), member))

/**
 * List_ListFirstEntry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define List_ListFirstEntry(ptr, type, member) \
    List_Entry((ptr)->next, type, member)
/*@}*/


#endif

#ifdef __cplusplus
}
#endif



#endif /* __LIST_H */
