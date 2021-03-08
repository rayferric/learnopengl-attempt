#ifndef EYESPARK_RENDERER_H
#define EYESPARK_RENDERER_H

#include "../node/light/DirectionalLight.h"
#include "../node/Model.h"
#include "../node/Node.h"
#include "../resource/effect/Effect.h"
#include "../resource/mesh/CubeMesh.h"
#include "../resource/mesh/Mesh.h"
#include "../resource/mesh/PlaneMesh.h"
#include "../resource/Environment.h"
#include "../resource/Scene.h"
#include "../resource/Shader.h"
#include "../util/Error.h"
#include "../util/Matrix4.h"
#include "../util/Vector2.h"
#include "../util/Vector3.h"
#include "../util/Vector4.h"

class Renderer {
private:
    int width;
    int height;

    Mesh *plane;
    Mesh *cube;

    unsigned int gBuffers[2];
    unsigned int gBufferRenderbuffers[2];
    unsigned int gBufferTextures[2][6];
    unsigned int brdfLUT;

    Shader *lightingShader;
    int lightingShaderLightTypeLocation;
    int lightingShaderLightColorLocation;
    int lightingShaderLightAmbientLocation;
    int lightingShaderLightDirectionLocation;
    int lightingShaderLightPositionLocation;
    int lightingShaderLightRangeLocation;
    int lightingShaderLightAngleLocation;
    int lightingShaderCameraPositionLocation;
    Shader *skyShader;
    int skyShaderViewMatrixLocation;
    int skyShaderProjectionMatrixLocation;
    Shader *environmentShader;
    int environmentShaderCameraPositionLocation;
    Shader *brdfLUTShader;

    void createGBuffers() {
        glGenFramebuffers(2, gBuffers);
        glGenRenderbuffers(2, gBufferRenderbuffers);
        for(int i=0; i<2; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, gBuffers[i]);

            glBindRenderbuffer(GL_RENDERBUFFER, gBufferRenderbuffers[i]);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gBufferRenderbuffers[i]);

            glGenTextures(6, gBufferTextures[i]);
            glBindTexture(GL_TEXTURE_2D, gBufferTextures[i][0]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
            glBindTexture(GL_TEXTURE_2D, gBufferTextures[i][1]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
            glBindTexture(GL_TEXTURE_2D, gBufferTextures[i][2]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glBindTexture(GL_TEXTURE_2D, gBufferTextures[i][3]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glBindTexture(GL_TEXTURE_2D, gBufferTextures[i][4]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glBindTexture(GL_TEXTURE_2D, gBufferTextures[i][5]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gBufferTextures[i][0], 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gBufferTextures[i][1], 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gBufferTextures[i][2], 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gBufferTextures[i][3], 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gBufferTextures[i][4], 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, gBufferTextures[i][5], 0);

            unsigned int attachments[6] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };
            glDrawBuffers(6, attachments);
            glReadBuffer(GL_COLOR_ATTACHMENT5);

            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                Error::error(Error::FRAMEBUFFER_CREATION_ERROR, 0, nullptr);
            }
        }
    }
    void deleteGBuffers() {
        glDeleteFramebuffers(2, gBuffers);
        glDeleteRenderbuffers(2, gBufferRenderbuffers);
        glDeleteTextures(6, gBufferTextures[0]);
        glDeleteTextures(6, gBufferTextures[1]);
    }
    void bindGBufferTextures(int index) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gBufferTextures[index][0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gBufferTextures[index][1]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gBufferTextures[index][2]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, gBufferTextures[index][3]);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, gBufferTextures[index][4]);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, gBufferTextures[index][5]);
    }
    void createLightingShader() {
        lightingShader = new Shader("shaders/lighting.glsl");
        lightingShader->bind();
        Shader::setUniform(lightingShader->getUniformLocation("gPosition"), 0);
        Shader::setUniform(lightingShader->getUniformLocation("gNormal"), 1);
        Shader::setUniform(lightingShader->getUniformLocation("gAlbedo"), 2);
        Shader::setUniform(lightingShader->getUniformLocation("gMRA"), 3);
        Shader::setUniform(lightingShader->getUniformLocation("gEmission"), 4);
        lightingShaderLightTypeLocation = lightingShader->getUniformLocation("uLight.type");
        lightingShaderLightColorLocation = lightingShader->getUniformLocation("uLight.color");
        lightingShaderLightAmbientLocation = lightingShader->getUniformLocation("uLight.ambient");
        lightingShaderLightDirectionLocation = lightingShader->getUniformLocation("uLight.direction");
        lightingShaderLightPositionLocation = lightingShader->getUniformLocation("uLight.position");
        lightingShaderLightRangeLocation = lightingShader->getUniformLocation("uLight.range");
        lightingShaderLightAngleLocation = lightingShader->getUniformLocation("uLight.angle");

        lightingShaderCameraPositionLocation = lightingShader->getUniformLocation("uCameraPosition");
    }
    void createSkyShader() {
        skyShader = new Shader("shaders/sky.glsl");
        skyShader->bind();
        Shader::setUniform(skyShader->getUniformLocation("uCubeMap"), 0);
        skyShaderViewMatrixLocation = skyShader->getUniformLocation("uViewMatrix");
        skyShaderProjectionMatrixLocation = skyShader->getUniformLocation("uProjectionMatrix");
    }
    void createEnvironmentShader() {
        environmentShader = new Shader("shaders/environment.glsl");
        environmentShader->bind();
        Shader::setUniform(environmentShader->getUniformLocation("gPosition"), 0);
        Shader::setUniform(environmentShader->getUniformLocation("gNormal"), 1);
        Shader::setUniform(environmentShader->getUniformLocation("gAlbedo"), 2);
        Shader::setUniform(environmentShader->getUniformLocation("gMRA"), 3);
        Shader::setUniform(environmentShader->getUniformLocation("gEmission"), 4);
        Shader::setUniform(environmentShader->getUniformLocation("gColor"), 5);
        Shader::setUniform(environmentShader->getUniformLocation("uIrradianceMap"), 6);
        Shader::setUniform(environmentShader->getUniformLocation("uBrdfLUT"), 7);

        environmentShaderCameraPositionLocation = environmentShader->getUniformLocation("uCameraPosition");
    }
    void generateBrdfLUT() {
        Shader *shader = new Shader("shaders/brdf.glsl");

        glGenTextures(1, &brdfLUT);
        glBindTexture(GL_TEXTURE_2D, brdfLUT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUT, 0);

        glViewport(0, 0, 512, 512);
        glDisable(GL_DEPTH_TEST);
        shader->bind();
        plane->bind();
        glDrawElements(GL_TRIANGLES, plane->getIndexCount(), GL_UNSIGNED_INT, 0);

        delete shader;
        glDeleteFramebuffers(1, &framebuffer);
    }
