#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "FrameCapturer.h"
#include "FrameProcessor.h"
#include "SVMPredictor.h"

typedef std::pair<double, double> PanTilt; // (pan, tilt)
typedef std::pair<double,double>  Point;  // (x,y)
typedef std::pair<int,Point>      Data;   // (label, P)

int main(int argc, char* argv[]) {

    //Init
    FrameCapturer fc("192.168.50.81",80,"demo","demo");
    fc.setZoom(3000);
    FrameProcessor fp(fc);
    SVMPredictor p("xDataModel", "yDataModel");
    XY result;
    std::string host = "localhost";
    std::string port = "3000";
    int delay = 1000;

    std::shared_ptr<boost::asio::ip::tcp::iostream> socket(new boost::asio::ip::tcp::iostream(host, port));

    //Search the point
    std::vector<PanTilt> path;
    int pathSize = 0;
    int pathIndex = 0;
    int numPointFound;
    std::string line;
    std::ifstream pathFile("path");
    if (pathFile.is_open()) {
        while (getline (pathFile, line)) {
            std::size_t panIndex = line.find("1:");
            std::size_t tiltIndex = line.find("2:");
            std::string panString = line.substr(panIndex+2, tiltIndex-2);
            std::string tiltString = line.substr(tiltIndex+2);
            PanTilt pantilt(atof(panString.c_str()), atof(tiltString.c_str()));
            path.push_back(pantilt);
        }
        pathFile.close();
    } else {
        std::cout << "path file not found!" << std::endl;
        //break;
    }

    pathSize = path.size();
    std::cout << "pathSize: " << pathSize << std::endl;

    //Globle loop
    while (1) {
        do {
            std::cout << "pathIndex" << pathIndex << std::endl;
            //fc.setPanTilt(path.at(pathSize - pathIndex - 1 ).first, path.at(pathSize - pathIndex - 1).second);
            fc.setPanTilt(path.at(pathIndex).first, path.at(pathIndex).second);
            pathIndex = (pathIndex + 2) % pathSize;
            fp.nextFrame();
            //fp.writeFrame("output1.jpg");
            fp.filterColor(35);
            //fp.writeFrame("output2.jpg");
            std::vector<std::pair<double, double>> pt;
            pt = fp.findPositions();
            numPointFound = pt.size();
            //boost::this_thread::sleep(boost::posix_time::milliseconds(500));
        } while (numPointFound == 0);

        std::cout << "Found the point!" << std::endl;

        //Start to track
        while(1) {
            double pan, tilt, zoom;
            fc.getPanTiltZoom(pan, tilt, zoom);
            std::cout << "Current status:" << std::endl;
            std::cout << "Pan: " << pan << std::endl;
            std::cout << "Tilt: " << tilt << std::endl;
            std::cout << "Zoom: " << zoom << std::endl;

            fp.nextFrame();
            fp.writeFrame("output1.jpg");
            fp.filterColor(35);
            fp.writeFrame("output2.jpg");
            std::vector<std::pair<double, double>> pt;
            pt = fp.findPositions();
            std::cout << "Num of point finded: " << pt.size() << std::endl;
            if (pt.size() > 0) {
                fc.setPanTilt(pt[0].first, pt[0].second);
                int label = 1;
                for (auto i : pt) {
                    std::cout << "Point " << label << std::endl;
                    std::cout << "Pan: " << i.first << std::endl;
                    std::cout << "Tilt: " << i.second << std::endl;
                    result = p.predict(i.first, i.second);
                    std::cout << "X: " << result.first << std::endl;
                    std::cout << "Y: " << result.second << std::endl;
                    Point point(result.first, result.second);
                    *socket << "put ";
                    *socket << label++;
                    *socket << " ";
                    *socket << point.first;
                    *socket << " ";
                    *socket << point.second;
                    *socket << " \n";
                }
            } else {
                break;
            }
        }
    }

    return 0;
}
