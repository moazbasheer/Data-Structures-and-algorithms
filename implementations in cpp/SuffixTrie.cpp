#include <iostream>

using namespace std;
class LinkedList;
class SuffixTrieNode {
public:
    LinkedList* children;
    int index;
    SuffixTrieNode();
    SuffixTrieNode(int i);
};
class LinkedListNode {
public:
    char c;
    SuffixTrieNode* suffixNode;
    LinkedListNode* next;
};
class LinkedList{
public:
    LinkedListNode* root;
    LinkedList() {
        root = NULL;
    }
    SuffixTrieNode* getChar(int ch) {
        LinkedListNode* node = root;
        while(node) {
            if(node->c == ch)
                return node->suffixNode;
            node = node->next;
        }
        return NULL;
    }
    void addChar(int ch) {
        if(root == NULL) {
            root = new LinkedListNode();
            root->c = ch;
            root->suffixNode = new SuffixTrieNode();
            root->next = NULL;
        }else {
            LinkedListNode* node = root;
            while(node->next) {
                if(node->c == ch)
                    return ;
                node = node->next;
            }
            node->next = new LinkedListNode();
            node->next->c = ch;
            node->next->suffixNode = new SuffixTrieNode();
            node->next->next = NULL;
        }
    }
};

SuffixTrieNode::SuffixTrieNode() {
    this->children = new LinkedList();
    this->index = -1; // means that it does not have an index.
}
SuffixTrieNode::SuffixTrieNode(int i) {
    this->children = new LinkedList();
    this->index = i;
}
class SuffixTrie {
public:
    SuffixTrieNode* root;
public:
    SuffixTrie(char arr[]) {
        int i = 0;
        this->root = new SuffixTrieNode();

        while(arr[i] != '\0') {
            int j = i;
            SuffixTrieNode* cur = this->root;
            while(arr[j] != '\0') {

                if(cur->children->getChar(arr[j]) == NULL) {
                    cur->children->addChar(arr[j]);
                }

                cur = cur->children->getChar(arr[j]);

                if(arr[j] == '$') {
                    cur->index = i;
                }
                j++;
            }
            i++;
        }
    }

    void Search(char arr[]) {
        int i = 0;
        SuffixTrieNode* cur = this->root;
        while(arr[i] != '\0') {

            if(cur->children->getChar(arr[i]) == NULL) {
                return; // this sub-string is not found at all.
            }

            cur = cur->children->getChar(arr[i]);
            i++;
        }

        // Do dfs on the current node.
        dfs(cur);
        cout << "\n";
    }

    void dfs(SuffixTrieNode* cur) {
        if(cur->index != -1) {
            cout << cur->index << " ";
            return;
        }
        LinkedListNode* node = cur->children->root;
        while(node) {
            if(node->suffixNode != NULL) {
                dfs(node->suffixNode);
            }
            node = node->next;
        }
    }

};
int main()
{
    // Construct a suffix trie containing all suffixes of "bananabanaba$"

    //            0123456789012
    SuffixTrie t("bananabanaba$");

    t.Search("ana"); // Prints: 1 3 7
    t.Search("naba"); // Prints: 4 8

    return 0;
}
