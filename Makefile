########################################################################
#                                                                      
#              --- CAEN VME V1718 interface makefile  ---                   
#                                                                      
#   For testing connection of PC to V1718 via USB 2.0 interface                                           
#                                                                      
#   Created  :                                            
#                                                                      
#   Hyunmin Yang                                                 
#                                                                      
########################################################################

EXE	= daq.exe

CC	=	g++ -std=c++11

COPTS	=	-fPIC -DLINUX -Wall 
#COPTS	=	-g -fPIC -DLINUX -Wall 

FLAGS	=	-Wall -s $(COPTS)
#FLAGS	=	-Wall

# RTLIBS    = $(shell root-config --libs)
# RTINCLUDE = $(shell root-config --cflags)

INCLUDE	= -I. -I/usr/include/  

CCFLAGS = $(FLAGS) $(INCLUDE) $(RTINCLUDE)
SOURCEDIR	= .

LIBS	= -lCAENVME -lc -lm -lstdc++ $(RTLIBS)

OBJS	= daq.o CAENVMEV1718.o CAENVMEV1290N.o CAENVMEV792N.o
#########################################################################

all	:	$(EXE)

clean	:
		/bin/rm -f $(OBJS) $(EXE)

$(EXE)	:	$(OBJS)
		/bin/rm -f $(EXE)
		$(CC) $(CCFLAGS) -o $(EXE) $(OBJS) $(LIBS)

%.o :	$(SOURCEDIR)/%.c
		$(CC) $(CCFLAGS) $(INCLUDE) -c -o $@ $<
%.o	:	%.c
		$(CC) $(CCFLAGS) $(INCLUDE) -c -o $@ $<

