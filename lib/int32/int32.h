extern int swap32(low, high);

typedef union {
	struct {
		int n0, n1;
	} v16;
	struct {
		char n0, n1, n2, n3;
	} v8;
} int32;


