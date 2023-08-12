#include <iostream>
#include <vector>

using namespace std;

vector<int> next;

void GetNext(string T, vector<int>& next) {
    next.resize(T.size());

    int j = 0, k = -1;
    next[0] = -1;       // initialization
    while(j < T.size()) {
        if (k == -1 || T[j] == T[k]) {
            // transfer to the next position
            j++; 
            k++;
            next[j] = k;
        } else {
            // use the next array to shift to the left position
            k = next[k];
        }
    }

}

int StrMatch_KMP(string S, string T, int pos=0) {
    int i = pos, j = 0;
    while(i < S.size() && j < T.size()) {
        if(S[i] == T[j]) {
            // just go to the next step
            i++; 
            j++;
        } else {
            j = next[j];
            if (j == 0) {    // 模式串的初始字符就匹配失败了
                i++;        // 此时就要略过主串的失配位置，向后移
                j++;        // 模式串位置也要从 -1 变成 0
            }
        }
    }

    // check if the j get to the final position
    if(j == T.size()) {
        return i-T.size();
    } else {
        return -1;
    }
}


int main() {
    string P("abacab"), T("abacaabaccabacab");
    GetNext(P, next);
    cout << "The KMP corresponding solution is:" << StrMatch_KMP(T, P);
    return 0;
}
