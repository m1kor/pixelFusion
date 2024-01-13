//
//  InputNode.cpp
//  pixelFusion
//
//  Created by ミカエル on 08/05/2023.
//

#include "InputNode.hpp"
#include "UID.hpp"

#include <ImGuiFileDialog.h>

namespace pf
{
    InputNode::InputNode()
    {
        dialogId = UID::Register("Dialog");
        InitOutputAttribut();
        loaded = false;
        preview = false;
    }
    
    InputNode::~InputNode()
    {
        UID::Free("Dialog", dialogId);
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
                    ImGui::Image((void*)(uintptr_t)textureId, ImVec2(OutputImage->columns(), OutputImage->rows()));
                }
            }
        ImNodes::EndOutputAttribute();
        
        if (ImGui::Button("Open image"))
        {
            ImGuiFileDialog::Instance()->OpenModal(std::string("ChooseFileDlgKey") + std::to_string(dialogId), "Open image", ".png", ".");
            RequestBlockInput = true;
        }

        if (ImGuiFileDialog::Instance()->Display(std::string("ChooseFileDlgKey") + std::to_string(dialogId)))
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
