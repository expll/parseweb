#include <iostream>
#include <curl/curl.h>
#include "web.h"

WebMob::WebMob(const std::string& url) : Web(url){}

size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

int WebMob::checkout() {
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
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content); // &得到webpageContent的指针

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
    std::string cs = checkoutCharset(content);
    std::cout << cs << std::endl;

    // 转码成utf-8
    int result = convertStringToUTF8(content, cs);
    if (result == 0) {
        std::cout << "convert utf8 success" << std::endl;
    } else {
        std::cerr << "Error converting string: " << result << std::endl;
        return result;
    }

    // 去除空白符号
    // result = removeWhitespace(content);
    // if (result == 0) {
    //     std::cout << "remove whitespace success!" << std::endl;
    // } else {
    //     std::cerr << "Error remove whitespace: " << result << std::endl;
    //     return result;
    // }

    return 0;
}

// 采集ChapterGroup
int WebMob::checkoutChapterGroups() {
    return 0;
}


WebMob::~WebMob(){
    // 释放资源或执行清理任务
}