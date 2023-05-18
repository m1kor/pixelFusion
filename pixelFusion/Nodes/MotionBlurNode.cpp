//
//  MotionBlurNode.cpp
//  pixelFusion
//
//  Created by ミカエル on 18/05/2023.
//

#include "MotionBlurNode.hpp"

#include "MotionBlurNode.hpp"
#include "UID.hpp"

namespace pf
{
    MotionBlurNode::MotionBlurNode()
    {
        InitOutputAttribut();
        InputAttributIds.push_back(UID::Register("Attribut"));
        radius = 5.0f;
        sigma = 5.0f;
    }
    
    MotionBlurNode::~MotionBlurNode()
    {
        
    }
    
    void MotionBlurNode::Update()
    {
        if (!InputImages.empty())
        {
            OutputImage = std::make_shared<Magick::Image>(*InputImages[InputAttributIds[0]]);
            if (InputImages[InputAttributIds[0]]->isValid())
            {
                OutputImage->composite(*InputImages[InputAttributIds[0]], 0, 0, Magick::OverCompositeOp);
                OutputImage->motionBlur(radius, sigma, angle);
            }
        }
    }
    
    void MotionBlurNode::Render()
    {
        ImNodes::BeginNode(Id);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Motion blur");
        ImNodes::EndNodeTitleBar();
        ImNodes::BeginInputAttribute(InputAttributIds[0]);
        ImGui::Text("in");
        ImNodes::EndInputAttribute();
        ImGui::PushItemWidth(128);
        if (ImGui::SliderFloat("Radius", &radius, 0.1f, 5.0f))
        {
            RequestUpdate = true;
        }
        ImGui::PushItemWidth(128);
        if (ImGui::SliderFloat("Sigma", &sigma, 0.1f, 10.0f))
        {
            RequestUpdate = true;
        }
        ImGui::PushItemWidth(128);
        if (ImGui::SliderFloat("Angle", &angle, 0.0f, 360.0f))
        {
            RequestUpdate = true;
        }
        ImNodes::BeginOutputAttribute(OutputAttributId);
        ImGui::Text("out");
        ImNodes::EndOutputAttribute();
        
        ImNodes::EndNode();
    }
}
