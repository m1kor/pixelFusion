//
//  OutputNode.cpp
//  pixelFusion
//
//  Created by ミカエル on 07/05/2023.
//

#include "OutputNode.hpp"
#include "UID.hpp"

#include <ImGuiFileDialog.h>

namespace pf
{
    OutputNode::OutputNode()
    {
        dialogId = UID::Register("Dialog");
        InputAttributIds.push_back(UID::Register("Attribut"));
    }
    
    OutputNode::~OutputNode()
    {
        UID::Free("Dialog", dialogId);
    }
    
    void OutputNode::Update()
    {
        if (!InputImages.empty())
        {
            OutputImage = std::make_shared<Magick::Image>(*InputImages[InputAttributIds[0]]);
            if (InputImages[InputAttributIds[0]]->isValid())
            {
                OutputImage->composite(*InputImages[InputAttributIds[0]], 0, 0, Magick::OverCompositeOp);
            }
        }
    }
    
    void OutputNode::Render()
    {
        ImNodes::BeginNode(Id);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Output");
        ImNodes::EndNodeTitleBar();
        ImNodes::BeginInputAttribute(InputAttributIds[0]);
            ImGui::Text("Preview");
            ImGui::Image((void*)(uintptr_t)textureId, ImVec2(OutputImage->columns(), OutputImage->rows()));
        ImNodes::EndInputAttribute();
        
        if (ImGui::Button("Save image"))
        {
            ImGuiFileDialog::Instance()->OpenModal(std::string("ChooseFileDlgKey") + std::to_string(dialogId), "Save image", ".jpg", ".");
            RequestBlockInput = true;
        }

        if (ImGuiFileDialog::Instance()->Display(std::string("ChooseFileDlgKey") + std::to_string(dialogId)))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                OutputImage->type(MagickCore::TrueColorType);
                OutputImage->magick("JPG");
                OutputImage->write(ImGuiFileDialog::Instance()->GetFilePathName());
            }
            RequestBlockInput = false;
            ImGuiFileDialog::Instance()->Close();
        }
        
        ImNodes::EndNode();
    }
}
