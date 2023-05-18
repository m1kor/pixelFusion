//
//  InputNode.cpp
//  pixelFusion
//
//  Created by ミカエル on 08/05/2023.
//

#include "InputNode.hpp"

#include <ImGuiFileDialog.h>

namespace pf
{
    InputNode::InputNode()
    {
        InitOutputAttribut();
        loaded = false;
        preview = false;
    }
    
    InputNode::~InputNode()
    {
        
    }
    
    void InputNode::Update()
    {
        
    }
    
    void InputNode::Render()
    {
        ImNodes::BeginNode(Id);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Input");
        ImNodes::EndNodeTitleBar();
        ImNodes::BeginOutputAttribute(OutputAttributId);
            if (loaded)
            {
                ImGui::Checkbox("Preview", &preview);
                if (preview)
                {
                    ImGui::Image((void*)textureId, ImVec2(OutputImage->columns(), OutputImage->rows()));
                }
            }
        ImNodes::EndOutputAttribute();
        
        if (ImGui::Button("Open image"))
        {
            ImGuiFileDialog::Instance()->OpenModal(std::string("ChooseFileDlgKey") + std::to_string(Id), "Open image", ".png", ".");
            RequestBlockInput = true;
        }

        if (ImGuiFileDialog::Instance()->Display(std::string("ChooseFileDlgKey") + std::to_string(Id)))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                OutputImage->read(ImGuiFileDialog::Instance()->GetFilePathName());
                loaded = true;
                Process();
                RequestUpdate = true;
            }
            RequestBlockInput = false;
            ImGuiFileDialog::Instance()->Close();
        }
        
        ImNodes::EndNode();
    }
}
