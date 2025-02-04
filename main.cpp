#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <memory>

#include "AndrewsAlgorithm.h"
#include "InOut.h"
#include "GraphicMode.h"
#include "Algorithm.h"
#include "QuickHull.h"
#include "ReleaseMode.h"


int main() {
    char respond;
    InOut in_out;
    auto points = std::make_shared<std::vector<std::pair<float,float>>>();

    //Call if Random Coords or from File
    std::cout << "Points - Random Coordinates (1) or File (any other key)?";
    std::cin >> respond;

    if (respond == '1') {
        in_out.points_from_random(points);
    }
    else {
        in_out.points_from_file(points);
    }

    // Choose Algorithm
    std::cout << "Choose Algorithm - Andrews (1) or QuickHull (any other key)?";
    std::cin >> respond;
    std::unique_ptr<Algorithm> algorithm;
    if (respond == '1') {
        //algorithm = std::make_unique<AndrewsAlgorithm>(points);
        algorithm = std::make_unique<AndrewsAlgorithm>(points);
        std::cout << "Algorithm 'Andrews' chosen." << std::endl;
    } else {
        algorithm = std::make_unique<QuickHull>(points);
        std::cout << "Algorithm 'QuickHull' chosen." << std::endl;
    }

    // Choose Mode
    std::cout << "Graphical Mode (1) or Performance Mode (any other key)?";
    std::cin >> respond;
    std::unique_ptr<Mode> mode;
    if (respond == '1') {
        mode = std::make_unique<GraphicMode>(*algorithm);
        std::cout << "Graphical Mode selected" << std::endl;
    } else {
        mode = std::make_unique<ReleaseMode>(*algorithm, 10);
        std::cout << "Performance Mode selected" << std::endl;
    }

    // Start Mode
    mode->start();

    // Export Convex Hull
    in_out.export_convexHull(*algorithm);
    return 0;
}
