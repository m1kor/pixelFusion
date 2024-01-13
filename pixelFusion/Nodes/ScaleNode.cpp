//
//  ScaleNode.cpp
//  pixelFusion
//
//  Created by ミカエル on 30/05/2023.
//

#include "ScaleNode.hpp"
#include "UID.hpp"

namespace pf
{
    ScaleNode::ScaleNode()
    {
        InitOutputAttribut();
        InputAttributIds.push_back(UID::Register("Attribut"));
        scale = 1.0f;
    }
    
    ScaleNode::~ScaleNode()
    {
        
    }
    
    void ScaleNode::Update()
    {
        if (!InputImages.empty())
        {
            OutputImage = std::make_shared<Magick::Image>(*InputImages[InputAttributIds[0]]);
            if (InputImages[InputAttributIds[0]]->isValid())
            {
                OutputImage->composite(*InputImages[InputAttributIds[0]], 0, 0, Magick::OverCompositeOp);
                OutputImage->scale(Magick::Geometry(InputImages[InputAttributIds[0]]->size().width() * scale, InputImages[InputAttributIds[0]]->size().height() * scale));
            }
        }
    }
    
    void ScaleNode::Render()
    {
        ImNodes::BeginNode(Id);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Scale");
        ImNodes::EndNodeTitleBar();
        ImNodes::BeginInputAttribute(InputAttributIds[0]);
        ImGui::Text("in");
        ImNodes::EndInputAttribute();
        ImGui::PushItemWidth(128);
        if (ImGui::SliderFloat("Scale", &scale, 0.1f, 5.0f))
        {
            RequestUpdate = true;
        }
        ImNodes::BeginOutputAttribute(OutputAttributId);
        ImGui::Text("out");
        ImNodes::EndOutputAttribute();
        ImNodes::EndNode();
    }
}
