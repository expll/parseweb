#include <iostream>
#include <string>
#include <curl/curl.h>
#include <iconv.h>

int ConvertGBKToUTF8(std::string *input);

// 回调函数，用于处理Curl接收到的数据
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) 
{
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

int checkout(std::string url, std::string *webpageContent, std::string charset) 
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
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, webpageContent);



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


    int result = ConvertGBKToUTF8(webpageContent);
    if (result == 0) {
        std::cout << webpageContent << std::endl;
    } else {
        std::cerr << "Error converting string: " << result << std::endl;
    }


    return 0;
}


int ConvertGBKToUTF8(std::string *input) {
    if (!input) {
        return -1; // 输入为nullptr
    }

    size_t inBytes = input->size();
    size_t outBytes = inBytes * 4; // 提供充足的输出空间

    char* inBuffer = const_cast<char*>(input->c_str());
    char* outBufferInitial = new char[outBytes];
    char* outBuffer = outBufferInitial;

    iconv_t conv = iconv_open("UTF-8", "GBK");
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

    input->assign(outBufferInitial, outBuffer - outBufferInitial);
    delete[] outBufferInitial;
    iconv_close(conv);

    return 0; // 成功
}