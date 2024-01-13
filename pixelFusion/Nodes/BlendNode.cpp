//
//  BlendNode.cpp
//  pixelFusion
//
//  Created by ミカエル on 30/05/2023.
//

#include "BlendNode.hpp"
#include "UID.hpp"

namespace pf
{
    BlendNode::BlendNode()
    {
        InitOutputAttribut();
        InputAttributIds.push_back(UID::Register("Attribut"));
        InputAttributIds.push_back(UID::Register("Attribut"));
        selectedBlendMode = Magick::BlendCompositeOp;
    }
    
    BlendNode::~BlendNode()
    {
        
    }
    
    void BlendNode::Update()
    {
        if (!InputImages.empty())
        {
            if (InputImages.size() == 2 && InputImages[InputAttributIds[0]]->isValid() && InputImages[InputAttributIds[1]]->isValid())
            {
                OutputImage = std::make_shared<Magick::Image>(*InputImages[InputAttributIds[0]]);
                OutputImage->composite(*InputImages[InputAttributIds[0]], 0, 0, Magick::OverCompositeOp);
                OutputImage->composite(*InputImages[InputAttributIds[1]], 0, 0, (Magick::CompositeOperator)selectedBlendMode);
            }
            else if (InputImages.size() == 1 && InputImages.contains(InputAttributIds[0]) && InputImages[InputAttributIds[0]]->isValid())
            {
                OutputImage = std::make_shared<Magick::Image>(*InputImages[InputAttributIds[0]]);
                OutputImage->composite(*InputImages[InputAttributIds[0]], 0, 0, Magick::OverCompositeOp);
            }
            else if (InputImages.size() == 1 && InputImages.contains(InputAttributIds[1]) && InputImages[InputAttributIds[1]]->isValid())
            {
                OutputImage = std::make_shared<Magick::Image>(*InputImages[InputAttributIds[1]]);
                OutputImage->composite(*InputImages[InputAttributIds[1]], 0, 0, Magick::OverCompositeOp);
            }
        }
    }
    
    void BlendNode::Render()
    {
        ImNodes::BeginNode(Id);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Blend");
        ImNodes::EndNodeTitleBar();
        ImNodes::BeginInputAttribute(InputAttributIds[0]);
        ImGui::Text("in 1");
        ImNodes::EndInputAttribute();
        ImNodes::BeginInputAttribute(InputAttributIds[1]);
        ImGui::Text("in 2");
        ImNodes::EndInputAttribute();
        if (ImGui::RadioButton("Blend", &selectedBlendMode, Magick::BlendCompositeOp))
        {
            RequestUpdate = true;
        }
        if (ImGui::RadioButton("Screen", &selectedBlendMode, Magick::ScreenCompositeOp))
        {
            RequestUpdate = true;
        }
        if (ImGui::RadioButton("Overlay", &selectedBlendMode, Magick::OverlayCompositeOp))
        {
            RequestUpdate = true;
        }
        if (ImGui::RadioButton("Multiply", &selectedBlendMode, Magick::MultiplyCompositeOp))
        {
            RequestUpdate = true;
        }
        ImNodes::BeginOutputAttribute(OutputAttributId);
        ImGui::Text("out");
        ImNodes::EndOutputAttribute();
        ImNodes::EndNode();
    }
}
