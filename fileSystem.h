/*
 * FileSystem.h
 *
 *  Created on: Aug 9, 2019
 *      Author: Sasa Pejasinovic
 */
#include <string>
#include <fstream>
#include <bitset>
#include "Inode.h"
#include <algorithm>
#include "properties.h"
#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_



class FileSystem {
    private:
        std::string name;
        std::string path="root\\";
        unsigned short i=0;

    public:
        void mkdir(const char *);
        void create(const char *);
        void put(const char *);
        void get(const char *);
        void ls();
        void cp(const char *);
        void mv(const char *name);
        void rename(const char *);
        void echo(const char*);
        void cat(const char* );
        void rm(const char*,std:: string mode);
        void stat(const char *name);
        void reset();
        unsigned short setPath(std::string);
        FileSystem(std::string);

        private:
        //pomocne funkcije
        unsigned short deleteFile(const char* name);
        unsigned short deleteFileInformations(std::string);
        unsigned short checkForFile(std::string, int i);
        unsigned short filePos(std::string, int i);
        unsigned short createNode(const char *,const char *);
        unsigned short freeBlock();
        unsigned short nextFreeBlock(unsigned short );
        unsigned short freeNode();
        unsigned short nextClosestBlock(unsigned short);
        unsigned short setBlock(unsigned short);
        unsigned short setInode(unsigned short);
        unsigned short writeInFile(const char *filename,const char* c,unsigned short);
        void writeFilename(unsigned short ,const char *);
        void writeFileType(unsigned short,const char *);
        void writeFileSize(unsigned short , unsigned short );
        void writeNumOfAllocatedBlocks(unsigned short ,unsigned short);
        void writePointers(unsigned short ,Inode::Extent *);
        void readFilename(unsigned short , char *);
        void readFileType(unsigned short, char *);
        void readFileSize(unsigned short , unsigned short* );
        void readNumOfAllocatedBlocks(unsigned short ,unsigned short*);
        Inode getNode(unsigned short);
};

#endif /* FILESYSTEM_H_ */
