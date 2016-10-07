/* 
 * File:   File.cpp
 * Author: User
 * 
 * Created on 14 Январь 2012 г., 23:35
 */

#include "File.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <stdlib.h>
#include <io.h>
#include <iostream>
#include <direct.h>
#include <dirent.h>
#include <string.h>
#include <winbase.h>

#include "IOError.h"

File::File(const String& name) {
    this->name.assign(name);
}

File::File(const File& orig) {
    name.assign(orig.name);
}

File::File(const File& parent,const String& childName){
    name.assign(parent.name);
    name = name.trimEnd("/");
    name = name.trimEnd("\\");
    String cn(childName.c_str());
    cn.assign(cn.trimEnd("/"));
    cn.assign(cn.trimEnd("\\"));
    cn.assign(cn.trimStart("/"));
    cn.assign(cn.trimStart("\\"));
    name = name.append("\\");
    name = name.append(cn);
}

File::~File() {
}

bool File::isExists(){
    struct stat s;
    if( stat(name.c_str(),&s)==0 ){
        return true;
    }
    return false;
}

bool File::isFile(){
    struct stat s;
    if( stat(name.c_str(),&s)!=0 ){
        return false;
    }
    return S_ISREG(s.st_mode);
}

bool File::isDir(){
    struct stat s;
    if( stat(name.c_str(),&s)!=0 ){
        return false;
    }
    return S_ISDIR(s.st_mode);
}

String File::getPath(){
    return String(name);
}

String File::getName(){
    char* res = (char *)malloc(strlen(name.c_str())+1);
    char* res2 = 0;
    strcpy(res,name.c_str());
    res2 = basename(res);
    String res3 = String(res2);
    free(res);
    return String(res3);
}

File File::getParent(){
    char* res = (char *)malloc(strlen(name.c_str())+1);
    char* res2 = 0;
    strcpy(res,name.c_str());
    res2 = dirname(res);
    String res3 = String(res2);
    free(res);
    return File(res3);
}

long File::getSize(){
    struct stat s;
    if( stat(name.c_str(),&s)!=0 ){
        return -1;
    }
    return s.st_size;
}

//using namespace std;

bool File::mkdir(bool withParents){
//    cout << "mkdir " << name << endl;
    if( isDir() ){
//        cout << "is already created" << endl;
//        cout << "success" << endl;
        return true;
    }

    File fParent = getParent();
    if( fParent.getPath().equals(".") ){
//        cout << name << " is root" << endl;
//        cout << "failed" << endl;
        return false;
    }

    if( fParent.isDir() ){
//        cout << "parent exists" << endl;
        if( _mkdir( name.c_str() )==0 ){
//            cout << "must be created" << endl;
            return true;
        }
//        cout << "failed" << endl;
        return false;
    }else{
//        cout << "parent not exists" << endl;
        if( withParents ){
            if( fParent.mkdir(true) ){
                if( _mkdir( name.c_str() )==0 ){
//                    cout << "must be created" << endl;
//                    cout << "success" << endl;
                    return true;
                }
            }
        }else{
//            cout << "failed" << endl;
            return false;
        }
    }
//    cout << "failed" << endl;
    return false;
}

RandomAccessFile File::open(const String& mode) throw (IOError) {
    //RandomAccessFile rafile(name.replaceAll("\\","/").c_str());
    RandomAccessFile rafile(name.c_str());
    rafile.open(mode);
    return rafile;
}

void File::copy(const File& target){
    File trgt(target);
//    std::cout << "copy " << name.c_str() << " to " << trgt.getPath() << std::endl;

    if( isFile() ){
        File tarDir = trgt.getParent();
        if( !tarDir.isExists() ){
            if( !tarDir.mkdir(true) ){
                String mess("can't create dir: ");
                mess = mess.append(tarDir.getPath());
                throw new IOError(-1,mess);
            }
        }
        RandomAccessFile rSrc = open("r");
        RandomAccessFile rTarget = trgt.open("w+");

        char buff[4096];
        while(true){
            bool eof = rSrc.isEOF();
            if( eof ){
                break;
            }
            size_t rd = rSrc.read(buff,sizeof(buff));
//            std::cout << "readed " << rd << " bytes" << std::endl;
            if( rd>0 ){
                rTarget.write(buff,rd);
            }
            if( rd<0 )break;
        }
        rTarget.flush();

        rTarget.close();
        rSrc.close();
        return;
    }
    if( isDir() ){
        if( !trgt.isExists() ){
            if( !trgt.mkdir(true) ){
                String mess("can't create dir: ");
                mess = mess.append(trgt.getPath());
                throw new IOError(-1,mess);
            }
        }
        if( !trgt.isDir() ){
            String mess("is not dir: ");
            mess = mess.append(trgt.getPath());
            throw new IOError(-1,mess);
        }
        std::vector<File> files = readdir();
        for( int idx=0; idx<files.size(); idx++ ){
            File srcFile = files[idx];
            File tFile = File(*this,srcFile.getName());
            srcFile.copy(tFile);
        }
    }
}

std::vector<File> File::readdir(){
    std::vector<File> files;
    DIR *dir=0;
    dir = opendir(name.c_str());
    if( !dir ){
        String mess("can't open dir");
        mess.append(name.c_str());
        throw new IOError(-1,mess);
    }
    
    dirent* dirEntry;
    while(true){
        dirEntry = ::readdir(dir);
        if( dirEntry==NULL )break;
        
        char* _entryName = (char *)malloc(dirEntry->d_namlen+1);
        for( int is=0; is<dirEntry->d_namlen; is++ ){
            _entryName[is] = dirEntry->d_name[is];
        }
        _entryName[dirEntry->d_namlen] = 0;

        String fileName(_entryName);
        free( _entryName );

        if( fileName.equals(".") )continue;
        if( fileName.equals("..") )continue;
        File fDir(name.c_str());
        File f(fDir,fileName);
        files.push_back(f);
    }

    closedir(dir);
    return files;
}

File File::getTempDir(){
    String tmpDirString( getenv("TEMP") );
    File tmpDir(tmpDirString);
    return tmpDir;
}

File File::getCurrentDir(){
    char currentDir[16384];
    if( getcwd( currentDir,16384 )==0 ){
        String mess("can't read current dir (getcwd)");
        throw new IOError(-1,mess);
    }
    String curDirString( currentDir );
    File curDir(curDirString);
    return curDir;
}

bool File::setCurrentDir(File& file){
    String p = file.getPath();
    return chdir( p.c_str() )==0;
}

bool File::remove(){
    if( isFile() ){
        if( DeleteFile( name.c_str() )==0 ){
            std::cout << "error delete file " << name << std::endl;
            std::cout << "GetLastError()=" << GetLastError() << std::endl;
            return false;
        }
        return true;
    }
    if( isDir() ){
        std::vector<File> content = readdir();
        bool succDel = true;
        for( int i=0; i<content.size(); i++ ){
            if( !content[i].remove() )succDel = false;
        }
        if( succDel ){
            if( RemoveDirectory( name.c_str() )==0 ){
                std::cout << "error delete dir " << name << std::endl;
                std::cout << "GetLastError()=" << GetLastError() << std::endl;
                return false;
            }
        }
        return true;
    }
    std::cout << "can't delete " << name << std::endl;
    return false;
}