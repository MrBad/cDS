/**
 * Generic test - inspired by http://eradman.com/posts/tdd-in-c.html
 */
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "list.h"
#include "tree.h"

int tests_run = 0;
int assertions = 0;

#define FAIL() printf("\n\033[31mfailure in %s() line %d\033[0m\n", __func__, __LINE__)
#define _assert(test) do { assertions++; if (!(test)) { FAIL(); return 1; } } while(0)
#define _verify(test) do { int r = test(); tests_run++; if (r) return r; } while(0)

// List tests
int test_list()
{
    list_t *l = list_open(NULL);
    
    _assert(l != NULL);
    _assert(l->del_func == NULL);
    _assert(l->items == 0);
    _assert(l->head == 0);
    _assert(l->head == l->tail);

    list_node_t *n1 = list_add(l, "first element");
    _assert(l->head == n1);
    _assert(l->head == l->tail);
    _assert(n1 != NULL);
    _assert(l->items == 1);

    list_node_t *n2 = list_add(l, "second element");
    _assert(n2 != NULL);
    _assert(l->head != l->tail);
    _assert(l->tail == n2);
    _assert(l->head->next == l->tail);
    _assert(l->items == 2);

    list_node_t *n3 = list_add(l, "third element");
    _assert(n3 != NULL);
    _assert(l->tail == n3);
    _assert(l->head->next->next == l->tail);
    _assert(l->items == 3);

    _assert(list_del(l, n2) == 0);
    _assert(l->items == 2);
    _assert(l->head->next == l->tail);
    _assert(list_del(l, n3) == 0);
    _assert(l->items == 1);
    _assert(l->head == l->tail);
    _assert(l->head == n1);

    list_close(l);

    return 0;
}

// Tree tests
typedef struct {
    char **sortedStrings;
    unsigned int idx;
} strSortedContext_t;

int testTreeSortAsc(void *data, void *context)
{
    strSortedContext_t *strSortedContext = (strSortedContext_t *) context;
    _assert(strcmp(data, strSortedContext->sortedStrings[strSortedContext->idx]) == 0);
    strSortedContext->idx++;

    return 0;
}

int testTreeSortDesc(void *data, void *context)
{
    strSortedContext_t *strSortedContext = (strSortedContext_t *) context;
    _assert(strcmp(data, strSortedContext->sortedStrings[strSortedContext->idx]) == 0);
    strSortedContext->idx--;

    return 0;
}

int tree_str_cmp(void *a, void *b)
{
    return strcmp(a, b);
}

int test_tree()
{
    char *str[] = {
        "popescu",
        "ionescu",
        "other",
        "vasilescu",
        "blah blah",
        "testing",
        "zamolxe",
        "mrbadnews"
    };

    char *strSorted[] = {
        "blah blah",
        "ionescu",
        "mrbadnews",
        "other",
        "popescu",
        "testing",
        "vasilescu",
        "zamolxe"
    };

    unsigned int i;
    tree_t *t = tree_open(tree_str_cmp, NULL);
    _assert(t != NULL);
    _assert(t->root == NULL);

    for (i = 0; i < sizeof(str) / sizeof(str[0]); i++) {
        _assert(tree_add(t, str[i]) == 0);
    }

    for (i = 0; i < sizeof(str) / sizeof(str[0]); i++) {
        _assert(tree_exists(t, str[i]) == 0);
    }
    _assert(tree_exists(t, "Not existing") < 0);
    strSortedContext_t strSortedContext;
    strSortedContext.sortedStrings = strSorted;
    strSortedContext.idx = 0;

    tree_foreach(t, testTreeSortAsc, SORT_ASC, &strSortedContext);
    strSortedContext.idx = sizeof(str) / sizeof(str[0]) - 1;
    tree_foreach(t, testTreeSortDesc, SORT_DESC, &strSortedContext);

    _assert(tree_exists(t, "ionescu") == 0);
    _assert(tree_del(t, "ionescu") == 0);
    _assert(tree_exists(t, "ionescu") < 0);

    _assert(tree_close(t) == 0);

    return 0;
}

int all_tests()
{
    _verify(test_list);
    _verify(test_tree);

    return 0;
}

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    int result = all_tests();
    if (result == 0) 
        printf("\n\033[32mTESTS PASSED\033[0m\n");

    printf ("Tests run: %d, assertions: %d\n", tests_run, assertions);

    return result != 0;
}

