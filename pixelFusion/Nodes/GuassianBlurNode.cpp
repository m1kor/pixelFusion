//
//  GuassianBlurNode.cpp
//  pixelFusion
//
//  Created by ミカエル on 17/05/2023.
//

#include "GuassianBlurNode.hpp"
#include "UID.hpp"

namespace pf
{
    GuassianBlurNode::GuassianBlurNode()
    {
        InitOutputAttribut();
        InputAttributIds.push_back(UID::Register("Attribut"));
        radius = 5.0f;
        sigma = 5.0f;
    }
    
    GuassianBlurNode::~GuassianBlurNode()
    {
        
    }
    
    void GuassianBlurNode::Update()
    {
        if (!InputImages.empty())
        {
            OutputImage = std::make_shared<Magick::Image>(*InputImages[InputAttributIds[0]]);
            if (InputImages[InputAttributIds[0]]->isValid())
            {
                OutputImage->composite(*InputImages[InputAttributIds[0]], 0, 0, Magick::OverCompositeOp);
                OutputImage->gaussianBlur(radius, sigma);
            }
        }
    }
    
    void GuassianBlurNode::Render()
    {
        ImNodes::BeginNode(Id);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Guassian blur");
        ImNodes::EndNodeTitleBar();
        ImNodes::BeginInputAttribute(InputAttributIds[0]);
        ImGui::Text("in");
        ImNodes::EndInputAttribute();
        ImGui::PushItemWidth(128);
        if (ImGui::SliderFloat("Radius", &radius, 0.1f, 10.0f))
        {
            RequestUpdate = true;
        }
        ImGui::PushItemWidth(128);
        if (ImGui::SliderFloat("Sigma", &sigma, 0.1f, 10.0f))
        {
            RequestUpdate = true;
        }
        ImNodes::BeginOutputAttribute(OutputAttributId);
        ImGui::Text("out");
        ImNodes::EndOutputAttribute();
        
        ImNodes::EndNode();
    }
}
