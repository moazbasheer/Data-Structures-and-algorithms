#include <iostream>

using namespace std;
template<class T>
class BTreeNode{
public:
    BTreeNode<T>* parent;
    T* keys;
    BTreeNode<T>** children;
    int numKeys;

    BTreeNode(int order) {
        this->numKeys = 0;
        this->keys = new T[order + 3];
        this->children = new BTreeNode<T>*[order + 4];
        for(int i = 0; i < order + 4; i++)
            this->children[i] = NULL;
    }

    void print(string indentation) {
        cout << indentation;
        for(int i = 0; i < this->numKeys; i++) {
            cout << this->keys[i];
            if(i < this->numKeys - 1) {
                cout << ",";
            }
        }
        cout << "\n";
        for(int i = 0; i < this->numKeys + 1; i++) {
            if(children[i]) {
                children[i]->print(indentation + "  ");
            }
        }
    }
    void free() {
        this->parent = NULL;
        delete [] this->keys;
        for(int i = 0; i < this->numKeys + 1; i++) {
            if(this->children[i]){
                this->children[i]->free();
                delete [] this->children[i];
            }
        }
        delete [] this->children;
    }
};
template<class T, int n>
class BTree{
    BTreeNode<T>* root;
    int order;
public:
    BTree() {
        this->order = n;
        root = NULL;
    }

    int InsertInNode(T val, BTreeNode<T>*& node) {
        int idx = node->numKeys - 1;
        while(idx >= 0 && node->keys[idx] >= val) {
            node->keys[idx + 1] = node->keys[idx];
            idx--;
        }
        node->keys[idx + 1] = val;
        node->numKeys++;
        return idx;
    }

    BTreeNode<T>* splitNode(BTreeNode<T>*& node, int mid) {
        BTreeNode<T>* node2 = new BTreeNode<T>(order);
        node2->parent = node->parent;
        node2->numKeys = mid;

        for(int i = 0; i < mid; i++) {
            node2->keys[i] = node->keys[i + mid + 1];
        }
        for(int i = 0; i <= mid; i++) {
            node2->children[i] = node->children[i + mid + 1];
            node->children[i + mid + 1] = NULL;
            if(node2->children[i])
                node2->children[i]->parent = node2;
        }
        return node2;
    }

    void splitAndRaiseUp(BTreeNode<T>*& node) {
        int mid = node->numKeys / 2;
        if(node->parent == NULL) { // if it's root
            root = new BTreeNode<T>(order);
            root->keys[0] = node->keys[mid];
            root->numKeys = 1;
            root->parent = NULL;

            node->parent = root;
            node->numKeys = mid;

            BTreeNode<T>* node2 = splitNode(node, mid);
            node2->parent = root;

            root->children[0] = node;
            root->children[1] = node2;

        } else {

            int idx = InsertInNode(node->keys[mid], node->parent);
            node->numKeys = mid;

            for(int i = node->parent->numKeys; i >= idx + 1; i--) {
                node->parent->children[i + 1] = node->parent->children[i];
            }

            BTreeNode<T>* node2 = splitNode(node, mid);
            node->parent->children[idx + 1] = node2;

            swap(node2->keys, node->keys);
            swap(node2->children, node->children);
        }
    }

    void Insert(T val, BTreeNode<T>* node) {
        if(node->children[0] == NULL) { // if the node is leaf
            // whatever case, just insert.
            InsertInNode(val, node);
            if(node->numKeys > order - 1) { // if more than maximum.
                splitAndRaiseUp(node);
            }
            return;
        }

        bool found = false;
        for(int i = 0; i < node->numKeys; i++) {
            if(node->keys[i] > val) {
                found = true;
                Insert(val, node->children[i]);
                break;
            }
        }
        if(!found) {
            Insert(val, node->children[node->numKeys]);
        }
        if(node->numKeys > order - 1) { // if increased more than maximum.
            splitAndRaiseUp(node);
        }
    }

    void Insert(T value) {
        if(root == NULL) { // if the tree is empty.
            root = new BTreeNode<T>(order);
            root->keys[0] = value;
            root->numKeys = 1;
            root->parent = NULL;
            return ;
        }
        Insert(value, root);
    }

    void Print() {
        string indentation = "";
        root->print(indentation);
    }

    ~BTree() {
        root->free();
    }
};

int main()
{
    // Construct a BTree of order 3, which stores int data
    BTree<int,3> t1;

    t1.Insert(1);
    t1.Insert(5);
    t1.Insert(0);
    t1.Insert(4);
    t1.Insert(3);
    t1.Insert(2);

    t1.Print(); // Should output the following on the screen:
    /*
    1,4
      0
      2,3
      5
    */

    // Construct a BTree of order 5, which stores char data
    BTree<char,5> t;

    // Look at the example in our lecture:
    t.Insert('G');
    t.Insert('I');
    t.Insert('B');
    t.Insert('J');
    t.Insert('C');
    t.Insert('A');
    t.Insert('K');
    t.Insert('E');
    t.Insert('D');
    t.Insert('S');
    t.Insert('T');
    t.Insert('R');
    t.Insert('L');
    t.Insert('F');
    t.Insert('H');
    t.Insert('M');
    t.Insert('N');
    t.Insert('P');t.Print();
    return 0;
    t.Insert('Q');

    t.Print(); // Should output the following on the screen:

    /*
    K
      C,G
        A,B
        D,E,F
        H,I,J
      N,R
        L,M
        P,Q
        S,T
    */

    return 0;
}
