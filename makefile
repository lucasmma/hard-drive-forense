CFLAGS=/W4 /EHsc /std:c++17 /Iinclude /utf-8 /nologo

LIB_NAMES=sysfiles utils fat32
LIBS=$(patsubst %,obj\\%.obj,$(LIB_NAMES))

all: dirs main

main: src\main.cpp $(LIBS)
    cl $(CFLAGS) /DDISKEXP /Fo:obj\ /Fe:main.exe $?

obj\sysfiles.obj::            src\$(@B).cpp include\$(@B).h
    cl $(CFLAGS) /c /Fo:obj\ src\$(@B).cpp

obj\fat32.obj::      src\$(@B).cpp include\$(@B).h
    cl $(CFLAGS) /c /Fo:obj\ src\$(@B).cpp

obj\utils.obj::      src\$(@B).cpp include\$(@B).h
    cl $(CFLAGS) /c /Fo:obj\ src\$(@B).cpp

dirs:
    (IF NOT EXIST obj (MKDIR obj))

clean:
    FOR %I IN (obj\*) DO @((echo Removing file %I) & (del %I))
    IF EXIST obj (rmdir obj)
.PHONY: clean