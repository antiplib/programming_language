#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

#include <fcntl.h>

#include <map>


class Bor {
public:
    void add(const std::string& s) {
        node *cur = root;
        for (auto u : s) {
            if (cur->next[u - 'a'] == nullptr) {
                cur->next[u - 'a'] = new node();
            }
            cur = cur->next[u - 'a'];
        }
        ++cur->strings;
    }
    bool check(const std::string& s) {
        node *cur = root;
        for (auto u: s) {
            if(u - 'a' < 0)
                return 0;
            if (cur->next[u - 'a'] != nullptr) {
                cur = cur->next[u - 'a'];
            } else return false;
        }
        if (cur->strings > 0) return 1;
        else return 0;
    }
private:
    struct node {
        node *next[26];
        int strings;
        node() {
            for (int i = 0; i < 26; i++) {
                next[i] = nullptr;
            }
            strings = 0;
        }
    };
    node *root = new node();
};
