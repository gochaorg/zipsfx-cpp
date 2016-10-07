rm zipsfx test.exe
app1.exe -loaderName zipsfx
copy /b zipsfx + options.txt + javadoc.zip test.exe
test.exe