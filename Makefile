LIBUSB_INC = /usr/include/libusb-1.0
LIBUSB_LIB = /usr/lib64

FILENAME1 = runUOSDAQ3
FILENAME2 = getPedestal

EXE1	=	$(FILENAME1).exe
EXE2	=	$(FILENAME2).exe

CC	=	g++

COPTS	=	-fPIC -Wall

FLAGS	=	-shared -Wall,-soname -nostartfiles -s

LDFLAGS =        -O2 -Wall 

OutPutOpt = -o 

LIBS	=	-L$(LIBUSB_LIB) -lusb-1.0 -L$(NKHOME)/lib -lNoticeUOSDAQ3 -lm

INCLUDEDIR =	-I$(NKHOME)/include -I$(LIBUSB_INC)

INSTALLDIR     =	$(NKHOME)/bin
INSTALLINCLUDE =	$(NKHOME)/include

OBJS1	=	$(FILENAME1).o
OBJS2	=	$(FILENAME2).o

INCLUDES =	 


#########################################################################

all	:	$(EXE1) $(EXE2)
clean	:
		/bin/rm -f $(OBJS1) $(EXE1) $(OBJS2) $(EXE2)

install	:	$(EXE1) $(EXE2)
		#/bin/rm -f $(INSTALLDIR)/$(EXE)
		install $(EXE1) $(INSTALLDIR); install $(EXE2) $(INSTALLDIR)

$(EXE1):      $(OBJS1)
		$(CC) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		@echo "$@ done"

$(EXE2):      $(OBJS2)
		$(CC) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		@echo "$@ done"

$(OBJS1)	:	$(INCLUDES) 
$(OBJS2)	:	$(INCLUDES) 

%.o	:	%.c
		$(CC) $(COPTS) $(INCLUDEDIR) -c -o $@ $<

