#include <string>
#include <iostream>
#include <glog/logging.h>
#include <ctime>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "FrameCapturer.h"
#include "FrameProcessor.h"
#include "SVMPredictor.h"

typedef std::pair<double,double>  Point;  // (x,y)
typedef std::pair<int,Point>      Data;   // (label, P)

void loggerInit(char* argv0) {
    google::InitGoogleLogging(argv0);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 2;
}

void updatePoints(std::shared_ptr<boost::asio::ip::tcp::iostream> socket,
        std::vector<Data> datas, int delay, FrameProcessor fp) {
            while(1) {


    for (auto &data : datas) {
        *socket << "put ";
        *socket << data.first;
        *socket << " ";
        auto point = data.second;
        *socket << point.first;
        *socket << " ";
        *socket << point.second;
        *socket << " \n";
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(delay));

            }

}

int main(int argc, char* argv[]) {
    loggerInit(argv[0]);

    //Init
    FrameCapturer fc("192.168.50.81",80,"demo","demo");
    fc.setZoom(3000);
    FrameProcessor fp(fc);
    SVMPredictor p("xDataModel", "yDataModel");
    std::vector<Data> datas;
    XY result;
    std::string host = "localhost";
    std::string port = "3000";
    int delay = 1000;

    std::shared_ptr<boost::asio::ip::tcp::iostream> socket(new boost::asio::ip::tcp::iostream(host, port));

    while(1) {
        double pan, tilt, zoom;
        fc.getPanTiltZoom(pan, tilt, zoom);
        std::cout << "Pan: " << pan << std::endl;
        std::cout << "Tilt: " << tilt << std::endl;
        std::cout << "Zoom: " << zoom << std::endl;

        fp.nextFrame();
        fp.writeFrame("output1.jpg");
        fp.filterColor(35);
        fp.writeFrame("output2.jpg");
        std::vector<std::pair<double, double>> pt;
        pt = fp.findPositions();
        fc.setPanTilt(pt[0].first, pt[0].second);
        int label = 1;
        for (auto i : pt) {
            std::cout << i.first << std::endl;
            std::cout << i.second << std::endl;
            result = p.predict(i.first, i.second);
            std::cout << "X: " << 10 - result.first - 11.4 << std::endl;
            std::cout << "Y: " << result.second << std::endl;
            Point point(10 - result.first - 11.4, result.second);
            Data data(label++, point);
            datas.push_back(data);
        }

        for (auto &data : datas) {
            *socket << "put ";
            *socket << data.first;
            *socket << " ";
            auto point = data.second;
            *socket << point.first;
            *socket << " ";
            *socket << point.second;
            *socket << " \n";
        }

        //boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
    }


    //Point point(2.5, 2.5);
    //Data data(0, point);
    //datas.push_back(data);

//    std::shared_ptr<boost::asio::ip::tcp::iostream> socket(new boost::asio::ip::tcp::iostream(host, port));

//    boost::thread updateThread(updatePoints, socket, datas, delay);
//    updateThread.join();

    return 0;
}

