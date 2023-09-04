#include <string>
#include <iostream>
#include "parseweb.h"
#include <fstream>
#include "web.h"
#include <gumbo.h>
#include "ChapterGroup.h"

void search_for_select_options(GumboNode* node) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }

    std::vector<ChapterGroup> chapters;

    // 检查节点是否是<select>标签
    if (node->v.element.tag == GUMBO_TAG_SELECT) {
        GumboVector* children = &node->v.element.children;

        // 遍历<select>的子节点
        for (unsigned int i = 0; i < children->length; ++i) {
            GumboNode* child = (GumboNode*) children->data[i];

            GumboAttribute* value_attr = gumbo_get_attribute(&child->v.element.attributes, "value");

            // 检查子节点是否是<option>标签
            if (child->type == GUMBO_NODE_ELEMENT && child->v.element.tag == GUMBO_TAG_OPTION) {
                GumboNode* textNode = (GumboNode*)child->v.element.children.data[0];
                if (textNode->type == GUMBO_NODE_TEXT || textNode->type == GUMBO_NODE_WHITESPACE) {
                    printf("Option content: %s, %s\n", textNode->v.text.text, value_attr ? value_attr->value : "N/A");
                }
            }
        }
    }

    // 递归搜索其他节点
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        search_for_select_options((GumboNode*) children->data[i]);
    }
}

int main() {
    std::string url = "https://wap.ibiquges.org/wapbook/7552.html";
    Web* web = new WebMob(url);
    web->checkout();
    // std::cout << web->content << std::endl;


    GumboOutput* output = gumbo_parse((web->content).c_str());
    search_for_select_options(output->root);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    


    // std::string result;
    // int ret = checkout(url, result);
    // if (ret == 0) {
    //     std::cout << result << std::endl;
    // } else {
    //     std::cout << "checkout error:" << ret << std::endl;
    // }
    
    


    // 创建一个输出文件流对象，并尝试打开文件
    std::ofstream outfile("example.txt");

    // 检查文件是否成功打开
    if (!outfile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    // 将字符串写入文件
    outfile << web->content;

    // 关闭文件
    outfile.close();

    std::cout << "Data written to example.txt" << std::endl;


    delete web;

    return 0;
}
