//
//  UID.cpp
//  pixelFusion
//
//  Created by ミカエル on 07/05/2023.
//

#include "UID.hpp"

namespace pf
{
    std::map<std::string, std::set<int>> UID::allocatedIDs;
    std::mutex UID::mutex;
    
    int UID::Register(const std::string& category)
    {
        std::lock_guard<std::mutex> lock(mutex);
        int id = 0;
        while (allocatedIDs[category].find(id) != allocatedIDs[category].end())
        {
            ++id;
        }
        allocatedIDs[category].insert(id);
        return id;
    }

    void UID::Free(const std::string& category, int id)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (allocatedIDs[category].find(id) != allocatedIDs[category].end())
        {
            allocatedIDs[category].erase(id);
        }
    }
    
    const std::set<int> UID::All(const std::string& category)
    {
        return allocatedIDs[category];
    }
}
