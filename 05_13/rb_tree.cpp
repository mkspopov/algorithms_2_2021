#include <cassert>
#include <iostream>
#include <vector>

enum class Color {
    BLACK,
    RED,
};

struct Node {
    int key = 0;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    Color color = Color::BLACK;
};

class RbTree {
public:
    ~RbTree() {
        Delete(root_);
    }

    bool Contains(int key) const {
        auto candidate = Find(key, root_);
        return candidate && candidate->key == key;
    }

    void Insert(int key) {
        auto candidate = Find(key, root_);
        if (candidate && candidate->key == key) {
            return;
        }
        ++size_;

        auto node = new Node();
        node->parent = candidate;
        node->left = NIL;
        node->right = NIL;
        node->key = key;
        node->color = Color::RED;

        if (!candidate) {
            assert(!root_);
            root_ = node;
        } else if (key < candidate->key) {
            candidate->left = node;
        } else {
            candidate->right = node;
        }

        InsertCaseOne(node);
    }

    void Print() const {
        std::cout << size_ << '\n';
        Print(root_);
    }

    void Print(Node* node) const {
        if (node && node != NIL) {
            Print(node->left);
            std::cout << node->key << ' ' << GetKey(node->left) << ' ' <<
                GetKey(node->right) << ' ' << GetColor(node) << '\n';
            Print(node->right);
        }
    }

    std::string GetKey(const Node* node) const {
        if (node == NIL) {
            return "null";
        }
        return std::to_string(node->key);
    }

    char GetColor(const Node* node) const {
        if (IsBlack(node)) {
            return 'B';
        }
        return 'R';
    }

private:
    void Delete(Node* node) {
        if (node && node != NIL) {
            Delete(node->left);
            Delete(node->right);
            delete node;
        }
    }

    Node* Find(int key, Node* node) const {
        if (!node) {
            return nullptr;
        }
        if (key < node->key) {
            if (node->left == NIL) {
                return node;
            }
            return Find(key, node->left);
        }
        if (key > node->key) {
            if (node->right == NIL) {
                return node;
            }
            return Find(key, node->right);
        }
        return node;
    }

    Node* GetUncle(Node* parent, Node* grandparent) const {
        assert(parent);
        assert(grandparent);
        if (parent == grandparent->left) {
            return grandparent->right;
        }
        return grandparent->left;
    }

    bool IsBlack(const Node* node) const {
        return node->color == Color::BLACK;
    }

    void InsertCaseOne(Node* node) {
        assert(node);
        if (node == root_) {
            node->color = Color::BLACK;
            return;
        }
        InsertCaseTwo(node);
    }

    void InsertCaseTwo(Node* node) {
        assert(node);
        assert(!IsBlack(node));
        auto parent = node->parent;
        assert(parent);
        if (IsBlack(parent)) {
            return;
        }
        InsertCaseThree(node, parent);
    }

    void InsertCaseThree(Node* node, Node* parent) {
        assert(node);
        assert(!IsBlack(node));
        assert(parent);
        assert(!IsBlack(parent));
        // Your code goes here
    }

    void InsertCaseFour(Node* node, Node* parent, Node* uncle, Node* grandparent) {
        assert(node);
        assert(!IsBlack(node));
        assert(parent);
        assert(!IsBlack(parent));
        assert(uncle);
        assert(IsBlack(uncle));
        assert(grandparent);
        // Your code goes here
    }

    void InsertCaseFive(/* Your code goes here */) {
        // Your code goes here
    }

    Node* RotateLeft(Node* node) {
        assert(node);
        auto right = node->right;
        assert(right && node->right != NIL);
        // Your code goes here
    }

    Node* RotateRight(Node* node) {
        assert(node);
        auto left = node->left;
        assert(left && node->left != NIL);
        // Your code goes here
    }

    static inline Node nil_;
    static inline Node* NIL = &nil_;
    Node* root_ = nullptr;
    size_t size_ = 0;
};

#include <random>

// Don't forget to look in the debugger to see the actual state of your tree.
// Or write more complicated tests.
void StressTest() {
    std::mt19937 gen;
    std::uniform_int_distribution<> dis(-100, 100);
    int numQueries = 1000;
    RbTree tree;
    std::vector<int> keys;
    for (int i = 0; i < numQueries; ++i) {
        int key = dis(gen);
        keys.push_back(key);
        tree.Insert(key);
        for (auto inserted : keys) {
            assert(tree.Contains(inserted));
        }
//        assert(IsRedBlack(tree));
    }
}

int main() {
    StressTest();
}
