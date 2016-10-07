#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <contrib/minizip/unzip.h>
#include <sys/stat.h>
#include <libgen.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <unistd.h>
#include "String.h"
#include "File.h"
#include "IOError.h"

using namespace std;

#define QOUTE_ARGS_AUTO 0
#define QOUTE_ARGS_ALWAYS 1
#define QOUTE_ARGS_NEVER 2

struct SFX_OPTIONS{
    // Удалять файлы при завершении
    bool deleteFilesOnExit;
    
    // runing command (template) after unpack
    String run;
    
    // Шаблон текущего каталога для выполняемой комманды
    String workDir;
    
    // Брать в кавычки аргрументы: AUTO/auto (0), ALWAYS/always (1), NEVER/never (2)
    int quoteArgs;
    
    // Возвращать код выхода команды run
    bool returnExitCode;
    
    // Приветственное сообщение
    bool showHello;
    
    // Показывать сообщение при старте
    String hello;
    
    // Показывать процесс распаковки
    bool showLog;
    
    // true - Использовать времменый каталог; false - использовать текущий каталог
    bool useTemp;
    
    // Переменные окружения
    map<String,String> env;
};

void listContent(unzFile zfile);
void extract(unzFile zfile,String targetDir,SFX_OPTIONS& sfxOptions);
void extractFile(unzFile zfile,File& targetFile,const String& targetDir,SFX_OPTIONS& sfxOptions);
long getLoaderLength();
void makeLoader(const String& exeFileName,const String& loaderName);
long toLong(const String& text);
__int64 findStringInFile(const File& srcFile,const String& srcText,__int64 from,__int64 to);
__int64 findStringInFile(RandomAccessFile& file,const String& srcText,__int64 from,__int64 to);
String toString(long size);
void setLoaderLength(const File& loaderFile,__int64 markerOffset);
vector<File> extractZipFromExe(
        const String& exeFileName,
        long loaderLen,
        SFX_OPTIONS& sfxOptions,
        int& exitCode,
        int argc, 
        char** argv
);
String checkOptionsString(RandomAccessFile& file);
map<String,String> readProperties(const String& optionsString);
void hexDump(void *ptr,long size);
void readOptions(RandomAccessFile& file, SFX_OPTIONS& options);
File generateTempFile(File &tempDir);

using namespace std;

// Size of buffer for searching in file
#define SEARCH_IN_FILE_BUFF 4096
#define OPTION_MAX_SIZE 1024*256
#define OPTION_BEGIN "OPTIONS BEGIN {"
#define OPTION_END "} OPTIONS END"
#define HELLO "SFX ZIP v 0.4 by nt.gocha@gmail.com"

// DEFINE Functions
int main(int argc, char** argv) {
    SFX_OPTIONS sfxOptions;
    String loaderName("");

    long loaderLen = getLoaderLength();
    if( loaderLen==0 ){
        for( int i=1; i<(argc-1); i++ ){
            String argName(argv[i]);
            String argVal(argv[i+1]);
            if( argName.startWiths("-") ){
                if( argName.compare("-loaderName")==0 ){
                    loaderName = String(argVal);
                }
            }
        }
        if( argc>0 ){
            String exeFileName( argv[0] );
            makeLoader(exeFileName,loaderName);
            return (EXIT_SUCCESS);
        }
    }
    int exitCode = EXIT_SUCCESS;
    vector<File> files = extractZipFromExe(
            argv[0],
            loaderLen,
            sfxOptions,
            exitCode,
            argc,
            argv);
    for( int iF=0; iF<files.size(); iF++ ){
        File f = files.at(iF);
        if( sfxOptions.showLog )cout << "removing " << f.getPath() << endl;
        f.remove();
    }
    
    return (exitCode);
}

File generateTempFile(File &tempDir){
    long tempIdx = 0;
    File tempZip(tempDir,"temp.zip");
    while(true){
        if( !tempZip.isExists() ){
            break;
        }
        tempIdx++;
        String tempName("temp.");
        tempName = tempName.append(toString(tempIdx));
        tempName = tempName.append(".zip");
        tempZip = File(tempDir,tempName);
    }
    return tempZip;
}

