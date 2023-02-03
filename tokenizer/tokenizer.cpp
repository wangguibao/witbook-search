/*
 * tokenizer/tokenizer.h
 * @author Wang Guibao
 * @brief This is part of WITBOOK Search Project. The goal is to provide a
 * universal searching utility, with which you can search the Internet for
 * freely available books by title, author name, year etc.
 */

namespace witbook {
namespace search {
int Tokenizer::tokenize() {
}

std::vector Tokenizer::get_tokens() {
    return _tokens;
}
};

}   // End namespace search
}   // End namespace witbook
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
