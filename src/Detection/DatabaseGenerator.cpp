#include <string>
#include <sstream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include "FrameCapturer.h"
#include "FrameProcessor.h"

using namespace std;
using namespace boost::filesystem;

int main(int argc, char* argv[]) {

    FrameCapturer fc("192.168.50.81",80,"demo","demo");
    fc.setZoom(2000);
    FrameProcessor fp(fc);

    string x_filename("xData");
    string y_filename("yData");
    path x_dataFilePath(x_filename);
    path y_dataFilePath(y_filename);
    if (exists(x_dataFilePath))
        remove(x_filename);
    if (exists(y_dataFilePath))
        remove(y_filename);

    ofstream x_dataFile(x_filename);
    ofstream y_dataFile(y_filename);
    stringstream ss;
    string x, y, wConfirm;
    double pan, tilt, zoom;
    while(wConfirm != "quit") {
        wConfirm = "no";
        cout << "X: ";
        cin >> x;
        cout << "Y: ";
        cin >> y;
        //cout << "X: " << x << endl;
        //cout << "Y: " << y << endl;
        while(wConfirm != "yes" && wConfirm != "quit") {
            cout << "Ready to write data?(yes/no/quit): ";
            cin >> wConfirm;
        }
        if (wConfirm == "quit")
            break;

        if (wConfirm == "yes"){
            fc.getPanTiltZoom(pan,tilt,zoom);
            x_dataFile << x << " ";
            x_dataFile << "1:" << pan << " ";
            x_dataFile << "2:" << tilt << " ";
            x_dataFile << endl;

            y_dataFile << y << " ";
            y_dataFile << "1:" << pan << " ";
            y_dataFile << "2:" << tilt << " ";
            y_dataFile << endl;

            ss << "X" << x << ":";
            ss << "Y" << y << ":";
            ss << "P" << pan << ":";
            ss << "T" << tilt << ":";
            ss << "Z" << tilt << ":.jpg";
            fp.writeFrame(ss.str());
            //Clear ss
            ss.str(string());

            cout << "X:" << x << " ";
            cout << "Y:" << y << " ";
            cout << "Pan:" << pan << " ";
            cout << "Tilt:" << tilt << " ";
            cout << "Zoom:" << zoom << " ";
            cout << endl;
            cout << endl;
        }
    }

    x_dataFile.close();
    y_dataFile.close();

    return 0;
}