vector<File> extractZipFromExe(
        const String& exeFileName,
        long loaderLen,
        SFX_OPTIONS& sfxOptions,
        int& exitCode,
        int argc,
        char** argv
){
    vector<File> res;
    File fExe( exeFileName );
    File tempZip( exeFileName );
    try{
        // create zip file from exe
        __int64 ldrLen = (__int64)loaderLen;
        //cout << "loader len " << loaderLen << "(" << ldrLen << ")" << endl;
        //cout << "extract zip from exe to " << tempZip.getPath() << endl;
        RandomAccessFile fSrc = fExe.open("r");
        fSrc.setPosition(ldrLen);
        
        //check option
        readOptions(fSrc,sfxOptions);

        // generate temp file name for zip archive
        File tempDir = File::getTempDir();
        if( !sfxOptions.useTemp ){
            tempDir = File::getCurrentDir();
//            cout << "use current " << tempDir.getPath().c_str() << endl;
        }else{
//            cout << "use temp " << tempDir.getPath().c_str() << endl;
        }
            
        tempZip = generateTempFile( tempDir );

        //...
        RandomAccessFile fDest = tempZip.open("w");
        char buff[1024*16];
        long total = 0;
        while(true){
            if( fSrc.isEOF() ){
//                cout << "end of exe" << endl;
                break;
            }
            size_t rd = fSrc.read(buff,sizeof(buff));
            if( rd>0 ){
                total += rd;
//                cout << "write " << total << " bytes" << endl;
                fDest.write(buff,rd);
                fDest.flush();
            }
            if( rd<0 ){
                if( sfxOptions.showLog )cout << "readed " << rd << " bytes, end" << endl;
                break;
            }
        }        
        fDest.close();
        fSrc.close();

        // extract zip file to directory
        String tempZipD( tempZip.getPath() );
        tempZipD = tempZipD.append(".content");
        File tempZipDir(tempZipD);
        tempZipDir.mkdir(true);
        res.push_back(tempZipDir);

        if( sfxOptions.showLog )cout << "unpack zip to " << tempZipD << endl;
        unzFile zfile = unzOpen(tempZip.getPath().c_str());
        if( !zfile ){
            cout << "can't open zip " << tempZip.getPath() << endl;
            return res;
        }
        extract(zfile,tempZipD,sfxOptions);
        unzClose(zfile);

        // run setup.exe or other....
        if( sfxOptions.run.length()>0 ){
            String runTmpl(sfxOptions.run);
            runTmpl = runTmpl.trimEnd("/");
            runTmpl = runTmpl.trimEnd("\\");

            // making template varaibles
            map<String,String> values;
            values["$dir"] = String(tempZipD);
            values["$dir_rs"] = String(tempZipD).replaceAll("\\","/");

            String arguments("");
            for( long i=1; i<argc; i++ ){
                if( i>1 )arguments = arguments.append(" ");
                
                String argStr(argv[i]);
                
                bool qouteArg = false;
                if( sfxOptions.quoteArgs==QOUTE_ARGS_ALWAYS )qouteArg = true;

                // Обрамление кавычками если найден пробел или перевод строки,....
                if( sfxOptions.quoteArgs==QOUTE_ARGS_AUTO && argStr.find(" ",0) != String::npos )
                    qouteArg = true;
                if( sfxOptions.quoteArgs==QOUTE_ARGS_AUTO && argStr.find("\n",0) != String::npos )
                    qouteArg = true;
                if( sfxOptions.quoteArgs==QOUTE_ARGS_AUTO && argStr.find("\r",0) != String::npos )
                    qouteArg = true;
                if( sfxOptions.quoteArgs==QOUTE_ARGS_AUTO && argStr.find("\t",0) != String::npos )
                    qouteArg = true;
                if( sfxOptions.quoteArgs==QOUTE_ARGS_AUTO && argStr.find("\f",0) != String::npos )
                    qouteArg = true;
                
                if( qouteArg ){
                    arguments = arguments.append("\"");
                }
                arguments = arguments.append(argStr.c_str());
                if( qouteArg ){
                    arguments = arguments.append("\"");
                }
            }
            values["$args"] = String(arguments.c_str());
            values["$zip"] = String(tempZip.getPath());
            values["$zip_rs"] = String(tempZip.getPath()).replaceAll("\\","/");
            
            bool restoreWorkDir = false;
            File savedWorkDir = File::getCurrentDir();
            if( sfxOptions.workDir.length()>0 ){
                String workDir = sfxOptions.workDir.replaceAll(values);
                File wd(workDir);
                File::setCurrentDir(wd);
                restoreWorkDir = true;
            }

            String run = runTmpl.replaceAll(values);
            int resCode = system(run.c_str());
            if( restoreWorkDir )File::setCurrentDir( savedWorkDir );
            if( sfxOptions.returnExitCode )exitCode = resCode;
        }
    }
    catch(IOError err){
        cout << "error " << err.getError() << " " << err.getMessage() << endl;
    }
    if( !sfxOptions.deleteFilesOnExit ){
        res.clear();
    }
    res.push_back(tempZip);
    return res;
}

