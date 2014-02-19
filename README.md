# How to Compile

## Position server:
    make positionserver

## Detector (Module "Chercheur"):
    make detector

## GUI client and Analyser (Module "Analyse"):
    cd src/GUI
    cmake .
    make

# Setting for Detector
    bash detectorSetting.sh

# How to run
    bash runServer.sh
    bash runDetector.sh
    bash runAnalyser.sh
    bash runGUI.sh
