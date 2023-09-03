#include <string>
#include <iostream>
#include "parseweb.h"
#include <fstream>

int main() {
    std::string url = "https://www.biqiuge8.cc/book/34285864/";
    std::string result;
    int ret = checkout(url, &result, "GBK");
    std::cout << result << std::endl;


    // 创建一个输出文件流对象，并尝试打开文件
    std::ofstream outfile("example.txt");

    // 检查文件是否成功打开
    if (!outfile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    // 将字符串写入文件
    outfile << result;

    // 关闭文件
    outfile.close();

    std::cout << "Data written to example.txt" << std::endl;

    return 0;
}