String checkOptionsString(RandomAccessFile& file){
    //cout << "check option begin" << endl;
    String res("");

    __int64 savedPointer = file.getPosition();
    __int64 restorePointer = savedPointer;

    char buff[1024];
    size_t rd = file.read(buff,sizeof(buff));
    //cout << "readed " << rd << endl;
    //hexDump(buff,rd);

    if( rd>0 ){
        //check matched OPTION_BEGIN
        char* optionBegin = OPTION_BEGIN;
        int idx=0;
        int optionBeginLen = strlen(optionBegin);
        boolean matched = true;
        //cout << "strlen(" << optionBegin << ")=" << optionBeginLen << endl;
        if( optionBeginLen<=rd ){
            for( int i1=0;i1<optionBeginLen;i1++ ){
                if( buff[i1]!=optionBegin[i1] ){
                    matched = false;
                    break;
                }
            }
        }else{
            matched = false;
        }

        //check matched OPTION_END
        if( matched ){
            //cout << "options begin matched" << endl;
            String opEnd(OPTION_END);
            __int64 opEndBeginIdx = findStringInFile(
                    file,opEnd,savedPointer,savedPointer+OPTION_MAX_SIZE);
            //cout << "opEndBeginIdx=" << opEndBeginIdx << endl;

            if( opEndBeginIdx>((__int64)0) ){
                restorePointer = opEndBeginIdx + opEnd.length();
                //cout << "restorePointer=" << restorePointer << endl;
            }
        }else{
            //cout << "option begin not matched" << endl;
        }
    }

    // check exists option data
    if( restorePointer > savedPointer ){
        //cout << "has options, reading" << endl;
        file.setPosition(savedPointer);
        long total = 0;
        long max = (long)(restorePointer - savedPointer);
        //cout << "max=" << max << endl;
        while(true){
            if( file.isEOF() )break;

            __int64 pos = file.getPosition();
            if( pos>=restorePointer )break;

            rd = file.read(buff,sizeof(buff));
            total += (long)rd;
            //cout << "total=" << total << endl;
            if( rd>0 ){
                char cbuf[2];
                bool skip = false;
                long need = 0;
                if( total>max ) need = max; else max - total;

                //cout << "need=" << need << endl;
                if( need>0 ){
                    long transltCo = rd;
                    if( transltCo>need )transltCo = need;
                    //cout << "transltCo=" << transltCo << endl;
                    for( long i=0; i<transltCo; i++ ){
                        if( skip ){
                            skip = false;
                            continue;
                        }
                        char c = buff[i];
                        char c2 = i>=(transltCo-1) ? 0 : buff[i+1];
                        if( c==0 ){
                            cbuf[0] = '\n';
                            cbuf[1] = 0;
                            res = res.append(cbuf);
                            continue;
                        }
                        if( c=='\n' && c2=='\r' ){
                            cbuf[0] = '\n';
                            cbuf[1] = 0;
                            res = res.append(cbuf);
                            skip = true;
                            continue;
                        }
                        if( c=='\r' && c2=='\n' ){
                            cbuf[0] = '\n';
                            cbuf[1] = 0;
                            res = res.append(cbuf);
                            skip = true;
                            continue;
                        }
                        if( c=='\n' && c2!='\r' ){
                            cbuf[0] = '\n';
                            cbuf[1] = 0;
                            res = res.append(cbuf);
                            continue;
                        }
                        if( c=='\r' && c2!='\n' ){
                            cbuf[0] = '\n';
                            cbuf[1] = 0;
                            res = res.append(cbuf);
                            continue;
                        }
                        cbuf[0] = c;
                        cbuf[1] = 0;
                        res = res.append(cbuf);
                    }
                }
            }
        }
        //cout << "options:" << endl << res << endl;
    }

    file.setPosition(restorePointer);
    return res;
}

