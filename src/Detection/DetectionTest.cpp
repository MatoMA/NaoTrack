#include <string>
#include "FrameCapturer.h"
#include "FrameProcessor.h"

int main(int argc, char* argv[]) {

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
    return 0;
}

