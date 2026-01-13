#include <array>
#include "DrawScratchSpace.h"

class MonkeySprite {
public:
    //Can I do this in a nicer way?
    std::array<RGB, 64> GetSmileRGB() const {
        return {
            // Row 0
            RGB{0,0,0}, RGB{0,0,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{0,0,0}, RGB{0,0,0},
            // Row 1
            RGB{0,0,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{0,0,0},
            // Row 2
            RGB{255,255,0}, RGB{255,255,0}, RGB{1,1,1}, RGB{255,255,0}, RGB{255,255,0}, RGB{1,1,1}, RGB{255,255,0}, RGB{255,255,0},
            // Row 3
            RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0},
            // Row 4
            RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0},
            // Row 5
            RGB{255,255,0}, RGB{255,255,0}, RGB{1,1,1}, RGB{255,255,0}, RGB{255,255,0}, RGB{1,1,1}, RGB{255,255,0}, RGB{255,255,0},
            // Row 6
            RGB{0,0,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{1,1,1}, RGB{1,1,1}, RGB{255,255,0}, RGB{255,255,0}, RGB{0,0,0},
            // Row 7
            RGB{0,0,0}, RGB{0,0,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{255,255,0}, RGB{0,0,0}, RGB{0,0,0}
        };
    }
};