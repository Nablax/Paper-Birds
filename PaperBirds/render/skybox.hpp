//
// Created by cosmos on 8/3/20.
//
#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <utility>
#include "../utils/libs.hpp"
#include "../utils/shader.hpp"
#include "../utils/texture.hpp"
#include "base.hpp"

namespace skybox{
    const std::string kVsPath = "shader/skybox.vs";
    const std::string kFsPath = "shader/skybox.fs";
	const std::vector<std::string> kFaces{
		"texture/skybox/right.jpg",
		"texture/skybox/left.jpg",
		"texture/skybox/top.jpg",
		"texture/skybox/bottom.jpg",
		"texture/skybox/front.jpg",
		"texture/skybox/back.jpg"
	};
	const std::vector<std::string> kFaces1{
		"texture/skybox1/right.jpg",
		"texture/skybox1/left.jpg",
		"texture/skybox1/top.jpg",
		"texture/skybox1/bottom.jpg",
		"texture/skybox1/front.jpg",
		"texture/skybox1/back.jpg"
	};
	const std::vector<std::string> kFaces2{
		"texture/skybox2/right.png",
		"texture/skybox2/left.png",
		"texture/skybox2/top.png",
		"texture/skybox2/bottom.png",
		"texture/skybox2/front.png",
		"texture/skybox2/back.png"
	};
    const float kSkyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    const unsigned int kSkyboxTriangle = 36;
class Skybox: public base{
    public:
        explicit Skybox(std::vector<std::string> facePath = kFaces1, const std::string& vsp = skybox::kVsPath,
               const std::string& fsp = skybox::kFsPath){
            mTexture = textureutils::loadCubemap(std::move(facePath));
            mNumTriangle = skybox::kSkyboxTriangle;
            mShader = std::make_shared<shader::MyShader>(vsp, fsp);
            mShader->use();
            mShader->setInt("skybox", 0);
            activateSkyboxTexture(0, mTexture);
            glGenVertexArrays(1, &mVAO);
            glGenBuffers(1, &mVBO);
            glBindVertexArray(mVAO);
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(kSkyboxVertices), &kSkyboxVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        }
        virtual ~Skybox() = default;
        void render(glm::mat4 &projection, glm::mat4 &view){
            glDepthFunc(GL_LEQUAL);
            mShader->use();
            mShader->setMat4("view", glm::mat4(glm::mat3(view)));
            mShader->setMat4("projection", projection);
            glBindVertexArray(mVAO);
            glDrawArrays(GL_TRIANGLES, 0, mNumTriangle);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);
        }
        void activateSkyboxTexture(int textureOffset, unsigned textureID) const{
            glActiveTexture(GL_TEXTURE0 + textureOffset);
            glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        }
    private:

    };
}


#endif //RAY_TRACING_SKYBOX_HPP