#include <iostream>
#include <math.h>
#include "QuickHull.h"
using Point = std::pair<float, float>;

int QuickHull::findSide(const Point& l1, const Point& l2, const Point& pt) {
    // Crossproduct to find Side
    float result = (l2.first - l1.first) * (pt.second - l1.second) - (l2.second - l1.second) * (pt.first - l1.first);
    if (result > 0) return 1;  // Left Side
    if (result < 0) return -1; // Right Side
    return 0;   // On the side
}

float QuickHull::distanceLine(const Point& l1, const Point& l2, const Point& pt) {
    //Euklidean Distance between Point and Line
    return std::abs((pt.second - l1.second) * (l2.first - l1.first) - (l2.second - l1.second) * (pt.first - l1.first)) /
            std::sqrt((l2.first - l1.first) * (l2.first - l1.first) + (l2.second - l1.second) * (l2.second - l1.second));
    }

const Point* QuickHull::findFarthestPoint(const Point& l1, const Point& l2, std::vector<const Point*>& points) const {
    float maxDistance = 0;
    const Point* maxPt = nullptr;
    for (const Point* pt : points) {
        float distance = distanceLine(l1, l2, *pt);
        if (distance >= maxDistance) {
            maxDistance = distance;
            maxPt = pt;
        }
    }
    return maxPt;
}

bool QuickHull::addStepStack_bothdir(const Point& l1, const Point& l2, const std::vector<const Point*>& points) {
    std::vector<const Point*> remainingpoints_lsle;
    remainingpoints_lsle.reserve(points.size());
    std::vector<const Point*> remainingpoints_lels;
    remainingpoints_lels.reserve(points.size());

    for(const Point* pt : points) {
        int side = findSide(l1, l2, *pt);
        if (side == 1) {
            remainingpoints_lsle.push_back(pt);
        } else if (side == -1) {
            remainingpoints_lels.push_back(pt);
        }
    }

    if (remainingpoints_lsle.empty() && remainingpoints_lels.empty()) {
        return false;
    }
    if (!remainingpoints_lsle.empty()) {
        stepStack.push({l1, l2, remainingpoints_lsle});
    }
    if (!remainingpoints_lels.empty()) {
        stepStack.push({l2, l1, remainingpoints_lels});
    }
    return true;
}

bool QuickHull::addStepStack(const Point& l1, const Point& l2, const std::vector<const Point*>& points) {
    std::vector<const Point*> remainingpoints;
    remainingpoints.reserve(points.size());
    for(const Point* pt : points) {
        if (findSide(l1, l2, *pt) == 1) {
            remainingpoints.push_back(pt);
        }
    }

    if (remainingpoints.empty()) {
        return false;
    }
    stepStack.push({l1, l2, remainingpoints});
    return true;
}

void QuickHull::initialize() {
    //Find Point with x and y max and fill list remainingpoints
    int idx_xmin, idx_xmax;
    idx_xmin = (points->at(0).first < points->at(1).first) ? 0 : 1;
    idx_xmax = 1 - idx_xmin;

    std::vector<const Point*> remainingpoints;
    remainingpoints.reserve(points->size() - 2);
    for (int i=2; i<points->size(); i++)
    {
        if (points->at(i).first < points->at(idx_xmin).first) {
            remainingpoints.push_back(&(*points)[idx_xmin]);
            idx_xmin = i;
        }
        else if (points->at(i).first > points->at(idx_xmax).first) {
            remainingpoints.push_back(&(*points)[idx_xmax]);
            idx_xmax = i;
        }
        else {
            remainingpoints.push_back(&(*points)[i]);
        }
    }

    if (points->at(idx_xmin).first == points->at(idx_xmax).first) {
        idx_xmin = (points->at(0).second < points->at(1).second) ? 0 : 1;
        idx_xmax = 1 - idx_xmin;

        for (int i=2; i<points->size(); i++)
        {
            if (points->at(i).second < points->at(idx_xmin).second) {
                remainingpoints.push_back(&(*points)[idx_xmin]);
                idx_xmin = i;
            }
            else if (points->at(i).second > points->at(idx_xmax).second) {
                remainingpoints.push_back(&(*points)[idx_xmax]);
                idx_xmax = i;
            }
            else {
                remainingpoints.push_back(&(*points)[i]);
            }
        }
        if (points->at(idx_xmin).second == points->at(idx_xmax).second) {
            throw std::invalid_argument("All points are on the same coordinate.");
        }
    }

    this->lines.clear();
    this->convex_hull.clear();
    auto status = addStepStack_bothdir(points->at(idx_xmin), points->at(idx_xmax), remainingpoints);
    if (!status) {
        this->convex_hull.emplace_back(&points->at(idx_xmin), &points->at(idx_xmax));
    } else if (graphicMode) {
        this->lines.emplace_back(&points->at(idx_xmin), &points->at(idx_xmax));
    }
};


bool QuickHull::step() {
    if (stepStack.empty()) {
        return true;
    }
    Stepdata cur_step = stepStack.top();
    stepStack.pop();

    // New Hull-Point
    const Point* new_hullpt = findFarthestPoint(cur_step.linestart, cur_step.lineend, cur_step.remainingpoints);
    // Add new Lines to Stack
    if (graphicMode) {
        if (this->lines.size()+this->convex_hull.size()>1) {
            for (auto it = lines.begin(); it != lines.end(); ) {
                if ((it->first == &cur_step.linestart) && (it->second == &cur_step.lineend)) {
                    it = lines.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    bool status = addStepStack(cur_step.linestart, *new_hullpt, cur_step.remainingpoints);
    if (!status) {
        this->convex_hull.emplace_back(&cur_step.linestart, new_hullpt);

    } else if (graphicMode) {
        this->lines.emplace_back(&cur_step.linestart, new_hullpt);
    }
    status = addStepStack(*new_hullpt, cur_step.lineend, cur_step.remainingpoints);
    if (!status) {
        this->convex_hull.emplace_back(new_hullpt, &cur_step.lineend);
    } else if (graphicMode) {
        this->lines.emplace_back(new_hullpt, &cur_step.lineend);
    }
    if (stepStack.empty()) {
        // Finished
        return true;
    }
    return false;

};