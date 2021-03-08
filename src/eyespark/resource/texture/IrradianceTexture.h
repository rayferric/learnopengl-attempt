#ifndef EYESPARK_IRRADIANCETEXTURE_H
#define EYESPARK_IRRADIANCETEXTURE_H

#include "../../resource/mesh/Mesh.h"
#include "../../resource/mesh/CubeMesh.h"
#include "../../resource/texture/CubeMapTexture.h"
#include "../../resource/Shader.h"
#include "../../util/Math.h"
#include "../../util/Matrix4.h"
#include "../../util/Vector3.h"

class IrradianceTexture : public CubeMapTexture {
public:
    IrradianceTexture(CubeMapTexture *cubeMap, int resolution) {
        this->resolution = resolution;
        //Prepare for rendering
        Mesh *cube = new CubeMesh(2, true);
        cube->bind();
        Shader *shader = new Shader("shaders/irradiance.glsl");
        shader->bind();
        Shader::setUniform(shader->getUniformLocation("uCubeMap"), 0);
        Shader::setUniform(shader->getUniformLocation("uProjectionMatrix"), Matrix4::perspective(90, 1, 0.1, 10));
        Shader::setUniform(shader->getUniformLocation("uResolution"), cubeMap->getResolution());
        int viewMatrixLocation = shader->getUniformLocation("uViewMatrix");
        int roughnessLocation = shader->getUniformLocation("uRoughness");
        //Create new framebuffer and the cube map
        unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        for(unsigned int i=0; i<6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, resolution, resolution, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        //Render the irradiance map
        Matrix4 viewMatrices[6] = {
                Matrix4::transform(Vector3(0), Vector3(  0, -90, 180), Vector3(1)),
                Matrix4::transform(Vector3(0), Vector3(  0,  90, 180), Vector3(1)),
                Matrix4::transform(Vector3(0), Vector3(-90,   0,   0), Vector3(1)),
                Matrix4::transform(Vector3(0), Vector3( 90,   0,   0), Vector3(1)),
                Matrix4::transform(Vector3(0), Vector3(  0, 180, 180), Vector3(1)),
                Matrix4::transform(Vector3(0), Vector3(  0,   0, 180), Vector3(1))
        };
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glActiveTexture(GL_TEXTURE0);
        cubeMap->bind();
        unsigned int mipmapLevels = 5;
        for(unsigned int level=0; level<mipmapLevels; level++) {
            glViewport(0, 0, resolution*Math::pow(0.5, level), resolution*Math::pow(0.5, level));
            Shader::setUniform(roughnessLocation, (float)level / (mipmapLevels - 1));
            for(unsigned int i=0; i<6; i++) {
                Shader::setUniform(viewMatrixLocation, viewMatrices[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture, level);
                glDrawElements(GL_TRIANGLES, cube->getIndexCount(), GL_UNSIGNED_INT, 0);
            }
        }
        //Clean up
        delete cube;
        delete shader;
        glDeleteFramebuffers(1, &framebuffer);
    }
};

#endif // EYESPARK_IRRADIANCETEXTURE_H
