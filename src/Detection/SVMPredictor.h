#ifndef SVMPREDICTOR_H
#define SVMPREDICTOR_H

#include <string>
#include <utility>
#include <ml-libsvm.hpp>

typedef std::pair<double,double> PanTilt;
typedef std::pair<double,double> XY;

class SVMPredictor {
    public:
        SVMPredictor(const std::string& xModel, const std::string& yModel);
        ~SVMPredictor();

        XY& predict(const double& pan, const double& tilt);

    private:
        ml::libsvm::Predictor<PanTilt> xPredictor;
        ml::libsvm::Predictor<PanTilt> yPredictor;
        struct svm_node nodes[3];

};

#endif // SVMPREDICTOR_H
