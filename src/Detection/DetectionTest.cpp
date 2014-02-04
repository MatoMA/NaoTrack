#include <string>
#include <glog/logging.h>
#include "FrameCapturer.h"
#include "FrameProcessor.h"

void loggerInit(char* argv0) {
    google::InitGoogleLogging(argv0);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 2;
}

int main(int argc, char* argv[]) {
    loggerInit(argv[0]);

    //std::string host("ptz1.grid.metz.supelec.fr");
    //int port = 80;
    //std::string user("frezza");
    //std::string password("gloubi");

    FrameCapturer fc("192.168.50.81",80,"demo","demo");

    fc.setZoom(0);

    double pan, tilt, zoom;
    fc.getPanTiltZoom(pan, tilt, zoom);
    std::cout << "Pan: " << pan << std::endl;
    std::cout << "Tilt: " << tilt << std::endl;
    std::cout << "Zoom: " << zoom << std::endl;

    fc.setZoom(2000);
    FrameProcessor fp(fc);
    fp.nextFrame();
    //fp.writeFrame("output1.jpg");
    fp.filterColor(35);
    //fp.writeFrame("output2.jpg");
    std::vector<std::pair<double, double>> pt;
    pt = fp.findPositions();
    //fc.setPanTilt(pt[0].first, pt[0].second);
    for (auto i : pt) {
        std::cout << "haha" << std::endl;
        std::cout << i.first << std::endl;
        std::cout << i.second << std::endl;
    }

    //fp.nextFakeFrame("fakeFrame2.jpg");
    //fp.filterColor(40);
    //fp.writeFrame("output2.jpg");
    //pt = fp.findPositions();
    //for (auto i : pt) {
        //std::cout << "hoho" << std::endl;
        //std::cout << i.first << std::endl;
        //std::cout << i.second << std::endl;
    //}

    //double pan, tilt, zoom;
    //fc.getPanTiltZoom(pan, tilt, zoom);
    //std::cout << "pan:" << pan << std::endl;
    //std::cout << "tilt:" << tilt << std::endl;
    //std::cout << "zoom:" << zoom << std::endl;
    //FrameProcessor * fp = new FrameProcessor();
    //delete fp;
    return 0;
}

