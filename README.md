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
    bash runServer.sh
    bash runDetector.sh
    bash runAnalyser.sh
    bash runGUI.sh
