template <typename T>
concept StringOrVector = std::is_same_v<T, std::string> || requires(T t) {
    typename T::value_type;
    requires std::ranges::range<T>;
};
template <StringOrVector T>
struct groupBy {
    std::vector<T> grouped;
    std::vector<int> length;

public:
    groupBy() {}
    groupBy(T& s) {
        grouped.push_back({s[0]});
        for (size_t i = 1; i < s.size(); ++i) {
            if (s[i] == s[i - 1]) {
                grouped.back().push_back(s[i]);
            } else {
                grouped.push_back({s[i]});
            }
        }
        for (auto& vec : grouped) {
            length.emplace_back(vec.size());
        }
    }

    auto getGroup(const T& s) {
        grouped.push_back({s[0]});
        for (size_t i = 1; i < s.size(); ++i) {
            if (s[i] == s[i - 1]) {
                grouped.back().push_back(s[i]);
            } else {
                grouped.push_back({s[i]});
            }
        }
    }

    auto getLength(const T& s) {
        length.push_back(1);
        for (size_t i = 1; i < s.size(); ++i) {
            if (s[i] == s[i - 1]) {
                length.back()++;
            } else {
                length.push_back(1);
            }
        }
    }
};
