//
//  OutputNode.hpp
//  pixelFusion
//
//  Created by ミカエル on 07/05/2023.
//

#pragma once

#include "Node.hpp"

namespace pf
{
    class OutputNode : public Node
    {
    public:
        OutputNode();
        ~OutputNode();
        void Update() override;
        void Render() override;
    };
}
