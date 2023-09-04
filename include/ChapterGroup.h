#include <string>

class ChapterGroup {
private:
    std::string title; 
    std::string link; 

public:
    ChapterGroup(const std::string& t, const std::string& l);
    std::string getTitle() const;
    void setTitle(const std::string& t);
    std::string getLink() const;
    void setLink(const std::string& l);
};