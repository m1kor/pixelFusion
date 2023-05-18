//
//  EmbossNode.hpp
//  pixelFusion
//
//  Created by ミカエル on 18/05/2023.
//

#pragma once

#include "Node.hpp"

namespace pf
{
    class EmbossNode : public Node
    {
    public:
        EmbossNode();
        ~EmbossNode();
        void Update() override;
        void Render() override;
    };
}
