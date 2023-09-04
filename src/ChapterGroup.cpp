#include <string>
#include "ChapterGroup.h"

// 参数化构造函数
ChapterGroup::ChapterGroup(const std::string& t, const std::string& l) : title(t), link(l) {}

// title的getter
std::string ChapterGroup::getTitle() const {
    return title;
}

// title的setter
void ChapterGroup::setTitle(const std::string& t) {
    title = t;
}

// link的getter
std::string ChapterGroup::getLink() const {
    return link;
}

// link的setter
void ChapterGroup::setLink(const std::string& l) {
    link = l;
}