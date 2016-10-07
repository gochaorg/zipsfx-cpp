#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc.exe
CCC=g++.exe
CXX=g++.exe
FC=gfortran
AS=as.exe

# Macros
CND_PLATFORM=MinGW-Windows
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/lib/zlib/incdir/inftrees.o \
	${OBJECTDIR}/lib/zlib/incdir/gzlib.o \
	${OBJECTDIR}/String.o \
	${OBJECTDIR}/lib/zlib/incdir/contrib/infback9/inftree9.o \
	${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/mztools.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/lib/zlib/incdir/gzread.o \
	${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/unzip.o \
	${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/minizip.o \
	${OBJECTDIR}/File.o \
	${OBJECTDIR}/lib/zlib/incdir/gzclose.o \
	${OBJECTDIR}/lib/zlib/incdir/deflate.o \
	${OBJECTDIR}/RandomAccessFile.o \
	${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/miniunz.o \
	${OBJECTDIR}/lib/zlib/incdir/contrib/infback9/infback9.o \
	${OBJECTDIR}/lib/zlib/incdir/example.o \
	${OBJECTDIR}/lib/zlib/incdir/inffast.o \
	${OBJECTDIR}/lib/zlib/incdir/crc32.o \
	${OBJECTDIR}/lib/zlib/incdir/zutil.o \
	${OBJECTDIR}/lib/zlib/incdir/trees.o \
	${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/zip.o \
	${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/iowin32.o \
	${OBJECTDIR}/IOError.o \
	${OBJECTDIR}/lib/zlib/incdir/inflate.o \
	${OBJECTDIR}/lib/zlib/incdir/infback.o \
	${OBJECTDIR}/lib/zlib/incdir/contrib/masmx64/inffas8664.o \
	${OBJECTDIR}/lib/zlib/incdir/contrib/blast/blast.o \
	${OBJECTDIR}/lib/zlib/incdir/compress.o \
	${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/ioapi.o \
	${OBJECTDIR}/lib/zlib/incdir/adler32.o \
	${OBJECTDIR}/lib/zlib/incdir/gzwrite.o \
	${OBJECTDIR}/lib/zlib/incdir/contrib/puff/puff.o \
	${OBJECTDIR}/lib/zlib/incdir/uncompr.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-Wa
CXXFLAGS=-Wa

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/zipsfx-cpp.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/zipsfx-cpp.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/zipsfx-cpp ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/lib/zlib/incdir/inftrees.o: lib/zlib/incdir/inftrees.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/inftrees.o lib/zlib/incdir/inftrees.c

${OBJECTDIR}/lib/zlib/incdir/gzlib.o: lib/zlib/incdir/gzlib.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/gzlib.o lib/zlib/incdir/gzlib.c

${OBJECTDIR}/String.o: String.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Ilib/zlib/incdir -MMD -MP -MF $@.d -o ${OBJECTDIR}/String.o String.cpp

${OBJECTDIR}/lib/zlib/incdir/contrib/infback9/inftree9.o: lib/zlib/incdir/contrib/infback9/inftree9.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir/contrib/infback9
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/contrib/infback9/inftree9.o lib/zlib/incdir/contrib/infback9/inftree9.c

${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/mztools.o: lib/zlib/incdir/contrib/minizip/mztools.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir/contrib/minizip
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/mztools.o lib/zlib/incdir/contrib/minizip/mztools.c

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Ilib/zlib/incdir -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/lib/zlib/incdir/gzread.o: lib/zlib/incdir/gzread.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/gzread.o lib/zlib/incdir/gzread.c

${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/unzip.o: lib/zlib/incdir/contrib/minizip/unzip.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir/contrib/minizip
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/unzip.o lib/zlib/incdir/contrib/minizip/unzip.c

${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/minizip.o: lib/zlib/incdir/contrib/minizip/minizip.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir/contrib/minizip
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/minizip.o lib/zlib/incdir/contrib/minizip/minizip.c

${OBJECTDIR}/File.o: File.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Ilib/zlib/incdir -MMD -MP -MF $@.d -o ${OBJECTDIR}/File.o File.cpp

${OBJECTDIR}/lib/zlib/incdir/gzclose.o: lib/zlib/incdir/gzclose.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/gzclose.o lib/zlib/incdir/gzclose.c

${OBJECTDIR}/lib/zlib/incdir/deflate.o: lib/zlib/incdir/deflate.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/deflate.o lib/zlib/incdir/deflate.c

${OBJECTDIR}/RandomAccessFile.o: RandomAccessFile.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Ilib/zlib/incdir -MMD -MP -MF $@.d -o ${OBJECTDIR}/RandomAccessFile.o RandomAccessFile.cpp

${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/miniunz.o: lib/zlib/incdir/contrib/minizip/miniunz.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir/contrib/minizip
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/miniunz.o lib/zlib/incdir/contrib/minizip/miniunz.c

${OBJECTDIR}/lib/zlib/incdir/contrib/infback9/infback9.o: lib/zlib/incdir/contrib/infback9/infback9.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir/contrib/infback9
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/contrib/infback9/infback9.o lib/zlib/incdir/contrib/infback9/infback9.c

${OBJECTDIR}/lib/zlib/incdir/example.o: lib/zlib/incdir/example.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/example.o lib/zlib/incdir/example.c

${OBJECTDIR}/lib/zlib/incdir/inffast.o: lib/zlib/incdir/inffast.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/inffast.o lib/zlib/incdir/inffast.c

${OBJECTDIR}/lib/zlib/incdir/crc32.o: lib/zlib/incdir/crc32.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/crc32.o lib/zlib/incdir/crc32.c

${OBJECTDIR}/lib/zlib/incdir/zutil.o: lib/zlib/incdir/zutil.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/zutil.o lib/zlib/incdir/zutil.c

${OBJECTDIR}/lib/zlib/incdir/trees.o: lib/zlib/incdir/trees.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/trees.o lib/zlib/incdir/trees.c

${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/zip.o: lib/zlib/incdir/contrib/minizip/zip.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir/contrib/minizip
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/zip.o lib/zlib/incdir/contrib/minizip/zip.c

${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/iowin32.o: lib/zlib/incdir/contrib/minizip/iowin32.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir/contrib/minizip
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/iowin32.o lib/zlib/incdir/contrib/minizip/iowin32.c

${OBJECTDIR}/IOError.o: IOError.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Ilib/zlib/incdir -MMD -MP -MF $@.d -o ${OBJECTDIR}/IOError.o IOError.cpp

${OBJECTDIR}/lib/zlib/incdir/inflate.o: lib/zlib/incdir/inflate.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/inflate.o lib/zlib/incdir/inflate.c

${OBJECTDIR}/lib/zlib/incdir/infback.o: lib/zlib/incdir/infback.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/infback.o lib/zlib/incdir/infback.c

${OBJECTDIR}/lib/zlib/incdir/contrib/masmx64/inffas8664.o: lib/zlib/incdir/contrib/masmx64/inffas8664.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir/contrib/masmx64
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/contrib/masmx64/inffas8664.o lib/zlib/incdir/contrib/masmx64/inffas8664.c

${OBJECTDIR}/lib/zlib/incdir/contrib/blast/blast.o: lib/zlib/incdir/contrib/blast/blast.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir/contrib/blast
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/contrib/blast/blast.o lib/zlib/incdir/contrib/blast/blast.c

${OBJECTDIR}/lib/zlib/incdir/compress.o: lib/zlib/incdir/compress.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/compress.o lib/zlib/incdir/compress.c

${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/ioapi.o: lib/zlib/incdir/contrib/minizip/ioapi.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir/contrib/minizip
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/contrib/minizip/ioapi.o lib/zlib/incdir/contrib/minizip/ioapi.c

${OBJECTDIR}/lib/zlib/incdir/adler32.o: lib/zlib/incdir/adler32.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/adler32.o lib/zlib/incdir/adler32.c

${OBJECTDIR}/lib/zlib/incdir/gzwrite.o: lib/zlib/incdir/gzwrite.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/gzwrite.o lib/zlib/incdir/gzwrite.c

${OBJECTDIR}/lib/zlib/incdir/contrib/puff/puff.o: lib/zlib/incdir/contrib/puff/puff.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir/contrib/puff
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/contrib/puff/puff.o lib/zlib/incdir/contrib/puff/puff.c

${OBJECTDIR}/lib/zlib/incdir/uncompr.o: lib/zlib/incdir/uncompr.c 
	${MKDIR} -p ${OBJECTDIR}/lib/zlib/incdir
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/zlib/incdir/uncompr.o lib/zlib/incdir/uncompr.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/zipsfx-cpp.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
