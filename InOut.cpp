#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <random>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <memory>

#include "InOut.h"
#include "Algorithm.h"

void InOut::points_from_random(std::shared_ptr<std::vector<std::pair<float,float>>> points) const {
    size_t numPoints=0;
    while (true) {
        std::cout << "How many points (3 <= x < " << max_num_points << ")? " << std::endl;
        std::cin >> numPoints;

        // Überprüfen, ob die Eingabe erfolgreich war und im gültigen Bereich liegt
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input, please enter a valid number." << std::endl;
        } else if (numPoints >= 3 && numPoints <= max_num_points) {
            break;
        } else {
            std::cout << "Number of points must be between 3 and " << max_num_points << "." << std::endl;
        }
    }

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<float> dis(0.0f,1000.0f); // distribution in range [1, 6]

    points->reserve(numPoints);
    for (size_t i = 0; i < numPoints; ++i) {
        points->emplace_back(dis(rng), dis(rng));
    }
    std::cout << points->size() << " points with random coordinates created." << std::endl;
}

std::string InOut::choose_file(const std::string& directory) const {
    std::vector<std::string> csv_files;

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            csv_files.push_back(entry.path().filename().string());
        }
    }

    // No file found --> Programm exit
    if (csv_files.empty()) {
        throw std::runtime_error("No CSV files found in directory "+directory);
    }

    std::cout << "Available CSV files:" << std::endl;
    for (size_t i = 0; i < csv_files.size(); ++i) {
        std::cout << i + 1 << ": " << csv_files[i] << std::endl;
    }

    int choice = 0;
    while (true) {
        std::cout << "Enter the number of the file to open: ";
        std::cin >> choice;

        if (std::cin.fail() || choice < 1 || choice > static_cast<int>(csv_files.size())) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Please try again." << std::endl;
        } else {
            break;
        }
    }

    return directory + "\\" + csv_files[choice - 1];;
}


void InOut::points_from_file(std::shared_ptr<std::vector<std::pair<float,float>>> points) const {

    // Get Files in folder point_coords
    std::string file_path = choose_file(folder_points);

    std::ifstream file(file_path);
    if (!file) {
        throw std::runtime_error("Error: Could not open file "+file_path);
    }

    // First Line --> number of points
    std::string line;
    if (!std::getline(file, line)) {
        throw std::runtime_error("Error: Could not read number of points from file " + file_path);
    }
    size_t numPoints;
    try {
        numPoints = std::stoul(line);  // Umwandlung in eine Ganzzahl (size_t)
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Error: Invalid number of points format in file " + file_path);
    }
    if (numPoints < 3 || numPoints > max_num_points) {
        throw std::runtime_error("Error: Number of points out of range: " + std::to_string(numPoints));
    }

    points->reserve(numPoints);
    size_t point_count = 0;
    while (std::getline(file, line)) {
        // read first Line --> number of points
        std::stringstream ss(line);
        float x, y;

        if (!(ss >> x)) {
            throw std::runtime_error("Error: Invalid x-coordinate format in file " + file_path);
        }
        if (ss.peek() == ',') ss.ignore();
        if (!(ss >> y)) {
            throw std::runtime_error("Error: Invalid y-coordinate format in file " + file_path);
        }

        points->emplace_back(x, y);
        point_count++;

        if (point_count > numPoints) {
            throw std::runtime_error("Error: More points found in file than declared in file " + file_path);
        }
    }
    if (point_count != numPoints) {
        throw std::runtime_error("Error: Fewer points found than declared in file " + file_path);
    }

    file.close();
    std::cout << "Successfully read " << points->size() << " points from: " << file_path << std::endl;
}

void InOut::export_convexHull(Algorithm &algorithm) const {
    auto convex_hull = algorithm.getConvexHull();
    if (convex_hull.empty()) {
        throw std::runtime_error("Convex hull is empty.");
    }

    // Sort convex_hull lines
    std::vector<const std::pair<float, float>*> sorted_hull;
    sorted_hull.reserve(convex_hull.size());
    sorted_hull.push_back(convex_hull[0].first);
    auto endpunkt = convex_hull[0].second;
    convex_hull.erase(convex_hull.begin());
    while (!convex_hull.empty()) {
        auto it = std::find_if(convex_hull.begin(), convex_hull.end(),
                               [&endpunkt](const auto &line) {
                                   return line.first == endpunkt;
                               });
        if (it != convex_hull.end()) {
            sorted_hull.push_back(it->first);
            endpunkt=it->second;
            convex_hull.erase(it);
        } else {
            break; // No matching line found, break the loop
        }
    }
    if (sorted_hull.size() == 1) {
        // Only one line in convex hull
        sorted_hull.push_back(endpunkt);
    }

    // Consoleoutput
    std::cout << "Convex Hull (Size: "<< sorted_hull.size()<<"):" << std::endl;
    for (const auto& p : sorted_hull) {
        std::cout << p->first << "," << p->second << std::endl;
    }

    // Create New Folder with date
    std::time_t now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm now_tm = *std::localtime(&now_time);
    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y%m%d_%H%M%S");
    std::string timestamp = oss.str();

    // Outputfolder
     std::filesystem::path outputfolder = folder_output;
    if (!std::filesystem::exists(outputfolder))
        std::filesystem::create_directories(outputfolder);
    outputfolder /= algorithm.algorithmName+"_" + timestamp;
    if (!std::filesystem::exists(outputfolder))
        std::filesystem::create_directories(outputfolder);

    // Export Convex Hull to .csv file
    {
        std::ofstream file((outputfolder / "convexhull.csv").string());
        if (!file) {
            throw std::runtime_error("Could not open file for convex hull output.");
        }
        file << sorted_hull.size() << std::endl;
        for (const auto& p : sorted_hull) {
            file << p->first << "," << p->second << std::endl;
        }
    }

    // Export Points to .csv file
    const auto& pointsRef = algorithm.getPoints();
    {
        std::ofstream file((outputfolder / "points.csv").string());
        if (!file) {
            throw std::runtime_error("Could not open file for points output.");
        }
        file << pointsRef.size() << std::endl;
        for (const auto& p : pointsRef) {
            file << p.first << "," << p.second << std::endl;
        }
    }
}
