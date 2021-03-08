#ifndef EYESPARK_SCENE_H
#define EYESPARK_SCENE_H

#include "../node/Model.h"
#include "../node/Node.h"
#include "../util/List.h"

class Scene {
private:
    List<Model *> models;
    List<DirectionalLight *> directionalLights;
    List<PointLight *> pointLights;
    List<SpotLight *> spotLights;
    List<Node *> scripted;
public:
    Camera *camera = nullptr;
    Environment *environment;

    Scene(Environment *environment) {
        this->environment = environment;
    }

    void add(Node *node) {
        if(Model *model = dynamic_cast<Model *>(node)) {
            if(models.find(model) < 0)models.add(model);
        }
        else if(DirectionalLight *light = dynamic_cast<DirectionalLight *>(node)) {
            if(directionalLights.find(light) < 0)directionalLights.add(light);
        }
        else if(PointLight *light = dynamic_cast<PointLight *>(node)) {
            if(pointLights.find(light) < 0)pointLights.add(light);
        }
        else if(SpotLight *light = dynamic_cast<SpotLight *>(node)) {
            if(spotLights.find(light) < 0)spotLights.add(light);
        }
        for(Node *child : node->getChildren()) {
            add(child);
        }
    }
    List<Model *> getModels() {
        return models;
    }
    List<DirectionalLight *> getDirectionalLights() {
        return directionalLights;
    }
    List<PointLight *> getPointLights() {
        return pointLights;
    }
    List<SpotLight *> getSpotLights() {
        return spotLights;
    }
    void requestScript(Node *node) {
        if(scripted.find(node) < 0)scripted.add(node);
    }
    void init() {
        for(Node *node : scripted)node->init();
    }
    void process(double delta) {
        for(Node *node : scripted)node->process(delta);
    }
};

#endif // EYESPARK_SCENE_H
