#  Copyright(c) 2024 Red Hat Inc.,
#  and individual contributors as indicated by the @authors tag.
#  See the copyright.txt in the distribution for a
#  full listing of individual contributors.
#
#  This is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2 of the License, or (at your option) any later version.
#
#  This software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this software in the file COPYING.LIB;
#  if not, write to the Free Software Foundation, Inc.,
#  59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
#
# @author Mladen Turk
#
CC = cl.exe
LN = link.exe
RC = rc.exe

SRCDIR = .
PROJECT = nawk

YACC   = bison -y
YFLAGS = -d
TARGET = EXE
CPU = X64
MACHINE = x64
WINVER  = 0x0A00

WIN_CFLAGS = -D_WIN32_WINNT=$(WINVER) -DWINVER=$(WINVER) -DWIN32_LEAN_AND_MEAN
WIN_CFLAGS = $(WIN_CFLAGS) -D_CRT_SECURE_NO_WARNINGS  -D_CRT_SECURE_NO_DEPRECATE

CMN_CFLAGS = -c -nologo -W3 -wd4244 -wd4267 -DUNICODE -D_UNICODE
OPT_CFLAGS = -O2 -Ob2 -GF -Gs0
CRT_CFLAGS = -MT

CFLAGS = $(CMN_CFLAGS) $(WIN_CFLAGS) $(CPU_CFLAGS) $(OPT_CFLAGS) $(CRT_CFLAGS)
!IF DEFINED(EXTRA_CFLAGS)
CFLAGS = $(CFLAGS) $(EXTRA_CFLAGS)
!ENDIF

# Linker section
LIBS = kernel32.lib

!IF DEFINED(EXTRA_LIBS)
LIBS = $(LIBS) $(EXTRA_LIBS)
!ENDIF

# Always add debugging to the linker
CMN_LFLAGS = /nologo /DEBUG /INCREMENTAL:NO /OPT:REF /NODEFAULTLIB:libucrt.lib /DEFAULTLIB:ucrt.lib
LFLAGS = $(CMN_LFLAGS) /SUBSYSTEM:CONSOLE /MACHINE:$(MACHINE)

# Resource compiler flags

BASE_RCFLAGS = /nologo /l 0x409 /n
BASE_RCFLAGS = $(BASE_RCFLAGS) -d NDEBUG -d WINVER=$(WINVER) -d _WIN32_WINNT=$(WINVER) -d WIN32_LEAN_AND_MEAN
RCFLAGS = $(BASE_RCFLAGS) $(EXTRA_RFLAGS)

WORKDIR = $(CPU)_RELEASE_$(TARGET)

PDBFLAGS = -Fo$(WORKDIR)\ -Fd$(WORKDIR)\$(PROJECT)-src

OBJECTS = \
	$(WORKDIR)\b.obj \
	$(WORKDIR)\main.obj \
	$(WORKDIR)\parse.obj \
	$(WORKDIR)\proctab.obj \
	$(WORKDIR)\tran.obj \
	$(WORKDIR)\lib.obj \
	$(WORKDIR)\run.obj \
	$(WORKDIR)\missing95.obj \
	$(WORKDIR)\ytab.obj \
	$(WORKDIR)\lex.obj

BUILDRES = \
	$(WORKDIR)\nawk.res


OBJDEPS = $(SRCDIR)\ytab.h

BUILDLOC = $(PREFIX)\bin
BUILDEXE = $(WORKDIR)\$(PROJECT).exe
BUILDPDB = $(WORKDIR)\$(PROJECT).pdb
BUILDRES = $(WORKDIR)\$(PROJECT).res
BUILDMAN = $(BUILDEXE).manifest

all : $(WORKDIR) $(BUILDEXE)

$(WORKDIR):
	@-md $(WORKDIR) 2>NUL

{$(SRCDIR)}.c{$(WORKDIR)}.obj:
	$(CC) $(CFLAGS) $(INCLUDES) $(PDBFLAGS) $<

{$(SRCDIR)}.rc{$(WORKDIR)}.res:
	$(RC) $(CRFLAGS) -fo $@ $<

$(BUILDEXE): $(WORKDIR) $(OBJECTS) $(BUILDRES)
	$(LN) $(LFLAGS) $(OBJECTS) $(BUILDRES) $(LIBS) /pdb:$(BUILDPDB) /out:$(BUILDEXE)

clean:
	@-rd /S /Q $(WORKDIR) 2>NUL

install: $(BUILDLOC) $(WORKDIR) $(BUILDEXE)
	@xcopy "$(WORKDIR)\*.exe" "$(BUILDLOC)" /Y /Q
