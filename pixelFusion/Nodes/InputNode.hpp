//
//  InputNode.hpp
//  pixelFusion
//
//  Created by ミカエル on 08/05/2023.
//

#pragma once

#include "Node.hpp"

namespace pf
{
    class InputNode : public Node
    {
    public:
        InputNode();
        ~InputNode();
        void Update() override;
        void Render() override;
    private:
        bool loaded, preview;
    };
}
