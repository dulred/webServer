#include <iostream>
#include "utils/json_utils.h"

// 在 .cpp 文件中实现函数，同时保持命名空间
namespace json_utils {

    json
    create_json_response(int code, const std::string& msg, const json& data)
    {
        json response;
        response["code"] = code;
        response["msg"] = msg;
        response["data"] = data;
        return response;
    }

    std::string json_to_stirng(const json& j) {
        return j.dump();  // 使用 nlohmann::json 的 dump 函数
    }

    json string_to_json(const std::string& str) {
        return json::parse(str);  // 使用 nlohmann::json 的 parse 函数
    }

    bool is_valid_json(const std::string& str) {
        try {
            json temp =json::parse(str);  // 尝试解析字符串为 json
            return true;
        } catch (const json::parse_error&) {
            return false;  // 如果解析失败，返回 false
        }
    }

}
