
#define parofs 0
#define T_NEMP ((char)1)	/* not empty */

typedef struct {
	int *next, *prev;	/* cast to (node *) */
} dlnode;

dlnode *dlInsB(firstpp, lastpp, nearp, insp);
dlnode *dlInsA(firstpp, lastpp, nearp, insp);
/* basis.next means the first element, basis.prev means the last. */
/* dir=0 to insert after, =1 before */
dlnode *dlIns(dlist, nearp, dir, insp);

dlnode *dlDel(firstpp, lastpp, delp);

