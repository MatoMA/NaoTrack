#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS = -Wall -fexceptions
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = 

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/NaoTrack

INC_POSITIONSERVER = $(INC)
CFLAGS_POSITIONSERVER = $(CFLAGS) -Wall -ansi -pedantic -O3 -std=c++0x
RESINC_POSITIONSERVER = $(RESINC)
RCFLAGS_POSITIONSERVER = $(RCFLAGS)
LIBDIR_POSITIONSERVER = $(LIBDIR)
LIB_POSITIONSERVER = $(LIB)
LDFLAGS_POSITIONSERVER = $(LDFLAGS) -lboost_thread-mt -lboost_system-mt -lpthread
OBJDIR_POSITIONSERVER = obj/PositionServer
DEP_POSITIONSERVER = 
OUT_POSITIONSERVER = bin/PositionServer/position_server

INC_FAKESOURCE = $(INC) -Ithird_party/local/include
CFLAGS_FAKESOURCE = $(CFLAGS) -Wall -ansi -O3 -std=c++0x
RESINC_FAKESOURCE = $(RESINC)
RCFLAGS_FAKESOURCE = $(RCFLAGS)
LIBDIR_FAKESOURCE = $(LIBDIR)
LIB_FAKESOURCE = $(LIB)third_party/local/lib/libglog.a
LDFLAGS_FAKESOURCE = $(LDFLAGS) -lpthread -lboost_thread-mt -lboost_system-mt
OBJDIR_FAKESOURCE = obj/FakeSource
DEP_FAKESOURCE = 
OUT_FAKESOURCE = bin/FakeSource/fakesource

INC_DETECTIONTEST = $(INC) -Ithird_party/local/include
CFLAGS_DETECTIONTEST = $(CFLAGS) -g -Wall -ansi -std=c++0x `pkg-config --cflags mirage axisPTZ`
RESINC_DETECTIONTEST = $(RESINC)
RCFLAGS_DETECTIONTEST = $(RCFLAGS)
LIBDIR_DETECTIONTEST = $(LIBDIR) -Lthird_party/local/lib
LIB_DETECTIONTEST = $(LIB)
LDFLAGS_DETECTIONTEST = $(LDFLAGS) -lpthread -lglog `pkg-config --libs mirage axisPTZ`
OBJDIR_DETECTIONTEST = obj/DetectionTest
DEP_DETECTIONTEST = 
OUT_DETECTIONTEST = bin/DetectionTest/detection_test

INC_DATABASEGENERATOR = $(INC) -Ithird_party/local/include
CFLAGS_DATABASEGENERATOR = $(CFLAGS) -O2 -g -Wall -ansi -std=c++0x `pkg-config --cflags mirage axisPTZ`
RESINC_DATABASEGENERATOR = $(RESINC)
RCFLAGS_DATABASEGENERATOR = $(RCFLAGS)
LIBDIR_DATABASEGENERATOR = $(LIBDIR) -Lthird_party/local/lib
LIB_DATABASEGENERATOR = $(LIB)
LDFLAGS_DATABASEGENERATOR = $(LDFLAGS) -s -lpthread -lglog -lboost_system-mt -lboost_filesystem `pkg-config --libs mirage axisPTZ`
OBJDIR_DATABASEGENERATOR = obj/DatabaseGenerator
DEP_DATABASEGENERATOR = 
OUT_DATABASEGENERATOR = bin/DatabaseGenerator/database_generator

OBJ_DEBUG = $(OBJDIR_DEBUG)/src/Position/PositionServer/position-server.o

OBJ_POSITIONSERVER = $(OBJDIR_POSITIONSERVER)/src/Position/PositionServer/position-server.o

OBJ_FAKESOURCE = $(OBJDIR_FAKESOURCE)/src/Position/Fakesource/fakesource.o

OBJ_DETECTIONTEST = $(OBJDIR_DETECTIONTEST)/src/Detection/DetectionTest.o $(OBJDIR_DETECTIONTEST)/src/Detection/FrameCapturer.o $(OBJDIR_DETECTIONTEST)/src/Detection/FrameProcessor.o

