#include "eyespark/EyeSpark.h"
#include "node/Player.h"

using namespace EyeSpark;

int main(int argc, char *argv[]) {
    Window::init("App", 960, 540);
    Scene *scene = new Scene(new Environment());

    ////////////////////////////////////////////

    Input::addActionKey("forward", 0x57);
    Input::addActionKey("backward", 0x53);
    Input::addActionKey("left", 0x41);
    Input::addActionKey("right", 0x44);
    Input::addActionKey("jump", VK_SPACE);
    Input::addActionKey("snake", VK_LSHIFT);

    scene->environment->effects.add(new SSREffect());
    scene->environment->effects.add(new Effect(new Shader("shaders/tonemap.glsl")));
    CubeMapTexture *tempIrradianceTexture = new EnvironmentTexture("textures/environment/helipad.hdr", 1024);
    scene->environment->background = new IrradianceTexture(tempIrradianceTexture, 1024);
    delete tempIrradianceTexture;

    scene->camera = new Player();
    scene->requestScript(scene->camera);

    Light *light = new DirectionalLight(Vector3(2.0), Vector3(0.02));
    light->rotation.x = -30;
    light->rotation.y = -30;
    light->updateTransform();
    //scene->add(light);
    Light *light2 = new DirectionalLight(Vector3(2.0), Vector3(0.02));
    light2->rotation.x = -30;
    light2->rotation.y = 30;
    light2->updateTransform();
    //scene->add(light2);

    BasicMaterial *groundMaterial = new BasicMaterial(Vector4(1.0), 1.0, 1.0, Vector3(0.0));
    groundMaterial->normalMap = new ImageTexture("textures/test_n.png", TextureFilter::TRILINEAR);
    groundMaterial->albedoMap = new ImageTexture("textures/test_a.png", TextureFilter::TRILINEAR);
    groundMaterial->metallicMap = new ImageTexture("textures/test_mra.png", TextureFilter::TRILINEAR);
    groundMaterial->roughnessMap = new ImageTexture("textures/test_mra.png", TextureFilter::TRILINEAR);
    groundMaterial->aoMap = new ImageTexture("textures/test_mra.png", TextureFilter::TRILINEAR);
    Model *groundModel = new Model(new ArrayMesh("models/plane.msh"), groundMaterial);
    scene->add(groundModel);

    Model *sphereModel = new Model(new ArrayMesh("models/sphere.msh"), new BasicMaterial(Vector4(1.0), 1.0, 0.1, Vector3(0.0)));
    sphereModel->translation.y = 1;
    sphereModel->updateTransform();
    scene->add(sphereModel);

    /*Model *alienModel = new Model(new ArrayMesh("models/alien.msh"), new BasicMaterial(Vector4(0.9,0.9,0.9,1.0), 0.2, 0.7, Vector3(0.0)));
    alienModel->translation = Vector3(1, 0, 2);
    alienModel->updateTransform();
    scene->add(alienModel);*/
    /*Mesh *mesh = new ArrayMesh("models/sphere.msh");
    int gridSize = 7;
    for(int i=0; i<gridSize; i++) {
        for(int j=0; j<gridSize; j++) {
            BasicMaterial *material = new BasicMaterial(Vector4(1.0), (float)j/gridSize, (float)i/gridSize, Vector3(0.0));
            Model *model = new Model(mesh, material);
            model->translation.x = (i-gridSize/2)*3;
            model->translation.y = (j-gridSize/2)*3;
            model->updateTransform();
            scene->add(model);
        }
    }*/

    /*BasicMaterial *cerberusMaterial = new BasicMaterial(Vector4(1.0), 1.0, 1.0, Vector3(0.0));
    cerberusMaterial->normalMap = new ImageTexture("textures/cerberus_n.png", TextureFilter::TRILINEAR);
    cerberusMaterial->albedoMap = new ImageTexture("textures/cerberus_a.png", TextureFilter::TRILINEAR);
    cerberusMaterial->metallicMap = new ImageTexture("textures/cerberus_m.png", TextureFilter::TRILINEAR);
    cerberusMaterial->roughnessMap = new ImageTexture("textures/cerberus_r.png", TextureFilter::TRILINEAR);
    cerberusMaterial->aoMap = new ImageTexture("textures/cerberus_a.png", TextureFilter::TRILINEAR);
    Model *cerberusModel = new Model(new ArrayMesh("models/cerberus.msh"), cerberusMaterial);
    scene->add(cerberusModel);*/




    ////////////////////////////////////////////

    int w, h;
    Window::getClientSize(&w, &h);
    Window::renderer = new Renderer(w, h);
    Window::camera = scene->camera;
    scene->init();
    Timer delta, fpsCounterTimer;
    delta.start();
    fpsCounterTimer.start();
    int frameCount = 0;
    while(Window::isOpen()) {
        Window::process();

        if(fpsCounterTimer.getElapsed() > 2000) {
            fpsCounterTimer.reset();
            Logger::getInstance().info("FPS: " + String::toString(frameCount/2));
            frameCount = 0;
        } else frameCount++;

        scene->process(delta.getElapsed() / 1000);
        delta.reset();

        Window::renderer->render(scene);

        Window::swapBuffers();
    }

    ////////////////////////////////////////////

    delete scene->environment->background;
    delete scene->camera;

    ////////////////////////////////////////////

    delete Window::renderer;
    delete scene->environment;
    delete scene;
    Window::terminate();
    return 0;
}
