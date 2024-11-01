#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json; // 简化命名空间

namespace json_utils {

    json create_json_response(int code, const std::string& msg, const json& data = json{});
    std::string json_to_stirng (const json& j );
    json string_to_json (const std::string& str);
    bool is_valid_json (const std::string& str);

} 




#endif // JSON_UTILS_H