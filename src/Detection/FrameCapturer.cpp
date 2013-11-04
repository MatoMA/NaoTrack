#include <glog/logging.h>
#include "FrameCapturer.h"

FrameCapturer::FrameCapturer(string host, int port, string user, string password)
    :host(host), port(port), username(user), password(password), axis(host, port)
{
    LOG(INFO) << __PRETTY_FUNCTION__;
    LOG(INFO) << "host: " << host;
    LOG(INFO) << "port: " << port;
    LOG(INFO) << "username: " << user;
    LOG(INFO) << "password: " << password;

    init();
}

//FrameCapturer::FrameCapturer(FrameCapturer& fc)
    //:host(fc.getHost()), port(fc.getPort()), username(fc.getUsername()), password(fc.getPassword()), axis(fc.getHost(), fc.getPort()){

    //LOG(INFO) << __PRETTY_FUNCTION__;
    //LOG(INFO) << "host: " << host;
    //LOG(INFO) << "port: " << port;
    //LOG(INFO) << "username: " << username;
    //LOG(INFO) << "password: " << password;

    //init();
//}

FrameCapturer::~FrameCapturer()
{
    LOG(INFO) << __PRETTY_FUNCTION__;
    axis.end();
}

void FrameCapturer::getPanTiltZoom(double &pan, double &tilt, double &zoom){
    LOG(INFO) << __PRETTY_FUNCTION__;
    axis.getPosition(pan, tilt, zoom);
    LOG(INFO) << "Pan: " << pan;
    LOG(INFO) << "Tilt: " << tilt;
    LOG(INFO) << "Zoom: " << zoom;
}

void FrameCapturer::setPanTilt(double &pan, double &tilt) {
    LOG(INFO) << __PRETTY_FUNCTION__;
    LOG(INFO) << "Pan: " << pan;
    LOG(INFO) << "Tilt: " << tilt;

    axis.setPanTilt(pan, tilt);
    axis.wait();
}

void FrameCapturer::setZoom(double zoom){
    LOG(INFO) << __PRETTY_FUNCTION__;
    LOG(INFO) << "Zoom: " << zoom;

    axis.setZoom(zoom);
    axis.wait();
    ost::Thread::sleep(2000);
}

ImageRGB FrameCapturer::getFrame(){
    LOG(INFO) << __PRETTY_FUNCTION__;
    //TODO
    //int width, height, depth;
    //unsigned char *imgBytes = axis.getImageBytes(width, height, depth);
    //mirage::img::Coordinate img_size(width, height);

    axis.getDefaultBMPImage();
    int dummy;
    mirage::img::Coordinate img_size(axis.getWidth(), axis.getHeight());
    frame.resize(img_size,
            (ImageRGB::value_type*)axis.getImageBytes(dummy, dummy, dummy));
    rgb2bgr(frame);
    return frame;
}

ImageRGB FrameCapturer::getFakeFrame(std::string filename) {
    LOG(INFO) << __PRETTY_FUNCTION__;
    //if(fakeFrame == nullptr) {
    LOG(INFO) << "Init fakeFrame...";
    mirage::img::JPEG::read(fakeFrame, filename);
    //}
    return fakeFrame;
}

void FrameCapturer::init() {
    LOG(INFO) << __PRETTY_FUNCTION__;
    LOG(INFO) << "Init axis connection...";

    if(!axis.connect(username, password)){
        LOG(FATAL) << "Can't connect " << username
	      << " (" << password << ") on "
	      << host << ':' << port << ". Aborting.";
    }

    //TODO  No verifications of these two settings
    //      Can't catch the url timeout error
    //      Bugs of axisPTZ
    axis.setAutoiris((char*)"off");
    axis.setIris(1500);

    //TODO set frame_width frame_height ???
}

void FrameCapturer::rgb2bgr(ImageRGB& img) {
  ImageRGB::pixel_type pix,pix_end;

  for(pix=img.begin(),pix_end=img.end();
      pix!=pix_end;
      ++pix)
    rgb2bgr(*pix);
}

void FrameCapturer::rgb2bgr(ImageRGB::value_type& rgb) {
  ImageRGB::value_type::value_type tmp;

  tmp       = rgb._red;
  rgb._red  = rgb._blue;
  rgb._blue = tmp;
}
