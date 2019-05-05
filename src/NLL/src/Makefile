# Makefile for NonLinLoc software package
#
# Invocation:
#     Solaris: make all
#     Linux:   make -R all


#  IMPORTANT: change here to set the directory for binary executable files
ifdef MYBIN
BINDIR=${MYBIN}
else
BINDIR=/home/efrain/Universidad/Doctorado/
endif

CC=gcc

# normal compile
CFLAGS=-O3 -Wall
#
# profile
#CCFLAGS=-O3 -pg -Wall
#
# debug - gdb, valgrind, ...
#CCFLAGS=-g -O0 -Wall
# valgrind --leak-check=yes  NLLoc <nll_controlfile>
# valgrind --leak-check=full --show-reachable=yes NLLoc <nll_controlfile>


# Custom builds
#
# GNU
GNU_SOURCE=-D _GNU_SOURCE
# IMPORTANT: use following if the compiler you are using does not support GNU extensions such as:
#   fmemopen, open_memstream -  open memory as stream (you will not be able to pass observations file lines as parameters to NLLoc()
#GNU_SOURCE=
#
#
# Custom ETH functions
CUSTOM=
#CUSTOM=ETH


# Linux
TIME3D_CCFLAGS=


# --------------------------------------------------------------------------
# Top level variables and rules
#
GRID_LIB_OBJS=GridLib.o util.o geo.o octtree.o nrutil.o nrmatrix.o ran1.o map_project.o
NLLOC_LIB_OBJS=calc_crust_corr.o velmod.o GridMemLib.o phaselist.o loclist.o

all : NLLoc Vel2Grid Grid2Time Grid2GMT LocSum Time2EQ PhsAssoc hypoe2hyp fpfit2hyp oct2grid
distrib : NLLoc Vel2Grid Grid2Time Grid2GMT LocSum Time2EQ PhsAssoc hypoe2hyp fpfit2hyp oct2grid


# --------------------------------------------------------------------------
# NLLoc
#
OBJS0=NLLocLib.o ${GRID_LIB_OBJS} ${NLLOC_LIB_OBJS}
OPTIONS0= $(GNU_SOURCE)
# Custom builds
ifeq ($(CUSTOM), ETH)
	OBJS1=$(OBJS0) eth_functions.o get_region_name_nr.o magnitude.o new_sedlib.o complex.o
	OPTIONS=$(OPTIONS0) -D CUSTOM_ETH
else
	OBJS1=$(OBJS0)
	OPTIONS=$(OPTIONS0)
endif
PVER=1
# --------------------------------------------------------------------------
# NLLoc general
NLLoc${PVER}.o : NLLoc${PVER}.c NLLocLib.h GridLib.h GridMemLib.h nrutil.h
	${CC} -c ${CFLAGS}  NLLoc${PVER}.c  $(OPTIONS)
# --------------------------------------------------------------------------
# NLLoc program (NLLoc)
NLLoc : ${BINDIR}/NLLoc
${BINDIR}/NLLoc : NLLoc_main.o NLLoc${PVER}.o ${OBJS1}
	${CC} NLLoc_main.o NLLoc${PVER}.o ${OBJS1} ${CFLAGS} -o ${BINDIR}/NLLoc -lm
NLLoc_main.o : NLLoc_main.c NLLocLib.h GridLib.h GridMemLib.h nrutil.h
	${CC} -c ${CFLAGS}  NLLoc_main.c  $(OPTIONS)
# --------------------------------------------------------------------------
# NLLoc function test (NLLoc_func_test)
NLLoc_func_test : ${BINDIR}/NLLoc_func_test
${BINDIR}/NLLoc_func_test : NLLoc_func_test.o NLLoc${PVER}.o ${OBJS1}
	gcc NLLoc_func_test.o NLLoc${PVER}.o ${OBJS1} ${CFLAGS} -o ${BINDIR}/NLLoc_func_test -lm
NLLoc_func_test.o : NLLoc_func_test.c NLLocLib.h GridLib.h GridMemLib.h ${INCLUDE_DIR}nrutil.h
	gcc -c ${CFLAGS}  NLLoc_func_test.c  $(OPTIONS)
# --------------------------------------------------------------------------



# --------------------------------------------------------------------------
# NLDiffLoc
#
OBJS100=NLLocLib.o ${GRID_LIB_OBJS} calc_crust_corr.o velmod.o GridMemLib.o
NLDiffLoc : ${BINDIR}/NLDiffLoc
${BINDIR}/NLDiffLoc : NLDiffLoc.o ${OBJS100}
	${CC} NLDiffLoc.o ${OBJS100} ${CFLAGS} -o ${BINDIR}/NLDiffLoc -lm
