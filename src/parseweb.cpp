#include <iostream>
#include <string>
#include <curl/curl.h>
#include <iconv.h>
#include <cctype>
#include <regex>

int convertStringToUTF8(std::string &input, std::string charset);
int removeWhitespace(std::string &content);
std::string checkoutCharset(std::string &content);

// 回调函数，用于处理Curl接收到的数据
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) 
{
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

/*
* 将URL的内容检出，存放到webpageContent中
*/
int checkout(std::string url, std::string &webpageContent) 
{
    // 初始化Curl
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize Curl." << std::endl;
        return 1;
    }

    // 设置Curl选项
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // 设置回调函数，将网页内容存储到webpageContent中
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:51.0) Gecko/20100101 Firefox/51.0");
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &webpageContent); // &得到webpageContent的指针

    // 执行HTTP请求
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Curl request failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return 1;
    }

    char *ct;
    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
    if((CURLE_OK == res) && ct)
      std::cout << "We received Content-Type: " << ct << std::endl;

    // 清理Curl资源
    curl_easy_cleanup(curl);

    // 提取 charset
    std::string cs = checkoutCharset(webpageContent);
    std::cout << cs << std::endl;

    // 转码成utf-8
    int result = convertStringToUTF8(webpageContent, cs);
    if (result == 0) {
        std::cout << webpageContent << std::endl;
    } else {
        std::cerr << "Error converting string: " << result << std::endl;
        return result;
    }

    // 去除空白符号
    result = removeWhitespace(webpageContent);
    if (result == 0) {
        std::cout << "remove whitespace success!" << std::endl;
    } else {
        std::cerr << "Error remove whitespace: " << result << std::endl;
        return result;
    }

    return 0;
}

/*
* 将字符串转换成UTF-8
*/
int convertStringToUTF8(std::string &input, std::string charset) {
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
int removeWhitespace(std::string &content)
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
std::string checkoutCharset(std::string &content) {
    // 正则表达式，匹配 meta 标签内的 charset 值
    std::regex re(R"(\<meta[^>]*charset=["']?([^"'>\s]+))", std::regex_constants::icase);  // icase 表示不区分大小写

    std::smatch match;
    if (std::regex_search(content, match, re) && match.size() > 1) {
        return match.str(1);  // 返回匹配的字符集
    } else {
        return "UTF-8";  // 如果未找到匹配，则返回空字符串
    }
}