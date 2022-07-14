#include <stdio.h>
#include "avltree.h"

char dump(nodes, rootp)
	int *nodes;
	tree *rootp;
{
	tree *nodep = (tree *)nodes;
	char i;
       	printf("Root: %04X, array: %04X\n", rootp, nodes);
       	for (i=26; i>0; --i) {
       		printf("%c%01d/%02d:%04X %04X%04X ",*(((char *)nodep)+10),(int)nodep->height,nodep->weight,nodep->parent,*getBr(nodep,6),*getBr(nodep,8)); 
       		nodep = (tree *)(((char *)nodep)+16);
       	}
       	putchar('\n');
}

recursive char drTree(parPos, node)
	char parPos;
	tree *node;
{
	char i, lWidth, rWidth;
	tree *lNode;
	tree *rNode;
	if (!node) return 0;
	lNode= *getBr(node, 6);
	rNode= *getBr(node, 8);
	lWidth = lNode?lNode->weight+1:1;
	rWidth = rNode?rNode->weight:0;
	putchar('\37');
	drTree(lWidth, lNode);
	if (parPos < lWidth) {
	       	putchar('\36');
		for (i=lWidth-1; i; --i) putchar('\35');
		for (i=lWidth-1; i; --i) puts("\01W");
		puts("\01Y\37\35");
	}
	putchar(((char *)node)[10]);	
	drTree(0, rNode);
	putchar('\36');
	if (parPos > lWidth) {
		for (i=rWidth+1; i; --i) putchar('\35');
		puts("\01X");
		for (i=rWidth; i; --i) puts("\01W");
	}
}

int trDMrk(from, to)	/* Tree, decide by mark */
	tree *from;
	tree *to;
{
	from = (tree *)((int)(((char *)to)[10]) - (int)(((char *)from)[10]));
	return ((int)from)==0?0:((int)from)>0?8:6;
}

char main(argc, argv)
	int argc;
	char *argv[];
{
	int nodes[26*8];
	char key, code;
	tree *rootp = 0;
	tree *nodep = 0;
	tree **nodepp = 0;
	char *msg = "Start";

       	nodep = (tree *)nodes;
	memset(nodes, 0, 26*16);
	key = (char)64;
	for(code=27; code>1; --code) {
		((char *)nodep)[10] = ++key;
		nodep = (tree *)(((char *)nodep)+16);
	}
	key = code = 0;
	while (key != 27) {
		printf("%s\n", msg);

		/* Draw the tree */
		dump(nodes, rootp);

		nodep = rootp?*getBr(rootp, 6):rootp;
		key = nodep?nodep->weight+1:(char)1;

		drTree(key, rootp);
		putchar('\n');

		key = getch() & (char)0xDF;	/* Latin to uppercase */
		/* Clear screen */
		putchar('\f');
		/* Handle the key,
		   8=bkspc, 27=esc, 28=right, 29=left, 30=up, 31=down, 127=del */
		if (key>='A' && key<='Z') { 
			key -= 'A';
			nodep = (tree *)(((char *)nodes)+((int)key)*16);
			nodepp = trFind((char)0, &rootp, nodep, trDMrk);
			if (*nodepp == nodep) {
				msg = "Deleted";
				trIter(&rootp, trGCPP(&rootp, trDel(nodepp, trDMrk)), trUpd);
			} else {
				msg = "Inserted";
				trIter(&rootp, trGCPP(&rootp, trIns(&rootp, nodep, trDMrk)), trUpd);
			}
		}
	}
	return (char)0;
}

