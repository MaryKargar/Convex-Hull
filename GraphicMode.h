#ifndef GRAPHICMODE_H
#define GRAPHICMODE_H

#include <vector>
#include <utility>

#include "Mode.h"
#include "Algorithm.h"
#include <SFML/Graphics.hpp>

class GraphicMode : public Mode {
    private:
        static sf::FloatRect get_screen_size(const std::vector<std::pair<float,float>> &points);
    public:
        GraphicMode(Algorithm &algorithm) : Mode(algorithm) {
        }

        ~GraphicMode() = default;
        void start();
};

#endif //GRAPHICMODE_H
