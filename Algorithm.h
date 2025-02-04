#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <utility>
#include <vector>
#include <string>
#include <memory>

class Algorithm {
    protected:
        const std::shared_ptr<std::vector<std::pair<float,float>>> points;
        std::vector<std::pair<const std::pair<float, float>*, const std::pair<float, float>*>> lines;
        std::vector<std::pair<const std::pair<float, float>*, const std::pair<float, float>*>> convex_hull;
        bool graphicMode = false;

    public:
        explicit Algorithm(std::shared_ptr<std::vector<std::pair<float,float>>> newPoints, std::string name)
            : points(std::move(newPoints)), algorithmName(std::move(name)) {};
        virtual ~Algorithm() = default;

        virtual void initialize() = 0;
        virtual bool step() = 0;
        std::string algorithmName;

        std::vector<std::pair<const std::pair<float, float>*, const std::pair<float, float>*>> getLines();
        std::vector<std::pair<const std::pair<float, float>*, const std::pair<float, float>*>> getConvexHull();
        [[nodiscard]] const std::vector<std::pair<float, float>>& getPoints() const;
        void setGraphicMode(bool mode);
};



#endif //ALGORITHM_H
