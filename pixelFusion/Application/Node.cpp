//
//  Node.cpp
//  pixelFusion
//
//  Created by ミカエル on 07/05/2023.
//

#include "Node.hpp"
#include "UID.hpp"

#ifdef __APPLE__
// Defined before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION
#endif


namespace pf
{
    Node::Node() : Id(id)
    {
        id = UID::Register("Node");
        textureCreated = false;
        OutputAttributId = -1;
        OutputImage = std::make_shared<Magick::Image>();
        RequestUpdate = false;
        RequestBlockInput = false;
    }
    
    Node::~Node()
    {
        UID::Free(typeid(*this).name(), Id);
        if (textureCreated)
        {
            glDeleteTextures(1, &textureId);
            textureCreated = false;
        }
        if (OutputAttributId >= 0)
        {
            UID::Free("Attribut", OutputAttributId);
        }
        for (int id : InputAttributIds)
        {
            UID::Free("Attribut", id);
        }
    }
    
    void Node::Process()
    {
        Update();
        
        if (OutputImage->isValid())
        {
            if (textureCreated)
            {
                glDeleteTextures(1, &textureId);
                textureCreated = false;
            }
            
            GLsizei width = static_cast<GLsizei>(OutputImage->columns());
            GLsizei height = static_cast<GLsizei>(OutputImage->rows());
            
            // Convert Magick++ Image to RGBA format with 8 bits per channel
            Magick::Blob imageBlob;
            OutputImage->magick("RGBA");
            OutputImage->write(&imageBlob);
            
            // Create OpenGL texture
            glGenTextures(1, &textureId);
            glBindTexture(GL_TEXTURE_2D, textureId);
            
            // Set texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            // Load image data into OpenGL texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBlob.data());
            
            // Generate mipmaps
            glGenerateMipmap(GL_TEXTURE_2D);
            
            // Unbind the texture
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    
    void Node::Render() { }
    
    void Node::Update() { }
    
    void Node::InitOutputAttribut()
    {
        OutputAttributId = UID::Register("Attribut");
    }
}
