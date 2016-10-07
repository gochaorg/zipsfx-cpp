rm zipsfx test.exe data.zip
zipsfx-cpp.exe -loaderName zipsfx
7z a -tzip data.zip launcher.bat data
copy /b zipsfx + options.txt + data.zip test.exe
test.exe arg1 arg2 "arg3 ZZ"
