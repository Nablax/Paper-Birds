#pragma once
#include "particle_base.hpp"
namespace boids {

	const float kVertices[] = {
		// positions          // Normal           // texture coords
		 0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int kIndices[] = {
		0, 3, 1, // first triangle
		1, 3, 2  // second triangle
	};

	const std::string kDefaultVsPath = "shader/boids.vs";
	const std::string kDefaultFsPath = "shader/boids.fs";
	const std::string kDefaultTexture = "texture/paperbird.png";

	unsigned int VAO = 0, VBO = 0, EBO = 0;
	const int kNumParticles = 1000;

	const int kRadiusCohesion = 10;
	const int kRadiusAlignment = 10;
	const int kRadiusSeperation = 5;
	const float initSphereRange = 20.0f;
	const float kMinDistance = 0.1f;

	const float kMaxSpeed = 15;
	const float kMaxCohesionForce = 1;
	const float kMaxSeparationForce = 1;
	const float kMaxAlignmentForce = 1;
	const float kMaxToCenterForce = 1;

	glm::vec3 center(0.0f);
	const float kCohesionFactor = 0.5;
	const float kAlignmentFactor = 0.5;
	const float kSeperationFactor = 2.5;
	const float kToCenterFactor = 2;

	void genBoids() {
		if (VAO != 0)
			return;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndices), kIndices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(0);
	}
	struct myBoids : particle::baseParticle {

	};

