# How to Compile

## Position server:
    make positionserver

## Detector (Module "Chercheur"):
    make detector

## GUI client and Analyser (Module "Analyse"):
    cd src/GUI
    cmake .
    make

# Settings 
    bash detectorSetting.sh
    gedit src/GUI/config.xml (optional)

# How to run

## Position server:
    bash runServer1.sh
    bash runServer2.sh

## Detector:
    cd bin/Detector
    ./detector

## Analyser
    cd src/GUI
    ./Analyser

## GUI
    cd src/GUI
    ./position_viewer
