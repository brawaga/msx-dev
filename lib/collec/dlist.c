#include <stdio.h>
#include "dlist.h"

dlnode *dlInsC(afterpp, beforepp, insp)	/* DList insert common part */
	dlnode **afterpp, **beforepp, *insp;
{
	insp->next = (int *)*afterpp;
	insp->prev = (int *)*beforepp;
	*afterpp = *beforepp = insp;
	return 0;
}

dlnode *dlInsB(firstpp, lastpp, nearp, insp)
	dlnode **firstpp, **lastpp, *nearp, *insp;
{
	dlnode **pairpp = nearp?(dlnode **)&nearp->prev:lastpp;
	dlnode *pairp = *pairpp;
	if (insp) return dlInsC(pairp? (dlnode **)&pairp->next : firstpp, pairpp, insp);
	return insp;
}

dlnode *dlInsA(firstpp, lastpp, nearp, insp)
	dlnode **firstpp, **lastpp, *nearp, *insp;
{
	dlnode **pairpp = nearp?(dlnode **)&nearp->next:firstpp;
	dlnode *pairp = *pairpp;
	if (insp) return dlInsC(pairpp, pairp? (dlnode **)&pairp->prev : lastpp, insp);
	return insp;
}

dlnode **dlInsS(first, second, idx)	/* Select for insertion */
	dlnode *first, *second;
	char idx;
{
	return ((dlnode **)(first?first:second))+idx;
}

dlnode *dlIns(dlist, nearp, dir, insp)
	dlnode *dlist, *nearp, *insp;
	char dir;
{
	dlnode **pairpp = dlInsS(nearp, dlist, dir);
	dlnode *pairp = *pairpp;
	if (insp) {
		((dlnode **)insp)[dir] = pairp;
		*pairpp = insp;
		dir = (char)1-dir;
		pairpp = dlInsS(pairp, dlist, dir);
		((dlnode **)insp)[dir] = *pairpp;
		*pairpp = insp;
	}
	return insp;
}

dlnode *dlDel(firstpp, lastpp, delp)
	dlnode **firstpp, **lastpp, *delp;
{
	dlnode *next, *prev;
	if (*firstpp && *lastpp) {
		next = (dlnode *)delp->next;
		if (next) lastpp=(dlnode **)&next->prev;
		prev = (dlnode *)delp->prev;
		if (prev) firstpp=(dlnode **)&prev->next;
		*firstpp = next;
		*lastpp = prev;
		memset(delp, (char)0, 2*sizeof (dlnode*));

		return 0;
	}
	return delp;
}

