#ifndef EYESPARK_BASICMATERIAL_H
#define EYESPARK_BASICMATERIAL_H

#include "../../resource/material/Material.h"
#include "../../resource/texture/Texture2D.h"
#include "../../resource/Shader.h"
#include "../../util/Matrix4.h"
#include "../../util/Vector3.h"
#include "../../util/Vector4.h"

class BasicMaterial : public Material {
private:
    int uAlbedoLocation;
    int uMetallicLocation;
    int uRoughnessLocation;
    int uEmissionLocation;
    int uUseNormalMapLocation;
    int uUseAlbedoMapLocation;
    int uUseMetallicMapLocation;
    int uUseRoughnessMapLocation;
    int uUseAoMapLocation;
    int uUseEmissionMapLocation;
public:
    Vector4 albedo;
    float metallic;
    float roughness;
    Vector3 emission;
    Texture2D *normalMap = nullptr;
    Texture2D *albedoMap = nullptr;
    Texture2D *metallicMap = nullptr;
    Texture2D *roughnessMap = nullptr;
    Texture2D *aoMap = nullptr;
    Texture2D *emissionMap = nullptr;

    BasicMaterial(Vector4 albedo, float metallic, float roughness, Vector3 emission) : Material(new Shader("shaders/basic.glsl")) {
        this->albedo = albedo;
        this->metallic = metallic;
        this->roughness = roughness;
        this->emission = emission;

        uAlbedoLocation = shader->getUniformLocation("uAlbedo");
        uMetallicLocation = shader->getUniformLocation("uMetallic");
        uRoughnessLocation = shader->getUniformLocation("uRoughness");
        uEmissionLocation = shader->getUniformLocation("uEmission");

        Shader::setUniform(shader->getUniformLocation("uNormalMap"), 0);
        Shader::setUniform(shader->getUniformLocation("uAlbedoMap"), 1);
        Shader::setUniform(shader->getUniformLocation("uMetallicMap"), 2);
        Shader::setUniform(shader->getUniformLocation("uRoughnessMap"), 3);
        Shader::setUniform(shader->getUniformLocation("uAoMap"), 4);
        Shader::setUniform(shader->getUniformLocation("uEmissionMap"), 5);

        uUseNormalMapLocation = shader->getUniformLocation("uUseNormalMap");
        uUseAlbedoMapLocation = shader->getUniformLocation("uUseAlbedoMap");
        uUseMetallicMapLocation = shader->getUniformLocation("uUseMetallicMap");
        uUseRoughnessMapLocation = shader->getUniformLocation("uUseRoughnessMap");
        uUseAoMapLocation = shader->getUniformLocation("uUseAoMap");
        uUseEmissionMapLocation = shader->getUniformLocation("uUseEmissionMap");
    }

    void bind(Matrix4 modelMatrix, Matrix4 viewMatrix, Matrix4 projectionMatrix) override {
        Material::bind(modelMatrix, viewMatrix, projectionMatrix);

        Shader::setUniform(uUseNormalMapLocation, normalMap != nullptr);
        Shader::setUniform(uUseAlbedoMapLocation, albedoMap != nullptr);
        Shader::setUniform(uUseMetallicMapLocation, metallicMap != nullptr);
        Shader::setUniform(uUseRoughnessMapLocation, roughnessMap != nullptr);
        Shader::setUniform(uUseAoMapLocation, aoMap != nullptr);
        Shader::setUniform(uUseEmissionMapLocation, emissionMap != nullptr);
        if(normalMap) {
            glActiveTexture(GL_TEXTURE0);
            normalMap->bind();
        }
        if(albedoMap) {
            glActiveTexture(GL_TEXTURE1);
            albedoMap->bind();
        }
        if(metallicMap) {
            glActiveTexture(GL_TEXTURE2);
            metallicMap->bind();
        }
        if(roughnessMap) {
            glActiveTexture(GL_TEXTURE3);
            roughnessMap->bind();
        }
        if(aoMap) {
            glActiveTexture(GL_TEXTURE4);
            aoMap->bind();
        }
        if(emissionMap) {
            glActiveTexture(GL_TEXTURE5);
            emissionMap->bind();
        }

        Shader::setUniform(uAlbedoLocation, albedo);
        Shader::setUniform(uMetallicLocation, metallic);
        Shader::setUniform(uRoughnessLocation, roughness);
        Shader::setUniform(uEmissionLocation, emission);
    }
};

#endif // EYESPARK_BASICMATERIAL_H
