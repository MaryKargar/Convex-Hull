#include <chrono>
#include <iostream>
#include <numeric>
#include <windows.h>
#include "ReleaseMode.h"

void ReleaseMode::start() {
    SetConsoleOutputCP(CP_UTF8);

    std::vector<double> runtimes;

    std::cout << "Algorithm starts - Runs "<< runs << " times." << std::endl;
    for (int i = 0; i < runs; ++i) {
        auto start = std::chrono::high_resolution_clock::now();

        algorithm_.initialize();
        bool step = algorithm_.step();
        while (step==false) {
            step = algorithm_.step();
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        runtimes.push_back(elapsed.count());
    }

    double sum = std::accumulate(runtimes.begin(), runtimes.end(), 0.0);
    double average_runtime = sum / runtimes.size();

    if (average_runtime<0.01) {
        auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::duration<double>(average_runtime)).count();
        std::cout << "Algorithm finished in average in " << elapsed_us << " µs." << std::endl;
    } else {
        std::cout << "Algorithm finished in average in " << average_runtime << " s." << std::endl;
    }

}

