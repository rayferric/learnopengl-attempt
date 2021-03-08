#ifndef EYESPARK_H
#define EYESPARK_H

#include <cmath>
//#include <pthread.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb_image.h"

namespace EyeSpark {

#include "node/camera/Camera.h"
#include "node/camera/OrthographicCamera.h"
#include "node/camera/PerspectiveCamera.h"
#include "node/light/DirectionalLight.h"
#include "node/light/Light.h"
#include "node/light/PointLight.h"
#include "node/light/SpotLight.h"
#include "node/Model.h"
#include "node/Node.h"
#include "renderer/Renderer.h"
#include "resource/effect/Effect.h"
#include "resource/effect/SSREffect.h"
#include "resource/material/BasicMaterial.h"
#include "resource/material/Material.h"
#include "resource/mesh/ArrayMesh.h"
#include "resource/mesh/CubeMesh.h"
#include "resource/mesh/Mesh.h"
#include "resource/mesh/PlaneMesh.h"
#include "resource/texture/CubeMapTexture.h"
#include "resource/texture/EnvironmentTexture.h"
#include "resource/texture/ImageTexture.h"
#include "resource/texture/IrradianceTexture.h"
#include "resource/texture/Texture.h"
#include "resource/texture/Texture2D.h"
#include "resource/Environment.h"
#include "resource/Scene.h"
#include "resource/Shader.h"
#include "util/Error.h"
#include "util/Input.h"
#include "util/List.h"
#include "util/Logger.h"
#include "util/Map.h"
#include "util/Math.h"
#include "util/Matrix4.h"
#include "util/Pair.h"
#include "util/String.h"
#include "util/Timer.h"
#include "util/Vector2.h"
#include "util/Vector3.h"
#include "util/Vector4.h"
#include "util/WinAPI.h"
#include "util/Window.h"

}

#endif // EYESPARK_H
