#include <iostream>
#include <vector>
#include <iterator>

using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::max;
using std::min;
using std::ios_base;


class ZFunction {
public:
    explicit ZFunction(const string& pattern) : pattern_(pattern){
        size_ = pattern.size();
        zf_.resize(pattern_.size(), 0);
        std::ostream_iterator<int> output(cout," ");
        vector<char> garbich(0,0);
        universalZFunction(pattern, output , 1, garbich, zf_, false);
    }
    template <class OutputIterator>
    void universalZFunction(const string &text, OutputIterator &output, int size, vector<char> &text_part, vector<int> &text_Zf, bool withIterator){
        int current = 0, right = 0, left = 0;
        int i = 0;
        int j = size - 1;
        while(i != text.size() - size + 1) {
            if (withIterator) {
                text_part[j % size_] = text[i + size_ - 1];
                text_Zf[j % size_] = 0;
                current = i % size_;
            }
            if (i <= right) {
                text_Zf[current] = min(zf_[i - left], right - i + 1);
            }
            if (withIterator) {
                while (text_Zf[current] < size_ &&
                       text_part[(i + text_Zf[current]) % size_] == pattern_[text_Zf[current]]) {
                    text_Zf[current]++;
                }
            }
            else {
                while(i + zf_[i] < pattern_.size() && pattern_[zf_[i]] == pattern_[i + zf_[i]])
                    ++zf_[i];
            }
            if (i + text_Zf[current] - 1 > right) {
                left = i;
                right = i + text_Zf[current] - 1;
            }
            if (withIterator) {
                if (text_Zf[current] == size_) {
                    *output = i;
                    ++output;
                }
            }
            ++i;
            ++j;

        }
    }
    template <class OutputIterator>
    OutputIterator searcher(const string &text, OutputIterator &output){
        vector<int> text_Zf(size_);
        vector<char> text_part(size_);
        int j;
        for(j = 0; j < size_ - 1; j++) {
            text_part[j] = text[j];
        }
        universalZFunction(text, output, size_, text_part, text_Zf, true);
        return output;
    }


private:
    string pattern_;
    int size_;
    vector<int> zf_;

};


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    string pattern, text;
    cin >> pattern;
    cin >> text;
    ZFunction z(pattern);
    std::ostream_iterator<int> output(cout," ");
    std::ostream_iterator<int> outputIterator = z.searcher<std::ostream_iterator<int>>(text, output);
    return 0;
}