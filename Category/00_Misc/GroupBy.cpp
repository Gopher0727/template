template <typename T>
concept StringOrVector = std::is_same_v<T, std::string> || requires(T t) {
    typename T::value_type;
    requires std::ranges::range<T>;
};
template <StringOrVector T>
auto getLength(const T& s) {
    vector<int> length {1};
    for (size_t i = 1; i < s.size(); ++i) {
        if (s[i] == s[i - 1]) {
            length.back()++;
        } else {
            length.push_back(1);
        }
    }
    return length;
}
template <StringOrVector T>
auto getGroup(const T& s) {
    vector<T> grouped;
    grouped.push_back({s[0]});
    for (size_t i = 1; i < s.size(); ++i) {
        if (s[i] == s[i - 1]) {
            grouped.back().push_back(s[i]);
        } else {
            grouped.push_back({s[i]});
        }
    }
    return grouped;
}
template <StringOrVector T>
auto groupBy(T& s) {
    vector<T> grouped;
    grouped.push_back({s[0]});
    for (size_t i = 1; i < s.size(); ++i) {
        if (s[i] == s[i - 1]) {
            grouped.back().push_back(s[i]);
        } else {
            grouped.push_back({s[i]});
        }
    }
    vector<int> length;
    for (auto& vec : grouped) {
        length.emplace_back(vec.size());
    }
    return pair(grouped, length);
}