map<String,String> readProperties(const String& optionsString){
    map<String,String> res;
    String options(optionsString);
    options = options.trimStart(OPTION_BEGIN);
    options = options.trimEnd(OPTION_END);

    vector<String> lines = options.split("\n",-1);
    for( long i=0; i<lines.size(); i++ ){
        String line = lines[i];
        if( line.trimStart().startWiths("#")){
            continue;
        }
        vector<String> kv = line.split("=",2);
//        for( long kvi=0;kvi<kv.size();kvi++ ){
//        }

        if( kv.size()==2 ){
            String key = kv[0].trimEnd().trimStart();
            String value = kv[1].trimEnd().trimStart();
//            key = key.trimEnd().trimStart();
            if( key.length()>0 ){
                res[key] = value;
//                cout << "prop " << key << " = " << value << endl;
            }
        }
    }
    return res;
}

bool readBoolProp(const String& val,bool def){
    String v(val);
    if( v.equals("yes") ){
        return true;
    }else if(v.equals("true") ){
        return true;
    }else if(v.equals("1") ){
        return true;
    }else if(v.equals("no") ){
        return false;
    }else if(v.equals("false") ){
        return false;
    }else if(v.equals("0") ){
        return false;
    }
    return def;
}

void readOptions(RandomAccessFile& file, SFX_OPTIONS& options){
    options.deleteFilesOnExit = true;
    options.showHello = true;
    options.showLog = true;
    options.returnExitCode = false;
    options.useTemp = true;
    options.quoteArgs = QOUTE_ARGS_AUTO;

    String empty("");
    options.run = empty;
    options.workDir = empty;
    
    options.hello = String(HELLO);

    String optionsData = checkOptionsString(file);
    if( optionsData.length()>0 ){
        map<String,String> props = readProperties(optionsData);
        if( props.count("deleteFiles")>0 ){
            options.deleteFilesOnExit = readBoolProp( props["deleteFiles"], options.deleteFilesOnExit );
        }
        if( props.count("run")>0 ){
            options.run = props["run"];
        }
        if( props.count("workDir")>0 ){
            options.workDir = props["workDir"];
        }
        if( props.count("hello")>0 ){
            options.hello = props["hello"];
        }
        if( props.count("showHello")>0 ){
            options.showHello = readBoolProp( props["showHello"], options.showHello );
        }
        if( props.count("showLog")>0 ){
            options.showLog = readBoolProp( props["showLog"], options.showLog );
        }
        if( props.count("useTemp")>0 ){
            options.useTemp = readBoolProp( props["useTemp"], options.useTemp );
        }
        if( props.count("return")>0 ){
            options.returnExitCode = readBoolProp( props["return"], options.returnExitCode );
        }
        if( props.count("quoteArgs") ){
            String quoteArgs = props["quoteArgs"];
            if( quoteArgs.equals("auto") ||
                quoteArgs.equals("AUTO")
                ){
                options.quoteArgs = QOUTE_ARGS_AUTO;
            }else if( quoteArgs.equals("always") ||
                      quoteArgs.equals("ALWAYS")){
                options.quoteArgs = QOUTE_ARGS_ALWAYS;
            }else if( quoteArgs.equals("never") ||
                      quoteArgs.equals("NEVER")){
                options.quoteArgs = QOUTE_ARGS_NEVER;
            }
        }
    }
    
    map<String,String>::iterator itr;
    String envPrefix("env_");
    for( itr=options.env.begin(); itr!=options.env.end(); itr++ ){
        String key ((*itr).first);
        cout << "prop!! " << key << endl;
        if( key.startWiths(envPrefix) ){
            String envName = key.substr(4,key.length()-4);
            String envVal = (*itr).second;
            String v(envName);
            v.append("=");
            v.append(envVal);
            putenv(v.c_str());
            cout << "putenv( " << v << " )" << endl;
        }
    }

    if( options.showHello ){
        cout << options.hello << endl;
    }
}

