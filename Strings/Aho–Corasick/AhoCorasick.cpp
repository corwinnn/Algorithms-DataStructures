#include <iostream>
#include <vector>
#include <map>
#include <cstdio>

using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::map;

struct AhoCorasickNode {
    vector<int> ends;
    map<char, int> next;
    map<char, int> automaton;
    vector<int> terminals;
    bool isTerminalCounted = false;
    int parent;
    char prevSymbol;
    int suffixLink = -1;
    int compressedSuffixLink = -1;
    AhoCorasickNode(int parent, char symbol) :
            parent(parent),
            prevSymbol(symbol)
    {}
};

class Trie {
public:
    friend class Automaton;
    explicit Trie(const vector<string> &strings) {
        vertexes.emplace_back(-1, '#');
        for (size_t i = 0; i < strings.size(); i++) {
            addString(strings[i], i);
        }
    }

private:
    vector<AhoCorasickNode> vertexes;

    void addString(const string &s, int stringId) {
        int curVertexId = 0;
        for (auto symbol : s) {
            auto &next = vertexes[curVertexId].next[symbol];
            if (!next) {
                vertexes.emplace_back(AhoCorasickNode(curVertexId, symbol));
                next = vertexes.size() - 1;
            }
            curVertexId = next;
        }
        vertexes[curVertexId].ends.push_back(stringId);
    }

};
class Automaton {
public:
    explicit Automaton(vector<string> givenPatterns, vector<int> patternIndexes) :
            trie(givenPatterns),
            patterns(givenPatterns),
            patternIndexes(std::move(patternIndexes))
    {}

    void findAll(const string &text, vector<unsigned short> &countSuitable) {
        int u = 0;
        for (int i = 0; i < text.length(); ++i) {
            u = getAutomaton(u, text[i]);
            check(u, i + 1, countSuitable);
        }
    }

private:
    Trie trie;
    vector<string> patterns;
    vector<int> patternIndexes;

    int getSuffixLink(int v) {
        if (trie.vertexes[v].suffixLink == -1) {
            if (v == 0 || trie.vertexes[v].parent == 0) {
                trie.vertexes[v].suffixLink = 0;
            }
            else {
                trie.vertexes[v].suffixLink = getAutomaton(getSuffixLink(trie.vertexes[v].parent),
                                                           trie.vertexes[v].prevSymbol);
            }
        }
        return trie.vertexes[v].suffixLink;
    }

    int getAutomaton(int v, char c) {
        auto &vertexAutomaton = trie.vertexes[v].automaton[c];
        if (!vertexAutomaton) {
            if (trie.vertexes[v].next[c])
                vertexAutomaton = trie.vertexes[v].next[c];
            else {
                if (v == 0) {
                    vertexAutomaton = 0;
                } else {
                    vertexAutomaton = getAutomaton(getSuffixLink(v), c);
                }
            }
        }
        return vertexAutomaton;
    }

    int getCompressedSuffixLink(int v) {
        int& link= trie.vertexes[v].compressedSuffixLink;
        if (link == -1) {
            int u = getSuffixLink(v);
            if (!u) {
                link = 0;
            } else {
                link = trie.vertexes[u].ends.size() ? u : getCompressedSuffixLink(u);
            }
        }
        return link;
    }

    void check(int v, int i, vector<unsigned short> &countSuitable) {
        if (trie.vertexes[v].isTerminalCounted) {
            for (int j = 0; j < trie.vertexes[v].terminals.size(); j++) {
                if (trie.vertexes[v].terminals[j] < patternIndexes.size()) {
                    int pos = (i - patterns[trie.vertexes[v].terminals[j]].size()) - patternIndexes[trie.vertexes[v].terminals[j]];
                    if (pos >= 0) {
                        countSuitable[pos]++;
                    }
                }
            }
        } else {
            trie.vertexes[v].isTerminalCounted = true;
            for (int u = v; u != 0; u = getCompressedSuffixLink(u)) {
                if (trie.vertexes[u].ends.size()) {
                    for (auto j : trie.vertexes[u].ends) {
                        trie.vertexes[v].terminals.push_back(j);
                        int pos = (i - patterns[j].size()) - patternIndexes[j];
                        if (pos >= 0) {
                            countSuitable[pos]++;
                        }
                    }
                }
            }
        }
    }

};

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    vector<string> patterns;
    vector<int> patternsIndexes;
    string templateString, tmp = "";
    cin >> templateString;
    string templateStringWithEnd = templateString + "?";

    for (size_t i = 0; i < templateStringWithEnd.size(); ++i) {
        if (templateStringWithEnd[i] == '?') {
            if (tmp != "") {
                patterns.push_back(tmp);
                patternsIndexes.push_back(i - tmp.size());
            }
            tmp = "";
        }
        else {
            tmp += templateString[i];
        }
    }

    Automaton automaton(patterns, patternsIndexes);

    string text;
    cin >> text;
    vector<unsigned short> countSuitable(text.size(), 0);

    automaton.findAll(text, countSuitable);

    if (templateString.size() > text.size()) {
        return 0;
    }

    for (int i = 0; i < text.size() - templateString.size() + 1; i++) {
        if (countSuitable[i] == patterns.size()) {
            cout << i << " ";
        }
    }

    return 0;
}