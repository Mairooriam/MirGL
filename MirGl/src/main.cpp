#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

void errorCallback(int error, const char* description) {
    spdlog::error("GLFW Error {}: {}", error, description);
}

// Set up logging
void setupLogging() {
    auto console = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(console);
    spdlog::set_pattern("[%^%l%$] [%H:%M:%S] %v");
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Logging system initialized");
}

int main() {
    // Set up logging first
    setupLogging();
    
    spdlog::info("Starting OpenGL/GLFW test...");
    
    // Initialize GLFW
    if (!glfwInit()) {
        spdlog::critical("Failed to initialize GLFW");
        return -1;
    }
    
    // Set error callback
    glfwSetErrorCallback(errorCallback);
    
    // Configure GLFW
    spdlog::debug("Configuring GLFW with OpenGL 3.3 Core Profile");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create a window
    spdlog::debug("Creating window (800x600)");
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL/GLFW Test", NULL, NULL);
    if (!window) {
        spdlog::critical("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    
    // Make the window's context current
    glfwMakeContextCurrent(window);
    
    // Initialize GLAD
    spdlog::debug("Initializing GLAD");
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        spdlog::critical("Failed to initialize GLAD");
        glfwTerminate();
        return -1;
    }
    
    // Print OpenGL information
    spdlog::info("OpenGL Info:");
    spdlog::info("  Vendor: {}", (const char*)glGetString(GL_VENDOR));
    spdlog::info("  Renderer: {}", (const char*)glGetString(GL_RENDERER));
    spdlog::info("  Version: {}", (const char*)glGetString(GL_VERSION));
    
    // Set viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    spdlog::debug("Viewport set to {}x{}", width, height);
    
    // Main loop
    spdlog::info("Entering main loop");
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Poll for and process events
        glfwPollEvents();
        
        // Swap front and back buffers
        glfwSwapBuffers(window);
    }
    
    // Clean up
    spdlog::info("Cleaning up GLFW");
    glfwTerminate();
    spdlog::info("OpenGL/GLFW test completed successfully!");
    return 0;
}