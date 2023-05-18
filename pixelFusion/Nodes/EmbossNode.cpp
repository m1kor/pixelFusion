//
//  EmbossNode.cpp
//  pixelFusion
//
//  Created by ミカエル on 18/05/2023.
//

#include "EmbossNode.hpp"
#include "UID.hpp"

namespace pf
{
    EmbossNode::EmbossNode()
    {
        InitOutputAttribut();
        InputAttributIds.push_back(UID::Register("Attribut"));
    }
    
    EmbossNode::~EmbossNode()
    {
        
    }
    
    void EmbossNode::Update()
    {
        if (!InputImages.empty())
        {
            OutputImage = std::make_shared<Magick::Image>(*InputImages[InputAttributIds[0]]);
            if (InputImages[InputAttributIds[0]]->isValid())
            {
                OutputImage->composite(*InputImages[InputAttributIds[0]], 0, 0, Magick::OverCompositeOp);
                OutputImage->emboss();
            }
        }
    }
    
    void EmbossNode::Render()
    {
        ImNodes::BeginNode(Id);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Emboss");
        ImNodes::EndNodeTitleBar();
        ImNodes::BeginInputAttribute(InputAttributIds[0]);
        ImGui::Text("in");
        ImNodes::EndInputAttribute();
        ImNodes::BeginOutputAttribute(OutputAttributId);
        ImGui::Text("out");
        ImNodes::EndOutputAttribute();
        
        ImNodes::EndNode();
    }
}
