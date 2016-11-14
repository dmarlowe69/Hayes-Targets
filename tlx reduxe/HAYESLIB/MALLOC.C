/*
 * Memory Allocation Functions (Near Heap Only)
 *
 * tabsize = 3
 *
 * void *malloc(unsigned)
 * void free(void *)
 *
 * The heap is a single linked list of memory blocks.
 * The block is idenitified by an unsigned number that is the 
 * number of bytes in the block. If the number is odd then it is NOT 
 * allocated, even is allocated.  Allocation is only done in even 
 * size numbers.
 *
 */

 void *malloc(unsigned), free(void *);
 printf(char *,...);

 #define TEST_VER 0

/* defined only for Turbo C compiler */
 #if __TURBOC__

 #if __SMALL__
 #define M_I86SM 1
 #endif

 #if __MEDIUM__
 #define M_I86MM 1
 #endif

 #endif

 #define SMALLEST 10		/* smallest block allocated */
 #define ALLOCATED(p)	 (((p)->size & 1) == 0)
 #define SIZE(p)			((p)->size & 0xfffe)
 #define END_OF_LIST(p)  ((p) >= heap_end)
 #define NEXT(p)		 ((struct block *)((char *)p + (SIZE(p) + 2)))

 #define MK_FP(seg, off) ((void far *) ((seg << 16L) | off))
 #define NULL 0

 extern unsigned  _atopsp, _asizds;

 struct block {
	unsigned size;
	char mem[2];
	};

 static struct block *heap_start;
 static void *heap_end;

 #if TEST_VER

/*
 *  this main simply tests the malloc and free.  Set TEST_VER to 1
 *  and main() along with printf()s, etc, will be included.
 *
 */
 main()
 {
	char *p1,*p2,*p3,*p4;

	printf("test allocation : p1 = malloc(2000);\n");
	p1 = malloc(2000);
	printf("  p1 = %p\n",p1);
	chk_mem();

	printf("test allocation of odd byte sizes : p2 = malloc(9);\n");
	p2 = malloc(9);
	printf("  p2 = %p\n",p2);
	chk_mem();

	printf("test freeing : free(p1);\n");
	free(p1);
	chk_mem();

	printf("test allocating some in the middle : p1 = malloc(1000);\n");
	p1 = malloc(1000);
	printf("  p1 = %p\n",p1);
	chk_mem();

	printf("test allocating too much for 1st found space : p3 = malloc(1000);\n");
	p3 = malloc(1000);
	printf("  p3 = %p\n",p3);
	chk_mem();

	printf("test freeing of two contiguous areas of memory : free(p1);\n");
	free(p1);
	chk_mem();

	printf("test 'smallest' block code : p1 = malloc(1990);\n");
	p1 = malloc(1990);
	printf("  p1 = %p\n",p1);
	chk_mem();

	printf("test no enough memory : p4 = malloc(65000);\n");
	p4 = malloc(65000);
	printf("  p4 = %p\n",p4);
	chk_mem();

	printf("test free all : free(p1), free(p2), free(p3), free(p4)\n");
	free(p1);
	free(p2);
	free(p3);
	free(p4);
	chk_mem();
 }

 #endif

/*
 *  allocate a number of bytes.  returns a pointer to that memory
 *  or NULL if none available.
 *
 */
 void *malloc(unsigned count)
 {
	static char first_time = 1;
	struct block *p, *next;
	unsigned leftover;

	/* Init Mem system */
	if(first_time)
	{
		first_time = 0;

 #if M_I86SM || M_I86MM
		/* Small Data Model */
		heap_start = (struct block *)( (_atopsp & 0xFFFE) + 2);
		heap_end = (void *)(_asizds & 0xFFFE);
 #else
		/* Large Data Model */
		heap_start = MK_FP(getds(), (_atopsp & 0xFFFE) + 2);
		heap_end = MK_FP(getds(), (_asizds & 0xFFFE));
 #endif
		/* all memory is free : odd = unallocated */
		heap_start->size = ((char *)heap_end - (char *)heap_start - 2) | 1;

 #if TEST_VER
		chk_mem();
 #endif

	}

	/* allocate only even # of bytes */
	if(count & 1)
		++count;

	/* Allocate "count" bytes of Memory */
	p = heap_start;
	while(!END_OF_LIST(p))
	{
		if(!ALLOCATED(p) && SIZE(p) >= count)
		{
			leftover = SIZE(p) - count;
			if(leftover > SMALLEST)	      /* break up a bigger block into 2 */
			{
				next = (struct block *)((char *)p + count + 2);
				next->size = (leftover -2) | 1 ;
			}
			else
				count = p->size & 0xFFFE;	/* allocate it */

			p->size = count;
			return(p->mem);
		}
		else
			p = NEXT(p);
	}
	return (NULL);
 }

/*
 *
 *
 *
 */
 void free(void *block)
 {
	struct block *p, *last, *freeblk;
	last = p = heap_start;
	while(!END_OF_LIST(p))
	{
		if(block == p->mem)
		{
			/* Found it */
			if(!ALLOCATED(last) && p != heap_start)
				last->size += SIZE(p) + 2;
			else
				last = p;
			if(!ALLOCATED(NEXT(p)))
				last->size += (SIZE(NEXT(p)) + 2);
			last->size |= 1;		/* Mark as NOT allocated */
			return;
		}
		else
		{
			last = p;
			p = NEXT(p);
		}
	}
 }

 #if TEST_VER
/*
 * FOR TESTING PURPOSES ONLY!!
 * chk_mem is used to verify if the memory system is not currupted
 * It produces a list of memory blocks
 */
 chk_mem()
 {
	struct block *p;
	p = heap_start;

	while(!END_OF_LIST(p))
	{
 #if M_I86SM || M_I86MM
		printf("%04X Bytes %s at %04X\n", p->size & 0xFFFE,
 #else
		printf("%04X Bytes %s at %p\n", p->size & 0xFFFE,
 #endif
			ALLOCATED(p) ? "    ALLOCATED" : "NOT ALLOCATED", p);
		p = NEXT(p);
	}
 #ifndef __TURBOC__
	bdos(1);		/* wait for a keypress */
 #endif
 }
 #endif

 #if TEST_VER
 #if __TURBOC__

 printf(char *p,...)
 {
 }

 #endif
 #endif
