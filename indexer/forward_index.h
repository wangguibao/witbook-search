/*
 * indexer/forward_index.h
 * @author Wang Guibao
 * @brief This is part of WITBOOK Search Project. The goal is to provide a
 * universal searching utility, with which you can search the Internet for
 * freely available books by title, author name, year etc.
 */
#include "types.h"
#include "constants.h"

namespace witbook {
namespace search {
struct WebPage {
    std::string anchor_title;
    std::string real_title;
    std::string head;
    std::vector<std::string> meta;
    std::string content;
};

struct 
class ForwardIndexer {
public:
    int consume_one_page(const WebPage& page);
private:
    // Local URL no counter
    static int url_no = 0;
};

}   // End namespace search
}   // End namespace witbook
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
