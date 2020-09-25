//
// Created by cosmos on 9/5/20.
//

#ifndef TEST_SPHERE_HPP
#define TEST_SPHERE_HPP

#include "base.hpp"
namespace sphere{
    unsigned int VAO = 0, VBO = 0;
    unsigned int sphereTriangle;

    const std::string kDefaultVsPath = "shader/sphere.vs";
    const std::string kDefaultFsPath = "shader/sphere.fs";
    const std::string kDefaultTexture = "texture/container.jpg";

    void genSphere()
    {
        if(sphere::VAO)
            return;
        unsigned int vbo, ebo;
        glGenVertexArrays(1, &sphere::VAO);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.emplace_back(xPos, yPos, zPos);
                uv.emplace_back(xSegment, ySegment);
                normals.emplace_back(xPos, yPos, zPos);
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        sphereTriangle = indices.size();

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (!uv.empty())
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
            if (!normals.empty())
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
        }
        glBindVertexArray(sphere::VAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        float stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    }

    class MySphere : public base{
    public:
        MySphere(){
            genSphere();
            mVBO = sphere::VBO;
            mVAO = sphere::VAO;
            mNumTriangle = sphere::sphereTriangle;
        }
        explicit MySphere(const std::shared_ptr<shader::MyShader> &inShader):MySphere(){linkShader(inShader);}
        void render(glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model) override{
            if(!mShader){
                std::cerr << "Shader not linked, using linkShader()!" << std::endl;
                return;
            }
            mShader->use();
            mShader->setMat4("view", view);
            mShader->setMat4("projection", projection);
            mShader->setMat4("model", model);
            glBindVertexArray(mVAO);
            if(mTexture){
                activate2DTexture(0, mTexture);
            }
            glDrawElements(GL_TRIANGLE_STRIP, sphereTriangle, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    };
};

#endif //TEST_SPHERE_HPP