	class MyBoids : public particle::Particle<myBoids> {
	public:
		MyBoids(int numParticles = kNumParticles) {
			boids::genBoids();
			mVAO = VAO;
			mVBO = VBO;
			mEBO = EBO;
			mNumParticles = numParticles;
			particles.resize(mNumParticles);
			particleModel = new glm::mat4[mNumParticles];
			mTexture = textureutils::loadTexture(kDefaultTexture, true);
			initParticles(mNumParticles);
		}
		explicit MyBoids(const std::shared_ptr<shader::MyShader> &inShader): MyBoids(){
			linkShader(inShader);
			mShader->use();
			mShader->setInt("texture_diffuse1", 0);
		}
		~MyBoids()
		{
			delete[] particleModel;
		}
	private:
		glm::mat4 *particleModel;
		bool curParticleModel = 0;
		unsigned int instanceBuffer = 0;
		void initParticles(int initParticle) override {
			srand(glfwGetTime());
			float randomScale;
			float phi, theta;
			glm::vec3 initVel, initPos;
			//initVel = glm::vec3(0.01, 0.01, 0.0);
			initVel = glm::vec3(randomgenerator::getUniformRandom(-1, 1), randomgenerator::getUniformRandom(-1, 1), randomgenerator::getUniformRandom(-1, 1));
			for (int i = 0; i < mNumParticles; i++) {
				
				particleModel[i] = glm::mat4(1.0f);
				randomScale = sqrt(randomgenerator::getUniformZeroToOne());
				theta = static_cast<float>(i)/ mNumParticles * constvalue::kTwoPiDegree;
				phi = sqrt(randomgenerator::getUniformZeroToOne()) * constvalue::kTwoPiDegree;
				initPos = glm::vec3(sin(theta) * cos(phi), sin(phi), cos(theta) * cos(phi)) * randomScale * initSphereRange;
				particleModel[i] = glm::translate(particleModel[i], initPos);
				particles[i].position = initPos;
				particleModel[i] = glm::rotate(particleModel[i], glm::radians(90.0f), initVel);	
				particles[i].velocity = initVel;
				particles[i].acceleration = glm::vec3(0.0);
			}

			glGenBuffers(1, &instanceBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
			glBufferData(GL_ARRAY_BUFFER, mNumParticles * sizeof(glm::mat4), &particleModel[0], GL_STATIC_DRAW);

			glBindVertexArray(mVAO);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}
		void updateParticles(float deltaTime) override {
			glm::vec3 deltaPostion, deltaVelocity;
			float distance, cohesionCount, alignmentCount, separationCount;
			glm::vec3 cohesionPosition, alignmentVelocity, separationForce;
			cohesionPosition = alignmentVelocity = separationForce = glm::vec3(0);
			for (int i = 0; i < mNumParticles; i++) {
				cohesionCount = alignmentCount = separationCount = 0;
				for (int j = 0; j < mNumParticles; j++) {
					if (i == j)
						continue;
					distance = glm::distance(particles[i].position, particles[j].position);
					if (distance < kRadiusCohesion) {
						cohesionPosition += particles[j].position;
						cohesionCount++;
					}
					if (distance < kRadiusAlignment) {
						alignmentVelocity += particles[i].velocity;
						alignmentCount++;
					}
					if (distance < kRadiusSeperation) {
						if (distance > 0) {
							if (distance < kMinDistance)
								distance = kMinDistance;
							separationForce += glm::normalize(particles[i].position - particles[j].position) / distance;
							separationCount++;
						}
					}
				}
				//std::cout << cohesionCount << " " << alignmentCount << " " << separationCount << std::endl;
				//std::cout << i << " ";
				if (cohesionCount > 0) {
					cohesionPosition /= cohesionCount;
					auto attractionForce = (cohesionPosition - particles[i].position);
					attractionForce /= (kRadiusCohesion * 2);
					attractionForce *= kCohesionFactor;
					clampVector3(attractionForce, kMaxCohesionForce);
					particles[i].acceleration += attractionForce ;
					//std::cout << "cohesion: " << attractionForce.x << " ";
				}
				if (alignmentCount > 0) {
					alignmentVelocity /= alignmentCount * kMaxSpeed;
					auto alignmentForce = alignmentVelocity * kAlignmentFactor;
					clampVector3(alignmentForce, kMaxAlignmentForce);
					particles[i].acceleration += alignmentForce;
					//std::cout << "align: " << alignmentForce.x << " ";
				}
				if (separationCount > 0) {
					separationForce /= separationCount;
					separationForce *= kSeperationFactor;
					clampVector3(separationForce, kMaxSeparationForce);
					particles[i].acceleration += separationForce;
					//std::cout << "seperate: " << separationForce.x << " ";
				}
				glm::vec3 toCenterForce = center - particles[i].position;
				if (glm::length(toCenterForce) > 0) {
					particles[i].acceleration += glm::normalize(toCenterForce) * kMaxToCenterForce * kToCenterFactor;
					//std::cout <<"tocenter: " << particles[i].acceleration.x << " ";
				}
				//std::cout << std::endl;
			}
			for (int i = 0; i < mNumParticles; i++) {
				//std::cout << i << "position: " << particles[i].position.x << " acc: " << particles[i].acceleration.x << " vel: " << particles[i].velocity.x << std::endl;
				auto deltaPosition = particles[i].velocity * deltaTime;
				particles[i].position += deltaPosition;
				particles[i].velocity += particles[i].acceleration * deltaTime;
				clampVector3(particles[i].velocity, kMaxSpeed);
				particles[i].acceleration = glm::vec3(0);
				particleModel[i] = glm::inverse(glm::lookAt(particles[i].position, particles[i].position + particles[i].velocity, glm::vec3(0.0f, 1.0f, 0.0f)));
				particleModel[i] = glm::rotate(particleModel[i], (float)90, glm::vec3(-1.0f, 0.0f, 0.0f));
			}

			glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
			glBufferData(GL_ARRAY_BUFFER, mNumParticles * sizeof(glm::mat4), &particleModel[0], GL_STATIC_DRAW);

		}
		void renderParticles(glm::mat4 &projection, glm::mat4 &view) override {
			mShader->use();
			mShader->setMat4("projection", projection);
			mShader->setMat4("view", view);
			mShader->setVec3("lightPos", constvalue::kLightPosition);
			mShader->setVec3("lightColor", constvalue::kLightColor);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mTexture);
			glBindVertexArray(mVAO);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			mShader->setBool("renderFace", constvalue::kBackFace);
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, mNumParticles);
			glCullFace(GL_BACK);
			mShader->setBool("renderFace", constvalue::kFrontFace);
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, mNumParticles);
			glDisable(GL_CULL_FACE);
			glBindVertexArray(0);
		}
	};
}