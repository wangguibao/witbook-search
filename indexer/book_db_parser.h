/*
 * indexer/book_db_parser.h
 * @author Wang Guibao
 * @brief This is part of WITBOOK Search Project. The goal is to provide a
 * universal searching utility, with which you can search the Internet for
 * freely available books by title, author name, year etc.
 */

/*
 * This parser tool parses and generates invert index records from book
 * description databases.
 *
 * Input is text format, consisting of below fields separated by TABs
 * TITLE    GENRE    AUTHOR    YEAR    DESCRIPTION    COUNTRY    COVER_IMAGE_URL    ONLINE_URL_LIST    DOWNLOAD_URL_LIST
 *
 * where
 * COVER_IMAGE_URL is a book cover image URL
 * ONLINE_URL_LIST is a list of URLs where you can read the book online
 * DOWNLOAD_URL_LIST is a list of URLs where you can download the e-book
 */
namespace witbook {
namespace search {
const int BOOK_INFO_FIELD_NUM = 9;

struct BasicBookInfo{
    std::string title;
    std::string genre;
    std::string author;
    short year;
    std::string description;
    std::string country;
    std::string cover_img_url;
    std::string online_url_list;
    std::string download_url_list;
};

class BookDBParser : public Parser {
public:
    /*
     * Read input from text file
     * Each line consists of below fields:
     *
     * TITLE
     * GENRE
     * AUTHOR
     * YEAR
     * DESCRIPTION
     * COUNTRY
     * COVER_IMAGE_URL
     * ONLINE_URL_LIST
     * DOWNLOAD_URL_LIST
     */
    virtual int read_input(const std::istream& input_stream);

    virtual int parse();
    virtual int write_output(const std::ostream& output_stream);
private:
    std::vector<BasicBookInfo> _book_info_vec;
    ForwardIndexer _fwd_indexer;
};

}   // End namespace search
}   // End namespace witbook
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
