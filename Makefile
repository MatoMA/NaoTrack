#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.135                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS =  -Wall -fexceptions
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = 

INC_DEBUG =  $(INC)
CFLAGS_DEBUG =  $(CFLAGS) -g
RESINC_DEBUG =  $(RESINC)
RCFLAGS_DEBUG =  $(RCFLAGS)
LIBDIR_DEBUG =  $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG =  $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/NaoTrack

INC_POSITIONSERVER =  $(INC)
CFLAGS_POSITIONSERVER =  $(CFLAGS) -Wall -ansi -pedantic -O3 -std=c++0x
RESINC_POSITIONSERVER =  $(RESINC)
RCFLAGS_POSITIONSERVER =  $(RCFLAGS)
LIBDIR_POSITIONSERVER =  $(LIBDIR)
LIB_POSITIONSERVER = $(LIB)
LDFLAGS_POSITIONSERVER =  $(LDFLAGS) -lboost_thread-mt -lboost_system-mt -lpthread
OBJDIR_POSITIONSERVER = obj/PositionServer
DEP_POSITIONSERVER = 
OUT_POSITIONSERVER = bin/PositionServer/position_server

INC_FAKESOURCE =  $(INC) -Ithird_party/local/include
CFLAGS_FAKESOURCE =  $(CFLAGS) -Wall -ansi -O3 -std=c++0x
RESINC_FAKESOURCE =  $(RESINC)
RCFLAGS_FAKESOURCE =  $(RCFLAGS)
LIBDIR_FAKESOURCE =  $(LIBDIR)
LIB_FAKESOURCE = $(LIB) third_party/local/lib/libglog.a
LDFLAGS_FAKESOURCE =  $(LDFLAGS) -lpthread -lboost_thread-mt -lboost_system-mt
OBJDIR_FAKESOURCE = obj/FakeSource
DEP_FAKESOURCE = 
OUT_FAKESOURCE = bin/FakeSource/fakesource

INC_DETECTIONTEST =  $(INC) -Ithird_party/local/include
CFLAGS_DETECTIONTEST =  $(CFLAGS) -g -Wall -ansi -std=c++0x `pkg-config --cflags mirage axisPTZ`
RESINC_DETECTIONTEST =  $(RESINC)
RCFLAGS_DETECTIONTEST =  $(RCFLAGS)
LIBDIR_DETECTIONTEST =  $(LIBDIR) -Lthird_party/local/lib
LIB_DETECTIONTEST = $(LIB)
LDFLAGS_DETECTIONTEST =  $(LDFLAGS) -lpthread -lglog `pkg-config --libs mirage axisPTZ`
OBJDIR_DETECTIONTEST = obj/DetectionTest
DEP_DETECTIONTEST = 
OUT_DETECTIONTEST = bin/DetectionTest/detection_test

INC_DATABASEGENERATOR =  $(INC) -Ithird_party/local/include
CFLAGS_DATABASEGENERATOR =  $(CFLAGS) -O2 -g -Wall -ansi -std=c++0x `pkg-config --cflags mirage axisPTZ`
RESINC_DATABASEGENERATOR =  $(RESINC)
RCFLAGS_DATABASEGENERATOR =  $(RCFLAGS)
LIBDIR_DATABASEGENERATOR =  $(LIBDIR) -Lthird_party/local/lib
LIB_DATABASEGENERATOR = $(LIB)
LDFLAGS_DATABASEGENERATOR =  $(LDFLAGS) -s -lpthread -lglog -lboost_system-mt -lboost_filesystem `pkg-config --libs mirage axisPTZ`
OBJDIR_DATABASEGENERATOR = obj/DatabaseGenerator
DEP_DATABASEGENERATOR = 
OUT_DATABASEGENERATOR = bin/DatabaseGenerator/database_generator

INC_SVMPREDICTORTEST =  $(INC) -Ithird_party/local/include
CFLAGS_SVMPREDICTORTEST =  $(CFLAGS) -g -Wall -ansi -std=c++0x `pkg-config --cflags ml-libsvm`
RESINC_SVMPREDICTORTEST =  $(RESINC)
RCFLAGS_SVMPREDICTORTEST =  $(RCFLAGS)
LIBDIR_SVMPREDICTORTEST =  $(LIBDIR) -Lthird_party/local/lib
LIB_SVMPREDICTORTEST = $(LIB)
LDFLAGS_SVMPREDICTORTEST =  $(LDFLAGS) -lglog `pkg-config --libs ml-libsvm`
OBJDIR_SVMPREDICTORTEST = obj/SVMPredictorTest
DEP_SVMPREDICTORTEST = 
OUT_SVMPREDICTORTEST = bin/SVMPredictorTest/svm_predictor_test

