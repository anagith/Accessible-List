#include <iostream>

using namespace std;

class List {
    struct Node {
        int info_;
        bool isLeaf_;
        Node *parent_;
        Node *llink_;
        Node *rlink_;

        Node(int info = 0, bool isLeaf = true, Node *parent = nullptr, Node *llink = nullptr, Node *rlink = nullptr) :
                info_(info), isLeaf_(isLeaf), parent_(parent), llink_(llink), rlink_(rlink) {}
    };


    static void clear(Node *&tree);

    static void print(const Node *tree);

    pair<Node *, int> find(int pos) const;

public:
    List() : tree_(nullptr) {};

    ~List() { clear(tree_); }

    void print() const { print(tree_); }
    int &operator[](int pos) { return find(pos).first->info_; }

    void insert(int pos, int element);    

    void remove(int pos);

private:
    Node *tree_;
};

void
List::clear(Node *&tree) {
    if (tree == nullptr) return;

    if (!tree->isLeaf_) {
        clear(tree->llink_);
        clear(tree->rlink_);
    }

    delete tree;
    tree = nullptr;
}

void
List::print(const Node *tree) {
    if (tree == nullptr) return;

    if (tree->isLeaf_) {
        cout << tree->info_ << endl;
        return;
    }
    print(tree->llink_);
    print(tree->rlink_);
}

pair<List::Node *, int>
List::find(int pos) const {
    if (tree_ == nullptr)
        return make_pair(tree_, pos);

    Node *ptr = tree_;
    while (!ptr->isLeaf_)
        if (pos <= ptr->info_)
            ptr = ptr->llink_;
        else {
            pos -= ptr->info_;
            ptr = ptr->rlink_;
        }
    return make_pair(ptr, pos);
}

void
List::insert(int pos, int element) {
    Node *ptr = nullptr;
    if (tree_ == nullptr) {
        tree_ = new Node(element);
        return;
    }
    auto Pair = find(pos);
    ptr = Pair.first;
    if (Pair.second == 1) {
        ptr->rlink_ = new Node(ptr->info_);
        ptr->llink_ = new Node(element);
    } else {
        ptr->rlink_ = new Node(element);
        ptr->llink_ = new Node(ptr->info_);
    }
    ptr->rlink_->parent_ = ptr;
    ptr->llink_->parent_ = ptr;
    ptr->info_ = 1;
    ptr->isLeaf_ = false;
    Node *parent = nullptr;
    while (ptr != tree_) {
        parent = ptr->parent_;
        if (ptr == parent->llink_) {
            parent->info_++;
        }
        ptr = parent;
    }
}

void
List::remove(int pos) {
    Node *ptr = find(pos).first;
    Node *parent = ptr->parent_;
    if (ptr == tree_) {
        delete tree_;
        tree_ = nullptr;
        return;
    }
    if (parent == tree_) {
        if (ptr == parent->llink_) {
            tree_ = parent->rlink_;
            delete ptr;
            ptr = nullptr;
            return;
        }
        if (ptr == parent->rlink_) {
            tree_ = parent->llink_;
            delete ptr;
            ptr = nullptr;
            return;
        }
    } else {
        Node *temp = nullptr;
        if (ptr == parent->llink_) {
            parent->info_ = parent->rlink_->info_;
            delete ptr;
            ptr = nullptr;
            delete parent->rlink_;
            parent->isLeaf_ = true;

        }
        if (ptr == parent->rlink_) {
            parent->info_ = parent->llink_->info_;
            delete ptr;
            ptr = nullptr;
            delete parent->llink_;
            parent->isLeaf_ = true;

        }
    }
    ptr = parent;
    while (ptr != tree_) {
        parent = ptr->parent_;
        if (ptr == parent->llink_) {
            parent->info_--;
        }
        ptr = parent;
    }
}


int main()
{
    List lst;
    cout << endl << "*****   INSERTIONS: *****" << endl;
    lst.insert(1, 10);
    lst.insert(1, 20);
    lst.insert(2, 30);
    lst.insert(4, 50);
    lst.insert(2, 60);
    lst.insert(4, 40);
    lst.print();
    cout << endl << "*****   REMOVAL IN 3-rd POSITION: *****" << endl;
    lst.remove(3);
    lst.print();

    cout << endl << "*****   REMOVAL IN 4-th POSITION: *****" << endl;
    lst.remove(4);
    lst.print();

    cout << endl << "*****   REMOVAL IN 1-st POSITION: *****" << endl;
    lst.remove(1);
    lst.print();

    return 0;
}
