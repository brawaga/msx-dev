
#define parofs 0
#define T_NEMP ((char)1)	/* not empty */

typedef struct {
	int *parent;	/* cast to (tree *)                        */
	int weight;	/* How many nodes                          */
	char height;	/* Longest path down count nodes           */
	char reserved;	/* May be used as color for Red&Black tree */
	/* Children, allocate as many as needed.
	   So, for binary tree, allocate 10 bytes for the struct.
	   left is (tree *)((int)&tree+6),
	   right is (tree *)((int)&tree+8). */
} tree;

tree **getBr(tr, ofs);
tree **trFind(flags, rootpp, insVal, decide);
tree **trIter(rootpp, nodepp, upd);  /* upd is trUpd for simple tree */
tree **trUpd(rootpp, nodepp);	/* updates and returns next node that needs update */
tree **trGCPP(rootpp, nodep);	/* returns child ptr to the given node */
tree *trStep(nodep, from, to);

/* These return the node ptr that is starting point for trIter(*, trUpd) */
tree *trRot(nodepp, from, to); 
tree *trIns(rootpp, nodep, decide);
tree *trDel(nodepp, decide);



