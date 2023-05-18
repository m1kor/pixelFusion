//
//  ThresholdNode.hpp
//  pixelFusion
//
//  Created by ミカエル on 18/05/2023.
//

#pragma once

#include "Node.hpp"

namespace pf
{
    class RotateNode : public Node
    {
    public:
        RotateNode();
        ~RotateNode();
        void Update() override;
        void Render() override;
    private:
        float angle;
    };
}
