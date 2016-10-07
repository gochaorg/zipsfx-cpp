/* 
 * File:   File.h
 * Author: User
 *
 * Created on 14 Январь 2012 г., 23:35
 */

#ifndef _FILE_H
#define	_FILE_H

#include <sys/stat.h>
#include <libgen.h>
#include <vector>

#include "String.h"

class RandomAccessFile;
#include "RandomAccessFile.h"
#include "IOError.h"

class File {
protected:
    String name;
public:
    File(const String& name);
    File(const File& orig);
    File(const File& parent,const String& childName);
    virtual ~File();

    bool File::isExists();
    bool File::isFile();
    bool File::isDir();

    String getPath();
    String getName();
    File getParent();
    long getSize();

    bool mkdir(bool withParents);
    RandomAccessFile open(const String& mode) throw (IOError);

    std::vector<File> readdir();
    void copy(const File& target);
    static File getTempDir();
    static File getCurrentDir();
    static bool setCurrentDir(File& file);
    bool remove();
};

#endif	/* _FILE_H */

