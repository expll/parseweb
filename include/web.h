#include <string>

class Web {
public:
    std::string url;
    std::string content;

public:
    Web(const std::string& url);
    int convertStringToUTF8(std::string &input, std::string charset);
    std::string checkoutCharset(std::string &content);
    int removeWhitespace(std::string &content);
    virtual int checkout() = 0;
    virtual int checkoutChapterGroups() = 0;
    virtual ~Web();
};

class WebMob : public Web {
public:
    WebMob(const std::string& url);
    int checkout() override;
    int checkoutChapterGroups() override;
    ~WebMob();
};

class WebPc : public Web {
public:
    WebPc(const std::string& url);
    int checkout() override;
    ~WebPc();
};