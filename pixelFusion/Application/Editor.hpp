//
//  Editor.hpp
//  pixelFusion
//
//  Created by ミカエル on 07/05/2023.
//

#pragma once

#include "Application.hpp"

#include "Node.hpp"
#include <memory>
#include <list>

#ifdef __APPLE__
// Defined before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION
#endif
#define GLFW_INCLUDE_GLCOREARB
#define IMGUI_DEFINE_MATH_OPERATORS

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imnodes.h>

namespace pf
{
    class Editor : public Application
    {
    public:
        Editor();
        ~Editor();
        int Run(int argc, char** argv) override;
        bool OnUpdate() override;
    private:
        bool stop;
        GLFWwindow* window;
        ImGuiIO *io;
        static void resizeCallback(GLFWwindow* window, int width, int height);
        void onWindowSizeChanged(GLFWwindow* window, int width, int height);
        std::vector<std::shared_ptr<Node>> nodes;
        std::vector<std::tuple<int, int, int>> links;
        void Process();
        const int toolbarHeight;
    };
}
