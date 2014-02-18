#include <math.h>
#include <glog/logging.h>
#include "FrameCapturer.h"
#include "FrameProcessor.h"

FrameProcessor::FrameProcessor(FrameCapturer& fc)
    //TODO
    //:frameCapturer(&fc), frame_in(fc.getFakeFrame("fakeFrame.jpg")), pantiltsCentered()
    :frameCapturer(&fc), frame_in(fc.getFrame()), pantiltsCentered()
{
    LOG(INFO) << __PRETTY_FUNCTION__;
    frameCapturer->getPanTiltZoom(pan, tilt, zoom);
}

FrameProcessor::~FrameProcessor()
{
    LOG(INFO) << __PRETTY_FUNCTION__;
}

void FrameProcessor::nextFrame() {
    LOG(INFO) << __PRETTY_FUNCTION__;
    frameCapturer->getPanTiltZoom(pan, tilt, zoom);
    frame_in = frameCapturer->getFrame();
}

void FrameProcessor::nextFakeFrame(std::string filename) {
    LOG(INFO) << __PRETTY_FUNCTION__;
    //TODO Bug potential of frame buffer copy operation
    frame_in = frameCapturer->getFakeFrame(filename);
    //frameCapturer->getPanTiltZoom(pan, tilt, zoom);
}

void FrameProcessor::writeFrame(std::string filename) {
    LOG(INFO) << __PRETTY_FUNCTION__;
    mirage::img::JPEG::write(frame_in, filename, 70);
}

void FrameProcessor::filterColor(int threshold) {
    LOG(INFO) << __PRETTY_FUNCTION__;
    try{
        ImageRGB::pixel_type p,p_end;
        ImageRGB::value_type black(0,0,0);
        ImageRGB::value_type green(0,255,0);
        for(p = frame_in.begin(), p_end = frame_in.end();
            p != p_end;
            ++p) {
            ImageRGB::value_type& v = *p;

            if (v._green > v._blue && v._green > v._red) {
                int temp = v._blue < v._red ? v._blue : v._red;
                if (v._green - temp > threshold)
                    *p = green;
                else
                    *p = black;
            }
            else
                *p = black;
        }
    }
    catch(mirage::Exception::Any& e) {
        LOG(ERROR) << "Error : " <<  e.what();
    }
    catch(...) {
        LOG(ERROR) << "Unknown error";
    }
}

std::vector<PanTiltCentered> FrameProcessor::findPositions() {
    LOG(INFO) << __PRETTY_FUNCTION__;
    try {
        labelizer.neighborhoodSurround(); // 8 neighbors considered
        labelizer(frame_in);

        mirage::img::Coordinate size = frame_in._dimension;
        LOG(INFO) << "Nb_labels: " << labelizer.nb_labels;
        LOG(INFO) << "Frame Width: " << size[0];
        LOG(INFO) << "Frame Height: " << size[1];
        double u0,v0,u,v,panCentered,tiltCentered;
        u0 = size[0]/2;
        v0 = size[1]/2;

        pantiltsCentered.clear();

        for (unsigned int i = 1; i<= labelizer.nb_labels; ++i) {
            const Labelizer::BoundingBox& box = labelizer.boundingBox(i);
            mirage::img::Coordinate A,C;

            A = box.min();
            C = box.max();

            //Filter box too small and calculate the center
            if (C[0] - A[0] > 3 && C[1] - A[1] > 3) {
                u = (A[0] + C[0]) / 2.0;
                v = (A[1] + C[1]) / 2.0;

                LOG(INFO) << "Label: " << i;
                LOG(INFO) << "Center_U: " << u;
                LOG(INFO) << "Center_V: " << v;

                pantiltzoom(&panCentered,&tiltCentered,u,v,u0,v0,pan,tilt,zoom);
                PanTiltCentered pantils(panCentered, tiltCentered);
                pantiltsCentered.push_back(pantils);
                LOG(INFO) << "PanCentered: " << panCentered;
                LOG(INFO) << "TiltCentered: " << tiltCentered;
            }
        }
    }
    catch(mirage::Exception::Any& e) {
        LOG(ERROR) << "Error : " <<  e.what();
    }
    catch(...) {
        LOG(ERROR) << "Unknown error";
    }

    return pantiltsCentered;
}

void FrameProcessor::pantiltzoom(double* ppan,double* ptilt,double u,double v,double u0,double v0,double pan0,double tilt0,double zoom)
{
	double focale,theta,alpha0,beta0,alpha,beta;
	double x,y,z,X,Y,Z,norme;
	theta=4.189301e+001-6.436043e-003*zoom+2.404497e-007*zoom*zoom;
	focale=u0/tan((M_PI*theta/180.0)/2);

	x=u-u0;y=v-v0;z=focale;
	norme=sqrt(x*x+y*y+z*z);
	x/=norme;y/=norme;z/=norme;

	beta0=-(M_PI*pan0/180.0);
	alpha0=-(M_PI*tilt0/180.0);

	X=cos(beta0)*x+sin(alpha0)*sin(beta0)*y-cos(alpha0)*sin(beta0)*z;
	Y=cos(alpha0)*y+sin(alpha0)*z;
	Z=sin(beta0)*x-sin(alpha0)*cos(beta0)*y+cos(alpha0)*cos(beta0)*z;
	alpha=atan2(Y,sqrt(X*X+Z*Z));
	beta=-atan2(X,Z);

	*ppan=-(180.0*beta/M_PI);
	*ptilt=-(180.0*alpha/M_PI);
}
