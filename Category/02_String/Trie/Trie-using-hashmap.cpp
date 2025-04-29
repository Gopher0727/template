/**
* @brief Trie class, implementation of trie using hashmap in each trie node
* for all the characters of char16_t(UTF-16)type with methods to insert,
* delete, search, start with and to recommend words based on a given
* prefix.
*/
class Trie {
private:
    struct Node {
        // unordered map with key type char16_t and value is a shared pointer type of Node
        std::unordered_map<char16_t, std::shared_ptr<Node>> children;
        // boolean variable to represent the node end
        bool word_end = false;
    };

    // declaring root node of trie
    std::shared_ptr<Node> root_node = std::make_shared<Node>();

public:
    Trie() = default;

    void insert(const std::string& word) {
        std::shared_ptr<Node> curr = root_node;
        for (char ch : word) {
            if (curr->children.find(ch) == curr->children.end()) {
                curr->children[ch] = std::make_shared<Node>();
            }
            curr = curr->children[ch];
        }
        if (!curr->word_end && curr != root_node) {
            curr->word_end = true;
        }
    }

    bool search(const std::string& word) {
        std::shared_ptr<Node> curr = root_node;
        for (char ch : word) {
            if (curr->children.find(ch) == curr->children.end()) {
                return false;
            }
            curr = curr->children[ch];
            if (!curr) {
                return false;
            }
        }
        return curr->word_end;
    }

    bool startwith(const std::string& prefix) {
        std::shared_ptr<Node> curr = root_node;
        for (char ch : prefix) {
            if (curr->children.find(ch) == curr->children.end()) {
                return false;
            }
            curr = curr->children[ch];
        }
        return true;
    }

    void delete_word(std::string word) {
        std::shared_ptr<Node> curr = root_node;
        std::stack<std::shared_ptr<Node>> nodes;
        int cnt = 0;
        for (char ch : word) {
            if (curr->children.find(ch) == curr->children.end()) {
                return;
            }
            if (curr->word_end) {
                cnt++;
            }

            nodes.push(curr->children[ch]);
            curr = curr->children[ch];
        }
        // Delete only when it's a word, and it has children after
        // or prefix in the line
        if (nodes.top()->word_end) {
            nodes.top()->word_end = false;
        }
        // Delete only when it has no children after
        // and also no prefix in the line
        while (!(nodes.top()->word_end) && nodes.top()->children.empty()) {
            nodes.pop();
            nodes.top()->children.erase(word.back());
            word.pop_back();
        }
    }

    /**
    * @brief helper function to predict/recommend words that starts with a
    * given prefix from the end of prefix's node iterate through all the child
    * nodes by recursively appending all the possible words below the trie
    * @param prefix string to recommend the words
    * @param element node at the end of the given prefix
    * @param results list to store the all possible words
    * @returns list of recommended words
    */
    std::vector<std::string> get_all_words(std::vector<std::string> results, const std::shared_ptr<Node>& element, std::string prefix) {
        if (element->word_end) {
            results.push_back(prefix);
        }
        if (element->children.empty()) {
            return results;
        }
        for (auto const& x : element->children) {
            std::string key = "";
            key = x.first;
            prefix += key;
            results = get_all_words(results, element->children[x.first], prefix);
            prefix.pop_back();
        }
        return results;
    }

    // predict/recommend a word that starts with a given prefix and return a list of recommended words
    std::vector<std::string> predict_words(const std::string& prefix) {
        std::vector<std::string> result;
        std::shared_ptr<Node> curr = root_node;
        // traversing until the end of the given prefix in trie
        for (char ch : prefix) {
            if (curr->children.find(ch) == curr->children.end()) {
                return result;
            }
            curr = curr->children[ch];
        }
        // if the given prefix is the only word without children
        if (curr->word_end && curr->children.empty()) {
            result.push_back(prefix);
            return result;
        }
        // iteratively and recursively get the recommended words
        return get_all_words(result, curr, prefix);
    }
};
