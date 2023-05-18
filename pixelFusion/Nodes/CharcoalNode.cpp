//
//  CharcoalNode.cpp
//  pixelFusion
//
//  Created by ミカエル on 18/05/2023.
//

#include "CharcoalNode.hpp"
#include "UID.hpp"

namespace pf
{
    CharcoalNode::CharcoalNode()
    {
        InitOutputAttribut();
        InputAttributIds.push_back(UID::Register("Attribut"));
    }
    
    CharcoalNode::~CharcoalNode()
    {
        
    }
    
    void CharcoalNode::Update()
    {
        if (!InputImages.empty())
        {
            OutputImage = std::make_shared<Magick::Image>(*InputImages[InputAttributIds[0]]);
            if (InputImages[InputAttributIds[0]]->isValid())
            {
                OutputImage->composite(*InputImages[InputAttributIds[0]], 0, 0, Magick::OverCompositeOp);
                OutputImage->charcoal();
            }
        }
    }
    
    void CharcoalNode::Render()
    {
        ImNodes::BeginNode(Id);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Charcoal");
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
