//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief 
 * JSON utils by SaturninTheAlien.
 */

#include "types.hpp"
#include <string>
#include <vector>
#include <string>
#include <array>
#include "3rd_party/json.hpp"

#pragma once

namespace PJson{
    void jsonReadString(const nlohmann::json& j, const std::string& name, std::string& target);
    void jsonReadInt(const nlohmann::json& j, const std::string& name, int& target);
    void jsonReadDouble(const nlohmann::json& j, const std::string& name, double& target );
    void jsonReadBool(const nlohmann::json& j, const std::string& name, bool& target);
    void jsonReadEnumU8(const nlohmann::json& j, const std::string& name, u8& target);
}