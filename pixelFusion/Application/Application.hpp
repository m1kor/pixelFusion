//
//  Application.hpp
//  pixelFusion
//
//  Created by ミカエル on 07/05/2023.
//

#pragma once

namespace pf
{
    class Application
    {
    public:
        Application();
        virtual ~Application();
        virtual int Run(int argc, char** argv) = 0;
        virtual bool OnUpdate() = 0;
    };
}
