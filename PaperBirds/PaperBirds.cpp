#include "utils/camera.hpp"
#include "render/skybox.hpp"
#include "render/cube.hpp"
#include "render/quad.hpp"
#include "render/sphere.hpp"
#include "utils/libs.hpp"
#include "particles/Boids.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow *window);
// camera
camera::MyCamera myCamera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

double lastMouseX, lastMouseY;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(constvalue::kScreenWidth, constvalue::kScreenHeight, "Paper birds", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	skybox::Skybox skybox;
	boids::MyBoids boids(std::make_shared<shader::MyShader>(boids::kDefaultVsPath, boids::kDefaultFsPath));

	std::vector<sphere::MySphere> sphereSets;
	sphereSets.emplace_back(std::make_shared<shader::MyShader>(sphere::kDefaultVsPath, sphere::kDefaultFsPath));
	sphereSets.emplace_back(std::make_shared<shader::MyShader>(sphere::kDefaultVsPath, sphere::kDefaultFsPath));

	int countFrame = 0;
	float frame = 0;
	std::shared_ptr<collisionParams> obj;
	glm::mat4 model;
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		if (deltaTime > 0) {
			frame += 1 / deltaTime;
		}
		countFrame++;
		if (countFrame == 100) {
			frame /= countFrame;
			std::cout << frame << std::endl;
			countFrame = 0;
		}
		lastFrame = currentFrame;
		processInput(window);
		glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = myCamera.getViewMatrix();
		glm::mat4 projection =
			glm::perspective(glm::radians(camera::kZoom), static_cast<float>(constvalue::kScreenWidth)
				/ static_cast<float>(constvalue::kScreenHeight), 0.1f, 1000.0f);



		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		//obj = std::make_shared<collisionParams>(constvalue::kTypeSphere, glm::vec3(0.0f, 25.0f, 750.0f), 0.5f);
		//obj->modelTransform(model);
		//sphereSets[0].render(projection, view, model);
		//boids.pushBackObstacles(std::move(obj));

		obj = std::make_shared<collisionParams>(constvalue::kTypeSphere, glm::vec3(0.0f), 2.0f);
		obj->modelTransform(model);
		sphereSets[1].render(projection, view, model);
		boids.pushBackObstacles(std::move(obj));

		boids.render(deltaTime, projection, view);

		glCullFace(GL_BACK);
		skybox.render(projection, view);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		myCamera.processKeyboardInput(camera::kForward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		myCamera.processKeyboardInput(camera::kBackward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		myCamera.processKeyboardInput(camera::kLeft, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		myCamera.processKeyboardInput(camera::kRight, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		boids::center = myCamera.getPosition();
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastMouseX = xPos;
		lastMouseY = yPos;
		firstMouse = false;
	}
	myCamera.processMouseMovement(xPos - lastMouseX, lastMouseY - yPos);
	lastMouseX = xPos;
	lastMouseY = yPos;
}


void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	myCamera.processMouseScroll(yOffset);
}
