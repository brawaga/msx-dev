#include "tree.h"

/* define getBr(tr, ofs) ((tree **)(((char *)(tr))+(ofs))) */
tree **getBr(tr, ofs)
	tree *tr;
	int ofs;
{
	return ((tree **)(((char *)(tr))+(ofs)));
}

tree **trFind(flags, rootpp, insVal, decide)
	char flags;
	tree **rootpp;
	int insVal;
	int (*decide)();
/* decide(tree *currp, void *ins) returns branch offset */ 
/* if it returns 0, search ends, the function returns the address
 of the pointer to this node, otherwise finally the insertion point
 will be returned. */
{
	int br;
	tree **tpp;
	char retnod = flags & T_NEMP;
	tpp = rootpp;
	while (*rootpp) {
		br = (*decide)(*rootpp, insVal);
		if (!br) return rootpp;
		tpp = getBr(*rootpp, br);
		if (retnod && !*tpp) break;
		rootpp = tpp;
	}
	return rootpp;
}

tree **trIter(rootpp, nodepp, upd)
	tree **rootpp;
	tree **nodepp;	/* Must be a pointer to a child or root pointer */
	tree **(*upd)();	/* Takes a pointer(2) to a node */
{
	while (nodepp && *nodepp) nodepp = (*upd)(rootpp, nodepp);
	return nodepp;
}

tree **trGCPP(rootpp, nodep)	/* returns child ptr to the given node */
	tree **rootpp;
	tree *nodep;
{
	tree *parp;
	if (nodep) {
		parp = (tree *)nodep->parent;
		if (parp) {
			rootpp = getBr(parp, 6);
			if (*rootpp != nodep) ++rootpp;
		}
		return rootpp;
	}
	return 0;
}

tree **trUpd(rootpp, nodepp)
	tree **rootpp;
	tree **nodepp;
{
	/* TODO probable parameters: first branch offset, last branch offset */
	tree **subpp, *subp, *nodep;
	char maxh=0;
	char h=0;
	int w=1;
	if (!nodepp) return 0;
	nodep = *nodepp;
	if (!nodep) return 0;
	subpp = getBr(nodep, 6);
	subp = *subpp;
	if (subp) {
		maxh = subp->height;
		w += subp->weight;
	}
	subp = *(++subpp);
	if (subp) {
		h = subp->height;
		if (maxh<h) maxh = h;
		w += subp->weight;
	}
	nodep->height = maxh+1;
	nodep->weight = w;
	return trGCPP(rootpp, nodep->parent);
}

tree *trStep(nodep, from, to)
	tree *nodep;
	int from, to;
{
	tree *candp;
	candp = *getBr(nodep, to);
	if (candp) while (1) {
		nodep = candp;
		candp = *getBr(nodep, from);
		if (!candp) return nodep;
	}
	while (1) {
		candp = (tree *)nodep->parent;
		if (!candp) return 0;
		if (*getBr(candp, to) != nodep) return candp; 
		nodep = candp;
	}
}

tree *trRot(nodepp, from, to) 
	tree **nodepp;
	char from;
	char to;
{
	tree **midpp;
	tree *midp;
	tree **frompp;
	tree *nodep;
	int nodew;
	int midw;
	if (from==to) return 0;
	if (!nodepp) return 0;
	nodep = *nodepp;
	if (!nodep) return 0;
	nodew = nodep->weight;
	frompp = getBr(nodep, (int)from);
	if (!*frompp) return 0;
	midpp = getBr(*frompp, (int)to); 
	midp = *midpp;
	midw = midp==0?0:midp->weight;
	nodep->weight = nodew-(*frompp)->weight+midw;
	(*frompp)->weight = nodew;
	(*frompp)->parent = nodep->parent;
	nodep->parent = (int *)*frompp;
	if (midp) midp->parent = (int *)nodep;
	*midpp = nodep;
	*nodepp = *frompp;	
	*frompp = midp;
	return nodep;
}

tree *trIns(rootpp, nodep, decide)
	tree **rootpp;
	tree *nodep;
	int (*decide)();
{
	int *parent;
	int br;
	nodep->weight = 1;
	nodep->height = 1;
	if (!*rootpp) { /* Empty tree */
		*rootpp = nodep;
		nodep->parent = 0;
		return nodep;
	}
	rootpp = trFind(T_NEMP, rootpp, nodep, decide);
	parent = (int *)*rootpp;
	br = (*decide)((tree *)parent, nodep);
	if (!br) return 0;	/* Branch not chosen */
	rootpp = getBr((tree *)parent, br);
	*rootpp = nodep;
	nodep->parent = parent;
	((tree *)parent)->weight += nodep->weight;
	return nodep;
}

tree *trDel(nodepp, decide)
	tree **nodepp;
	int (*decide)();
{
	tree *nodep, *subp, **subpp, **iterpp, **tpp;
	char cnt, i;
	int tmp;
	while (-1) {
		nodep = *nodepp;
		if (!nodep) return 0;
		/* TODO probable parameters: first branch offset, last branch offset */
		cnt = (char)0;
		subp = 0;
		subpp = 0;
		iterpp = (tree **)(((char *)nodep) + 6);
		for(i=(char)2; i>(char)0; --i) {
			tmp = (int) *iterpp;
			if (tmp) {
				subpp = iterpp; ++cnt;
			}
			++iterpp;
		}
		if (!(cnt>>1)) {
			if (cnt) {
				subp = *subpp;
				subp->parent = nodep->parent;
				*subpp = 0;
			}
       			*nodepp = subp;
			subp = (tree *)nodep->parent;
			nodep->parent = 0;
			return subp;
		}
		subp = *subpp;
		iterpp = getBr(subp, 6);	/* Subp is not left branch, so now going left */
		while (*iterpp) {
			subpp = iterpp;
			subp = *subpp;
			iterpp = getBr(subp, 6);
		}

		/* Swap places */
		*subpp = nodep;
		*nodepp = subp;
		nodepp = subpp;

		/* Swap contents */
		iterpp = (tree **)subp;
		tpp = (tree **)nodep;
		for (i=(char)3 + /* Number of branches */ (char)2; i>0; --i) {
			tmp = (int) *iterpp;
			*(iterpp++) = *tpp;
			*(tpp++) = (tree *)tmp;
		}
		iterpp = getBr(subp, 6);
		for (i=2 /* num of branches */; i>0; --i) {
			tpp = (tree **)*iterpp;
			if (tpp) {
				((tree *)tpp)->parent = (int *)subp;
				if (tpp==(tree **)nodep) nodepp = iterpp;
			}
			++iterpp;
		}
		iterpp = getBr(nodep, 8);
		for (i=2 /* num of branches */; i>1; --i) {
			tpp = (tree **)*iterpp;
			if (tpp) ((tree *)tpp)->parent = (int *)nodep;
			++iterpp;
		}
	}
}

