#ifndef QUICKHULL_H
#define QUICKHULL_H

#include <stack>
#include "Algorithm.h"
using Point = std::pair<float, float>;

class QuickHull: public Algorithm {
    private:
        struct Stepdata {
            const Point& linestart;
            const Point& lineend;
            std::vector<const Point*> remainingpoints;
        };
        std::stack<Stepdata> stepStack;
        std::vector<const Point*> findPointsOnSide(const Point& l1, const Point& l2, const std::vector<Point*>& points, int side) const;

        static int findSide(const Point& l1, const Point& l2, const Point& pt);
        static float distanceLine(const Point& l1, const Point& l2, const Point& pt);

        const Point* findFarthestPoint(const Point& l1, const Point& l2, std::vector<const Point*>& points) const;
        bool addStepStack(const Point& l1, const Point& l2, const std::vector<const Point*>& points);
        bool addStepStack_bothdir(const Point& linestart, const Point& lineend, const std::vector<const Point *>& points);
public:
    QuickHull(std::shared_ptr<std::vector<Point>> newPoints)
            : Algorithm(std::move(newPoints),  "QuickHull") {};
        void initialize();
        bool step();
};



#endif //QUICKHULL_H
