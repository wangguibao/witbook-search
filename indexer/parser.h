/*
 * indexer/parser.h
 * @author Wang Guibao
 * @brief This is part of WITBOOK Search Project. The goal is to provide a
 * universal searching utility, with which you can search the Internet for
 * freely available books by title, author name, year etc.
 */

namespace witbook {
namespace search {
class Parser {
public:
    virtual int read_input(const std::istream& input) = 0;
    virtual int parse() = 0;
    virtual int write_output(const std::ostream& output) = 0;
};
}   // End namespace search
}   // End namespace witbook
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
