/*
 * FileSystem.cpp
 *

 *  Created on: Aug 9, 2019
 *      Author: Sasa Pejasinovic
 */

#include "FileSystem.h"
#include <iostream>
#include <bitset>

FileSystem::FileSystem(std::string name):name(name) {}

void FileSystem::mkdir(const char *named)
{
    std::fstream file;
    if(i)
    {
        std::cout<<"Cannot create directory here\n";//Direktorijum je moguce napraviti samo u root direktorijumu zato sto je apsolutna putanja max 2;
    }
    else
    {
        file.open(name, std::ios::out | std::ios::in | std::ios::binary);
        if(file.is_open())
        {
            unsigned short inode,last,block;
            Inode root=getNode(0);
            last=root.lastPointer();
            if((inode=createNode(named,"dir"))) //kreira direktorijum u root direktorijumu
            {
                if(root.freeSpace()!=0)     //provjerava da li ima slobodnog mjesta u vec postojecim ekstentima
                {
                    file.seekg(DATA_TABLE_OFFSET+((root.pointers[last].end*SIZE_OF_BLOCK)-root.freeSpace()));
                    file.write(named,10);
                    file.write((char *)&inode,2);
                    writeFileSize(0,root.fileSize+12);
                    writePointers(0,root.pointers);
                }
                else
                {
                    if((block=nextFreeBlock(root.pointers[last-1].end)))    //ako nema pokusava prosiriti ekstent, pa trazi sledeci blok
                    {
                        root.pointers[last-1].end=block+1;
                        root.numOfAllocatedBlocks=root.numOfAllocatedBlocks+1;
                        writeNumOfAllocatedBlocks(0,root.numOfAllocatedBlocks);
                        file.seekg(DATA_TABLE_OFFSET+((root.pointers[last-1].end*SIZE_OF_BLOCK)-root.freeSpace()));
                        file.write(named,10);
                        file.write((char *)&inode,2);
                        writeFileSize(0,root.fileSize+12);
                        writePointers(0,root.pointers);
                    }
                    else if((block=nextClosestBlock(root.pointers[last-1].end)))// ako je sledeci blok zauzet trazi prvi najblizi blok i kreira novi ekstent
                    {
                        root.pointers[last].begin=block;
                        root.pointers[last].end=block+1;
                        root.numOfAllocatedBlocks=root.numOfAllocatedBlocks+1;
                        writeNumOfAllocatedBlocks(0,root.numOfAllocatedBlocks);
                        file.seekg(DATA_TABLE_OFFSET+((root.pointers[last].end*SIZE_OF_BLOCK)-root.freeSpace()));
                        file.write(named,10);
                        file.write((char *)&inode,2);
                        writeFileSize(0,root.fileSize+12);
                        writePointers(0,root.pointers);
                    }
                    else
                        std::cout<<"Cannot create directory\n";
                }
            }
        file.close();
        }
    else
        std::cout<<"Error\n";

}}
void FileSystem::create(const char* filename)
{
    std::fstream file;
    std::string x(filename);
    std::string s=x.substr(((x.find('.')==-1)?x.size():x.find('.')+1));// odvaja tip datoteke od imena npr file.txt, odvaja txt
    const char* type;
    if(s.size()==3)
    {
        type=s.c_str();
    }
    else
        type="non"; //ako nema tipa vec samo ime datoteke, automatski se dodjeljuje moj izmisljeni tip non;
    if(path.compare("root\\")==0){     //ako je putanja root onda upisuje u root direktorijum
    file.open(name, std::ios::out | std::ios::in | std::ios::binary);
    if(file.is_open()){
    unsigned short inode,last,block;
    Inode root=getNode(0);// trazi cvor
    last=root.lastPointer();// dobija pokazivac na poslednji ekstent
    if((inode=createNode(filename,type))){
    if(root.freeSpace()!=0)// ako ima slobodnog prostora upisuje u postojeci ekstent(blok)
    {

        file.seekg(DATA_TABLE_OFFSET+((root.pointers[last].end*SIZE_OF_BLOCK)-root.freeSpace()));
        file.write(filename,10);
                    file.write((char *)&inode,2);
                    writeFileSize(0,root.fileSize+12);
                    writePointers(0,root.pointers);

    }
    else
    {
                if((block=nextFreeBlock(root.pointers[last-1].end)))// ako nema slobodnog prostora pokusava prosiriti ekstent(ako je blok do njega slobodan)
                {
                    root.pointers[last-1].end=block+1;
                    root.numOfAllocatedBlocks=root.numOfAllocatedBlocks+1;
                    writeNumOfAllocatedBlocks(0,root.numOfAllocatedBlocks);
                    file.seekg(DATA_TABLE_OFFSET+((root.pointers[last-1].end*SIZE_OF_BLOCK)-root.freeSpace()));
                    file.write(filename,10);
                    file.write((char *)&inode,2);
                    writeFileSize(0,root.fileSize+12);
                    writePointers(0,root.pointers);


                }
                else if((block=nextClosestBlock(root.pointers[last-1].end)))//ako prethodne dvije opsije nisu moguce kreira novi ekstent
                    {
                        root.pointers[last].begin=block;
                        root.pointers[last].end=block+1;
                        root.numOfAllocatedBlocks=root.numOfAllocatedBlocks+1;
                         writeNumOfAllocatedBlocks(0,root.numOfAllocatedBlocks);
                        file.seekg(DATA_TABLE_OFFSET+((root.pointers[last].end*SIZE_OF_BLOCK)-root.freeSpace()));
                        file.write(filename,10);
                        file.write((char *)&inode,2);
                        writeFileSize(0,root.fileSize+12);
                        writePointers(0,root.pointers);
                    }
               else
                        std::cout<<"Cannot create file\n";

            }
        }
        file.close();
    }
}
else
{
    if((i))// funkcionise na slican nacin kao i za root direktorijum
    {

        file.open(name, std::ios::out | std::ios::in | std::ios::binary);
        if(file.is_open()){
            unsigned short inode,last,block;
            Inode node=getNode(i);// trazi cvor;
            last=node.lastPointer();// trazi poslednji ekstent
            if((inode=createNode(filename,type))){// kreira novi cvor za fajl koji stvaramo
            if(node.freeSpace()!=0) // provjerava ima li mjesta u poslednjem ekstentu
            {
                file.seekg(DATA_TABLE_OFFSET+((node.pointers[last].end*SIZE_OF_BLOCK)-node.freeSpace()));
                file.write(filename,10);
                file.write((char *)&inode,2);
                writeFileSize(i,node.fileSize+12);
                writePointers(i,node.pointers);

            }
            else
            {
                if(last!=0 && (block=nextFreeBlock(node.pointers[last-1].end)))// ako nema pokusava produziti ekstent
                {
                    node.pointers[last-1].end=block+1;
                    node.numOfAllocatedBlocks=node.numOfAllocatedBlocks+1;
                    writeNumOfAllocatedBlocks(i,node.numOfAllocatedBlocks);
                    file.seekg(DATA_TABLE_OFFSET+((node.pointers[last-1].end*SIZE_OF_BLOCK)-node.freeSpace()));
                    file.write(filename,10);
                    file.write((char *)&inode,2);
                    writeFileSize(i,node.fileSize+12);
                    writePointers(i,node.pointers);
                }
                else if((last!=0) && (block=nextClosestBlock(node.pointers[last-1].end)))//trazi prvi sledeci slobodan blok i stvara novi ekstent
                {
                        node.pointers[last].begin=block;
                        node.pointers[last].end=block+1;
                        node.numOfAllocatedBlocks=node.numOfAllocatedBlocks+1;
                        writeNumOfAllocatedBlocks(i,node.numOfAllocatedBlocks);
                        file.seekg(DATA_TABLE_OFFSET+((node.pointers[last].end*SIZE_OF_BLOCK)-node.freeSpace()));
                        file.write(filename,10);
                        file.write((char *)&inode,2);
                        writeFileSize(i,node.fileSize+12);
                        writePointers(i,node.pointers);
                }
               else if((last==0) && (block=freeBlock()))//ako ne postoji ni jedan ekstent;
                {
                        node.pointers[last].begin=block;
                        node.pointers[last].end=block+1;
                        node.numOfAllocatedBlocks=node.numOfAllocatedBlocks+1;
                        writeNumOfAllocatedBlocks(i,node.numOfAllocatedBlocks);
                        file.seekg(DATA_TABLE_OFFSET+((node.pointers[last].end*SIZE_OF_BLOCK)-node.freeSpace()));
                        file.write(filename,10);
                        file.write((char *)&inode,2);
                        writeFileSize(i,node.fileSize+12);
                        writePointers(i,node.pointers);
                        }
                    }
                }
            file.close();
            }
        }

    }

}

