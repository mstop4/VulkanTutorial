#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>

class HelloTriangleApplication {
public:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;
    VkInstance instance;

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "Triangle!", nullptr, nullptr);
    }

    void initVulkan() {
        createInstance();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void createInstance() {
        // Init app info
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Triangle!";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        // Get available extensions
        std::cout << "Verifying extensions...\n\n";

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        std::cout << "Available extensions:\n";
        for (const auto& extension : extensions) {
            std::cout << '\t' << extension.extensionName << std::endl;
        }

        // Init create info
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;

        // Verify extensions
        std::cout << "\nEnabled extensions: \n";
        for (int i = 0; i < glfwExtensionCount; i++) {
            std::cout << '\t' << glfwExtensions[i];

            bool found = false;
            for (const auto& extension : extensions) {
                if (strcmp(extension.extensionName, glfwExtensions[i]) == 0) {
                    found = true;
                    break;
                }
            }

            if (found) {
                std::cout << " - OK\n";
            } else {
                std::cout << " - not found!" << std::endl;
                abortApp();
                throw std::runtime_error("Extension not found!");
            }
        }

        std::cout << "\nExtensions OK! Creating Vulkan instance...\n";

        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
        if (result != VK_SUCCESS) {
            abortApp();
            throw std::runtime_error("Failed to create instance!");
        }

        std::cout << "Success!" << std::endl;
    }

    void cleanup() {
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void abortApp() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}