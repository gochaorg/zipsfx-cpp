/* 
 * File:   RandomFile.h
 * Author: User
 *
 * Created on 15 Январь 2012 г., 1:55
 */

#ifndef _RANDOMACCESSFILE_H
#define	_RANDOMACCESSFILE_H

#include <stdio.h>
#include <windows.h>

class File;

#include "File.h"
#include "String.h"
#include "IOError.h"

class RandomAccessFile {
protected:
    File *file;
//    FILE *fp;
    HANDLE hdl;
public:
    RandomAccessFile(const char * filename);
    RandomAccessFile(const RandomAccessFile& orig);
    virtual ~RandomAccessFile();

    File getFile();
//    FILE* getFilePTR();
    bool isOpen();
    void open(const String& mode) throw (IOError);
    __int64 getPosition();
    void setPosition(__int64 pos);
    void setPositionToEnd();
    void close();
    void flush();
    size_t write(const void* ptr,size_t size);
    size_t read(void* ptr,size_t size);
    bool isEOF();
};

#endif	/* _RANDOMACCESSFILE_H */