void FileSystem::ls()
{
    std::    string fileType;
    std::fstream file;
    unsigned short begin,end;
    unsigned short po=0;
    char filename[10];
    file.open(name,std::ios::out | std::ios::in | std::ios::binary);
    if(file.is_open()){
    if(i || (path.compare("root\\")==0))
        {
            Inode node=getNode(i);
            unsigned short counter=0;
            while(counter<node.lastPointer())// prolazi kroz pune ekstente
                {
                    begin=node.pointers[counter].begin;
                    end=node.pointers[counter].end;

                while(begin<end)// prolazi kroz fajlove u ekstent
                    {
                        file.seekg(DATA_TABLE_OFFSET+node.pointers[counter].begin*SIZE_OF_BLOCK+po);
                         file.read(filename,10);
                        std::cout<<filename<<" ";
                        begin=begin+12;
                        po=po+12;

                    }
                    counter++;

                }
            if(node.freeSpace())//dolazi do poslednjeg ekstenta
                {
                    begin=node.pointers[node.lastPointer()].begin*SIZE_OF_BLOCK;
                    end=node.pointers[node.lastPointer()].end*SIZE_OF_BLOCK-node.freeSpace();//zadnji popunjeni bajt u ekstentu
                      while(begin<end)
                    {
                        file.seekg(DATA_TABLE_OFFSET+node.pointers[counter].begin*SIZE_OF_BLOCK+po);
                        file.read(filename,10);
                        std::cout<<filename<<" ";
                        begin=begin+12;
                        po=po+12;

                    }

                }
        }
        file.close();
    }
    else
        std::cout<<"Error\n";
}

