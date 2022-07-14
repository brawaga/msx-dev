#include <stdio.h>
#include <bdosfunc.h>

int main(argc, argv)
	int argc;
	char **argv;
{
	FD fd;
	int remain;
	char buf[512];
	int portion;
	char err;
	fd = open(argv[1], 1);  /* Read/write */
	if (fd > 0) {
		setmem(buf, 0xFF, 512);
		remain = (16384-call(
			5,  /* BDOS */
			(char)2,  /* from the end of file */
			0,  /* Position low */
			(int)0x4A + 256*fd,  /* SEEK function and file handle */
			0  /* Position high */
		)) & 0x3FFF;
		while (remain) {
			portion = remain > 512? 512 : remain;
			err = calla(5, 0, portion, 0x49 + 256*fd, (int)buf);
			remain -= portion;
			if (err) {
				printf("Error %d occured while writing.\n", (int)err);
				remain = 0;
			}
		}
		close(fd);
	} else printf("Error opening file.");
}