__int64 findStringInFile(RandomAccessFile& file,const String& srcText,__int64 from,__int64 to){
    String text(srcText);
    char buff[1024];
    __int64 idx = 0;
    __int64 idx2 = 0;
    __int64 matchedIdx = -1;

    if( from>=0 )file.setPosition(from);

    while(true){
        if( matchedIdx>=0 )break;
        if( file.isEOF() )break;
        idx = file.getPosition();
        if( (to>=(__int64)0) && (idx>=to) )break;

        //cout << "pos:" << idx << endl;
        size_t rd = file.read(buff,sizeof(buff));
        idx2 = file.getPosition();
        if( rd>0 ){
            for(int i=0;i<rd;i++){
                char c = text.at(0);
                if( buff[i]==c ){
                    //cout << "check:" << (idx+i) << endl;
                    char buff2[SEARCH_IN_FILE_BUFF];
                    file.setPosition(idx+i);
                    size_t rd2 = file.read(buff2,sizeof(buff2));
                    if( rd2>0 ){
                        int co = rd2;
                        if( co>(text.length()-1) ){
                            co = text.length()-1;
                            bool matched = true;
                            for( int i2=0;i2<co;i2++ ){
                                char c2 = text.at(i2);
                                if( buff2[i2]!=c2 ){
                                    matched = false;
                                }
                            }
                            if( matched ){
                                matchedIdx = idx+i;
                                //cout << "matched:" << (idx+i) << endl;
                            }
                        }
                    }
                }
            }
        }
        file.setPosition(idx2);
    }

    return matchedIdx;
}

__int64 findStringInFile(const File& srcFile,const String& srcText,__int64 from,__int64 to){
    File sf(srcFile);
    String text(srcText);
    if( text.length()<1 )return -1;

    RandomAccessFile file = sf.open("r");
    __int64 matchedIdx = -1;

    matchedIdx = findStringInFile(file,srcText,from,to);
    file.close();

    return matchedIdx;
}

void makeLoader(const String& exeFileName,const String& loaderName){
    String ldrName(loaderName);
    if( ldrName.length()==0 ){
        char loaderNameBuff[2048];
        cout << "enter new loader file name: ";
    //    cin.read(loaderNameBuff,sizeof(loaderNameBuff));
        gets(loaderNameBuff);
        ldrName = String(loaderNameBuff);
    }

    File exeFile(exeFileName);
    File loaderFile(ldrName);

    try{
        exeFile.copy(loaderFile);
        cout << "exe file copied" << endl;
        String matchedTxt("---");
        matchedTxt = matchedTxt.append("loader");
        matchedTxt = matchedTxt.append("---{");
        matchedTxt = matchedTxt.append("000000000000000");
        matchedTxt = matchedTxt.append("}");
        __int64 matchedIdx = findStringInFile(loaderFile,matchedTxt,0,-1);

        if( matchedIdx>=0 ){
            cout << "marker found" << endl;
            setLoaderLength( loaderFile, matchedIdx );
        }
    }catch(IOError err){
        cout << "error " << err.getError() << " " << err.getMessage() << endl;
    }
}

String toString(long size){
    if( size==0 )return String("0");
    if( size<0 )size = -size;
    String txt;
    while( size>0 ){
        long d = size % 10;
        size = size - d;
        size = (long)(size / 10);
        char c;
        switch(d){
            case 0: c='0'; break;
            case 1: c='1'; break;
            case 2: c='2'; break;
            case 3: c='3'; break;
            case 4: c='4'; break;
            case 5: c='5'; break;
            case 6: c='6'; break;
            case 7: c='7'; break;
            case 8: c='8'; break;
            case 9: c='9'; break;
        }
        if( txt.length()==0 ){
            char ttt[2];
            ttt[0] = c;
            ttt[1] = 0;
            txt = txt.append(ttt);
        }else{
            char ttt[2];
            ttt[0] = c;
            ttt[1] = 0;
            txt = txt.insert(0,ttt);
        }
    }
    return txt;
}

