//
// Created by cosmos on 9/7/20.
//

#ifndef TEST_PARTICLE_BASE_HPP
#define TEST_PARTICLE_BASE_HPP
#include "../render/base.hpp"
namespace particle{
    const float kParticleLauncher = 0.0f;
    const float kParticleShell = 1.0f;
    const glm::vec3 kMaxVelocity = glm::vec3(0.0,5.0,0.0);
    const glm::vec3 kMinVelocity = glm::vec3(0.0,3.0,0.0);
    const float kMaxLife = 2.0f * 1000;
    const float kMinLife = 1.0f * 1000;
    const float kParticleInitSize = 30.0f;
    const int kMaxParticle = 18000;
    const int kInitParticle = 10000;
    const glm::vec3 center(0.0f);
    const float r = 0.3f;

    struct baseParticle
    {
        glm::vec3 position;
        glm::vec3 velocity;
		glm::vec3 acceleration;
    };
    template<typename particleType>
    class Particle: public base{
    public:
        Particle() = default;
        virtual ~Particle() = default;
        void render(float deltaTime, glm::mat4& projection, glm::mat4 &view)
        {
			updateParticles(deltaTime);
            renderParticles(projection, view);
        }
    protected:
		virtual void initParticles(int initParticle) = 0;
        virtual void updateParticles(float deltaTime) = 0;
        virtual void renderParticles(glm::mat4 &projection, glm::mat4 &view) = 0;
		int mNumParticles;
        std::vector<particleType> mParticles;
    };
}

#endif //TEST_PARTICLE_BASE_HPP
