#ifndef INOUT_H
#define INOUT_H

#include <vector>
#include <utility>
#include <string>
#include <memory>
#include "Algorithm.h"

class InOut {
    private:
        const std::string folder_points = "..\\..\\point_coords";
        const std::string folder_output = "..\\..\\output";
        const int max_num_points = 100000000;
        [[nodiscard]] std::string choose_file(const std::string& directory) const ;
    public:
        void points_from_random(std::shared_ptr<std::vector<std::pair<float,float>>> points) const;
        void points_from_file(std::shared_ptr<std::vector<std::pair<float,float>>> points) const;
        void export_convexHull(Algorithm &algorithm) const;
};


#endif //INOUT_H
