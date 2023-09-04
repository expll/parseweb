#include <iostream>
#include <string>
#include <curl/curl.h>
#include <iconv.h>
#include <cctype>
#include <regex>
#include "web.h"

Web::Web(const std::string& url) : url(url) {}
/*
* 将字符串转换成UTF-8
*/
int Web::convertStringToUTF8(std::string &input, std::string charset) {
    if (input.empty()) {
        return -1; // 输入为nullptr
    }

    size_t inBytes = (&input)->size();
    size_t outBytes = inBytes * 4; // 提供充足的输出空间

    char* inBuffer = const_cast<char*>(input.c_str());
    char* outBufferInitial = new char[outBytes];
    char* outBuffer = outBufferInitial;

    iconv_t conv = iconv_open("UTF-8", charset.c_str());
    if (conv == (iconv_t)-1) {
        perror("iconv_open");
        delete[] outBufferInitial;
        return -2; // iconv打开失败
    }

    if (iconv(conv, &inBuffer, &inBytes, &outBuffer, &outBytes) == (size_t)-1) {
        perror("iconv");
        iconv_close(conv);
        delete[] outBufferInitial;
        return -3; // 转换失败
    }

    input.assign(outBufferInitial, outBuffer - outBufferInitial);
    delete[] outBufferInitial;
    iconv_close(conv);

    return 0; // 成功
}

/*
* 获取一个字符串的编码内容，由于是网页内容，所以可以获取到网页head中的charset
*/
int Web::removeWhitespace(std::string &content)
{
    // 使用remove_if和isspace删除所有空白字符
    content.erase(std::remove_if(content.begin(), content.end(), [](unsigned char ch) {
        return std::isspace(ch);
    }), content.end());

    return 0;
}

/*
* 返回一个网页的编码类型
*/
std::string Web::checkoutCharset(std::string &content) {
    // 正则表达式，匹配 meta 标签内的 charset 值
    std::regex re(R"(\<meta[^>]*charset=["']?([^"'>\s]+))", std::regex_constants::icase);  // icase 表示不区分大小写

    std::smatch match;
    if (std::regex_search(content, match, re) && match.size() > 1) {
        return match.str(1);  // 返回匹配的字符集
    } else {
        return "UTF-8";  // 如果未找到匹配，则返回空字符串
    }
}
Web::~Web() {}