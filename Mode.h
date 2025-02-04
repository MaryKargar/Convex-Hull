#ifndef MODE_H
#define MODE_H

#include "Algorithm.h"

class Mode {
    protected:
        Algorithm &algorithm_;
    public:
        Mode(Algorithm &algorithm) : algorithm_(algorithm) {}
        virtual void start() = 0;

        virtual ~Mode() = default;
        Mode(const Mode&) = delete;
        Mode& operator=(const Mode&) = delete;
        Mode(Mode&&) = default;
        Mode& operator=(Mode&&) = default;
};

#endif //MODE_H
