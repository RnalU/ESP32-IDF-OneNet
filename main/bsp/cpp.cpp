#include <iostream>
#include <cstdarg>
#include <cstring>

void format_json(char* topic_list, const char* json_template, ...) {
    // 初始化可变参数列表
    va_list args;
    va_start(args, json_template);

    // 计算格式化后的字符串长度
    int len = std::vsnprintf(nullptr, 0, json_template, args);
    va_end(args);

    // 分配足够的空间来存储格式化后的字符串
    char* formatted_str = new char[len + 1];

    // 重新初始化可变参数列表
    va_start(args, json_template);
    // 将可变参数按照JSON模板格式化到字符串中
    std::vsprintf(formatted_str, json_template, args);
    va_end(args);

    // 将格式化后的字符串复制到topic_list字符数组中
    std::strcpy(topic_list, formatted_str);

    // 释放分配的内存
    delete[] formatted_str;
}

int main() {
    char topic_list[100];
    const char* json_template = "{\"topic\": \"%s\", \"value\": %d}";

    // 调用格式化函数
    format_json(topic_list, json_template, "temperature", 25);

    // 输出结果
    std::cout << "Formatted JSON: " << topic_list << std::endl;

    return 0;
}
