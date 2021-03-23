/*
 * main.cpp
 *
 *  Created on: Aug 9, 2019
 *      Author: Sasa Pejasinovic
 */

#include <iostream>
#include <string>
#include "Inode.h"
#include <fstream>
#include "properties.h"
#include "FileSystem.h"
#include <bitset>

using namespace std;
int main()
{

string path;
string action;
char c[10];
FileSystem fil("filesystem5");
while(true)
{
    cin>>path;
if(fil.setPath(path) || (path.compare("root\\")==0)){
    cin>>action;
    if(action.compare("mkdir")==0)
    {
        cin>>c;
        fil.mkdir(c);
    }
    else if(action.compare("create")==0)
    {
        cin>>c;
        fil.create(c);
    }
    else if(action.compare("ls")==0)
    {
        fil.ls();
    }
    else if(action.compare("stat")==0)
    {
        cin>>c;
        fil.stat(c);
    }
    else if(action.compare("rename")==0)
    {
            cin>>c;
            fil.rename(c);
    }
    else if(action.compare("rm")==0)
    {
    cin>>c;
    std::string mode;
    cin>>mode;
    fil.rm(c,mode);
    }
    else if(action.compare("mv")==0)
    {
        cin>>c;
        fil.mv(c);
    }
    else if(action.compare("echo")==0)
    {
        cin>>c;
        fil.echo(c);
    }
    else if(action.compare("cat")==0)
    {
        cin>>c;
        fil.cat(c);
    }
    else if(action.compare("cp")==0)
    {
        cin>>c;
        fil.cp(c);
    }
    else if(action.compare("put")==0)
    {
        cin>>c;
        fil.put(c);
    }
    else if(action.compare("get")==0)
    {
        cin>>c;
        fil.get(c);
    }
    else if(action.compare("reset")==0)
    {
        fil.reset();
    }
    else
        std::cout<<"Try again\n";
    }

}

}