public:
    Renderer(int width, int height) {
        this->width = width;
        this->height = height;
        plane = new PlaneMesh(Vector2(2));
        cube = new CubeMesh(Vector3(2), true);
        createGBuffers();
        createLightingShader();
        createSkyShader();
        createEnvironmentShader();
        generateBrdfLUT();
    }

    void resize(int width, int height) {
        this->width = width < 1 ? 1 : width;
        this->height = height < 1 ? 1 : height;
        deleteGBuffers();
        createGBuffers();
    }
    void render(Scene *scene) {
        //GATHER REQUIRED VALUES
        Matrix4 cameraTransform = scene->camera->getGlobalTransform();
        Matrix4 viewMatrix = cameraTransform.inverse();
        Matrix4 projectionMatrix = scene->camera->getProjection();
        Vector3 cameraPosition = Vector3(cameraTransform * Vector4(0, 0, 0, 1));
        //RENDER MATERIAL DATA
        glViewport(0, 0, width, height);
        glClearColor(0, 0, 0, 1);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glDepthFunc(GL_LEQUAL);
        glCullFace(GL_BACK);
        glBlendFunc(GL_ONE, GL_ZERO);

        glBindFramebuffer(GL_FRAMEBUFFER, gBuffers[0]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(Model *model : scene->getModels()) {
            if(!model->isVisible())continue;
            model->material->bind(model->getGlobalTransform(), viewMatrix, projectionMatrix);
            if(!model->material->cullFace)glDisable(GL_CULL_FACE);
            else glEnable(GL_CULL_FACE);
            model->mesh->bind();
            glDrawElements(GL_TRIANGLES, model->mesh->getIndexCount(), GL_UNSIGNED_INT, 0);
        }
        //IRRADIANCE AND REFLECTIONS
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_ONE, GL_ONE);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffers[1]);
        glClear(GL_COLOR_BUFFER_BIT);
        plane->bind();
        bindGBufferTextures(0);

        if(scene->environment->background) { //To be changed when reflections probes go in
            glActiveTexture(GL_TEXTURE6);
            scene->environment->background->bind();
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D, brdfLUT);
            environmentShader->bind();
            Shader::setUniform(environmentShaderCameraPositionLocation, cameraPosition);
            glDrawElements(GL_TRIANGLES, plane->getIndexCount(), GL_UNSIGNED_INT, 0);
        }
        //CALCULATE LIGHTING
        lightingShader->bind();
        Shader::setUniform(lightingShaderCameraPositionLocation, cameraPosition);
        Shader::setUniform(lightingShaderLightTypeLocation, 0);
        for(DirectionalLight *light : scene->getDirectionalLights()) {
            if(!light->isVisible())continue;
            Shader::setUniform(lightingShaderLightColorLocation, light->color);
            Shader::setUniform(lightingShaderLightAmbientLocation, light->ambient);
            Shader::setUniform(lightingShaderLightDirectionLocation, Vector3(light->getGlobalTransform()*Vector4(0,0,-1,0)));
            glDrawElements(GL_TRIANGLES, plane->getIndexCount(), GL_UNSIGNED_INT, 0);
        }

        glBlendFunc(GL_ONE, GL_ZERO);
        //SKY RENDERING
        if(scene->environment->background) {
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);

            glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffers[0]);
            glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffers[1]);
            skyShader->bind();
            skyShader->setUniform(skyShaderViewMatrixLocation, viewMatrix);
            skyShader->setUniform(skyShaderProjectionMatrixLocation, projectionMatrix);
            glActiveTexture(GL_TEXTURE0);
            scene->environment->background->bind();
            cube->bind();
            glDrawElements(GL_TRIANGLES, cube->getIndexCount(), GL_UNSIGNED_INT, 0);

            glDisable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        }
        //POST PROCESSING USING PING-PONG BUFFERS
        for(int i=0; i<scene->environment->effects.length(); i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, gBuffers[i%2]);
            bindGBufferTextures((i+1)%2);
            scene->environment->effects[i]->bind(projectionMatrix, viewMatrix, cameraPosition);
            glDrawElements(GL_TRIANGLES, plane->getIndexCount(), GL_UNSIGNED_INT, 0);
        }
        //COPY THE RESULT TO MAIN FRAMEBUFFER
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

    ~Renderer() {
        delete plane;
        deleteGBuffers();
        delete lightingShader;
    }
};

#endif // EYESPARK_RENDERER_H
