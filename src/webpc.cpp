#include <iostream>
#include "web.h"

WebPc::WebPc(const std::string& url) : Web(url){}
int WebPc::checkout() {
    std::cout << "WebPc[checkout]" << std::endl;
    return 0;
}
WebPc::~WebPc() {
    // 释放资源或执行清理任务
}
