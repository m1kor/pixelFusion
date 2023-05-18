//
//  Editor.cpp
//  pixelFusion
//
//  Created by ミカエル on 07/05/2023.
//

#include "Editor.hpp"
#include <algorithm>
#include <memory>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <Magick++.h>

#include "InputNode.hpp"
#include "OutputNode.hpp"
#include "GuassianBlurNode.hpp"
#include "MotionBlurNode.hpp"
#include "EmbossNode.hpp"
#include "CharcoalNode.hpp"
#include "RotateNode.hpp"
#include "RotateNode.hpp"
#include "UID.hpp"

using namespace Magick;

namespace pf
{
    Editor::Editor() : toolbarHeight(36)
    {
        stop = false;
    }

    Editor::~Editor()
    {
    
    }

    void Editor::resizeCallback(GLFWwindow* window, int width, int height) {
        Editor* handler = static_cast<Editor*>(glfwGetWindowUserPointer(window));
        handler->onWindowSizeChanged(window, width, height);
    }

    void Editor::onWindowSizeChanged(GLFWwindow* window, int width, int height)
    {
        if (!stop && !OnUpdate())
        {
            stop = true;
        }
    }

    int Editor::Run(int argc, char** argv)
    {
        InitializeMagick(*argv);
          
        if (!glfwInit())
        {
            return -1;
        }
        
    #ifdef __APPLE__
        /* We need to explicitly ask for a 3.2 context on OS X */
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif

        window = glfwCreateWindow(1280, 720, "pixelFusion", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
        
        glfwSetWindowSizeCallback(window, resizeCallback);
        glfwSetWindowUserPointer(window, this);
        
        // Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImNodes::CreateContext();
        io = &ImGui::GetIO();
        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();
        // Set Dear ImGui style
        ImGui::StyleColorsLight();
        ImNodes::StyleColorsLight();
        
        while (!glfwWindowShouldClose(window))
        {
            if (!stop && !OnUpdate())
            {
                break;
            }
        }
        
        ImNodes::DestroyContext();
        ImGui::DestroyContext();
        
        glfwTerminate();
        
        return 0;
    }

    bool Editor::OnUpdate()
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
       
        ImGui::SetNextWindowSize(ImVec2(io->DisplaySize.x, io->DisplaySize.y - toolbarHeight));
        ImGui::SetNextWindowPos(ImVec2(0, toolbarHeight));
        bool isOpen;
        ImGui::Begin("pixelFusion", &isOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize);
        
        int hoveredNodeId;
        ImNodes::IsNodeHovered(&hoveredNodeId);
        
        ImNodes::BeginNodeEditor();
        
        // Nodes
        bool requestUpdate = false;
        bool requestBlockInput = false;
        std::shared_ptr<Node> nodeToDelete = nullptr;
        for (std::shared_ptr<Node> node : nodes)
        {
            if (node->RequestBlockInput)
            {
                requestBlockInput = true;
            }
        }
        for (std::shared_ptr<Node> node : nodes)
        {
            if (!requestBlockInput && ImNodes::IsNodeSelected(node->Id) && hoveredNodeId == node->Id && ImGui::IsMouseDoubleClicked(0))
            {
                // Delete selected node on double click
                nodeToDelete = node;
            }
            else
            {
                node->Render();
                if (node->RequestUpdate)
                {
                    node->RequestUpdate = false;
                    requestUpdate = true;
                }
            }
        }
        if (nodeToDelete != nullptr)
        {
            // Delete all the links
            int attribute = nodeToDelete->OutputAttributId;
            links.erase(std::remove_if(links.begin(), links.end(), [attribute](const std::tuple<int, int, int>& tuple) {
                return std::get<1>(tuple) == attribute;
            }), links.end());
            for (int attribute : nodeToDelete->InputAttributIds)
            {
                links.erase(std::remove_if(links.begin(), links.end(), [attribute](const std::tuple<int, int, int>& tuple) {
                    return std::get<2>(tuple) == attribute;
                }), links.end());
            }
            // Delete the node
            int id = nodeToDelete->Id;
            nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [id](const std::shared_ptr<Node>& node) {
                return node->Id == id;
            }), nodes.end());
            Process();
        }
        if (requestUpdate)
        {
            Process();
        }
        
        // Links
        int linkToDelete = -1;
        for (std::tuple<int, int, int> link : links)
        {
            if (ImNodes::IsLinkSelected(std::get<0>(link)) && ImGui::IsMouseDoubleClicked(0))
            {
                // Delete selected link on double click
                linkToDelete = std::get<0>(link);
            }
            else
            {
                ImNodes::Link(std::get<0>(link), std::get<1>(link), std::get<2>(link));
            }
        }
        links.erase(std::remove_if(links.begin(), links.end(), [linkToDelete](const std::tuple<int, int, int>& tuple) {
            return std::get<0>(tuple) == linkToDelete;
        }), links.end());
        if (linkToDelete != -1)
        {
            Process();
        }

        ImNodes::EndNodeEditor();
        
        int attribute1, attribute2, linkId;
        if (ImNodes::IsLinkCreated(&attribute1, &attribute2)) // Link created
        {
            // Erase existing link if exists
            links.erase(std::remove_if(links.begin(), links.end(), [attribute2](const std::tuple<int, int, int>& tuple) {
                return std::get<2>(tuple) == attribute2;
            }), links.end());
            // Add new link
            links.push_back(std::tuple<int, int, int>(UID::Register("Link"), attribute1, attribute2));
            Process();
        }
        else if (ImNodes::IsLinkDestroyed(&linkId)) // Link destroyed
        {
            links.erase(std::remove_if(links.begin(), links.end(), [linkId](const std::tuple<int, int, int>& tuple) {
                return std::get<0>(tuple) == linkId;
            }), links.end());
            Process();
        }

        // GUI
        ImGui::SetNextWindowSize(ImVec2(io->DisplaySize.x, toolbarHeight));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Add node", &isOpen, ImGuiWindowFlags_NoTitleBar |ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        if (ImGui::Button("Input"))
        {
            nodes.push_back(std::make_shared<InputNode>());
        }
        ImGui::SameLine();
        if (ImGui::Button("Output"))
        {
            nodes.push_back(std::make_shared<OutputNode>());
        }
        ImGui::SameLine();
        if (ImGui::Button("Guassian blur"))
        {
            nodes.push_back(std::make_shared<GuassianBlurNode>());
        }
        ImGui::SameLine();
        if (ImGui::Button("Motion blur"))
        {
            nodes.push_back(std::make_shared<MotionBlurNode>());
        }
        ImGui::SameLine();
        if (ImGui::Button("Emboss"))
        {
            nodes.push_back(std::make_shared<EmbossNode>());
        }
        ImGui::SameLine();
        if (ImGui::Button("Charcoal"))
        {
            nodes.push_back(std::make_shared<CharcoalNode>());
        }
        ImGui::SameLine();
        if (ImGui::Button("Threshold"))
        {
            nodes.push_back(std::make_shared<RotateNode>());
        }
        ImGui::SameLine();
        if (ImGui::Button("Rotate"))
        {
            nodes.push_back(std::make_shared<RotateNode>());
        }
        ImGui::End();
        
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        return true;
    }
    
    void Editor::Process()
    {
        std::list<std::shared_ptr<Node>> traverseNodes;
        for (std::shared_ptr<Node> node : nodes)
        {
            if (auto inputNode = std::dynamic_pointer_cast<InputNode>(node))
            {
                traverseNodes.push_back(inputNode);
                node->InputImages.clear();
            }
            else
            {
                node->InputImages.clear();
                node->OutputImage = std::make_shared<Image>();
                node->Process();
            }
        }
        // Traverse
        while (!traverseNodes.empty())
        {
            std::shared_ptr<Node> node = traverseNodes.front();
            // Find all connected links
            for (std::tuple<int, int, int> link : links)
            {
                if (std::get<1>(link) == node->OutputAttributId)
                {
                    // Find the connected node
                    for (std::shared_ptr<Node> connectedNode : nodes)
                    {
                        if (std::find(connectedNode->InputAttributIds.begin(), connectedNode->InputAttributIds.end(), std::get<2>(link)) != connectedNode->InputAttributIds.end())
                        {
                            connectedNode->InputImages[std::get<2>(link)] = node->OutputImage;
                            connectedNode->Process();
                            traverseNodes.push_back(connectedNode);
                            break;
                        }
                    }
                }
            }
            traverseNodes.pop_front();
        }
    }
}
