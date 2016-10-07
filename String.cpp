/* 
 * File:   string.cpp
 * Author: User
 * 
 * Created on 14 Январь 2012 г., 22:57
 */

#include "String.h"
#include <iostream>
#include <algorithm>

using namespace std;

String::String() {
}

String::String(const String& orig) : string(orig) {
}

String::~String() {
}

String::String(const string& orig) : string(orig) {
}

String::String(const char* orig) : string(orig) {
}

String String::replaceAll(const String& match,const String& replace){
    String res;
    size_t pos = 0;
    size_t find = 0;
    while(true){
        find = string::find(match.c_str(),pos);
        if( find<0 )break;
        if( find>(string::length()) )break;        
        if( (find - pos)>0 )res.append(substr(pos,find-pos));
        res.append(replace);
        pos = find + match.length();
    }
    if( pos<(length()) ){
        res.append(substr(pos,length()-pos));
    }
    return res;
}

bool _sortKeysInTemplate( String key1, String key2 ){
    if( key1.length() < key2.length() )return -1;
    if( key1.length() > key2.length() )return 1;
    return key1.compare( key2 ) < 0;
}

String String::replaceAll(const std::map<String,String>& replace){
    map<String,String> rMap;
    rMap = replace;
    vector<String>::iterator it;
    map<String,String>::iterator it2;

    vector<String> keys;
    for ( it2 = rMap.begin(); it2 != rMap.end(); it2++ ){
        keys.push_back( (*it2).first );
    }

//    cout << "not sorted keys:" << endl;
//    for( it=keys.begin(); it!=keys.end(); ++it ){
//        cout << (*it) << endl;
//    }

    sort( keys.begin(), keys.end(), _sortKeysInTemplate );

//    cout << "sorted keys:" << endl;
//    for( it=keys.begin(); it!=keys.end(); ++it ){
//        cout << (*it) << endl;
//    }

    String res;
    size_t pos = 0;
    while(true){
        size_t find = string::length()+10;
        String key;
        String rvalue;
        bool matched = false;
        for( it=keys.begin(); it!=keys.end(); ++it ){
            String rkey = (*it);
            if( rkey.length()<1 )continue;
            size_t rfind = string::find(rkey.c_str(),pos);
            if( rfind<find && rfind>=0 && rfind<(string::length()) ){
                find = rfind;
                key = rkey;
                rvalue = rMap[(*it)];
                matched = true;
            }
        }
        if( !matched )break;
        if( (find - pos)>0 )res = res.append(substr(pos,find-pos));
        res = res.append(rvalue);
        pos = find + key.length();
    }
    if( pos<(length()) ){
        res = res.append(substr(pos,length()-pos));
    }
    return res;
}

bool String::startWiths(const String& match){
    size_type mlen = match.length();
    size_type len = length();
    if( mlen > len )return false;
    if( mlen == 0 )return true;
    bool res = match.compare(string::substr(0,mlen))==0;
    return res;
}

bool String::endsWiths(const String& match){
    size_type mlen = match.length();
    size_type len = length();
    if( mlen > len )return false;
    if( mlen == 0 )return true;
    bool res = match.compare(string::substr(len-mlen,mlen))==0;
    return res;
}

bool String::equals(const String& match){
    bool res = match.compare(string::c_str())==0;
    return res;
}

String String::trimStart(const String& match){
    if( match.length()==0 )return String(string::c_str());

    String res(string::c_str());
    while(true){
        if( res.startWiths(match) ){
            res = res.substr(match.length(),res.length()-match.length());
        }else{
            break;
        }
    }
    return res;
}

String String::trimEnd(const String& match){
    if( match.length()==0 )return String(string::c_str());

    String res(string::c_str());
    while(true){
        if( res.endsWiths(match) ){
            res = res.substr(0,res.length()-match.length());
        }else{
            break;
        }
    }
    return res;
}

String String::trimStart(){
    if( string::length()==0 )return String("");
    String res("");
    char cbuf[2];
    bool f = false;
    for(size_type idx=0;idx<string::length();idx++){
        char c = at(idx);
        if( !f ){
            if( c<32 )continue;
            if( c==' ' )continue;
        }
        f = true;
        cbuf[0] = c;
        cbuf[1] = 0;
        res = res.append(cbuf);
    }
    return res;
}

String String::trimEnd(){
    if( string::length()==0 )return String("");
    String res("");
    char cbuf[2];
    bool f = false;
    for(size_type idx=string::length()-1;idx>=0;idx--){
//        std::cout << "idx=" << idx;
//        std::cout << " len=" << string::length() << std::endl;
        if( idx>(string::length()-1) )break;
        char c = at(idx);
        if( !f ){
            if( c<32 )continue;
            if( c==' ' )continue;
        }
        f = true;
        cbuf[0] = c;
        cbuf[1] = 0;
        if( res.length()>0 ){
            res = res.insert(0,cbuf);
        }else{
            res = res.append(cbuf);
        }
    }
    return res;
}

std::vector<String> String::split(const String& delim,long max){
    vector<String> result;
    String dl(delim);
    if( dl.length()==0 ){
        result.push_back(String(*this));
        return result;
    }
    if( max==1 ){
        result.push_back(String(*this));
        return result;
    }
    size_type pos = 0;
    size_type find = 0;
    size_type maxf = string::length()-1;
    while(true){
        if( max>0 && (result.size()==(max-1)) ){
            size_type sz = string::length()-pos;
            if( sz>0 ){
                String add( string::substr(pos, string::length()-pos) );
                result.push_back(add);
            }else{
                result.push_back(String(""));
            }
            break;
        }
        find = string::find(dl.c_str(),pos);
        if( find>maxf ){
            size_type sz = string::length()-pos;
            if( sz>0 ){
                String add( string::substr(pos, sz) );
                result.push_back(add);
            }else{
                result.push_back(String(""));
            }
            break;
        }
        if( find<0 ){
            size_type sz = string::length()-pos;
            if( sz>0 ){
                String add( string::substr(pos, string::length()-pos) );
                result.push_back(add);
            }else{
                result.push_back(String(""));
            }
            break;
        }
        size_type sz = find-pos;
        if( sz>0 ){
            String add( string::substr(pos, find-pos) );
            result.push_back(add);
        }else{
            result.push_back(String(""));
        }
        pos = find+dl.length();
    }
    return result;
}
