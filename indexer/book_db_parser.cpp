/*
 * indexer/book_db_parser.cpp
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
int BookDBParser::read_input(const std::istream& input) {
    std::string line;
    while (std::getline(input, line)) {
        std::vector<std::string> fields_vec;
        string_split(line, fields_vec);

        if (fields_vec.size() != BOOK_INFO_FIELD_NUM) {
            continue;
        }

        _book_info_vec.emplace_back(
                fields_vec[0],          // title
                fields_vec[1],          // genre
                fields_vec[2],          // author
                atoi(fields_vec[3]),    // year
                fields_vec[4],          // description
                fields_vec[5],          // country
                fields_vec[6],          // cover_image_url
                fields_vec[7],          // online_url_list
                fields_vec[8]);         // download_url_list
    }
}

int BookDBParser::parse() {}

int BookDBParser::write_output(std::ostream& output) {
}

}   // End namespace search
}   // End namespace witbook
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
