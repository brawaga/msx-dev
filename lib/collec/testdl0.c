#include <stdio.h>
#include "dlist.h"

#define NEWKEY 0xFBE5

char dump(nodes, first, last, curr)
	dlnode *nodes;
	dlnode *first, *last, *curr;
{
	char i;
	int *nodep=(int *)nodes;
       	printf("Current: %04X, first: %04X, last: %04X, array: %04X\n", curr, first, last, nodes);
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

char drList(nodes, first, last, curr)
	dlnode *nodes;
	dlnode *first, *last, *curr;
{
	dlnode *iter, *prev;
	if (!first) {
		if (!last) return '\0';
		puts("<?");
		return '\2';
	}
       	if (!last) {
		puts("?>");
		return '\1';
	}
	iter = first;
	prev = 0;
	while (iter) {
		putchar((dlnode *)iter->prev!=prev?'?':iter==curr?'[':prev==curr?']':' ');
		printf("%c%c#%01X",((char*)iter)[4],((char*)iter)[5],(((char*)iter)-((char*)nodes))>>3);
		prev = iter;
		iter = (dlnode *)prev->next;
		if (prev==last && iter) {
			putchar('?');
			break;
		}
	}
	if (curr==last) putchar(']');
	else if (first && !curr) putchar('[');
}

char main(argc, argv)
	int argc;
	char *argv[];
{
	int nodes[16*4];
	dlnode *first = 0;
	dlnode *last = 0;
	dlnode *curr = 0;
	int *aux = 0;
	char *msg = "Start";
	char *vocal="aiou";
	char *cons="bpmf";
	char key = (char)16;
	char code = 0;

	printf("Sizeof nodes: %d\n", sizeof nodes);
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
			/* dump(nodes, first, last, curr); */

			drList(nodes, first, last, curr);
			putchar('\n');
		}
		key = getch();
		if (key & (char)128) key=0; else if (*(char *)(NEWKEY+6) & (char)1) key |= (char)128;
		/* Handle the key,
		   8=bkspc, 12=home, 18=ins, 27=esc, 28=right, 29=left, 30=up, 31=down, 127=del */
		switch (key) {
			case (char)12:
				msg = "Positioned to end";
				curr = last;
				break;
			case (char)18:
				msg = "Inserted after";
				aux = nodes;
				for (code=0; code<16; ++code) {
					if (!aux[0] && !aux[1] && aux!=(int *)first && aux!=(int *)last) break;
					aux += 4;
				}
				if (code<16) dlInsA(&first, &last, curr, (dlnode *)aux); else msg="Cannot insert";
				break;
			case (char)136:
				msg = "Backspace";
				if (curr) {
					curr = (dlnode *)curr->prev;
					dlDel(&first, &last, curr?(dlnode *)curr->next:first);
				} else dlDel(&first, &last, last);
				break;
			case (char)139:
				msg = "Positioned to start";
				curr = first;
				break;
			case (char)146:
				msg = "Inserted before";
				aux = nodes;
				for (code=0; code<16; ++code) {
					if (!aux[0] && !aux[1] && aux!=(int *)first && aux!=(int *)last) break;
					aux += 4;
				}
				if (code<16) curr = dlInsB(&first, &last, curr, (dlnode *)aux); else msg="Cannot insert";
				break;
			case (char)155:
				break;	/* Escape */
			case (char)156:
				msg = "Positioned one step forward";
				curr = curr? (dlnode *)curr->next : first;
				break;
			case (char)157:
				msg = "Positioned one step backward";
				curr = curr? (dlnode *)curr->prev : last;
				break;
			case (char)255:
				msg = "Delete";
				if (curr) {
					curr = (dlnode *)curr->next;
					dlDel(&first, &last, curr?(dlnode *)curr->prev:last);
				} else dlDel(&first, &last, first);
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