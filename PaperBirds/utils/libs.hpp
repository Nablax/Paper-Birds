//
// Created by cosmos on 8/21/20.
//
#ifndef TEST_LIBS_HPP
#define TEST_LIBS_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace constvalue{
    const double gravity = 9.8;
    const glm::vec3 kFront(0.0f, 0.0f, -1.0f);
    const glm::vec3 kWorldUp(0.0f, 1.0f, 0.0f);
    const unsigned int kScreenWidth = 1920;
    const unsigned int kScreenHeight = 1080;
	const glm::vec3 kLightPosition(0.0f, 25.0f, 750.0f);
	const glm::vec3 kLightColor(250.0f / 255, 244.0f / 255, 218.0f / 255);
	const bool kFrontFace = 1, kBackFace = 0;
	const float kTwoPiDegree = 360;
}

namespace randomgenerator {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	double getUniformZeroToOne() {
		return dist(mt);
	}
	double getUniformRandom(float low = 0.0, float high = 1.0) {
		return dist(mt) * (high - low) + low;
	}
}

#endif //TEST_LIBS_HPP
