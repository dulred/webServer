#include <gtest/gtest.h>
#include "utils/json_utils.h"

// 测试 jsonToString 函数
TEST(JsonUtilsTest, JsonToString) {
    json j = {{"name", "John"}, {"age", 30}};
    std::string expected = R"({"age":30,"name":"John"})";
    
    // 比较预期输出和函数输出
    EXPECT_EQ(json_utils::json_to_stirng(j), expected);
}

// 测试 stringToJson 函数
TEST(JsonUtilsTest, StringToJson) {
    std::string jsonString = R"({"name":"John","age":30})";
    json expected = {{"name", "John"}, {"age", 30}};
    
    // 比较解析的 JSON 对象
    EXPECT_EQ(json_utils::string_to_json(jsonString), expected);
}

// 测试 isValidJson 函数（有效 JSON）
TEST(JsonUtilsTest, IsValidJson_Valid) {
    std::string validJsonString = R"({"name":"John","age":30})";
    
    // 测试是否为有效 JSON
    EXPECT_TRUE(json_utils::is_valid_json(validJsonString));
}

// 测试 isValidJson 函数（无效 JSON）
TEST(JsonUtilsTest, IsValidJson_Invalid) {
    std::string invalidJsonString = R"({"name":"John","age":30)";
    
    // 测试无效 JSON 字符串
    EXPECT_FALSE(json_utils::is_valid_json(invalidJsonString));
}
