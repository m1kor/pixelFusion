//
//  BlendNode.hpp
//  pixelFusion
//
//  Created by ミカエル on 30/05/2023.
//

#pragma once

#include "Node.hpp"

namespace pf
{
    class BlendNode : public Node
    {
    public:
        BlendNode();
        ~BlendNode();
        void Update() override;
        void Render() override;
    private:
        int selectedBlendMode;
    };
}
