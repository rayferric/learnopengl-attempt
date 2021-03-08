#ifndef EYESPARK_PANORAMASKY_H
#define EYESPARK_PANORAMASKY_H

#include "../../resource/sky/Sky.h"

class PanoramaSky : public Sky {
private:
    Texture2D *panorama;
public:
    PanoramaSky(int irradianceSize, Texture2D *panorama) : Sky(irradianceSize) {
        this->panorama = panorama;
        update();
    }

    void update() override {

    }
};

#endif // EYESPARK_PANORAMASKY_H
