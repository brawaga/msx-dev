#include "avltree.h"

tree **avlUpd(rootpp, nodepp)
	tree **rootpp;
	tree **nodepp;
{
	tree *nodep, **subpp, *subp;  
	char lh, rh;
	int w;
	lh = rh = 1;
	w = 1;
	if (!nodepp) return 0;
	nodep = *nodepp;
	if (!nodep) return 0;
	subpp = getBr(nodep, 6);
	subp = *subpp;
	if (subp) {
		lh += subp->height;
		w += subp->weight;
	}
	subp = *(++subpp);
	if (subp) {
		rh += subp->height;
		w += subp->weight;
	}
	nodep->weight = w;
	*((char *)&w) = (char)128+rh-lh;
	nodep->reserved = (char)w;
	nodep->height = rh>lh?rh:lh;
        if ((char)129 < (char)w) {
		if ((char)128 > subp->reserved) {
			avlUpd(rootpp, trGCPP(rootpp, trRot(subpp, 6, 8)));
		}
		return trGCPP(rootpp, trRot(nodepp, 8, 6));
	}
	subp = *(--subpp);
        if ((char)127 > (char)w) {
		if ((char)128 < subp->reserved) {
			avlUpd(rootpp, trGCPP(rootpp, trRot(subpp, 8, 6)));
		}
		return trGCPP(rootpp, trRot(nodepp, 6, 8));
	}
	return trGCPP(rootpp, nodep->parent);
}
