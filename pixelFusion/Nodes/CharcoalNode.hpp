//
//  CharcoalNode.hpp
//  pixelFusion
//
//  Created by ミカエル on 18/05/2023.
//

#pragma once

#include "Node.hpp"

namespace pf
{
    class CharcoalNode : public Node
    {
    public:
        CharcoalNode();
        ~CharcoalNode();
        void Update() override;
        void Render() override;
    };
}
