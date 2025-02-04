#ifndef RELEASEMODE_H
#define RELEASEMODE_H

#include "Mode.h"
#include "Algorithm.h"

class ReleaseMode : public Mode {
    public:
        int runs = 1;
        ReleaseMode(Algorithm &algorithm) : Mode(algorithm) {};
        ReleaseMode(Algorithm &algorithm, int runs) : Mode(algorithm), runs(runs) {};
        ~ReleaseMode() override = default;
        void start();
};


#endif //RELEASEMODE_H
