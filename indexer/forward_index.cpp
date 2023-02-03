/*
 * indexer/forward_index.cpp
 * @author Wang Guibao
 * @brief This is part of WITBOOK Search Project. The goal is to provide a
 * universal searching utility, with which you can search the Internet for
 * freely available books by title, author name, year etc.
 */
#include "types.h"
#include "constants.h"

namespace witbook {
namespace search {
int ForwardIndexer::consume_page_anchor_title(const std::string& anchor_title) {
    // Now we assume webpage is pure ASCII
    bool ascii = is_ascii(anchor_title);
    if (!ascii) {
        return -1;
    }

    // For now 
    std::vector<std::string> word_vec;
    util::string_split(str, ' ', word_vec);
}

int ForwardIndexer::consume_one_page(const WebPage& page) {
    consume_page_anchor_title(page.anchor_title);
    consume_page_real_title(page.real_title);
    consume_page_head(page.head);
    consume_page_meta(page.meta);
    consume_page_content(page.content);

    ++url_no;
}
};

}   // End namespace search
}   // End namespace witbook
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
