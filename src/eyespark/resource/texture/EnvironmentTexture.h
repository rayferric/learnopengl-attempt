#ifndef EYESPARK_ENVIRONMENTTEXTURE_H
#define EYESPARK_ENVIRONMENTTEXTURE_H

#include "../../resource/mesh/Mesh.h"
#include "../../resource/mesh/CubeMesh.h"
#include "../../resource/texture/CubeMapTexture.h"
#include "../../resource/Shader.h"
#include "../../util/Error.h"
#include "../../util/Logger.h"
#include "../../util/Matrix4.h"
#include "../../util/String.h"
#include "../../util/Vector3.h"
#include "../../util/WinAPI.h"

class EnvironmentTexture : public CubeMapTexture {
public:
    EnvironmentTexture(String path, int resolution) {
        this->resolution = resolution;
        //Load temporary equirectangular texture in HDR format
        unsigned int equirect;
        glGenTextures(1, &equirect);
        glBindTexture(GL_TEXTURE_2D, equirect);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);
        int width, height, channels;
        float *data = stbi_loadf(path.data(), &width, &height, &channels, 0);
        if(!data) {
            String args[1] = { WinAPI::fullPath(path) };
            Error::error(Error::FILE_OPENING_ERROR, 1, args);
        }
        if(channels == 3)glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
        else if(channels == 4)glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, data);
        stbi_image_free(data);
        //Prepare for rendering
        Mesh *cube = new CubeMesh(2, true);
        cube->bind();
        Shader *shader = new Shader("shaders/equirect.glsl");
        shader->bind();
        Shader::setUniform(shader->getUniformLocation("uEquirectMap"), 0);
        Shader::setUniform(shader->getUniformLocation("uProjectionMatrix"), Matrix4::perspective(90, 1, 0.1, 10));
        int viewMatrixLocation = shader->getUniformLocation("uViewMatrix");
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
        //Render the cube map
        Matrix4 viewMatrices[6] = {
                Matrix4::transform(Vector3(0), Vector3(  0, -90, 180), Vector3(1)),
                Matrix4::transform(Vector3(0), Vector3(  0,  90, 180), Vector3(1)),
                Matrix4::transform(Vector3(0), Vector3(-90,   0,   0), Vector3(1)),
                Matrix4::transform(Vector3(0), Vector3( 90,   0,   0), Vector3(1)),
                Matrix4::transform(Vector3(0), Vector3(  0, 180, 180), Vector3(1)),
                Matrix4::transform(Vector3(0), Vector3(  0,   0, 180), Vector3(1))
        };
        glViewport(0, 0, resolution, resolution);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, equirect);
        for(unsigned int i=0; i<6; i++) {
            Shader::setUniform(viewMatrixLocation, viewMatrices[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture, 0);
            glDrawElements(GL_TRIANGLES, cube->getIndexCount(), GL_UNSIGNED_INT, 0);
        }
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        //Clean up
        glDeleteTextures(1, &equirect);
        delete cube;
        delete shader;
        glDeleteFramebuffers(1, &framebuffer);
    }
};

#endif // EYESPARK_ENVIRONMENTTEXTURE_H
