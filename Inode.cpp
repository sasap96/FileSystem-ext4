/*
 * inode.cpp
 *
 *  Created on: Aug 10, 2019
 *      Author: Sasa Pejasinovic

 */
#include "Inode.h"
#include <algorithm>
Inode::Inode(unsigned short Id, const char *user,const char *type)
{
	nodeId=Id;
	std::copy(user,user+LENGTH_FILE_NAME,filename);
	std::copy(type,type+LENGTH_FILE_TYPE,fileType);
}
Inode::Inode(const Inode& other)
{
    copy(other);
}
Inode& Inode::operator=(const Inode& other)
{
    if(this!=&other)
    copy(other);
    return *this;
}
void Inode::copy(const Inode& other)
{
    nodeId=other.nodeId;
    fileSize=other.fileSize;
    numOfAllocatedBlocks=other.numOfAllocatedBlocks;
    std::copy(other.filename,other.filename+LENGTH_FILE_NAME,filename);
    std::copy(other.fileType,other.fileType+LENGTH_FILE_TYPE,fileType);
    std::copy(other.pointers,other.pointers+NUM_OF_POINTERS,pointers);

}

unsigned short Inode:: lastPointer()//pokazivac na poslednji ekstent
{
    unsigned short counter=0;
    int sum=0;
    while(pointers[counter].begin!=0 && counter<=NUM_OF_POINTERS)
    {
        sum=sum+((pointers[counter].end-pointers[counter].begin)*SIZE_OF_BLOCK);
        if(sum<=fileSize)
            counter++;
        else
            return counter;
    }
    return counter;
}
int Inode::freeSpace()// funkcija koja racuna slobodan prostor u poslednjem ekstentu
{
    int freeSpace;
    unsigned short i=lastPointer();// lastPointer() trazi poslednji ekstent tj redni broj njegovog pokazivaca
    if(pointers[i].begin!=0)
    {
        int thisExtentBlocks=pointers[i].end-pointers[i].begin;// broj bajtova u poslednjem ekstentu
        int sizeInExtent=fileSize-(numOfAllocatedBlocks-thisExtentBlocks)*SIZE_OF_BLOCK;//racuna dio fajla koji zauzima poslednji ekstent
        return (thisExtentBlocks*SIZE_OF_BLOCK )- sizeInExtent;//broj bajtova u ekstentu-zauzeti bajtovi
    }
    else return 0;
}