void setLoaderLength(const File& loaderFile,__int64 markerOffset){
    cout << "setLoaderLength()" << endl;
    File ff(loaderFile);
    long sizeLong = ff.getSize();
    String sizeStr = toString(sizeLong);
    if( sizeStr.length()<15 ){
        int strLen = sizeStr.length();
        int appLen = 15 - strLen;
        for( int i=0;i<appLen;i++ ){
            sizeStr = sizeStr.insert(0,"0");
        }
    }
    String marker("---");
    marker = marker.append("loader");
    marker = marker.append("---{");
    marker = marker.append(sizeStr);
    marker = marker.append("}");
    cout << "marker:" << marker << endl;

    RandomAccessFile f = ff.open("w+");
    f.setPosition(markerOffset);
    for( int i=0; i<marker.length(); i++ ){
        char buff[1];
        buff[0] = marker.at(i);
        f.write(buff,1);
    }
    f.flush();
    f.close();
    cout << "marker setted" << endl;
}

long getLoaderLength(){
    int digitCo = 15;
    String loaderLength("---loader---{000000000000000}");
    loaderLength = loaderLength.trimStart("---");
    loaderLength = loaderLength.trimStart("loader---");
    loaderLength = loaderLength.trimStart("{");
    loaderLength = loaderLength.trimEnd("}");
    return toLong(loaderLength);
}

long digit(char c){
    switch( c ){
        case '0':return 0;
        case '1':return 1;
        case '2':return 2;
        case '3':return 3;
        case '4':return 4;
        case '5':return 5;
        case '6':return 6;
        case '7':return 7;
        case '8':return 8;
        case '9':return 9;
    }
    return -1;
}

long toLong(const String& text){
    String txt(text);
    long num = 0;
    long kof = 1;
    while(txt.length()>0){
        long d = digit(txt.at(txt.length()-1));
        num = num + d*kof;
        kof = kof * 10;
        if( txt.length()>1 )
            txt = txt.substr(0,txt.length()-1);
        else
            txt = String("");
    }
    return num;
}

void listContent(unzFile zfile){
    cout << "list content" << endl;

    int err;
    unz_global_info ginf;
    err = unzGetGlobalInfo(zfile,&ginf);
    if (err!=UNZ_OK)cout << "error " << err << " with zipfile in unzGetGlobalInfo" << endl;

    cout << "count entries = " << ginf.number_entry << endl;
    for( int iEntry=0; iEntry<ginf.number_entry; iEntry++ ){
        unz_file_info fileinf;
        char fileName[2048];
        err = unzGetCurrentFileInfo(zfile,&fileinf,fileName,sizeof(fileName),0,0,0,0);
        if( err!=UNZ_OK ){
            cout << "error " << err << " in zipfile" << endl;
            break;
        }

        cout << (iEntry+1) << ". " << fileName << endl;

        bool lastEntry = iEntry==(ginf.number_entry-1);
        if( !lastEntry ){
            err = unzGoToNextFile(zfile);
            if( err!=UNZ_OK ){
                cout << "error " << err << " in zipfile" << endl;
                break;
            }
        }
    }
}

void extract(unzFile zfile,String targetDir,SFX_OPTIONS &sfxOptions){
    if( sfxOptions.showLog )cout << "extract content" << endl;

    int err;
    unz_global_info ginf;
    err = unzGetGlobalInfo(zfile,&ginf);
    if (err!=UNZ_OK){
        cout << "error " << err << " with zipfile in unzGetGlobalInfo" << endl;
        return;
    }

    if( sfxOptions.showLog )cout << "count entries = " << ginf.number_entry << endl;
    for( int iEntry=0; iEntry<ginf.number_entry; iEntry++ ){
        unz_file_info fileinf;
        char csFileName[2048];
        err = unzGetCurrentFileInfo(zfile,&fileinf,csFileName,sizeof(csFileName),0,0,0,0);
        if( err!=UNZ_OK ){
            cout << "error " << err << " in zipfile" << endl;
            break;
        }

        String fileName(csFileName);
        if( sfxOptions.showLog ){
            String t(csFileName);
//            t = t.replaceAll("\r","\\r").replaceAll("\n","\\n").replaceAll("\\0");
            cout << (iEntry+1) << "/" << ginf.number_entry << ". " << t << endl;
        }

        // extract content
        bool isDir = false;
        if( fileName.endsWiths("\\") || fileName.endsWiths("/") ) {
            isDir = true;
        }
        File _targetDir(targetDir);
        File targetFile(_targetDir,fileName);
        if( isDir ){
            if( sfxOptions.showLog )cout << targetFile.getPath() << endl;
            if( !targetFile.isDir() && !targetFile.mkdir(true) ){
                cout << "can't create directory:" << targetFile.getPath() << endl;
                break;
            }
        }else{
            File tarDir = targetFile.getParent();
            if( !tarDir.isDir() && !tarDir.mkdir(true) ){
                cout << "can't create directory:" << targetFile.getPath() << endl;
                break;
            }
        }
        if( !isDir ){
            //extract file content
            extractFile(zfile,targetFile,targetDir,sfxOptions);
        }
        // extract content

        bool lastEntry = iEntry==(ginf.number_entry-1);
        if( !lastEntry ){
            err = unzGoToNextFile(zfile);
            if( err!=UNZ_OK ){
                cout << "error " << err << " in zipfile" << endl;
                break;
            }
        }
    }
}

