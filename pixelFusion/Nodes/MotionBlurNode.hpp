//
//  MotionBlurNode.hpp
//  pixelFusion
//
//  Created by ミカエル on 18/05/2023.
//

#pragma once

#include "Node.hpp"

namespace pf
{
    class MotionBlurNode : public Node
    {
    public:
        MotionBlurNode();
        ~MotionBlurNode();
        void Update() override;
        void Render() override;
    private:
        float radius, sigma, angle;
    };
}
