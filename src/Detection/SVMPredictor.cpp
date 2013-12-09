#include <glog/logging.h>
#include "SVMPredictor.h"

SVMPredictor::SVMPredictor(const std::string& xModel, const std::string& yModel) : xPredictor(), yPredictor() {
    LOG(INFO) << __PRETTY_FUNCTION__;
    ml::libsvm::quiet();
    xPredictor.load_model(xModel);
    yPredictor.load_model(yModel);
}

SVMPredictor::~SVMPredictor() {
    LOG(INFO) << __PRETTY_FUNCTION__;
}

XY& SVMPredictor::predict(const double& pan, const double& tilt) {
    LOG(INFO) << __PRETTY_FUNCTION__;

    nodes[0].index = 1;
    nodes[0].value = pan;
    nodes[1].index = 2;
    nodes[1].value = tilt;
    nodes[2].index = -1;

    double x_predict = xPredictor.predict(nodes);
    double y_predict = yPredictor.predict(nodes);
    XY result(x_predict, y_predict);
    return result;
}
