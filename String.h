/* 
 * File:   string.h
 * Author: User
 *
 * Created on 14 Январь 2012 г., 22:57
 */

#ifndef _STRING_H
#define	_STRING_H

#include <string.h>
#include <string>
#include <vector>
#include <map>

class String : public std::string {
public:
    String();
    String(const std::string& src);
    String(const char * orig);
    String(const String& orig);
    virtual ~String();

    bool startWiths(const String& match);
    bool endsWiths(const String& match);
    bool equals(const String& match);
    
    String replaceAll(const String& match,const String& replace);
    String replaceAll(const std::map<String,String>& replace);
    String trimStart(const String& match);
    String trimEnd(const String& match);
    String trimStart();
    String trimEnd();

    std::vector<String> split(const String& delim,long max);
};

#endif	/* _STRING_H */

