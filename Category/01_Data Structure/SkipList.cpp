struct SkipList {
    int cnt;
    vector<int> key;
    vector<int> key_cnt;
    vector<int> level;
    vector<vector<int>> nxt;
    vector<vector<int>> len;

    explicit SkipList(int n) { init(n); }

    void init(int n) {
        cnt = 1;
        key.resize(n);
        key_cnt.resize(n);
        level.resize(n);
        nxt.resize(n, vector<int>(MAXL + 1));
        len.resize(n, vector<int>(MAXL + 1));
        key[cnt] = INT_MIN;
        level[cnt] = MAXL;
    }

    int randomLevel() {
        int ans = 1;
        while ((std::rand() / double(RAND_MAX)) < 0.5) {
            ans++;
        }
        return min(ans, MAXL);
    }

    int find(int i, int h, int num) {
        while (nxt[i][h] != 0 && key[nxt[i][h]] < num) {
            i = nxt[i][h];
        }
        if (h == 1) {
            if (nxt[i][h] != 0 && key[nxt[i][h]] == num) {
                return nxt[i][h];
            } else {
                return 0;
            }
        }
        return find(i, h - 1, num);
    }

    void addCount(int i, int h, int num) {
        while (nxt[i][h] != 0 && key[nxt[i][h]] < num) {
            i = nxt[i][h];
        }
        if (h == 1) {
            key_cnt[nxt[i][h]]++;
        } else {
            addCount(i, h - 1, num);
        }
        len[i][h]++;
    }

    int addNode(int i, int h, int j) {
        int rightCnt = 0;
        while (nxt[i][h] != 0 && key[nxt[i][h]] < key[j]) {
            rightCnt += len[i][h];
            i = nxt[i][h];
        }
        if (h == 1) {
            nxt[j][h] = nxt[i][h];
            nxt[i][h] = j;
            len[j][h] = key_cnt[nxt[j][h]];
            len[i][h] = key_cnt[nxt[i][h]];
            return rightCnt;
        } else {
            int downCnt = addNode(i, h - 1, j);
            if (h > level[j]) {
                len[i][h]++;
            } else {
                nxt[j][h] = nxt[i][h];
                nxt[i][h] = j;
                len[j][h] = len[i][h] + 1 - downCnt - key_cnt[j];
                len[i][h] = downCnt + key_cnt[j];
            }
            return rightCnt + downCnt;
        }
    }

    void add(int num) {
        if (find(1, MAXL, num) != 0) {
            addCount(1, MAXL, num);
        } else {
            key[++cnt] = num;
            key_cnt[cnt] = 1;
            level[cnt] = randomLevel();
            addNode(1, MAXL, cnt);
        }
    }

    void removeCount(int i, int h, int num) {
        while (nxt[i][h] != 0 && key[nxt[i][h]] < num) {
            i = nxt[i][h];
        }
        if (h == 1) {
            key_cnt[nxt[i][h]]--;
        } else {
            removeCount(i, h - 1, num);
        }
        len[i][h]--;
    }

    void removeNode(int i, int h, int j) {
        if (h < 1) {
            return;
        }
        while (nxt[i][h] != 0 && key[nxt[i][h]] < key[j]) {
            i = nxt[i][h];
        }
        if (h > level[j]) {
            len[i][h]--;
        } else {
            nxt[i][h] = nxt[j][h];
            len[i][h] += len[j][h] - 1;
        }
        removeNode(i, h - 1, j);
    }

    void remove(int num) {
        int j = find(1, MAXL, num);
        if (j != 0) {
            if (key_cnt[j] > 1) {
                removeCount(1, MAXL, num);
            } else {
                removeNode(1, MAXL, j);
            }
        }
    }

    int small(int i, int h, int num) {
        int rightCnt = 0;
        while (nxt[i][h] != 0 && key[nxt[i][h]] < num) {
            rightCnt += len[i][h];
            i = nxt[i][h];
        }
        if (h == 1) {
            return rightCnt;
        }
        return rightCnt + small(i, h - 1, num);
    }

    int getRank(int num) { return small(1, MAXL, num) + 1; }

    int index(int i, int h, int x) {
        int c = 0;
        while (nxt[i][h] != 0 && c + len[i][h] < x) {
            c += len[i][h];
            i = nxt[i][h];
        }
        if (h == 1) {
            return key[nxt[i][h]];
        }
        return index(i, h - 1, x - c);
    }

    int index(int x) { return index(1, MAXL, x); }

    int pre(int i, int h, int num) {
        while (nxt[i][h] != 0 && key[nxt[i][h]] < num) {
            i = nxt[i][h];
        }
        if (h == 1) {
            return i == 1 ? INT_MIN : key[i];
        }
        return pre(i, h - 1, num);
    }

    int pre(int num) { return pre(1, MAXL, num); }

    int post(int i, int h, int num) {
        while (nxt[i][h] != 0 && key[nxt[i][h]] < num) {
            i = nxt[i][h];
        }
        if (h == 1) {
            if (nxt[i][h] == 0) {
                return INT_MAX;
            }
            if (key[nxt[i][h]] > num) {
                return key[nxt[i][h]];
            }
            i = nxt[i][h];
            if (nxt[i][h] == 0) {
                return INT_MAX;
            } else {
                return key[nxt[i][h]];
            }
        }
        return post(i, h - 1, num);
    }

    int post(int num) { return post(1, MAXL, num); }
};
