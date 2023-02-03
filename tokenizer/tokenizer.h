/*
 * tokenizer/tokenizer.h
 * @author Wang Guibao
 * @brief This is part of WITBOOK Search Project. The goal is to provide a
 * universal searching utility, with which you can search the Internet for
 * freely available books by title, author name, year etc.
 */

namespace witbook {
namespace search {
struct TokenInfo {
    std::string token;
    uint32_t sign = 0;
    uint8_t weight = 0;
    uint8_t offset = 0;
}

class Tokenizer {
public:
    explicit Tokenizer(const std::string& str) : _str(str) {}
    std::vector<TokenInfo> get_tokens();
private:
    int tokenize();

private:
    std::string _str;
    std::vector<TokenInfo> _tokens;
};

}   // End namespace search
}   // End namespace witbook
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