void FileSystem::stat(const char *name)
{

     std::cout<<"\n";
    unsigned int nodee=checkForFile(name,i);//trazi fajl
    Inode node=getNode(nodee);// dobija njegov cvor
    unsigned short begin,end;
    std::cout<<"File name: "<<node.filename<<"\n";
    std::cout<<"Size: "<<node.fileSize<<"\n";
    std::cout<<"fileType: "<<node.fileType<<"\n";
    std::cout<<"Inode: "<<node.nodeId<<"\n";
    std::cout<<"Number of allocated blocks: "<<node.numOfAllocatedBlocks<<"\n";
    std::cout<<"Last pointer: "<<node.lastPointer()<<"\n";
    std::cout<<"Allocated blocks: ";
    int counter=0;
    // prolazi kroz ekstente i gleda alocirane blokove
    while(counter<node.lastPointer())
        {
            begin=node.pointers[counter].begin;
            end=node.pointers[counter].end;
            while(begin<end)
            {
                std::cout<<begin<<"  ";
                begin=begin+1;
            }
            counter++;
        }
    if(node.freeSpace())// posledni alocirani blok
        {
             begin=node.pointers[counter].begin;
            end=node.pointers[counter].end;
            while(begin<end)
            {
                std::cout<<begin<<"  ";
                begin=begin+1;
            }
        }
       std::cout<<"\n";
}
void FileSystem::rm(const char *nameF,std::string mode)
{
    std::string c(nameF);

    if(mode.compare("-r")==0)// gleda da li je ukljucena opcija -r
    {
        if(c.compare("root"))
        setPath(path+c);// ako jeste setuje putanju na taj folder
        std::fstream file;
        unsigned short begin,end;
        unsigned short po=0;
        char filename[10];
        file.open(name,std::ios::out | std::ios::in | std::ios::binary);
        if(file.is_open())
        {
            if(i || (path.compare("root\\")==0))//gleda postoji li takav cvor
            {
                Inode node=getNode(i); //ucitava cvor sa diska(binarne datoteke) u gl memoriju
                unsigned short counter=0;
                while(counter<node.lastPointer()) // prolazi kroz blokove
                {
                    begin=node.pointers[counter].begin;
                    end=node.pointers[counter].end;
                    while(begin<end)
                    {
                        file.seekg(DATA_TABLE_OFFSET+node.pointers[counter].begin*SIZE_OF_BLOCK+po);
                        file.read(filename,10);//ucitava ime fajla
                        deleteFile(filename);//brise fajl
                        deleteFileInformations(filename);//bise podatke u direktorijumu o fajlu
                        begin=begin+12;
                        po=po+12;
                        }
                    counter++;

                }
                if(node.freeSpace())//poslednji ekstent, brise do poslednjeg zauzetog
                {
                    begin=node.pointers[node.lastPointer()].begin*SIZE_OF_BLOCK;
                    end=node.pointers[node.lastPointer()].end*SIZE_OF_BLOCK-node.freeSpace();
                    while(begin<end)
                    {
                        file.seekg(DATA_TABLE_OFFSET+node.pointers[counter].begin*SIZE_OF_BLOCK+po);
                        file.read(filename,10);//ucitava ime datoteke
                        deleteFile(filename);//brise datoteku
                        deleteFileInformations(filename);//brise informacije o datoteci u direktorijumu
                        begin=begin+12;
                        po=po+12;

                    }

                }
            }
            file.close();
        }

    }
    else if(deleteFile(nameF))//ako nije ukljucena opcija -r onda brise samo jedan fajl;
    {
        std::cout<<"File is deleted\n";
        deleteFileInformations(c); // brise informacije u fajlu u direktorijumu
    }
    else{std::cout<<"Cannot delete a file\n";}

}
unsigned short FileSystem:: deleteFile(const char* nameF)// funkcija koja brise fajl// njegov cvor i oslobadja pokazivace
{
    std::fstream file;
    unsigned short d;
    unsigned short begin,end;
    file.open(name,std::ios::out | std::ios::in | std::ios::binary);
    if(file.is_open())
    {
        if((d=checkForFile(nameF,i))) // trazi taj fajl na putanji
        {
            Inode node=getNode(d); // ucitava ga iz diska(binarne datoteke) u memoriju
            std::  string fileType(node.fileType);// tip fajla
            setInode(d); // setuje cvor na 0 tj oslobadja ga
            int counter=0;
            while(counter<=node.lastPointer())
            {
                begin=node.pointers[counter].begin;
                end=node.pointers[counter].end;
                while(begin<end)
                {
                    setBlock(begin);//setuje blokove na 0
                    begin=begin+1;
                }
                counter++;
            }

        file.close();
        return d;
        }
        else
        {
            file.close();
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
unsigned short FileSystem:: deleteFileInformations(std::string fileNam)//brise informacije o fajlu u direktorijumu
{
    std::fstream file;

    unsigned short position,begin,end;
    unsigned short po=0;
    char filename[10];
    file.open(name,std::ios::out | std::ios::in | std::ios::binary);
    if(file.is_open())
    {
        if(i || (path.compare("root\\")==0))
        {
            Inode node=getNode(i);// ucitava cvor u memoriju
            unsigned short counter=0;
            while(counter<node.lastPointer())// trazimo poziciju fajla u direktorijumu
            {
                begin=node.pointers[counter].begin;
                end=node.pointers[counter].end;
                while(begin<end)
                {
                    file.seekg(DATA_TABLE_OFFSET+node.pointers[counter].begin*SIZE_OF_BLOCK+po,std::ios_base::beg);
                    file.read(filename,10);
                    std::string comp(filename);
                    if(fileNam.compare(comp))
                    {
                        begin=begin+12;
                        po=po+12;
                    }
                    else
                    {
                        position=(DATA_TABLE_OFFSET+(node.pointers[counter].begin*SIZE_OF_BLOCK)+po);
                        begin=end;
                    }
                }
                counter++;
            }
            if(node.freeSpace())// ako ne nadjemo u popunjenim ekstentima
            {
                begin=node.pointers[node.lastPointer()].begin*SIZE_OF_BLOCK;
                end=node.pointers[node.lastPointer()].end*SIZE_OF_BLOCK-node.freeSpace();
                while(begin<end)
                {
                    file.seekg(DATA_TABLE_OFFSET+node.pointers[node.lastPointer()].begin*SIZE_OF_BLOCK+po,std::ios_base::beg);
                    file.read(filename,10);
                    std::string comp(filename);
                    if(fileNam.compare(comp))
                    {
                        begin=begin+12;
                        po=po+12;
                    }
                    else
                    {
                        position=(DATA_TABLE_OFFSET+(node.pointers[node.lastPointer()].begin*SIZE_OF_BLOCK)+po);
                        begin=end;
                    }
                }
            }

            unsigned int last=node.lastPointer();
            file.seekg(DATA_TABLE_OFFSET+(node.pointers[node.lastPointer()].begin*SIZE_OF_BLOCK)+po,std::ios_base::beg);
            if(node.freeSpace() && position)// nadjemo poslednji fajl obrisemo trazeni fajl a poslednji fajll prebacimo na lokaciju obrisanog fajla
            {
                char filename1[10];
                unsigned short inode;
                file.seekg(DATA_TABLE_OFFSET+node.pointers[last].end*SIZE_OF_BLOCK-node.freeSpace()-12,std::ios_base::beg);
                file.read(filename1,10);
                file.read((char *)&inode,2);
                node.fileSize=node.fileSize-12;
                writeFileSize(i,node.fileSize);
                file.seekg(DATA_TABLE_OFFSET+(node.pointers[node.lastPointer()].begin*SIZE_OF_BLOCK)+po,std::ios_base::beg);
                file.write(filename1,10);
                file.write((char *)&inode,2);
                if(node.freeSpace()==SIZE_OF_BLOCK)
                {
                    setBlock(node.pointers[last].begin);
                    node.pointers[last].begin=0;
                    node.pointers[last].end=0;
                    node.numOfAllocatedBlocks=node.numOfAllocatedBlocks-1;
                    writePointers(i,node.pointers);
                    writeNumOfAllocatedBlocks(i,node.numOfAllocatedBlocks);
                }
            }
            else if(position)// ako nije bilo slobodnog prostora u poslednjem bloku uzimamo poslednji fajl iy prethodnog bloka
            {
                char filename1[10];
                unsigned short inode;
                file.seekg(DATA_TABLE_OFFSET+node.pointers[last-1].end*SIZE_OF_BLOCK-node.freeSpace()-12,std::ios_base::beg);
                file.read(filename1,10);
                file.read((char *)&inode,2);
                node.fileSize=node.fileSize-12;
                writeFileSize(i,node.fileSize);
                file.seekg(position,std::ios_base::beg);
                file.write(filename1,10);
                file.write((char *)&inode,2);
                if(node.freeSpace()==SIZE_OF_BLOCK)
                {
                    node.pointers[last].begin=0;
                    node.pointers[last].end=0;
                    node.numOfAllocatedBlocks=node.numOfAllocatedBlocks-1;
                    writePointers(i,node.pointers);
                    writeNumOfAllocatedBlocks(i,node.numOfAllocatedBlocks);
                }
            }
        }
    file.close();
    }
    else
        std::cout<<"Error\n";
}
unsigned short FileSystem::setPath(std::string path)
{
    unsigned short inode;
    this->path=path;
    int first=path.find_first_of('\\');
    int last=path.find_last_of('\\');
    if(path.compare("root\\")==0)
    {
       std::cout<<"root"<<": ";
       i=0;
       return 0;
    }
    if(first==last)
    {
        std::string file=path.substr(first+1, path.length()-first);
        if((inode=checkForFile(file,0)))
        {
            Inode dir=getNode(inode);
            std::string s="dir";
            if((s.compare(dir.fileType))==0)
            {
                std::cout<<path<<": ";
                i=inode;
                return inode;
            }
            else
            {
                std::cout<<"Wrong path";
            }
        }
        else
            std::cout<<"There is no such file or directory\n";
    }
    else
    {
        std::string directory=path.substr(first+1,last-first-1);
        std:: string fileInDir=path.substr(last+1);
        if((inode=checkForFile(directory,0)))
        {
            Inode dir=getNode(inode);
            std::string s="dir";
            if(s.compare(dir.fileType)==0)
            {
                inode=checkForFile(fileInDir,inode);
                if(inode)
                {
                    i=inode;
                    return inode;
                }
                else
                    std::cout<<"There is no such file or directory\n";

            }
            else
            {
                std::cout<<"Wrong path\n";

            }
        }
        else
            std::cout<<"Wrong path\n";

    }
    return 0;

}


unsigned short FileSystem::checkForFile(std::string fil,int j)//proverava ima li fajla
{
    Inode node=getNode(j);//ucitava cvor u memotiju
    int counter=0;
    unsigned short begin,end;
    char file[10];
    std::fstream fileR;
    fileR.open(name,std::ios::out | std::ios::in | std::ios::binary);
    if(fileR.is_open())
    {
    while(counter<node.lastPointer())//prolayi kroz sve ekstente osim poslednjeg sve dok ne nadje fajl, vraca broj cvora(inode)
    {
        begin=node.pointers[counter].begin*SIZE_OF_BLOCK;
        end=node.pointers[counter].end*SIZE_OF_BLOCK;
        while(begin<end)
        {
            fileR.seekg(DATA_TABLE_OFFSET+begin, std::ios_base::beg);
            fileR.read(file,10);
            if(!fil.compare(file))
            {
                unsigned short in;
                fileR.read((char *)&in,2);
                fileR.close();
                return in;
            }
            begin=begin+12;
            }
            counter++;
        }
        if(node.pointers[counter].begin!=0)//poslednji ekstent, ima slobodnog prostora zato je odvojen na isti nacin funkcionise
        {
            begin=node.pointers[counter].begin*SIZE_OF_BLOCK;
            end=((node.pointers[counter].begin-node.pointers[counter].end)*SIZE_OF_BLOCK)-node.freeSpace();
            while(begin<end)
            {
                fileR.seekg(DATA_TABLE_OFFSET+begin,std::ios_base::beg);
                fileR.read(file,10);
                if(!fil.compare(file))
                {
                    unsigned short in;
                    fileR.read((char *)&in,2);
                    fileR.close();
                    return in;
                }
                begin=begin+12;
            }
        }
        fileR.close();
        return 0;
    }
    else
    {
        std::cout<<"ERROR\n";
        return 0;
    }
    return 0;
}
Inode FileSystem::getNode(unsigned short j)//funkcija ya ucitavanje cvora sa diska(binarne datoteke) u memoriju
{
    Inode node;
    node.nodeId=j;
    std::fstream fileR;
    fileR.open(name,std::ios::out | std::ios::in  |  std::ios::binary);
    if(fileR.is_open())
    {
        fileR.seekg(INODE_TABLE_OFFSET+j*SIZE_OF_INODE,std::ios_base::beg);
        fileR.read(node.filename,10);
        fileR.read(node.fileType,4);
        fileR.read((char*) &(node.fileSize),2);
        fileR.read((char*) &(node.numOfAllocatedBlocks),2);
        fileR.read((char*) (node.pointers),512);
        fileR.close();
        return node;
    }
    else
    {
        std::cout<<"Error";
        return node;
    }
}
unsigned short FileSystem::freeBlock()// trazi slobodan blok
{
    std::fstream file;
    file.open(name ,std::ios::in | std::ios::out | std::ios::binary);
    int counter=0;
    char get;
    if(file.is_open())
    {
        while(counter<BLOCKS_BITMAP)//NODES_BITMAP)
        {
            file.seekg(BLOCK_BITMAP_OFFSET+counter,std::ios_base::beg);
            file.read(&get,1);
            for(int j=7;j>=0;j--)// krece se kroz bitmapu
            {
            int n=BitTst(get,j); //uzima bit po pit
            if((n==0)) //gleda da li je slobodan
            {
                get=BitSet(get,j); //setuje a na 1(zauzet)
                file.seekg(BLOCK_BITMAP_OFFSET+counter,std::ios_base::beg);
                file.write(&get,1);//upisuje bajt sa setovanim bitom na 1
                file.close();
                return (short)(counter*8+(7-j)); //vraca broj bloka
            }
            }
            counter++;
        }
        file.close();
    }
    else
        std::cout<<"ERROR";
    return 0;
}
unsigned short FileSystem::freeNode()//trazi slobodan cvor, funkcionise na isti nacin kao prethodna funkcija
{
    std::fstream file;
    file.open(name ,std::ios::in | std::ios::out | std::ios::binary);
    int counter=0;
    char get;
    if(file.is_open())
    {
        while(counter<32)//NODES_BITMAP)
        {
            file.seekg(INODE_BITMAP_OFFSET+counter,std::ios_base::beg);
            file.read(&get,1);
            for(int j=7;j>=0;j--)
            {
                int n=BitTst(get,j);
                if((n==0))
                {
                    get=BitSet(get,j);
                    file.seekg(INODE_BITMAP_OFFSET+counter,std::ios_base::beg);
                    file.write(&get,1);
                    file.close();
                    return (short)(counter*8+(7-j));
                }
            }
            counter++;
        }
        file.close();
    }
    else
        std::cout<<"ERROR";
    return 0;
}

unsigned short FileSystem::createNode(const char *filename,const char *fileType)//kreira cvor
{
    unsigned short j;
    if((j=freeNode())!=0)// provjerava da li postoji slobodan cvor
    {
        Inode node;
        node.nodeId=j;
        std::fstream fileW;
        fileW.open(name,std::ios::in | std::ios::out | std::ios::binary);
        if(fileW)
        {
                fileW.seekg(INODE_TABLE_OFFSET+j*SIZE_OF_INODE,std::ios_base::beg);
                fileW.write(filename,10); //upisuje informacije u cvoru u binarnu datotke na diks;
                fileW.write(fileType,4);
                fileW.write((char*) &(node.fileSize),2);
                fileW.write((char*) &(node.numOfAllocatedBlocks),2);
                fileW.write((char*) (node.pointers),512);
                fileW.close();
        }
        else
        {
            std::cout<<"ERROR\n";
            return 0;
        }
        return j;
    } //vraca nrpk cvpra
    else
        return 0;// vraca 0 ako nema slobodnih cvorova
}

//funkcije za upisivanje nekih informacija u cvorove
void FileSystem:: writeFilename(unsigned short j,const char *c)
{
    std::fstream fileW;
   fileW.open(name,std::ios::in | std::ios::out | std::ios::binary);
   if(fileW)
    {
        fileW.seekp(INODE_TABLE_OFFSET+j*SIZE_OF_INODE);
        fileW.write(c,LENGTH_FILE_NAME);
        fileW.close();
    }
    else
    {
        std::cout<< "cannot open";
    }
}
void FileSystem:: writeFileType(unsigned short j,const char *c)
{
    std::fstream fileW;
    fileW.open(name,std::ios::in | std::ios::out | std::ios::binary);
    if(fileW)
    {
        fileW.seekp(INODE_TABLE_OFFSET+j*SIZE_OF_INODE+LENGTH_FILE_NAME);
        fileW.write(c,LENGTH_FILE_TYPE);
        fileW.close();
    }
    else
    {
        std::cout<< "ERROR\n";
    }
}
void FileSystem:: writeFileSize(unsigned short j, unsigned short size )
{
    std::fstream fileW;
    fileW.open(name,std::ios::in | std::ios::out | std::ios::binary);
    if(fileW)
    {
        fileW.seekp(INODE_TABLE_OFFSET+j*SIZE_OF_INODE+LENGTH_FILE_TYPE+LENGTH_FILE_NAME);
        fileW.write((char*) &size,LENGTH_SIZE_FILE);
        fileW.close();
    }
    else
    {
        std::cout<< "ERROR\n";
    }
}
void FileSystem:: writeNumOfAllocatedBlocks(unsigned short j ,unsigned short num)
{
    std::fstream fileW;
    fileW.open(name,std::ios::in | std::ios::out | std::ios::binary);
    if(fileW)
    {
        fileW.seekg(INODE_TABLE_OFFSET+j*SIZE_OF_INODE+LENGTH_FILE_TYPE+LENGTH_FILE_NAME+LENGTH_SIZE_FILE);
        fileW.write((char*) &num,LENGTH_ALL_BLOCKS);
        fileW.close();
    }
    else
    {
        std::cout<< "ERROR\n";
    }

}
void FileSystem:: writePointers(unsigned short j ,Inode::Extent *num)
{
    std::fstream fileW;
    fileW.open(name,std::ios::in | std::ios::out | std::ios::binary);
    if(fileW)
    {
        fileW.seekp(INODE_TABLE_OFFSET+j*SIZE_OF_INODE+LENGTH_FILE_TYPE+LENGTH_FILE_NAME+LENGTH_SIZE_FILE+LENGTH_ALL_BLOCKS);
        fileW.write((char*) num,NUM_OF_POINTERS*4);
        fileW.close();
    }
    else
    {
        std::cout<< "ERROR\n";
    }

}
void FileSystem:: readFilename(unsigned short j, char *c)
{
    std::fstream fileR;
    fileR.open(name,std::ios::out | std::ios::in | std::ios::binary);
    if(fileR)
    {
        fileR.seekg(INODE_TABLE_OFFSET+j*SIZE_OF_INODE);
        fileR.read(c,LENGTH_FILE_NAME);
        fileR.close();
    }
    else
    {
        std::cout<< "ERROR\n";
    }
}
void FileSystem:: readFileType(unsigned short j, char *c)
{
    std::fstream fileR;
    fileR.open(name,std::ios::out | std::ios::in | std::ios::binary);
    if(fileR)
    {
        fileR.seekg(INODE_TABLE_OFFSET+j*SIZE_OF_INODE+LENGTH_FILE_NAME);
        fileR.read(c,LENGTH_FILE_TYPE);
        fileR.close();
    }
    else
    {
        std::cout<< "ERROR\n";
    }
}
void FileSystem:: readFileSize(unsigned short j, unsigned short *size )
{
    std::fstream fileR;
    fileR.open(name,std::ios::out | std::ios::in | std::ios::binary);
    if(fileR)
    {
        fileR.seekg(INODE_TABLE_OFFSET+j*SIZE_OF_INODE+LENGTH_FILE_NAME+LENGTH_FILE_TYPE);
        fileR.read((char*) size,LENGTH_SIZE_FILE);
        fileR.close();
    }
    else
    {
        std::cout<< "ERROR\n";
    }
}

void FileSystem:: readNumOfAllocatedBlocks(unsigned short j ,unsigned short *num)
{
    std::fstream fileR;
    fileR.open(name,std::ios::out | std::ios::in | std::ios::binary);
    if(fileR)
    {
        fileR.seekg(INODE_TABLE_OFFSET+j*SIZE_OF_INODE+LENGTH_FILE_NAME+LENGTH_FILE_TYPE+LENGTH_SIZE_FILE);
        fileR.read((char*) num,LENGTH_ALL_BLOCKS);
        fileR.close();
    }
    else
    {
        std::cout<< "ERROR\n";
    }

}
unsigned short FileSystem:: nextFreeBlock(unsigned short end)// funkcija koja ispituje da li je sledeci blok slobodan(tj da li je moguce produziti ekstent)
{
    std::fstream file1;
    file1.open(name,std::ios::out | std::ios::in | std::ios::binary);
    char get;
    int pos=7-(end%8);
    if(file1.is_open())
    {
        file1.seekp(BLOCK_BITMAP_OFFSET+end/8, std::ios_base::beg);
        file1.read(&get,1);
        int n=BitTst(get,pos);
        if(n==0)
        {
            get=BitSet(get,pos);
            file1.seekp(BLOCK_BITMAP_OFFSET+end/8,std::ios_base::beg);
            file1.write(&get,1);
            file1.close();
            return end;
        }
        file1.close();
        return 0;
    }
    else
        return 0;
}
unsigned short FileSystem:: nextClosestBlock(unsigned short end)// trazi prvi najblizi sloboddan blok
{
    unsigned short begin=end;
    while(begin<NUM_OF_BLOCKS)
    {
        if((end=nextFreeBlock(begin)))
            return end;
        else
            begin++;
    }
    return freeNode();
}
unsigned short FileSystem::setInode(unsigned short n)// ako brisemo cvor(fajl) setuje bit na 0 (BITMAPA CvOROVA)
{
    std::fstream file1;
    file1.open(name,std::ios::out | std::ios::in | std::ios::binary);
    char get;
    int pos=7-(n%8);
    if(file1.is_open())
    {
        file1.seekp(INODE_BITMAP_OFFSET+n/8, std::ios_base::beg);
        file1.read(&get,1);
        get=BitClr(get,pos);//setuje bit na 0
        file1.seekp(INODE_BITMAP_OFFSET+n/8,std::ios_base::beg);
        file1.write(&get,1);//upisuje bajt sa tim bitom u datoteku
        file1.close();
        return n;
    }
    else
        return 0;
}
unsigned short FileSystem::setBlock(unsigned short n)// isto kao prethodna funkcija samo za blokove
{
    std::fstream file1;
    file1.open(name,std::ios::out | std::ios::in | std::ios::binary);
    char get;
    int pos=7-(n%8);
    if(file1.is_open())
    {
        file1.seekp(BLOCK_BITMAP_OFFSET+n/8, std::ios_base::beg);
        file1.read(&get,1);
        get=BitClr(get,pos);
        file1.seekp(BLOCK_BITMAP_OFFSET+n/8,std::ios_base::beg);
        file1.write(&get,1);
        file1.close();
        return n;
    }
    else
    {
        std::cout<<"ERROR";
        return 0;
    }
}
void FileSystem::rename(const char *filename)
{
    unsigned short d;
    unsigned short begin;
    std::fstream file;
    file.open(name,std::ios::out | std::ios::in | std::ios_base::binary);
    if(file.is_open())
    {
        if(d=checkForFile(filename,i))//provjeri da li takav fajl postoji;
        {
            Inode node=getNode(d);
            std::cout<<"New name:"<<"\n";// novo ime
            std::cin>>node.filename;
            writeFilename(d,node.filename);// upise ime u inode
            begin=filePos(filename,i);// trazi poziciju tog fajla u direktorijumu
            file.seekg(DATA_TABLE_OFFSET+begin, std::ios_base::beg);
            file.write(node.filename,10);//upise novo ime u direktorijum
        }
        else
            std:: cout<<"Error cannot rename file";
    }
    else
        std::cout<<"Error cannot rename file";
}

void FileSystem::echo(const char* filename)
{
    unsigned short d,block;
    int j=0;
    std::fstream file;
    file.open(name,std::ios::out | std::ios::in | std::ios::binary);
    if(file.is_open())
    {
        char c=1;
        if((d=checkForFile(filename,i)))
        {
            Inode node=getNode(d);
            unsigned short last=node.lastPointer();
            std::string text;//unosenje teksta
            std::cin.ignore();
            getline(std::cin,text);
            while((j<=(text.size())))
            {
                c=text[j];
                j++;
                if(node.freeSpace())// ukoliko ima mjesta u poslednjem ekstentu upissuje;
                {
                    file.seekg(DATA_TABLE_OFFSET+node.pointers[node.lastPointer()].end*SIZE_OF_BLOCK-node.freeSpace(),std::ios_base::beg);
                    file.write(&c,1);
                    node.fileSize=node.fileSize+1;
                    writeFileSize(d,node.fileSize+1);
                }
                else if(last!=0)
                {
                    if((block=nextFreeBlock(node.pointers[last-1].end)))// ako nema pokusava produziti poslednji ekstent
                    {
                        node.pointers[last-1].end=block+1;
                        node.numOfAllocatedBlocks=node.numOfAllocatedBlocks+1;
                        writeNumOfAllocatedBlocks(d,node.numOfAllocatedBlocks);
                        writePointers(d,node.pointers);
                        file.seekg(DATA_TABLE_OFFSET+((node.pointers[last-1].end*SIZE_OF_BLOCK)-node.freeSpace()),std::ios_base::beg);
                        file.write(&c,1);
                        node.fileSize=node.fileSize+1;
                        writeFileSize(d,node.fileSize);


                    }
                    else if((block=nextClosestBlock(node.pointers[last-1].end)))// kreira novi ekstent ako je nije moguce produyiti ekstent
                    {
                        node.pointers[last].begin=block;
                        node.pointers[last].end=block+1;
                        node.numOfAllocatedBlocks=node.numOfAllocatedBlocks+1;
                        writeNumOfAllocatedBlocks(d,node.numOfAllocatedBlocks);
                        file.seekg(DATA_TABLE_OFFSET+((node.pointers[last].begin*SIZE_OF_BLOCK)),std::ios_base::beg);
                        file.write(&c,1);
                        node.fileSize=node.fileSize+1;
                        writeFileSize(d,node.fileSize);
                        writePointers(d,node.pointers);

                    }

                }
                else
                {
                    if((block=nextFreeBlock(node.pointers[0].end)) && (node.pointers[0].end!=0))// ako nema pokusava produziti poslednji ekstent
                    {
                        node.pointers[0].end=block+1;
                        node.numOfAllocatedBlocks=node.numOfAllocatedBlocks+1;
                        writeNumOfAllocatedBlocks(d,node.numOfAllocatedBlocks);
                        writePointers(d,node.pointers);
                        file.seekg(DATA_TABLE_OFFSET+((node.pointers[0].end*SIZE_OF_BLOCK)-node.freeSpace()),std::ios_base::beg);
                        file.write(&c,1);
                        node.fileSize=node.fileSize+1;
                        writeFileSize(d,node.fileSize);

                    }
                    else if((block=nextClosestBlock(1)))// kreira novi ekstent ako je nije moguce produyiti ekstent
                    {
                        node.pointers[last].begin=block;
                        node.pointers[last].end=block+1;
                        node.numOfAllocatedBlocks=node.numOfAllocatedBlocks+1;
                        writeNumOfAllocatedBlocks(d,node.numOfAllocatedBlocks);
                        file.seekg(DATA_TABLE_OFFSET+((node.pointers[last].end*SIZE_OF_BLOCK)-node.freeSpace()),std::ios_base::beg);
                        file.write(&c,1);
                        node.fileSize=node.fileSize+1;
                        writeFileSize(d,node.fileSize);
                        writePointers(d,node.pointers);

                    }


                }
                last=node.lastPointer();
            }
        }
        else
            std::cout<<"ERROR\n";
        file.close();
    }
    else
        std::cout<<"ERROR\n";
}

unsigned short FileSystem:: filePos(std::string fil,int j)//trazi poziciju fajla u direktorijumu
{
    Inode node=getNode(j);
    int counter=0;
    unsigned short begin,end;
    char file[10];
    std::fstream fileR;
    fileR.open(name,std::ios::out | std::ios::in | std::ios::binary);
    if(fileR.is_open())
    {
        while(counter<node.lastPointer())// prolazi kroz sve ekstente
        {
            begin=node.pointers[counter].begin*SIZE_OF_BLOCK;
            end=node.pointers[counter].end*SIZE_OF_BLOCK;
            while(begin<end)// // prolazi kroz ekstent i ucitava ime fajlova u memoriju zatim provjerava podudara li se ako se podudara vraca poziciju fajla(adresu)
            {
                fileR.seekg(DATA_TABLE_OFFSET+begin, std::ios_base::beg);
                fileR.read(file,10);
                if(!fil.compare(file))
                {
                    fileR.close();
                    return begin;
                }
                begin=begin+12;
            }
            counter++;
        }
        if(node.pointers[counter].begin!=0)// gleda u poslednjem ekstentu
        {
            begin=node.pointers[counter].begin*SIZE_OF_BLOCK;
            end=((node.pointers[counter].end-node.pointers[counter].begin)*SIZE_OF_BLOCK)-node.freeSpace();
            while(begin<end)// prolazi kroz ekstent i ucitava ime fajlova u memoriju zatim provjerava podudara li se ako se podudara vraca poziciju fajla(adresu)
            {
                fileR.seekg(DATA_TABLE_OFFSET+begin,std::ios_base::beg);
                fileR.read(file,10);
                if(!fil.compare(file))
                {
                    fileR.close();
                    return begin;
                }
                begin=begin+12;
            }
        }
        fileR.close();
        return 0;
    }
    else
    {
        std::cout<<"ERROR\n";
        return 0;
    }
    return 0;
}

void  FileSystem:: mv(const char* filename)//pomjeranje fajla
{
    unsigned short d,block;
    std::fstream file;
    file.open(name,std::ios::out | std::ios::in | std::ios::binary);
    if(file.is_open())
    {
        if(d=checkForFile(filename,i))// provjerava ima li tog fajla
        {
            deleteFileInformations(filename);//brise informacije o tome fajlu u direktorijumu
            std::cout<<"Where ";
            std::string newPath;
            std::cin>>newPath;
            i=setPath(newPath);
            Inode node=getNode(i);// trazi direktorijum u koji zelimo da pomjerimo fajl
            unsigned short last=node.lastPointer();
            if(node.freeSpace()) //provjeravamo ima li slobodnog mjesta u poslednjem ekstentu cvora koji predstavlja direktorijum
            {
                file.seekg(DATA_TABLE_OFFSET+node.pointers[node.lastPointer()].end*SIZE_OF_BLOCK-node.freeSpace(),std::ios_base::beg);
                file.write(filename,10);
                file.write((char *)&d,2);
                writeFileSize(i,node.fileSize+12);
                file.close();
                return;
            }
            else if(last!=0)
            {
                if((block=nextFreeBlock(node.pointers[last-1].end)))//ako nema slobodnog mjesta pokusava produziti ekstent
                {
                    node.pointers[last-1].end=block+1;
                    node.numOfAllocatedBlocks=node.numOfAllocatedBlocks+1;
                    writeNumOfAllocatedBlocks(0,node.numOfAllocatedBlocks);
                    file.seekg(DATA_TABLE_OFFSET+((node.pointers[last-1].end*SIZE_OF_BLOCK)-node.freeSpace()));
                    file.write(filename,10);
                    file.write((char *)&d,2);
                    writeFileSize(i,node.fileSize+12);
                    writePointers(i,node.pointers);
                }
                else if((block=nextClosestBlock(node.pointers[last-1].end)))// ako ne moze produziti trazi sledeci najblizi blok i kreira pokazivace na novi ekstent
                {
                    node.pointers[last].begin=block;
                    node.pointers[last].end=block+1;
                    node.numOfAllocatedBlocks=node.numOfAllocatedBlocks+1;
                    writeNumOfAllocatedBlocks(i,node.numOfAllocatedBlocks);
                    file.seekg(DATA_TABLE_OFFSET+((node.pointers[last].end*SIZE_OF_BLOCK)-node.freeSpace()));
                    file.write(filename,10);
                    file.write((char *)&d,2);
                    writeFileSize(i,node.fileSize+12);
                    writePointers(i,node.pointers);
                }
            }
            else
            {
                 if((block=nextFreeBlock(node.pointers[0].end)) && (node.pointers[0].end!=0))//ako nema slobodnog mjesta pokusava produziti ekstent
                {
                    node.pointers[0].end=block+1;
                    node.numOfAllocatedBlocks=node.numOfAllocatedBlocks+1;
                    writeNumOfAllocatedBlocks(0,node.numOfAllocatedBlocks);
                    file.seekg(DATA_TABLE_OFFSET+((node.pointers[0].end*SIZE_OF_BLOCK)-node.freeSpace()));
                    file.write(filename,10);
                    file.write((char *)&d,2);
                    writeFileSize(i,node.fileSize+12);
                    writePointers(i,node.pointers);
                }
                else if((block=nextClosestBlock(node.pointers[0].end)))// ako ne moze produziti trazi sledeci najblizi blok i kreira pokazivace na novi ekstent
                {
                    node.pointers[last].begin=block;
                    node.pointers[last].end=block+1;
                    node.numOfAllocatedBlocks=node.numOfAllocatedBlocks+1;
                    writeNumOfAllocatedBlocks(i,node.numOfAllocatedBlocks);
                    file.seekg(DATA_TABLE_OFFSET+((node.pointers[last].end*SIZE_OF_BLOCK)-node.freeSpace()));
                    file.write(filename,10);
                    file.write((char *)&d,2);
                    writeFileSize(i,node.fileSize+12);
                    writePointers(i,node.pointers);
                }

            }
        }
        else
            std::cout<<"No such file or directory";
    }
    else
        std::cout<<"Cannot move file\n";
}

void FileSystem::cat(const char *filename)//funkcija za citanje iz tekstualnog fajla
{
        unsigned short d,block;
        char c;
        unsigned short counter=0;
        unsigned short begin,end;
        std::fstream file;
        file.open(name,std::ios::out | std::ios::in | std::ios::binary);
        if(file.is_open()){
            if(d=checkForFile(filename,i))// postoji li takav fajl
            {
                Inode node=getNode(d);//ako postoji ucitaj u memoriju cvor tog fajla
                std::string type=node.fileType;
                if(type.compare("txt")==0){//provjeri da li je to tekstualni fajl
                while(counter<node.lastPointer())//krece se kroz ekstente i cita znak po znak
                {
                    begin=node.pointers[counter].begin*SIZE_OF_BLOCK;
                    end=node.pointers[counter].end*SIZE_OF_BLOCK;
                    while(begin<end)
                    {
                        file.seekg(DATA_TABLE_OFFSET+begin, std::ios_base::beg);
                        file.read(&c,1);
                        std::cout<<c;
                        begin=begin+1;
                    }
                    counter++;
                }
                //cita znak po znak u poslednjem ekstentu
                if(node.pointers[counter].begin!=0)
                {
                    begin=(node.pointers[counter].begin*SIZE_OF_BLOCK);
                    end=((node.pointers[counter].end*SIZE_OF_BLOCK)-node.freeSpace());
                    while(begin<end)
                    {
                        file.seekg(DATA_TABLE_OFFSET+begin,std::ios_base::beg);
                        file.read(&c,1);
                        std::cout<<c;
                        begin=begin+1;
                    }
                }
            }
            else
                std::cout<<"This is not a textual file";
        }
        else
            std::cout<<"No such file or directory";
            file.close();
    }
    else
        std::cout<<"Cannot read file\n";
}
void FileSystem::cp(const char *filename)//funkcija za kopiranje
{
    unsigned short d,block,e;
    char c[SIZE_OF_BLOCK+2];
    unsigned short counter=0;
    unsigned short begin,end;
    std::string newPath;
    std::fstream file;
    file.open(name,std::ios::out | std::ios::in | std::ios::binary);
    if(file.is_open())
    {
        if(d=checkForFile(filename,i))//postoji li takav fajl
        {
            std::cout<<"Where: ";
            std::cin>>newPath;
            setPath(newPath);
            create(filename);// kreiraj novi fajl na novoj putanji sa istim imenom
            Inode node=getNode(d);
            while(counter<=node.lastPointer() || counter==0)//prolazi kroz fajl i cita informacije
            {
                begin=(node.pointers[counter].begin*SIZE_OF_BLOCK);
                end=(node.pointers[counter].end*SIZE_OF_BLOCK);
                file.seekg(DATA_TABLE_OFFSET+begin, std::ios_base::beg);
                file.read(c,SIZE_OF_BLOCK);//cita informacije iz fajla

                writeInFile(filename,c,SIZE_OF_BLOCK);//upisuje u novonastali fajk
                counter++;
            }

            if(node.freeSpace() && (counter!=0))// radi sve isto samo u poslednjem ekstentu
            {
                unsigned short size;
                begin=((node.pointers[node.lastPointer()].end*SIZE_OF_BLOCK)-SIZE_OF_BLOCK);
                size=(node.pointers[node.lastPointer()].end*SIZE_OF_BLOCK)-node.freeSpace()-begin;
                file.seekg(DATA_TABLE_OFFSET+begin,std::ios_base::beg);
                file.read(c,size);
                writeInFile(filename,c,size);
            }
        }
        else
            std::cout<<"No such file or directory";
            file.close();
    }
    else
        std::cout<<"Cannot read file\n";
}
unsigned short FileSystem::writeInFile(const char *filename,const char *c,unsigned short size)//pomocna funkcija koja sluzi za upisivanje informacija u fajl
{

    unsigned short d,block;
    int j=0;
    std::fstream file;
    file.open(name,std::ios::out | std::ios::in | std::ios::binary);
    if(file.is_open())
    {
        if((d=checkForFile(filename,i)))//postoji li takav fajl
        {
            Inode node=getNode(d);// ucitaj cvor tog fajla u memoriju
            unsigned short last=node.lastPointer();
            if(node.freeSpace())//ako ima prostora u poslednjem ekstentu
            {
                file.seekg(DATA_TABLE_OFFSET+node.pointers[node.lastPointer()].end*SIZE_OF_BLOCK-node.freeSpace(),std::ios_base::beg);// pozicioniranje gdje se treba upisati
                file.write(c,size);//upisi
                node.fileSize=node.fileSize+size;//promijeni velicinu fajla
                writeFileSize(d,node.fileSize);//upisi velicinu fajla
            }
            else if(last!=0)
            {
                if((block=nextFreeBlock(node.pointers[last-1].end)))//probaj produziti ekstent
                {
                    node.pointers[last-1].end=block+1;
                    node.numOfAllocatedBlocks=node.numOfAllocatedBlocks+1;
                    writeNumOfAllocatedBlocks(d,node.numOfAllocatedBlocks);
                    file.seekg(DATA_TABLE_OFFSET+((node.pointers[last-1].end*SIZE_OF_BLOCK)-node.freeSpace()),std::ios_base::beg);
                    file.write(c,size);
                    node.fileSize=node.fileSize+size;
                    writeFileSize(d,node.fileSize);
                    writePointers(d,node.pointers);

                }
                else if((block=nextClosestBlock(node.pointers[last-1].end)))//kreiraj novi ekstent koji pocinje kod sledeceg najblizeg bloka
                {
                    node.pointers[last].begin=block;
                    node.pointers[last].end=block+1;
                    node.numOfAllocatedBlocks=node.numOfAllocatedBlocks+1;
                    writeNumOfAllocatedBlocks(d,node.numOfAllocatedBlocks);
                    file.seekg(DATA_TABLE_OFFSET+((node.pointers[last].end*SIZE_OF_BLOCK)-node.freeSpace()),std::ios_base::beg);
                    file.write(c,size);
                    node.fileSize=node.fileSize+size;
                    writeFileSize(d,node.fileSize);
                    writePointers(d,node.pointers);

                }
            }
            else
                {
                    if((block=nextFreeBlock(node.pointers[0].end)) && (node.pointers[0].end!=0))//probaj produziti ekstent
                {
                    node.pointers[0].end=block+1;
                    node.numOfAllocatedBlocks=node.numOfAllocatedBlocks+1;
                    writeNumOfAllocatedBlocks(d,node.numOfAllocatedBlocks);
                    file.seekg(DATA_TABLE_OFFSET+((node.pointers[0].end*SIZE_OF_BLOCK)-node.freeSpace()),std::ios_base::beg);
                    file.write(c,size);
                    node.fileSize=node.fileSize+size;
                    writeFileSize(d,node.fileSize);
                    writePointers(d,node.pointers);

                }
                else if((block=nextClosestBlock(1)))//kreiraj novi ekstent koji pocinje kod sledeceg najblizeg bloka
                {
                    node.pointers[last].begin=block;
                    node.pointers[last].end=block+1;
                    node.numOfAllocatedBlocks=node.numOfAllocatedBlocks+1;
                    writeNumOfAllocatedBlocks(d,node.numOfAllocatedBlocks);
                    file.seekg(DATA_TABLE_OFFSET+((node.pointers[last].end*SIZE_OF_BLOCK)-node.freeSpace()),std::ios_base::beg);
                    file.write(c,size);
                    node.fileSize=node.fileSize+size;
                    writeFileSize(d,node.fileSize);
                    writePointers(d,node.pointers);

                }

                }
        }
        else
            std::cout<<"ERROR\n";
        file.close();
    }
    else
        std::cout<<"ERROR\n";
}

void FileSystem::put(const char *filename)//upload fajl na drugi fajl sistem
{
    std::ifstream file(name);
    std::string filenameDS(filename);
    std::ofstream fileDS(filenameDS);//direktorijum na hard disku
    unsigned short d,begin,end;
    unsigned short counter=0;
    char c[SIZE_OF_BLOCK];
    if(fileDS.is_open())
    {
        if((d=checkForFile(filename,i)))//postoji li takav fajl
        {
             Inode node=getNode(d);// ucitaj ga u memorjiu
            while(counter<node.lastPointer() )// kopira podatke iz ekstenata u neki fajl na pravom fajl sistemu
            {
                begin=node.pointers[counter].begin*SIZE_OF_BLOCK;
                end=node.pointers[counter].end*SIZE_OF_BLOCK;
                unsigned short size=end-begin;
                file.seekg(DATA_TABLE_OFFSET+begin, std::ios_base::beg);
                file.read(c,size);
                fileDS.write(c,size);//upisuje u datoteku na pravom fajl sistemu
                counter++;
            }
            if(node.pointers[counter].begin!=0)// sve je isto kao u prethodnom bloku samo za poslednji ekstent
            {
                begin=(node.pointers[counter].begin*SIZE_OF_BLOCK);
                end=((node.pointers[counter].end*SIZE_OF_BLOCK)-node.freeSpace());
                unsigned short size;
                size=end-begin;
                file.seekg(DATA_TABLE_OFFSET+begin,std::ios_base::beg);
                file.read(c,size);
                fileDS.write(c,size);
            }
        }
        else
            std::cout<<"ERROR\n";
        fileDS.close();
        file.close();
    }
    else
        std::cout<<"ERROR\n";
}
void FileSystem::get(const char* filename)// funkcionise na slican nacin kao i put samo sto podatke sa fajla na pravom fajl sistemu kopira u ovaj fajl sistem
{
    std::string filenameDS(filename);
    std::fstream fileDS;
    fileDS.open(filename, std::ios::in |  std::ios::out | std::ios::binary);
    unsigned short d,begin,end;
    unsigned short fileSize;
    char c[SIZE_OF_BLOCK];
    if(fileDS.is_open())//pokusava otvoriti taj fajl
    {
        fileDS.seekg(0,std::ios_base::end);
        fileSize=fileDS.tellg();
        fileDS.seekg(0,std::ios_base::beg);
        if(fileSize<64000)// provjerava velicinu fajla
        {
            create(filename);
            int numOfblocks=fileSize/SIZE_OF_BLOCK;// potreban broj blokova
            unsigned short counter=0;
            while(counter<numOfblocks)//prolazi kroz blokove tj kroz datoteku na pravom fajl sistemu uzima podatke velicine blokova ovog fajl sistema
            {
                fileDS.read(c,SIZE_OF_BLOCK);//ocitava blok
                writeInFile(filename,c,SIZE_OF_BLOCK);//pokusava upisati u ovaj fajl sistem
                counter++;
            }
            int rest=fileSize%SIZE_OF_BLOCK;//ostatak tj nije puna velicina bloka
            fileDS.read(c,rest);// ocitava
            writeInFile(filename,c,rest);//upisuje
        }
        else
            std::cout<<"FILE TOO LARGE";// ako je fajl prevelik
       fileDS.close();

    }
    else
        std::cout<<"ERROR\n";
}
void FileSystem::reset()
{
    std::ofstream wr;
    wr.open(name, std::ios::binary);
    if(wr.is_open()){
        char pr[20000]={0};
        wr.write(pr,20000);
        char ce=128;//1000000 prci bajt bitmape
        char se;
        char root[10]="root";
        char filetype[4]="dir";
        unsigned short size=0;
        unsigned short all=1;
        unsigned short begin=1;
        unsigned short end=2;
        wr.seekp(INODE_BITMAP_OFFSET,std::ios::ios_base::beg);
        wr.write(&ce,1);
        wr.seekp(BLOCK_BITMAP_OFFSET,std::ios::ios_base::beg);
        se=192;
        wr.write(&se,1);
        wr.seekp((INODE_TABLE_OFFSET),std::ios_base::beg);
        wr.write(root,10);
        wr.write(filetype,4);
        wr.write((char *)&size,2);
        wr.write((char *)& all,2);
        wr.write((char *)&begin,2);
        wr.write((char *)&end,2);
        wr.close();
    }
    else
        std::cout<<"Error";
}