INC_DETECTOR =  $(INC) -Ithird_party/local/include
CFLAGS_DETECTOR =  $(CFLAGS) -g -Wall -ansi -std=c++0x `pkg-config --cflags mirage axisPTZ ml-libsvm`
RESINC_DETECTOR =  $(RESINC)
RCFLAGS_DETECTOR =  $(RCFLAGS)
LIBDIR_DETECTOR =  $(LIBDIR) -Lthird_party/local/lib
LIB_DETECTOR = $(LIB)
LDFLAGS_DETECTOR =  $(LDFLAGS) -lpthread -lpthread -lboost_thread-mt -lboost_system-mt -lboost_filesystem -lglog `pkg-config --libs mirage axisPTZ ml-libsvm`
OBJDIR_DETECTOR = obj/Detector
DEP_DETECTOR = 
OUT_DETECTOR = bin/Detector/detector

OBJ_DEBUG = $(OBJDIR_DEBUG)/src/Position/PositionServer/position-server.o

OBJ_POSITIONSERVER = $(OBJDIR_POSITIONSERVER)/src/Position/PositionServer/position-server.o

OBJ_FAKESOURCE = $(OBJDIR_FAKESOURCE)/src/Position/Fakesource/fakesource.o

OBJ_DETECTIONTEST = $(OBJDIR_DETECTIONTEST)/src/Detection/DetectionTest.o $(OBJDIR_DETECTIONTEST)/src/Detection/FrameCapturer.o $(OBJDIR_DETECTIONTEST)/src/Detection/FrameProcessor.o

OBJ_DATABASEGENERATOR = $(OBJDIR_DATABASEGENERATOR)/src/Detection/DatabaseGenerator.o $(OBJDIR_DATABASEGENERATOR)/src/Detection/FrameCapturer.o $(OBJDIR_DATABASEGENERATOR)/src/Detection/FrameProcessor.o

OBJ_SVMPREDICTORTEST = $(OBJDIR_SVMPREDICTORTEST)/src/Detection/SVMPredictor.o $(OBJDIR_SVMPREDICTORTEST)/src/Detection/SVMPredictorTest.o

OBJ_DETECTOR = $(OBJDIR_DETECTOR)/src/Detection/Detector.o $(OBJDIR_DETECTOR)/src/Detection/FrameCapturer.o $(OBJDIR_DETECTOR)/src/Detection/FrameProcessor.o $(OBJDIR_DETECTOR)/src/Detection/SVMPredictor.o

all: debug positionserver fakesource detectiontest databasegenerator svmpredictortest detector

clean: clean_debug clean_positionserver clean_fakesource clean_detectiontest clean_databasegenerator clean_svmpredictortest clean_detector

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG)/src/Position/PositionServer || mkdir -p $(OBJDIR_DEBUG)/src/Position/PositionServer

after_debug: 

debug: before_debug out_debug after_debug

out_debug: $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LDFLAGS_DEBUG) $(LIBDIR_DEBUG) $(OBJ_DEBUG) $(LIB_DEBUG) -o $(OUT_DEBUG)

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

out_positionserver: $(OBJ_POSITIONSERVER) $(DEP_POSITIONSERVER)
	$(LD) $(LDFLAGS_POSITIONSERVER) $(LIBDIR_POSITIONSERVER) $(OBJ_POSITIONSERVER) $(LIB_POSITIONSERVER) -o $(OUT_POSITIONSERVER)

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

out_fakesource: $(OBJ_FAKESOURCE) $(DEP_FAKESOURCE)
	$(LD) $(LDFLAGS_FAKESOURCE) $(LIBDIR_FAKESOURCE) $(OBJ_FAKESOURCE) $(LIB_FAKESOURCE) -o $(OUT_FAKESOURCE)

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

out_detectiontest: $(OBJ_DETECTIONTEST) $(DEP_DETECTIONTEST)
	$(LD) $(LDFLAGS_DETECTIONTEST) $(LIBDIR_DETECTIONTEST) $(OBJ_DETECTIONTEST) $(LIB_DETECTIONTEST) -o $(OUT_DETECTIONTEST)

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

out_databasegenerator: $(OBJ_DATABASEGENERATOR) $(DEP_DATABASEGENERATOR)
	$(LD) $(LDFLAGS_DATABASEGENERATOR) $(LIBDIR_DATABASEGENERATOR) $(OBJ_DATABASEGENERATOR) $(LIB_DATABASEGENERATOR) -o $(OUT_DATABASEGENERATOR)

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

