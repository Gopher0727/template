// implementation for small-case English alphabets `a-z`
class trie {
private:
    static constexpr uint8_t NUM_CHARS = 26; // Number of alphabets

    /** @brief Recursive tree nodes as an array of shared-pointers */
    std::array<std::shared_ptr<trie>, (NUM_CHARS << 1)> arr;

    bool isEndofWord = false; // identifier if a node is terminal node

    // Convert a character to unsigned integer for indexing
    uint8_t char_to_int(const char& ch) const {
        if (ch >= 'A' && ch <= 'Z') {
            return ch - 'A';
        } else if (ch >= 'a' && ch <= 'z') {
            return ch - 'a' + NUM_CHARS;
        }
        std::cerr << "Invalid character present. Exiting...";
        std::exit(EXIT_FAILURE);
        return 0;
    }

    // search a string exists inside a given root trie
    bool search(const std::shared_ptr<trie>& root, const std::string& str, int index) {
        if (index == str.length()) {
            if (!root->isEndofWord) {
                return false;
            }
            return true;
        }
        int j = char_to_int(str[index]);
        if (!root->arr[j]) {
            return false;
        }
        return search(root->arr[j], str, index + 1);
    }

public:
    trie() = default;

    void insert(const std::string& str) {
        std::shared_ptr<trie> root(nullptr);

        for (const char& ch : str) {
            int j = char_to_int(ch);
            if (root) {
                if (root->arr[j]) {
                    root = root->arr[j];
                } else {
                    std::shared_ptr<trie> temp(new trie());
                    root->arr[j] = temp;
                    root = temp;
                }
            } else if (arr[j]) {
                root = arr[j];
            } else {
                std::shared_ptr<trie> temp(new trie());
                arr[j] = temp;
                root = temp;
            }
        }
        root->isEndofWord = true;
    }

    bool search(const std::string& str, int index) {
        if (index == str.length()) {
            if (!isEndofWord) {
                return false;
            }
            return true;
        }
        int j = char_to_int(str[index]);
        if (!arr[j]) {
            return false;
        }
        return search(arr[j], str, index + 1);
    }

    // removes the string if it is not a prefix of any other string, if it is then just
    // sets the trie::isEndofWord to false, else removes the given string
    bool deleteString(const std::string& str, int index) {
        if (index == str.length()) {
            if (!isEndofWord) {
                return false;
            }
            isEndofWord = false;
            // following lines - possible source of error?
            // for (int i = 0; i < NUM_CHARS; i++)
            //     if (!arr[i])
            //         return false;
            return true;
        }
        int j = char_to_int(str[index]);
        if (!arr[j]) {
            return false;
        }
        bool var = deleteString(str, index + 1);
        if (var) {
            arr[j].reset();
            if (isEndofWord) {
                return false;
            } else {
                int i = 0;
                for (i = 0; i < NUM_CHARS; i++) {
                    if (arr[i]) {
                        return false;
                    }
                }
                return true;
            }
        }
        /* should not return here */
        std::cout << __func__ << ":" << __LINE__ << "Should not reach this line\n";
        return false;
    }
};
