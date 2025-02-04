#include "Algorithm.h"


std::vector<std::pair<const std::pair<float, float>*, const std::pair<float, float>*>> Algorithm::getLines() {
    return this->lines;
}

std::vector<std::pair<const std::pair<float, float>*, const std::pair<float, float>*>> Algorithm::getConvexHull() {
    return this->convex_hull;
}


const std::vector<std::pair<float, float>>& Algorithm::getPoints() const {
    return *points;
}

void Algorithm::setGraphicMode(bool mode) {
    graphicMode = mode;
}
