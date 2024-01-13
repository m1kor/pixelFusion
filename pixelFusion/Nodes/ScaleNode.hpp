//
//  ScaleNode.hpp
//  pixelFusion
//
//  Created by ミカエル on 30/05/2023.
//

#pragma once

#include "Node.hpp"

namespace pf
{
    class ScaleNode : public Node
    {
    public:
        ScaleNode();
        ~ScaleNode();
        void Update() override;
        void Render() override;
    private:
        float scale;
    };
}