before_svmpredictortest: 
	test -d bin/SVMPredictorTest || mkdir -p bin/SVMPredictorTest
	test -d $(OBJDIR_SVMPREDICTORTEST)/src/Detection || mkdir -p $(OBJDIR_SVMPREDICTORTEST)/src/Detection

after_svmpredictortest: 

svmpredictortest: before_svmpredictortest out_svmpredictortest after_svmpredictortest

out_svmpredictortest: $(OBJ_SVMPREDICTORTEST) $(DEP_SVMPREDICTORTEST)
	$(LD) $(LDFLAGS_SVMPREDICTORTEST) $(LIBDIR_SVMPREDICTORTEST) $(OBJ_SVMPREDICTORTEST) $(LIB_SVMPREDICTORTEST) -o $(OUT_SVMPREDICTORTEST)

$(OBJDIR_SVMPREDICTORTEST)/src/Detection/SVMPredictor.o: src/Detection/SVMPredictor.cpp
	$(CXX) $(CFLAGS_SVMPREDICTORTEST) $(INC_SVMPREDICTORTEST) -c src/Detection/SVMPredictor.cpp -o $(OBJDIR_SVMPREDICTORTEST)/src/Detection/SVMPredictor.o

$(OBJDIR_SVMPREDICTORTEST)/src/Detection/SVMPredictorTest.o: src/Detection/SVMPredictorTest.cpp
	$(CXX) $(CFLAGS_SVMPREDICTORTEST) $(INC_SVMPREDICTORTEST) -c src/Detection/SVMPredictorTest.cpp -o $(OBJDIR_SVMPREDICTORTEST)/src/Detection/SVMPredictorTest.o

clean_svmpredictortest: 
	rm -f $(OBJ_SVMPREDICTORTEST) $(OUT_SVMPREDICTORTEST)
	rm -rf bin/SVMPredictorTest
	rm -rf $(OBJDIR_SVMPREDICTORTEST)/src/Detection

before_detector: 
	test -d bin/Detector || mkdir -p bin/Detector
	test -d $(OBJDIR_DETECTOR)/src/Detection || mkdir -p $(OBJDIR_DETECTOR)/src/Detection

after_detector: 

detector: before_detector out_detector after_detector

out_detector: $(OBJ_DETECTOR) $(DEP_DETECTOR)
	$(LD) $(LDFLAGS_DETECTOR) $(LIBDIR_DETECTOR) $(OBJ_DETECTOR) $(LIB_DETECTOR) -o $(OUT_DETECTOR)

$(OBJDIR_DETECTOR)/src/Detection/Detector.o: src/Detection/Detector.cpp
	$(CXX) $(CFLAGS_DETECTOR) $(INC_DETECTOR) -c src/Detection/Detector.cpp -o $(OBJDIR_DETECTOR)/src/Detection/Detector.o

$(OBJDIR_DETECTOR)/src/Detection/FrameCapturer.o: src/Detection/FrameCapturer.cpp
	$(CXX) $(CFLAGS_DETECTOR) $(INC_DETECTOR) -c src/Detection/FrameCapturer.cpp -o $(OBJDIR_DETECTOR)/src/Detection/FrameCapturer.o

$(OBJDIR_DETECTOR)/src/Detection/FrameProcessor.o: src/Detection/FrameProcessor.cpp
	$(CXX) $(CFLAGS_DETECTOR) $(INC_DETECTOR) -c src/Detection/FrameProcessor.cpp -o $(OBJDIR_DETECTOR)/src/Detection/FrameProcessor.o

$(OBJDIR_DETECTOR)/src/Detection/SVMPredictor.o: src/Detection/SVMPredictor.cpp
	$(CXX) $(CFLAGS_DETECTOR) $(INC_DETECTOR) -c src/Detection/SVMPredictor.cpp -o $(OBJDIR_DETECTOR)/src/Detection/SVMPredictor.o

clean_detector: 
	rm -f $(OBJ_DETECTOR) $(OUT_DETECTOR)
	rm -rf bin/Detector
	rm -rf $(OBJDIR_DETECTOR)/src/Detection

.PHONY: before_debug after_debug clean_debug before_positionserver after_positionserver clean_positionserver before_fakesource after_fakesource clean_fakesource before_detectiontest after_detectiontest clean_detectiontest before_databasegenerator after_databasegenerator clean_databasegenerator before_svmpredictortest after_svmpredictortest clean_svmpredictortest before_detector after_detector clean_detector

