//
//  Node.hpp
//  pixelFusion
//
//  Created by ミカエル on 07/05/2023.
//

#pragma once

#include <memory>
#include <string>
#include <imgui.h>
#include <imnodes.h>
#include <Magick++.h>

#ifdef __APPLE__
// Defined before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION
#endif
#define GLFW_INCLUDE_GLCOREARB
#define IMGUI_DEFINE_MATH_OPERATORS

#include <GLFW/glfw3.h>

namespace pf
{
    class Node
    {
    public:
        Node();
        ~Node();
        int &Id;
        void Process();
        virtual void Render();
        virtual void Update();
        int OutputAttributId;
        std::shared_ptr<Magick::Image> OutputImage;
        std::vector<int> InputAttributIds;
        std::map<int, std::shared_ptr<Magick::Image>> InputImages;
        bool RequestUpdate;
        bool RequestBlockInput;
    protected:
        GLuint textureId;
        void InitOutputAttribut();
    private:
        int id;
        bool textureCreated;
    };
}
