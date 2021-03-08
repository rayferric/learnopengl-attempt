#ifndef EYESPARK_RESOURCE_H
#define EYESPARK_RESOURCE_H

#include "../util/String.h"

class Resource {
private:
    String path;
    String name;
public:
    Resource(String path, String name) {
        this->path = path;
        this->name = name;
    }
};

#endif // EYESPARK_RESOURCE_H
