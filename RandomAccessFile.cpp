/*
 * File:   RandomFile.cpp
 * Author: User
 *
 * Created on 15 Январь 2012 г., 1:55
 */

#include "RandomAccessFile.h"

//#include <errno.h>
#include <winerror.h>
#include <windows.h>
#include <iostream>

RandomAccessFile::RandomAccessFile(const char * filename) {
    this->file = new File(String(filename));
//    this->fp = 0;
    this->hdl = 0;
}

RandomAccessFile::RandomAccessFile(const RandomAccessFile& orig) {
    this->file = orig.file;
//    this->fp = orig.fp;
    this->hdl = orig.hdl;
}

RandomAccessFile::~RandomAccessFile() {
//    if( fp!=0 ){
//        fclose(fp);
//        fp = 0;
//    }
//    delete this->file;
    if( hdl!=0 ){
        close();
        hdl = 0;
    }
}

File RandomAccessFile::getFile(){
    return File(this->file->getPath());
}
//FILE* RandomAccessFile::getFilePTR(){
//    return fp;
//}
bool RandomAccessFile::isOpen(){
//    return fp!=0;
    return hdl!=0;
}
void RandomAccessFile::open(const String& mode) throw (IOError) {
//    fp = fopen(file->getPath().c_str(),mode.c_str());
//    if( fp==0 ){
//        String message("Can`t open file ");
//        message = message.append(file->getPath().c_str());
//        throw IOError(-1,message);
//    }
    DWORD openMode = 0;
    DWORD rwMode = 0;
    DWORD err = 0;

    String mod(mode);

    if( mod.equals("r") )openMode = OPEN_EXISTING;
    if( mod.equals("w") )openMode = OPEN_ALWAYS;
    if( mod.equals("w+") )openMode = OPEN_ALWAYS;
    if( mod.equals("rw") )openMode = OPEN_ALWAYS;
    if( mod.equals("wr") )openMode = OPEN_ALWAYS;

    if( mod.equals("r") )rwMode = GENERIC_READ;
    if( mod.equals("rw") ||
        mod.equals("w") ||
        mod.equals("w+") )rwMode = GENERIC_READ | GENERIC_WRITE;

    hdl = CreateFile(
            file->getPath().c_str(),
            rwMode,
            FILE_SHARE_READ,
            NULL,
            openMode,
            FILE_ATTRIBUTE_NORMAL,
            NULL
            );
    err = GetLastError();

    if( err==ERROR_ALREADY_EXISTS && openMode==OPEN_ALWAYS )return;
    if( err==ERROR_ALREADY_EXISTS && openMode==OPEN_EXISTING )return;
    if( err==ERROR_FILE_NOT_FOUND ){
        throw IOError(err,"File not found");
    }
    if( err==ERROR_SUCCESS )return;
    String mess("error (GetLastError) open file ");
    mess = mess.append(file->getPath());
    throw IOError(err,mess);
}
__int64 RandomAccessFile::getPosition(){
    if( hdl==0 )throw IOError(-1,"File not open");
    LARGE_INTEGER off;
    LARGE_INTEGER newOff;
    newOff.HighPart = 0;
    newOff.LowPart = 0;

    if( SetFilePointerEx(
            hdl,
            newOff,
            &off,
            FILE_CURRENT)
            ==0 ){
        throw IOError(GetLastError(),"SetFilePointerEx()");
    }
    return (__int64)off.QuadPart;
//    if( fp==0 )throw IOError(-1,"File not open");
//    long off = ftell(fp);
//    if( off<0 )throw IOError(ferror(fp),"ftell error");
//    return off;
}
void RandomAccessFile::setPosition(__int64 pos){
    if( hdl==0 )throw IOError(-1,"File not open");

    LARGE_INTEGER newOff;
    newOff.QuadPart = pos;

    if( SetFilePointerEx(hdl,newOff,NULL,FILE_BEGIN)==0 ){
        DWORD errNo = GetLastError();
        std::cout << "SetFilePointerEx to " << pos << " error " << errNo << std::endl;
        throw IOError(errNo,"SetFilePointerEx()");
    }
//    if( fp==0 )throw IOError(-1,"File not open");
//    if( fseek(fp,pos,SEEK_SET)<0 )throw IOError(ferror(fp),"fseek error");
}
void RandomAccessFile::setPositionToEnd(){
    if( hdl==0 )throw IOError(-1,"File not open");

    LARGE_INTEGER newOff;
    newOff.QuadPart = 0;

    if( SetFilePointerEx(hdl,newOff,NULL,FILE_END)==0 ){
        throw IOError(GetLastError(),"SetFilePointerEx()");
    }
//    if( fp==0 )throw IOError(-1,"File not open");
//    if( fseek(fp,0,SEEK_END)<0 )throw IOError(ferror(fp),"fseek error");
}
void RandomAccessFile::close(){
    if( hdl!=0 ){
        if( CloseHandle(hdl)==0 ){
            throw IOError(GetLastError(),"CloseHandle()");
        }
        hdl = 0;
    }
//    if( fp!=0 ){
//        fclose(fp);
//        fp = 0;
//    }
}
void RandomAccessFile::flush(){
    if( hdl==0 )throw IOError(-1,"File not open");
    if( FlushFileBuffers(hdl)==0 )
        throw IOError(GetLastError(),"FlushFileBuffers()");
//    if( fp==0 )throw IOError(-1,"File not open");
//    if( fflush(fp)!=0 )throw IOError(ferror(fp),"fflush error");
}
size_t RandomAccessFile::write(const void* ptr,size_t size){
    if( hdl==0 )throw IOError(-1,"File not open");
    DWORD writed;
    if( WriteFile( hdl,ptr,size,&writed,NULL)==0 )
        throw IOError(GetLastError(),"WriteFile()");
    return writed;

//    if( fp==0 )throw IOError(-1,"File not open");
//    size_t writed = fwrite(ptr,size,1,fp);
//    int err = ferror(fp);
//    if( err!=0 )throw IOError(err,"fwrite error");
//    return writed;
}
size_t RandomAccessFile::read(void* ptr,size_t size){
    if( hdl==0 )throw IOError(-1,"File not open");
    DWORD readed;
    if( ReadFile(hdl,ptr,size,&readed,NULL)==0 )
        throw IOError(GetLastError(),"ReadFile()");
    return readed;
//    if( fp==0 )throw IOError(-1,"File not open");
//    size_t readed = fread(ptr,1,size,fp);
//    int err = ferror(fp);
//    if( err!=0 )throw IOError(err,"fread error");
//    return readed;
}

bool RandomAccessFile::isEOF(){
    if( hdl==0 )throw IOError(-1,"File not open");

    DWORD highSize=0, lowSize=GetFileSize(hdl,&highSize);
    LONG highPos=0, lowPos=SetFilePointer(hdl,0,&highPos,FILE_CURRENT);
    // _tell(fd) = _lseek(fd,0,SEEK_CUR)
    return lowPos==lowSize && highPos==highSize;

//    if( fp==0 )throw IOError(-1,"File not open");
//    return !(feof(fp)==0);
}