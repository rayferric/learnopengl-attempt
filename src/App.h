#ifndef APP_H
#define APP_H

#include "eyespark/EyeSpark.h"
#include "node/Player.h"

class App : public EyeSpark::App {
private:
    void init() override {
        EyeSpark::Input::addActionKey("forward", 0x57);
        EyeSpark::Input::addActionKey("backward", 0x53);
        EyeSpark::Input::addActionKey("left", 0x41);
        EyeSpark::Input::addActionKey("right", 0x44);
        EyeSpark::Input::addActionKey("jump", VK_SPACE);
        EyeSpark::Input::addActionKey("snake", VK_LSHIFT);

        scene->environment->effects.add(new EyeSpark::SSREffect());
        EyeSpark::CubeMapTexture *tempIrradianceTexture = new EyeSpark::EnvironmentTexture("textures/environment/helipad.hdr", 256);
        scene->environment->background = new EyeSpark::IrradianceTexture(tempIrradianceTexture, 256);
        delete tempIrradianceTexture;

        scene->camera = new Player();
        scene->requestScript(scene->camera);

        EyeSpark::Light *light = new EyeSpark::DirectionalLight(EyeSpark::Vector3(2.0), EyeSpark::Vector3(0.02));
        light->rotation.x = -30;
        light->rotation.y = -30;
        light->updateTransform();
        //scene->add(light);
        EyeSpark::Light *light2 = new EyeSpark::DirectionalLight(EyeSpark::Vector3(2.0), EyeSpark::Vector3(0.02));
        light2->rotation.x = -30;
        light2->rotation.y = 30;
        light2->updateTransform();
        //scene->add(light2);

        EyeSpark::Model *modelGround = new EyeSpark::Model(new EyeSpark::PlaneMesh(EyeSpark::Vector2(50)), new EyeSpark::BasicMaterial(EyeSpark::Vector4(0.8,0.15,0.1, 1.0), 0.8, 0.2, EyeSpark::Vector3(0.0)));
        modelGround->translation.y = -1;
        modelGround->rotation.x = -90;
        modelGround->updateTransform();
        scene->add(modelGround);

        EyeSpark::Mesh *mesh = new EyeSpark::ArrayMesh("models/sphere.msh");
        EyeSpark::BasicMaterial *material = new EyeSpark::BasicMaterial(EyeSpark::Vector4(1.0), 1.0, 0.0, EyeSpark::Vector3(0.0));
        //material->normalMap = new EyeSpark::ImageTexture("textures/octostone_n.jpg", EyeSpark::TextureFilter::TRILINEAR);
        //material->albedoMap = new EyeSpark::ImageTexture("textures/octostone_a.jpg", EyeSpark::TextureFilter::TRILINEAR);
        //material->metallicMap = new EyeSpark::ImageTexture("textures/octostone_m.png", EyeSpark::TextureFilter::TRILINEAR);
        //material->roughnessMap = new EyeSpark::ImageTexture("textures/octostone_r.jpg", EyeSpark::TextureFilter::TRILINEAR);
        //material->aoMap = new EyeSpark::ImageTexture("textures/octostone_ao.jpg", EyeSpark::TextureFilter::TRILINEAR);
        EyeSpark::Model *model = new EyeSpark::Model(mesh, material);
        scene->add(model);

        EyeSpark::Model *model2 = new EyeSpark::Model(new EyeSpark::CubeMesh(EyeSpark::Vector3(2.0), false), new EyeSpark::BasicMaterial(EyeSpark::Vector4(0.1,0.2,0.8,1.0), 0.1, 0.8, EyeSpark::Vector3(0.0)));
        model2->translation = EyeSpark::Vector3(2, 0, 4);
        model2->updateTransform();
        scene->add(model2);
        /*EyeSpark::Mesh *mesh = new EyeSpark::ArrayMesh("models/sphere.msh");
        int gridSize = 7;
        for(int i=0; i<gridSize; i++) {
            for(int j=0; j<gridSize; j++) {
                EyeSpark::BasicMaterial *material = new EyeSpark::BasicMaterial(EyeSpark::Vector4(1.0), (float)j/gridSize, (float)i/gridSize, EyeSpark::Vector3(0.0));
                EyeSpark::Model *model = new EyeSpark::Model(mesh, material);
                model->translation.x = (i-gridSize/2)*3;
                model->translation.y = (j-gridSize/2)*3;
                model->updateTransform();
                scene->add(model);
            }
        }*/
    }
    void terminate() override {
        delete scene->environment->background;
        delete scene->camera;
    }
public:
    App() : EyeSpark::App("App", 960, 540) {}
};

#endif // APP_H
