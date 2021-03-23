/*
 * inode.h
 *
 *  Created on: Aug 10, 2019
 *      Author: Sasa Pejasinovic
 */

#ifndef INODE_H_
#define INODE_H_
#include "properties.h"
#include <algorithm>
#include <iostream>
#include <fstream>

struct Inode
{
        struct Extent
        {
            unsigned short begin=0;
            unsigned short end=0;
        };
		unsigned short nodeId;
		char filename[10];
        char fileType[4]="non";
        unsigned short fileSize=0;
        unsigned short numOfAllocatedBlocks=0;
        Extent pointers[NUM_OF_POINTERS];
	    Inode()=default;
        Inode(unsigned short,const char*,const char*);
        Inode(const Inode &);
        Inode& operator=(const Inode& other);
        unsigned short lastPointer();
        int freeSpace();
        private:
        void copy(const Inode &);
};





#endif /* INODE_H_ */
