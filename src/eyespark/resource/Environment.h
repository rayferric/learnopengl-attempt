#ifndef EYESPARK_ENVIRONMENT_H
#define EYESPARK_ENVIRONMENT_H

#include "../resource/effect/Effect.h"
#include "../resource/texture/CubeMapTexture.h"
#include "../util/List.h"

class Environment {
public:
    List<Effect *> effects;
    CubeMapTexture *background = nullptr;
    bool ambientOcclusionEnabled = false;
    int ambientOcclusionSamples = 32;
    bool bloomEnabled = false;
    bool depthOfFieldEnabled = false;
    bool motionBlurEnabled = false;
    bool screenSpaceReflectionsEnabled = false;
    int screenSpaceReflectionsSamples = 32;

    Environment() {}
};

#endif // EYESPARK_ENVIRONMENT_H