NLDiffLoc.o : NLDiffLoc.c NLDiffLoc.h NLLocLib.h GridLib.h GridMemLib.h nrutil.h
	${CC} -c ${CFLAGS}  NLDiffLoc.c  $(OPTIONS)
# --------------------------------------------------------------------------



# --------------------------------------------------------------------------
# Vel2Grid
#
OBJS2=${GRID_LIB_OBJS} velmod.o
PVER=1
Vel2Grid : ${BINDIR}/Vel2Grid
${BINDIR}/Vel2Grid : Vel2Grid${PVER}.o ${OBJS2}
	${CC} Vel2Grid${PVER}.o  ${OBJS2} ${CFLAGS} -o ${BINDIR}/Vel2Grid -lm
Vel2Grid${PVER}.o : Vel2Grid${PVER}.c GridLib.h
# --------------------------------------------------------------------------



# --------------------------------------------------------------------------
# Grid2Time
#
OBJS3=${GRID_LIB_OBJS} Time_3d.o
PVER=1
Grid2Time : ${BINDIR}/Grid2Time
${BINDIR}/Grid2Time : Grid2Time${PVER}.o ${OBJS3}
	${CC} Grid2Time${PVER}.o ${OBJS3} ${CFLAGS}  \
		-o ${BINDIR}/Grid2Time -lm
Grid2Time${PVER}.o : Grid2Time${PVER}.c GridLib.h
	${CC} -c Grid2Time${PVER}.c
Time_3d.o : Time_3d.c
	${CC} ${TIME3D_CCFLAGS} -c -DNO_IEEE_PROTOCOL Time_3d.c
#	${CC} -c -DNO_IEEE_PROTOCOL Time_3d.c
# --------------------------------------------------------------------------



# --------------------------------------------------------------------------
# Grid2GMT
#
OBJS4=Grid2GMT.o ${GRID_LIB_OBJS} vector.o GridGraphLib.o
Grid2GMT : ${BINDIR}/Grid2GMT
${BINDIR}/Grid2GMT : ${OBJS4}
	${CC} ${OBJS4} ${CFLAGS} -o ${BINDIR}/Grid2GMT -lm
Grid2GMT.o : Grid2GMT.c GridLib.h GridGraphLib.h
#
# --------------------------------------------------------------------------



# --------------------------------------------------------------------------
# LocSum
#
OBJS5=LocSum.o ${GRID_LIB_OBJS}
LocSum : ${BINDIR}/LocSum
${BINDIR}/LocSum : ${OBJS5}
	${CC} ${OBJS5} ${CFLAGS} -o ${BINDIR}/LocSum -lm
LocSum.o : LocSum.c GridLib.h
# --------------------------------------------------------------------------



# --------------------------------------------------------------------------
# Time2EQ
#
OBJS6=${GRID_LIB_OBJS}
PVER=1
Time2EQ : ${BINDIR}/Time2EQ
${BINDIR}/Time2EQ : Time2EQ${PVER}.o ${OBJS6}
	${CC} Time2EQ${PVER}.o ${OBJS6} ${CFLAGS} -o ${BINDIR}/Time2EQ -lm
Time2EQ${PVER}.o : Time2EQ${PVER}.c GridLib.h ran1.h
# --------------------------------------------------------------------------



# --------------------------------------------------------------------------
# hypoe2hyp
#
OBJS7=hypoe2hyp.o ${GRID_LIB_OBJS}
hypoe2hyp : ${BINDIR}/hypoe2hyp
${BINDIR}/hypoe2hyp : ${OBJS7}
	${CC} ${OBJS7} ${CFLAGS} -o ${BINDIR}/hypoe2hyp -lm
hypoe2hyp.o : hypoe2hyp.c GridLib.h
# --------------------------------------------------------------------------



# --------------------------------------------------------------------------
# fpfit2hyp
#
OBJS8=fpfit2hyp.o ${GRID_LIB_OBJS}
fpfit2hyp : ${BINDIR}/fpfit2hyp
${BINDIR}/fpfit2hyp : ${OBJS8}
	${CC} ${OBJS8} ${CFLAGS} -o ${BINDIR}/fpfit2hyp -lm
fpfit2hyp.o : fpfit2hyp.c GridLib.h
# --------------------------------------------------------------------------



