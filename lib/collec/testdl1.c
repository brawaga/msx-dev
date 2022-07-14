#include <stdio.h>
#include "dlist.h"

#define NEWKEY 0xFBE5

char dump(nodes, dlist, curr)
	dlnode *nodes;
	dlnode *dlist, *curr;
{
	char i;
	int *nodep=(int *)nodes;
       	printf("Current: %04X, first: %04X, last: %04X, array: %04X\n", curr, dlist->next, dlist->prev, nodes);
       	for (i=16; i>0; --i) {
       		printf("<%04X %04X=%c%c%c %04X>",
			nodep[1],
			(int)nodep,
			((char*)nodep)[4],
			((char*)nodep)[5],
			((char*)nodep)[6],
			nodep[0]
		); 
       		nodep+=4;
       	}
       	putchar('\n');
}

char drList(nodes, dlist, curr)
	dlnode *nodes;
	dlnode *dlist, *curr;
{
	dlnode *iter, *prev;
	if (!dlist->next) {
		if (!dlist->prev) return '\0';
		puts("<?");
		return '\2';
	}
       	if (!dlist->prev) {
		puts("?>");
		return '\1';
	}
	iter = (dlnode *)dlist->next;
	prev = 0;
	while (iter) {
		putchar((dlnode *)iter->prev!=prev?'?':iter==curr?'[':prev==curr?']':' ');
		printf("%c%c#%01X",((char*)iter)[4],((char*)iter)[5],(((char*)iter)-((char*)nodes))>>3);
		prev = iter;
		iter = (dlnode *)prev->next;
		if (prev==(dlnode *)dlist->prev && iter) {
			putchar('?');
			break;
		}
	}
	if (curr==(dlnode *)dlist->prev) putchar(']');
	else if (dlist->next && !curr) putchar('[');
}

char main(argc, argv)
	int argc;
	char *argv[];
{
	int nodes[16*4];
	dlnode dlist;
	dlnode *curr = 0;
	int *aux = 0;
	char *msg = "Start";
	char *vocal="aiou";
	char *cons="bpmf";
	char key = (char)16;
	char code = 0;

	memset(&dlist, (char)0, sizeof dlnode);
	memset(nodes, (char)0, 16*8);
	curr = (dlnode *)nodes;
	for(code=0; code<16; ++code) {
		*((char **)&curr) += 4; 
		*((*((char **)&curr))++) = cons[code>>2];
		*((*((char **)&curr))++) = vocal[code & 3];
		*((*((int **)&curr))++) = 0x2020;
	}
	curr = 0;
	while (key != (char)155) {
		if (key) {
			printf("%s\n", msg);

			/* Draw the list */
			/* dump(nodes, &dlist, curr); */

			drList(nodes, &dlist, curr);
			putchar('\n');
		}
		key = getch();
		if (key & (char)128) key=0; else if (*(char *)(NEWKEY+6) & (char)1) key |= (char)128;
		/* Handle the key,
		   8=bkspc, 12=home, 18=ins, 27=esc, 28=right, 29=left, 30=up, 31=down, 127=del */
		switch (key) {
			case (char)12:
				msg = "Positioned to end";
				curr = (dlnode *)dlist.prev;
				break;
			case (char)18:
				msg = "Inserted after";
				aux = nodes;
				for (code=0; code<16; ++code) {
					if (!aux[0] && !aux[1] && aux!=dlist.next && aux!=dlist.prev) break;
					aux += 4;
				}
				if (code<16) dlIns(&dlist, curr, (char)0, (dlnode *)aux); else msg="Cannot insert";
				break;
			case (char)136:
				msg = "Backspace";
				if (curr) {
					curr = (dlnode *)curr->prev;
					dlDel(&dlist.next, &dlist.prev, (dlnode *)(curr?curr:&dlist)->next);
				} else dlDel(&dlist.next, &dlist.prev, dlist.prev);
				break;
			case (char)139:
				msg = "Positioned to start";
				curr = (dlnode *)dlist.next;
				break;
			case (char)146:
				msg = "Inserted before";
				aux = nodes;
				for (code=0; code<16; ++code) {
					if (!aux[0] && !aux[1] && aux!=(int *)dlist.next && aux!=(int *)dlist.prev) break;
					aux += 4;
				}
				if (code<16) curr = dlIns(&dlist, curr, (char)1, (dlnode *)aux); else msg="Cannot insert";
				break;
			case (char)155:
				break;	/* Escape */
			case (char)156:
				msg = "Positioned one step forward";
				curr = (dlnode *)(curr?curr:&dlist)->next;
				break;
			case (char)157:
				msg = "Positioned one step backward";
				curr = (dlnode *)(curr?curr:&dlist)->prev;
				break;
			case (char)255:
				msg = "Delete";
				if (curr) {
					curr = (dlnode *)curr->next;
					dlDel((dlnode **)&dlist.next, (dlnode **)&dlist.prev, (dlnode *)(curr?curr:&dlist)->prev);
				} else dlDel((dlnode **)&dlist.next, (dlnode **)&dlist.prev, (dlnode *)dlist.next);
				break;
			default:
				printf("Key: %02X\n", (int)key);
				key=(char)0;
				continue;
		}
		/* Clear screen */
		putchar('\f');
	}
	return (char)0;
}

