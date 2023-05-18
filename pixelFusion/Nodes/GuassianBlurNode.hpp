//
//  GuassianBlurNode.hpp
//  pixelFusion
//
//  Created by ミカエル on 17/05/2023.
//

#pragma once

#include "Node.hpp"

namespace pf
{
    class GuassianBlurNode : public Node
    {
    public:
        GuassianBlurNode();
        ~GuassianBlurNode();
        void Update() override;
        void Render() override;
    private:
        float radius, sigma;
    };
}
