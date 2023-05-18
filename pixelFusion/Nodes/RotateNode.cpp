//
//  ThresholdNode.cpp
//  pixelFusion
//
//  Created by ミカエル on 18/05/2023.
//

#include "RotateNode.hpp"
#include "UID.hpp"

namespace pf
{
    RotateNode::RotateNode()
    {
        InitOutputAttribut();
        InputAttributIds.push_back(UID::Register("Attribut"));
        angle = 0.0f;
    }
    
    RotateNode::~RotateNode()
    {
        
    }
    
    void RotateNode::Update()
    {
        if (!InputImages.empty())
        {
            OutputImage = std::make_shared<Magick::Image>(*InputImages[InputAttributIds[0]]);
            if (InputImages[InputAttributIds[0]]->isValid())
            {
                OutputImage->composite(*InputImages[InputAttributIds[0]], 0, 0, Magick::OverCompositeOp);
                OutputImage->rotate(angle);
            }
        }
    }
    
    void RotateNode::Render()
    {
        ImNodes::BeginNode(Id);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Rotate");
        ImNodes::EndNodeTitleBar();
        ImNodes::BeginInputAttribute(InputAttributIds[0]);
        ImGui::Text("in");
        ImNodes::EndInputAttribute();
        ImGui::PushItemWidth(128);
        if (ImGui::SliderFloat("Angle", &angle, 0.1f, 360.0f))
        {
            RequestUpdate = true;
        }
        ImNodes::BeginOutputAttribute(OutputAttributId);
        ImGui::Text("out");
        ImNodes::EndOutputAttribute();
        
        ImNodes::EndNode();
    }
}
