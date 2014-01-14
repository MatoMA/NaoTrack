#include <string>
#include <iostream>
#include <glog/logging.h>
#include "FrameCapturer.h"
#include "FrameProcessor.h"
#include "SVMPredictor.h"

void loggerInit(char* argv0) {
    google::InitGoogleLogging(argv0);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 2;
}

int main(int argc, char* argv[]) {
    loggerInit(argv[0]);

    FrameCapturer fc("192.168.50.84",80,"demo","demo");
    fc.setZoom(3000);
    FrameProcessor fp(fc);
    SVMPredictor p("xDataModel", "yDataModel");
    XY result;

    while(1) {

    double pan, tilt, zoom;
    fc.getPanTiltZoom(pan, tilt, zoom);
//    std::cout << "Pan: " << pan << std::endl;
//    std::cout << "Tilt: " << tilt << std::endl;
//    std::cout << "Zoom: " << zoom << std::endl;

    fp.nextFrame();
//    fp.writeFrame("output1.jpg");
    fp.filterColor(35);
//    fp.writeFrame("output2.jpg");
    std::vector<std::pair<double, double>> pt;
    pt = fp.findPositions();
    fc.setPanTilt(pt[0].first, pt[0].second);
//    for (auto i : pt) {
//        std::cout << i.first << std::endl;
//        std::cout << i.second << std::endl;
//    }

    result = p.predict(pt[0].first, pt[0].second);
    std::cout << "X: " << result.first << std::endl;
    std::cout << "Y: " << result.second << std::endl;

    }

    return 0;
}

