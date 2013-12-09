#include <iostream>
#include "SVMPredictor.h"

int main(int argc, char* argv[]) {

    std::cout << "Hello" << std::endl;
    //ml::libsvm::quiet();
    SVMPredictor p("xDataModel", "yDataModel");
    //2 1:-33.64 2:-56.8166
    XY result;
    result = p.predict(-33.64, -56.8166);
    std::cout << "X: " << result.first << std::endl;
    std::cout << "Y: " << result.second << std::endl;
    return 0;
}