void extractFile(unzFile zfile,File& targetFile,const String& targetDir,SFX_OPTIONS &sfxOptions){
    unz_file_info zinf;
    int err;
    char _filename[2048];
    err = unzGetCurrentFileInfo(zfile,&zinf,_filename,sizeof(_filename),NULL,0,NULL,0);
    if( err!=UNZ_OK ){
        cout << "error " << err << " unpack " << endl;
        return;
    }

    char buff[4096];
    err = unzOpenCurrentFile(zfile);
    if( err!=UNZ_OK ){
        cout << "error " << err << " unpack " << endl;
        return;
    }

    try{
        RandomAccessFile file = File(targetFile).open("w+");
        while(true){
            err = unzReadCurrentFile(zfile,buff,sizeof(buff));
            if( err<0 ){
                cout << "error " << err << " unzReadCurrentFile" << endl;
                break;
            }
            if( err==0 )break;
            file.write(buff,err);
        }
        file.close();
    }catch(IOError err){
        cout << "error " << err.getError() << " " << err.getMessage() << endl;
    }

    err = unzCloseCurrentFile(zfile);
    if( err!=UNZ_OK ){
        cout << "error " << err << " unzCloseCurrentFile " << endl;
        return;
    }
}

char toDigit(int c,char radix){
    if( c==0 && radix>=2 )return '0';
    if( c==1 && radix>=2 )return '1';
    if( c==2 && radix>=3 )return '2';
    if( c==3 && radix>=4 )return '3';
    if( c==4 && radix>=5 )return '4';
    if( c==5 && radix>=6 )return '5';
    if( c==6 && radix>=7 )return '6';
    if( c==7 && radix>=8 )return '7';
    if( c==8 && radix>=9 )return '8';
    if( c==9 && radix>=10 )return '9';
    if( c==10 && radix>=11 )return 'A';
    if( c==11 && radix>=12 )return 'B';
    if( c==12 && radix>=13 )return 'C';
    if( c==13 && radix>=14 )return 'D';
    if( c==14 && radix>=15 )return 'E';
    if( c==15 && radix>=16 )return 'F';
    return '?';
}

void hexDump(void *ptr,long size){
    if( size<=0 )return;
    long ci=-1;

    String hexStr("");
    String chrStr("");
    for( long i=0; i<size; i++ ){
        ci++;
        if( ci>=8 ){
            ci=0;
            cout << hexStr << "| " << chrStr << endl;
            hexStr = String("");
            chrStr = String("");
        }

        unsigned char chr = ((unsigned char*)ptr)[i];
        unsigned char byte = chr;
        unsigned int lowBits = ((int)byte) & ((int)0x0F);
        unsigned int hiBits = ((int)byte) >> 4;

        char cbuf[2];
        cbuf[0] = toDigit(hiBits,16);
        cbuf[1] = 0;
        hexStr = hexStr.append( cbuf );

        cbuf[0] = toDigit(lowBits,16);
        cbuf[1] = 0;
        hexStr = hexStr.append( cbuf );

        hexStr = hexStr.append( " " );

        if( chr<32 ){
            cbuf[0] = ' ';
            cbuf[1] = 0;
            chrStr = chrStr.append(cbuf);
        }else{
            cbuf[0] = chr;
            cbuf[1] = 0;
            chrStr = chrStr.append(cbuf);
        }
    }

    if( chrStr.length()>0 ){
        cout << hexStr << "| " << chrStr << endl;
        hexStr = String("");
        chrStr = String("");
        cout << endl;
    }
}
