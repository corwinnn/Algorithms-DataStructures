#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

using std::string;
using std::vector;
using std::cout;
using std::cin;

const char smallestSymbol = '`';
const int differenceBetweenNumberInMapAndSymbolCode = 'a' - 1;
const int symbolsAmount = 27;

class SuffixArray {
public:
    explicit SuffixArray(const string& text) : sumNearLcp_(0),
                                               text_(text + smallestSymbol),
                                               textLen_(text.size() + 1)    {
        sortedSuffix_.resize(textLen_, 0);
        nearSuffixLcp_.resize(textLen_, 0);
        auto logSize = static_cast<unsigned int>(log2(textLen_) * 2 + 1);
        classes_.resize(logSize, vector<int> (textLen_, 0));
        int classes = firstStep();
        nextSteps(classes);
        makeLcp();
    }

    template <class OutputIterator>
    void show(OutputIterator &output) const {
        for (int i = 0; i < textLen_; i++) {
            *output =  sortedSuffix_[i];
            output++;
        }

        for (int i = 0; i < textLen_; i++) {
            for (int j = sortedSuffix_[i]; j < textLen_; j++) {
                *output = text_[j % textLen_];
                output++;
            }
        }
    }

    int getSumNearLcp() const {
        return sumNearLcp_;
    }

    int differentSubstrings() const {
        return textLen_*(textLen_ + 1) / 2 - getSumNearLcp();
    }

private:
    int firstStep() {
        vector<int> counted(symbolsAmount, 0);
        for (auto i: text_) {
            counted[i - differenceBetweenNumberInMapAndSymbolCode]++;
        }
        std::partial_sum(counted.begin(), counted.end(), counted.begin());
        for (int i = 0; i < textLen_; i++) {
            sortedSuffix_[--counted[text_[i] - differenceBetweenNumberInMapAndSymbolCode]] = i;
        }
        int classes = 1;
        for (int i = 1; i < textLen_; i++) {
            if (text_[sortedSuffix_[i]] != text_[sortedSuffix_[i - 1]]) {
                classes++;
            }
            classes_[0][sortedSuffix_[i]] = classes - 1;
        }
        return classes;
    }

    void nextSteps(int classesAmount) {
        vector<int> sortedSuffix(textLen_);
        vector<int> classes(textLen_);
        for (int h = 0; (1 << h) < textLen_; h++) {
            for (int i = 0; i < textLen_; i++) {
                sortedSuffix[i] = sortedSuffix_[i] - (1 << h);
                if (sortedSuffix[i] < 0) {
                    sortedSuffix[i] += textLen_;
                }
            }
            vector<int> counting(classesAmount, 0);
            for (int i = 0; i < textLen_; i++) {
                counting[classes_[h][sortedSuffix_[i]]]++;
            }
            std::partial_sum(counting.begin(), counting.end(), counting.begin());
            for (int i = textLen_ - 1; i >= 0; i--) {
                sortedSuffix_[--counting[classes_[h][sortedSuffix[i]]]] = sortedSuffix[i];
            }
            classes[sortedSuffix_[0]] = 0;
            classesAmount = 1;
            for (int i = 0; i < textLen_; i++) {
                int mid1 = (sortedSuffix_[i] + (1 << h)) % textLen_, mid2 =
                        (sortedSuffix_[i - 1] + (1 << h)) % textLen_;
                if (classes_[h][sortedSuffix_[i]] != classes_[h][sortedSuffix_[(i - 1 + textLen_) % textLen_]] ||
                    classes_[h][mid1] != classes_[h][mid2])
                    ++classesAmount;
                classes[sortedSuffix_[i]] = classesAmount - 1;
            }
            for (int i = 0; i < textLen_; i++) {
                classes_[h+1][i] = classes[i];
            }
        }
        sortedSuffix_.erase(sortedSuffix_.begin());
        textLen_--;
    }

    int lcp (int firstSuffixNumber, int secondSuffixNumber) const {
        int ans = 0;
        for (auto k = static_cast<int>(log2(textLen_)); k >= 0; --k)
            if (classes_[k][firstSuffixNumber] == classes_[k][secondSuffixNumber]) {
                ans += 1 << k;
                firstSuffixNumber += 1 << k;
                secondSuffixNumber += 1 << k;
            }
        return ans;
    }
    
    void makeLcp() {
        for (int i = 1; i < textLen_; i++) {
            nearSuffixLcp_[i] = lcp(sortedSuffix_[i-1], sortedSuffix_[i]);
            sumNearLcp_ += nearSuffixLcp_[i];
        }
    }
    vector<int> nearSuffixLcp_;
    int sumNearLcp_;
    string text_;
    size_t textLen_;
    vector<int> sortedSuffix_;
    vector<vector<int>> classes_;
};

int main() {
    string text;
    cin >> text;
    SuffixArray sufMas(text);
    int answer = sufMas.differentSubstrings();
    cout << answer;
    return 0;
}