OBJ_DATABASEGENERATOR = $(OBJDIR_DATABASEGENERATOR)/src/Detection/DatabaseGenerator.o $(OBJDIR_DATABASEGENERATOR)/src/Detection/FrameCapturer.o $(OBJDIR_DATABASEGENERATOR)/src/Detection/FrameProcessor.o

all: debug positionserver fakesource detectiontest databasegenerator

clean: clean_debug clean_positionserver clean_fakesource clean_detectiontest clean_databasegenerator

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG)/src/Position/PositionServer || mkdir -p $(OBJDIR_DEBUG)/src/Position/PositionServer

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/src/Position/PositionServer/position-server.o: src/Position/PositionServer/position-server.cc
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Position/PositionServer/position-server.cc -o $(OBJDIR_DEBUG)/src/Position/PositionServer/position-server.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)/src/Position/PositionServer

before_positionserver: 
	test -d bin/PositionServer || mkdir -p bin/PositionServer
	test -d $(OBJDIR_POSITIONSERVER)/src/Position/PositionServer || mkdir -p $(OBJDIR_POSITIONSERVER)/src/Position/PositionServer

after_positionserver: 

positionserver: before_positionserver out_positionserver after_positionserver

out_positionserver: before_positionserver $(OBJ_POSITIONSERVER) $(DEP_POSITIONSERVER)
	$(LD) $(LIBDIR_POSITIONSERVER) -o $(OUT_POSITIONSERVER) $(OBJ_POSITIONSERVER)  $(LDFLAGS_POSITIONSERVER) $(LIB_POSITIONSERVER)

$(OBJDIR_POSITIONSERVER)/src/Position/PositionServer/position-server.o: src/Position/PositionServer/position-server.cc
	$(CC) $(CFLAGS_POSITIONSERVER) $(INC_POSITIONSERVER) -c src/Position/PositionServer/position-server.cc -o $(OBJDIR_POSITIONSERVER)/src/Position/PositionServer/position-server.o

clean_positionserver: 
	rm -f $(OBJ_POSITIONSERVER) $(OUT_POSITIONSERVER)
	rm -rf bin/PositionServer
	rm -rf $(OBJDIR_POSITIONSERVER)/src/Position/PositionServer

before_fakesource: 
	test -d bin/FakeSource || mkdir -p bin/FakeSource
	test -d $(OBJDIR_FAKESOURCE)/src/Position/Fakesource || mkdir -p $(OBJDIR_FAKESOURCE)/src/Position/Fakesource

after_fakesource: 

fakesource: before_fakesource out_fakesource after_fakesource

out_fakesource: before_fakesource $(OBJ_FAKESOURCE) $(DEP_FAKESOURCE)
	$(LD) $(LIBDIR_FAKESOURCE) -o $(OUT_FAKESOURCE) $(OBJ_FAKESOURCE)  $(LDFLAGS_FAKESOURCE) $(LIB_FAKESOURCE)

$(OBJDIR_FAKESOURCE)/src/Position/Fakesource/fakesource.o: src/Position/Fakesource/fakesource.cpp
	$(CXX) $(CFLAGS_FAKESOURCE) $(INC_FAKESOURCE) -c src/Position/Fakesource/fakesource.cpp -o $(OBJDIR_FAKESOURCE)/src/Position/Fakesource/fakesource.o

clean_fakesource: 
	rm -f $(OBJ_FAKESOURCE) $(OUT_FAKESOURCE)
	rm -rf bin/FakeSource
	rm -rf $(OBJDIR_FAKESOURCE)/src/Position/Fakesource

before_detectiontest: 
	test -d bin/DetectionTest || mkdir -p bin/DetectionTest
	test -d $(OBJDIR_DETECTIONTEST)/src/Detection || mkdir -p $(OBJDIR_DETECTIONTEST)/src/Detection

after_detectiontest: 

detectiontest: before_detectiontest out_detectiontest after_detectiontest

out_detectiontest: before_detectiontest $(OBJ_DETECTIONTEST) $(DEP_DETECTIONTEST)
	$(LD) $(LIBDIR_DETECTIONTEST) -o $(OUT_DETECTIONTEST) $(OBJ_DETECTIONTEST)  $(LDFLAGS_DETECTIONTEST) $(LIB_DETECTIONTEST)

