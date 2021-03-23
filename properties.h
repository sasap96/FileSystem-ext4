#ifndef PROPERTIES_H_
#define PROPERTIES_H_
#define NUM_OF_NODES 256
#define NUM_OF_BLOCKS 39424
#define SIZE_OF_INODE 530
#define SIZE_OF_BLOCK 504
#define BLOCKS_BITMAP (NUM_OF_BLOCKS/8)// broj bajtova
#define NODES_BITMAP (NUM_OF_NODES/8) //broj bajtova
#define INODE_BITMAP_OFFSET 0 //0 for root
#define BLOCK_BITMAP_OFFSET (NODES_BITMAP)

#define INODE_TABLE_OFFSET (BLOCK_BITMAP_OFFSET+BLOCKS_BITMAP)
#define DATA_TABLE_OFFSET (INODE_TABLE_OFFSET+(NUM_OF_NODES*SIZE_OF_INODE))


#define NUM_OF_POINTERS 128
#define LENGTH_FILE_NAME 10
#define LENGTH_FILE_TYPE 4
#define LENGTH_ALL_BLOCKS 2
#define LENGTH_SIZE_FILE 2

//funkcije za manipulaciju bitima u bajtu
#define BOOL(x) (!(!(x)))

#define BitSet(arg,posn) ((arg) | (1L << (posn)))
#define BitClr(arg,posn) ((arg) & ~(1L << (posn)))
#define BitTst(arg,posn) BOOL((arg) & (1L << (posn)))
#define BitFlp(arg,posn) ((arg) ^ (1L << (posn)))

// 0 slobodno
//1 zauzeto
#endif

