#include <iostream>

using namespace std;

class SuffixArray {
    int sz;
    char* str;
    int* sArr;
public:
    struct Pair{
        int index;
        int first;
        int second;
        bool operator<= (Pair& y) {
            if(this->first == y.first)
                return this->second <= y.second;
            return this->first <= y.first;
        }

        void operator= (Pair& y) {
            this->index = y.index;
            this->first = y.first;
            this->second = y.second;
        }
    };

    void Merge(Pair arr[], int l, int mid, int r) {
        int n = mid - l + 1;
        int m = r - mid;
        Pair L[n], R[m];
        for (int i = 0; i < n; i++)
            L[i] = arr[l + i];
        for (int i = 0; i < m; i++)
            R[i] = arr[mid + i + 1];

        int i = 0, j = 0;
        for(int k = l; k <= r; k++){
            if(i >= n) {
                arr[k] = R[j];
                j++;
            } else if (j >= m || L[i] <= R[j]) {
                arr[k] = L[i];
                i++;
            } else {
                arr[k] = R[j];
                j++;
            }
        }
    }

    void Sort(Pair arr[], int l, int r) {
        if(l >= r){
            return ;
        }
        int mid = (l + r) / 2;
        Sort(arr, l, mid);
        Sort(arr, mid + 1, r);
        Merge(arr, l, mid, r);
    }

    SuffixArray(char arr[]) {
        int sz = 0;
        while(arr[sz] != '\0') sz++;
        this->sz = sz;
        this->str = new char[sz];
        int i = 0;
        while(arr[i] != '\0') {
            this->str[i] = arr[i];
            i++;
        }
    }

    void ConstructUsingPrefixDoubling() {
        this->sArr = new int[sz];
        // iteration 0
        int chars[128];
        int x = 0;
        while(str[x] != '\0') chars[str[x]] = 1, x++;

        int mx = 0;
        for(int i = 0; i < 128; i++)
            if(chars[i] == 1) chars[i] = mx++;

        int* temp = new int[sz];
        for(int i = 0; i < sz; i++) temp[i] = chars[str[i]];
        // starting from iteration 1 to end.
        for(int p = 2; p <= sz; p *= 2) {
            Pair tmp[sz];
            for(int i = 0; i < sz; i++) {
                tmp[i].index = i;
                tmp[i].first = temp[i];
                if(i + p / 2 >= sz)
                    tmp[i].second = -1;
                else
                    tmp[i].second = temp[i + p / 2];
            }
            Sort(tmp, 0, sz - 1);
            int mx2 = 0;
            temp[tmp[0].index] = mx2;
            for(int i = 1; i < sz; i++) {
                if(tmp[i].first != tmp[i - 1].first || tmp[i].second != tmp[i - 1].second) {
                    temp[tmp[i].index] = ++mx2;
                } else {
                    temp[tmp[i].index] = mx2;
                }
            }
        }
        for(int i = 0; i < sz; i++) {
            sArr[temp[i]] = i;
        }
        delete[] temp;
    }

    void Print() {
        for(int i = 0; i < sz; i++) {
            cout << sArr[i] << " ";
        }
    }

    ~SuffixArray() {
        delete[] str;
        delete[] sArr;
    }
};
int main()
{
    SuffixArray t("ACGACTACGATAAC$");

    t.ConstructUsingPrefixDoubling();

    t.Print(); // Prints:  14 11 12  0  6  3  9 13  1  7  4  2  8 10  5

    return 0;
}
