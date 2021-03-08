#ifndef EYESPARK_IMAGETEXTURE_H
#define EYESPARK_IMAGETEXTURE_H

#include "../../resource/texture/Texture2D.h"
#include "../../util/Error.h"
#include "../../util/Logger.h"
#include "../../util/String.h"
#include "../../util/WinAPI.h"

class ImageTexture : public Texture2D {
public:
    ImageTexture(String path, TextureFilter filter) {
        glBindTexture(GL_TEXTURE_2D, texture);
        unsigned int glFilter;
        switch(filter) {
        default:
        case TextureFilter::NEAREST: glFilter = GL_NEAREST; break;
        case TextureFilter::BILINEAR: glFilter = GL_LINEAR; break;
        case TextureFilter::TRILINEAR: glFilter = GL_LINEAR_MIPMAP_LINEAR; break;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);

        if(stbi_is_hdr(path.data())) {
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
        } else {
            stbi_set_flip_vertically_on_load(true);
            int width, height, channels;
            unsigned char *data = stbi_load(path.data(), &width, &height, &channels, 0);
            if(!data) {
                String args[1] = { WinAPI::fullPath(path) };
                Error::error(Error::FILE_OPENING_ERROR, 1, args);
            }
            if(channels == 3)glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            else if(channels == 4)glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }

        glGenerateMipmap(GL_TEXTURE_2D);
    }
};

#endif // EYESPARK_IMAGETEXTURE_H
