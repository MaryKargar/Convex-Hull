#include <algorithm>
#include "AndrewsAlgorithm.h"

void AndrewsAlgorithm::initialize() {

    upper_hull.clear();
    lower_hull.clear();
    convex_hull.clear();

    // Sort the points by x-coordinate
    std::sort(points->begin(), points->end());

    computeUpperHull();
    computeLowerHull();

    // Reset step_index for incremental drawing
    step_index = 0;
}

bool AndrewsAlgorithm::step() {
    // First, reveal upper hull
    if (step_index < upper_hull.size() - 1) {
        convex_hull.emplace_back(upper_hull[step_index], upper_hull[step_index + 1]);
        step_index++;
        return false;  // Still more to draw
    }

    // Then reveal lower hull
    size_t lower_start_index = step_index - (upper_hull.size() - 1);
    if (lower_start_index < lower_hull.size() - 1) {
        convex_hull.emplace_back(lower_hull[lower_start_index], lower_hull[lower_start_index + 1]);
        step_index++;
        return false;  // Still more to draw
    }

    /*// Close the convex hull by connecting the last point of the lower hull to the first point of the upper hull
    if (step_index == upper_hull.size() + lower_hull.size() - 2) {
        convex_hull.emplace_back(lower_hull.back(), upper_hull.front());
        step_index++;
        return true;  // Finished drawing the hull
    }*/

    return true;  // Completed drawing
}

void AndrewsAlgorithm::computeUpperHull() {
    for (const auto& p : *points) {
        while (upper_hull.size() >= 2) {
            auto& p1 = upper_hull[upper_hull.size() - 2];
            auto& p2 = upper_hull.back();
            if ((p2->first - p1->first) * (p.second - p1->second) - (p2->second - p1->second) * (p.first - p1->first) > 0) {
                break;
            }
            upper_hull.pop_back();
        }
        upper_hull.push_back(&p);
    }
}

void AndrewsAlgorithm::computeLowerHull() {
    for (auto it = points->rbegin(); it != points->rend(); ++it) {
        const auto& p = *it;
        while (lower_hull.size() >= 2) {
            auto& p1 = lower_hull[lower_hull.size() - 2];
            auto& p2 = lower_hull.back();
            if ((p2->first - p1->first) * (p.second - p1->second) - (p2->second - p1->second) * (p.first - p1->first) > 0) {
                break;
            }
            lower_hull.pop_back();
        }
        lower_hull.push_back(&p);
    }
}
