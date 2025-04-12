/* Avoid multiple inclusion. */
#if !defined (_LINKED_LIST_H)
#define _LINKED_LIST_H

/**
 * Given a list head and a new link, insert that link at the head.
 * Note that if `head` may actually point to any link in a list, and
 * insertion will occur in front of the link to which `head` points.
 */
#define INSERT(head,link) {			\
    link->next = head;				\
    link->prev = NULL;				\
    if (head != NULL) {				\
      head->prev = link;			\
    }						\
    head = link;				\
  }

/**
 * Given a list head and a link in that list, remove the link.  If the
 * link is the first, then update the head to point to its successor.
 */
#define REMOVE(head,link) {			\
    if (link->next != NULL)			\
      link->next->prev = link->prev;		\
    if (link->prev != NULL)			\
      link->prev->next = link->next;		\
    else					\
      head = link->next;			\
  }

  #define CIRCULAR_INSERT(head, tail, link)         \
  do {                                                      \
    if ((head) == NULL) {                                   \
      (link)->next = (link);                                \
      (link)->prev = (link);                                \
      (head) = (tail) = (link);                             \
    } else {                                                \
      (link)->next = (head);                                \
      (link)->prev = (tail);                                \
      (tail)->next = (link);                                \
      (head)->prev = (link);                                \
      (head) = (link);                                      \
    }                                                       \
  } while (0)

  #define CIRCULAR_REMOVE(head, tail, node)                           \
  do {                                                     \
    if ((node) == (head) && (node) == (tail)) {             \
      (head) = (tail) = NULL;                               \
    } else if ((node) == (head)) {                          \
      (head) = (node)->next;                                \
      (head)->prev = (tail);                                \
      (tail)->next = (head);                                \
    } else if ((node) == (tail)) {                          \
      (tail) = (node)->prev;                                \
      (tail)->next = (head);                                \
      (head)->prev = (tail);                                \
    } else {                                               \
      (node)->prev->next = (node)->next;                    \
      (node)->next->prev = (node)->prev;                    \
    }                                                      \
    free(node);                                            \
  } while (0)


#endif /* _LINKED_LIST_H */