# --------------------------------------------------------------------------
# PhsAssoc
#
OBJS9=PhsAssoc.o ${GRID_LIB_OBJS} calc_crust_corr.o
PhsAssoc : ${BINDIR}/PhsAssoc
${BINDIR}/PhsAssoc : ${OBJS9}
	${CC} ${OBJS9} ${CFLAGS} -o ${BINDIR}/PhsAssoc -lm
PhsAssoc.o : PhsAssoc.c GridLib.h
# --------------------------------------------------------------------------



# --------------------------------------------------------------------------
# oct2grid
#
OBJS10=oct2grid.o ${GRID_LIB_OBJS}
oct2grid : ${BINDIR}/oct2grid
${BINDIR}/oct2grid : ${OBJS10}
	${CC} ${OBJS10} ${CFLAGS} -o ${BINDIR}/oct2grid -lm
oct2grid.o : oct2grid.c GridLib.h
# --------------------------------------------------------------------------




# --------------------------------------------------------------------------
# ttime_func_test
#
OBJS11=ttime_func_test.o ${GRID_LIB_OBJS}
ttime_func_test : ${BINDIR}/ttime_func_test
${BINDIR}/ttime_func_test : ${OBJS11}
	${CC} ${OBJS11} ${CFLAGS} -o ${BINDIR}/ttime_func_test -lm
ttime_func_test.o : ttime_func_test.c GridLib.h
# --------------------------------------------------------------------------



# --------------------------------------------------------------------------
# mag_func_test
#
OBJS12=mag_func_test.o NLLocLib.o ${GRID_LIB_OBJS} ${NLLOC_LIB_OBJS}
mag_func_test : ${BINDIR}/mag_func_test
${BINDIR}/mag_func_test : ${OBJS12}
	${CC} ${OBJS12} ${CFLAGS} -o ${BINDIR}/mag_func_test -lm
mag_func_test.o : mag_func_test.c NLLocLib.h GridLib.h
# --------------------------------------------------------------------------



# --------------------------------------------------------------------------
# General compile rule
#

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

#
# --------------------------------------------------------------------------



# --------------------------------------------------------------------------
# NLL special compile rule
#

NLLocLib.o : NLLocLib.c NLLocLib.h GridLib.h

GridLib.o : GridLib.c GridLib.h  geometry.h util.h geo.h octtree.h nrutil.h nrmatrix.h ran1.h map_project.h

GridMemLib.o : GridMemLib.c GridMemLib.h  GridLib.h

GridGraphLib.o : GridGraphLib.c GridLib.h GridGraphLib.h

phaselist.o : phaselist.c phaseloclist.h  GridLib.h

loclist.o : loclist.c phaseloclist.h  GridLib.h

calc_crust_corr.o :   GridLib.h calc_crust_corr.c crust_corr_model.h  crust_type.h  crust_type_key.h

#
# --------------------------------------------------------------------------



# --------------------------------------------------------------------------
# Custom Librarires
#

# ETH ----------------------------------------------------------------------
ETH_DIR=custom_eth/
eth_functions.o : $(ETH_DIR)eth_functions.c $(ETH_DIR)eth_functions.h $(ETH_DIR)new_sedlib.h $(ETH_DIR)complex.h
	${CC} -c ${CFLAGS}  $(ETH_DIR)eth_functions.c
get_region_name_nr.o : $(ETH_DIR)get_region_name_nr.c $(ETH_DIR)get_region_name_nr.h $(ETH_DIR)new_sedlib.h
	${CC} -c ${CFLAGS}  $(ETH_DIR)get_region_name_nr.c
magnitude.o : $(ETH_DIR)magnitude.c $(ETH_DIR)new_sedlib.h $(ETH_DIR)complex.h
	${CC} -c ${CFLAGS}  $(ETH_DIR)magnitude.c
new_sedlib.o : $(ETH_DIR)new_sedlib.c $(ETH_DIR)new_sedlib.h
	${CC} -c ${CFLAGS}  $(ETH_DIR)new_sedlib.c  -D UNIX
complex.o : $(ETH_DIR)complex.c $(ETH_DIR)complex.h
	${CC} -c ${CFLAGS}  $(ETH_DIR)complex.c  -D UNIX
#gselib.o : $(ETH_DIR)gselib.c $(ETH_DIR)gselib.h
#	${CC} -c ${CFLAGS}  $(ETH_DIR)gselib.c  -D UNIX

clean_eth :
	rm eth_functions.o get_region_name_nr.o new_sedlib.o complex.o magnitude.o

#
# --------------------------------------------------------------------------



# --------------------------------------------------------------------------
# Misc
#

clean :
	rm *.o

#
# --------------------------------------------------------------------------