$(OBJDIR_DETECTIONTEST)/src/Detection/DetectionTest.o: src/Detection/DetectionTest.cpp
	$(CXX) $(CFLAGS_DETECTIONTEST) $(INC_DETECTIONTEST) -c src/Detection/DetectionTest.cpp -o $(OBJDIR_DETECTIONTEST)/src/Detection/DetectionTest.o

$(OBJDIR_DETECTIONTEST)/src/Detection/FrameCapturer.o: src/Detection/FrameCapturer.cpp
	$(CXX) $(CFLAGS_DETECTIONTEST) $(INC_DETECTIONTEST) -c src/Detection/FrameCapturer.cpp -o $(OBJDIR_DETECTIONTEST)/src/Detection/FrameCapturer.o

$(OBJDIR_DETECTIONTEST)/src/Detection/FrameProcessor.o: src/Detection/FrameProcessor.cpp
	$(CXX) $(CFLAGS_DETECTIONTEST) $(INC_DETECTIONTEST) -c src/Detection/FrameProcessor.cpp -o $(OBJDIR_DETECTIONTEST)/src/Detection/FrameProcessor.o

clean_detectiontest: 
	rm -f $(OBJ_DETECTIONTEST) $(OUT_DETECTIONTEST)
	rm -rf bin/DetectionTest
	rm -rf $(OBJDIR_DETECTIONTEST)/src/Detection

before_databasegenerator: 
	test -d bin/DatabaseGenerator || mkdir -p bin/DatabaseGenerator
	test -d $(OBJDIR_DATABASEGENERATOR)/src/Detection || mkdir -p $(OBJDIR_DATABASEGENERATOR)/src/Detection

after_databasegenerator: 

databasegenerator: before_databasegenerator out_databasegenerator after_databasegenerator

out_databasegenerator: before_databasegenerator $(OBJ_DATABASEGENERATOR) $(DEP_DATABASEGENERATOR)
	$(LD) $(LIBDIR_DATABASEGENERATOR) -o $(OUT_DATABASEGENERATOR) $(OBJ_DATABASEGENERATOR)  $(LDFLAGS_DATABASEGENERATOR) $(LIB_DATABASEGENERATOR)

$(OBJDIR_DATABASEGENERATOR)/src/Detection/DatabaseGenerator.o: src/Detection/DatabaseGenerator.cpp
	$(CXX) $(CFLAGS_DATABASEGENERATOR) $(INC_DATABASEGENERATOR) -c src/Detection/DatabaseGenerator.cpp -o $(OBJDIR_DATABASEGENERATOR)/src/Detection/DatabaseGenerator.o

$(OBJDIR_DATABASEGENERATOR)/src/Detection/FrameCapturer.o: src/Detection/FrameCapturer.cpp
	$(CXX) $(CFLAGS_DATABASEGENERATOR) $(INC_DATABASEGENERATOR) -c src/Detection/FrameCapturer.cpp -o $(OBJDIR_DATABASEGENERATOR)/src/Detection/FrameCapturer.o

$(OBJDIR_DATABASEGENERATOR)/src/Detection/FrameProcessor.o: src/Detection/FrameProcessor.cpp
	$(CXX) $(CFLAGS_DATABASEGENERATOR) $(INC_DATABASEGENERATOR) -c src/Detection/FrameProcessor.cpp -o $(OBJDIR_DATABASEGENERATOR)/src/Detection/FrameProcessor.o

clean_databasegenerator: 
	rm -f $(OBJ_DATABASEGENERATOR) $(OUT_DATABASEGENERATOR)
	rm -rf bin/DatabaseGenerator
	rm -rf $(OBJDIR_DATABASEGENERATOR)/src/Detection

.PHONY: before_debug after_debug clean_debug before_positionserver after_positionserver clean_positionserver before_fakesource after_fakesource clean_fakesource before_detectiontest after_detectiontest clean_detectiontest before_databasegenerator after_databasegenerator clean_databasegenerator
