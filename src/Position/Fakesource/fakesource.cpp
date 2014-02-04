#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
//#define _GLIBCXX_USE_NANOSLEEP 1

typedef std::pair<double,double>  Point;  // (x,y)
typedef std::pair<int,Point>      Data;   // (label, P)

double fRand(double fMin, double fMax) {
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

double arg2d(char* arg) {
    char* endptr;
    return strtod(arg, &endptr);
}

void updatePoints(std::shared_ptr<boost::asio::ip::tcp::iostream> socket,
        std::vector<Data> datas, int delay, double noiseLevel) {
    while(1) {
        for (auto &data : datas) {
            *socket << "put ";
            //*socket << data.first;
            *socket << 1;
            *socket << " ";
            //auto point = data.second;
            Point point(2, 2);
            *socket << point.first + fRand(-noiseLevel, noiseLevel);
            *socket << " ";
            *socket << point.second + fRand(-noiseLevel, noiseLevel);
            *socket << " \n";
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
    }
}

int main(int argc, char* argv[]) {
    //std::cout << fRand(-0.5, 0.5) << std::endl;
    if(argc%2!=1 || argc < 7) {
        std::cerr << "Usage : " << argv[0] << " <host> <port> <delay (ms)> <noise level> <points...>" << std::endl;
        return 1;
    }

    std::string host(argv[1]);
    std::string port(argv[2]);
    int delay = atoi(argv[3]);
    std::string noiseLevelString(argv[4]);
    double noiseLevel = arg2d(argv[4]);
    std::cout << "host: " << host << std::endl;
    std::cout << "port: " << port << std::endl;
    std::cout << "delay: " << delay << "ms" << std::endl;
    std::cout << "noiseLevel: " << noiseLevel << std::endl;

    std::vector<Data> datas;
    int label = 1;
    for (int i = 5; i < argc; i += 2) {
        Point p(arg2d(argv[i]), arg2d(argv[i+1]));
        Data data(label++, p);
        std::cout << data.first << " " << data.second.first << " " << data.second.second << std::endl;
        datas.push_back(data);
    }

    srand(time(NULL));
    std::shared_ptr<boost::asio::ip::tcp::iostream> socket(new boost::asio::ip::tcp::iostream(host, port));

    boost::thread updateThread(updatePoints, socket, datas, delay, noiseLevel);
    updateThread.join();

    //for (auto &data : datas) {
        //std::cout << "label: " << data.first << std::endl;
        //auto point = data.second;
        //std::cout << "X: " << point.first << std::endl;
        //std::cout << "Y: " << point.second << std::endl;
    //}
    return 0;
}
