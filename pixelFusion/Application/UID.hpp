//
//  UID.hpp
//  pixelFusion
//
//  Created by ミカエル on 07/05/2023.
//

#pragma once

#include <set>
#include <map>
#include <mutex>

namespace pf
{
    class UID
    {
    public:
        static int Register(const std::string& category);
        static void Free(const std::string& category, int id);
        static const std::set<int> All(const std::string& category);
    private:
        static std::map<std::string, std::set<int>> allocatedIDs;
        static std::mutex mutex;
    };
}
