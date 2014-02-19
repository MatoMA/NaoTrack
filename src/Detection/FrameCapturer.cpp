#include "FrameCapturer.h"

FrameCapturer::FrameCapturer(string host, int port, string user, string password)
    :host(host), port(port), username(user), password(password), axis(host, port)
{
    init();
}

FrameCapturer::~FrameCapturer()
{
    axis.end();
}

void FrameCapturer::getPanTiltZoom(double &pan, double &tilt, double &zoom){
    axis.getPosition(pan, tilt, zoom);
}

void FrameCapturer::setPanTilt(double &pan, double &tilt) {
    axis.setPanTilt(pan, tilt);
    axis.wait();
}

void FrameCapturer::setZoom(double zoom){
    axis.setZoom(zoom);
    axis.wait();
    ost::Thread::sleep(2000);
}

ImageRGB FrameCapturer::getFrame(){

    axis.getDefaultBMPImage();
    int dummy;
    mirage::img::Coordinate img_size(axis.getWidth(), axis.getHeight());
    frame.resize(img_size,
            (ImageRGB::value_type*)axis.getImageBytes(dummy, dummy, dummy));
    rgb2bgr(frame);
    return frame;
}

ImageRGB FrameCapturer::getFakeFrame(std::string filename) {
    mirage::img::JPEG::read(fakeFrame, filename);
    return fakeFrame;
}

void FrameCapturer::init() {

    if(!axis.connect(username, password)){
        std::cout << "Can't connect " << username
	      << " (" << password << ") on "
	      << host << ':' << port << ". Aborting." << std::endl;
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
