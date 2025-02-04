#ifndef ANDREWS_ALGORITHM_H
#define ANDREWS_ALGORITHM_H

#include <utility>
#include <vector>
#include "Algorithm.h"

class AndrewsAlgorithm : public Algorithm {
public:
    explicit AndrewsAlgorithm(std::shared_ptr<std::vector<std::pair<float,float>>> newPoints)
        : Algorithm(std::move(newPoints), "Andrews"), step_index(0) {};
    void initialize() override;
    bool step() override;
private:
    size_t step_index;
    std::vector< const std::pair<float, float>*> upper_hull, lower_hull;
    void computeUpperHull();
    void computeLowerHull();
};

#endif
