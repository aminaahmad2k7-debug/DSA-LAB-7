#include <iostream>
#include <string>
#include <stack>
#include <vector> 
#include <cctype>
using namespace std;

/* I used vector here instead of a simple array because <vector>provides a resizable array.
The lab asks for a method that takes a list of integers, so I wrote :
A vector works better than the raw array int keys[] because It carries its own size.*/

/* ===================== Data structures ===================== */

struct TreeNode {
    char val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(char c) : val(c), left(nullptr), right(nullptr) {}
};

struct BSTNode {
    int key;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int k) : key(k), left(nullptr), right(nullptr) {}
};

/* ===================== 1. Build tree from string ===================== */

/* Recursive-descent parser. 'i' is the current position in 's' and is
 * advanced as characters are consumed. Returns nullptr for an empty child.
 * Assumes the string was already validated. O(n). */
TreeNode* buildTree(const string& s, int& i) {
    if (i >= (int)s.size() || s[i] == ',' || s[i] == ')')
        return nullptr;

    TreeNode* root = new TreeNode(s[i++]);

    if (i < (int)s.size() && s[i] == '(') {
        i++;                                   // skip '('
        root->left = buildTree(s, i);
        if (i < (int)s.size() && s[i] == ',') i++;   // skip ','
        root->right = buildTree(s, i);
        if (i < (int)s.size() && s[i] == ')') i++;   // skip ')'
    }
    return root;
}

TreeNode* constructFromString(const string& s) {
    int i = 0;
    return buildTree(s, i);
}

/* Frees every node of the tree (postorder). */
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

/* ===================== 2. Preorder (recursive) ===================== */

/* Appends the preorder string form of the tree to 'result'.
 * Leaves print as a single letter; other nodes as X(left,right). */
void preorder(TreeNode* root, string& result) {
    if (!root) return;
    result += root->val;
    if (root->left || root->right) {
        result += '(';
        preorder(root->left, result);
        result += ',';
        preorder(root->right, result);
        result += ')';
    }
}

string getPreorder(TreeNode* root) {
    string res;
    preorder(root, res);
    return res;
}

/* ===================== 3. Inorder (iterative, stack) ===================== */

/* Left, Root, Right using an explicit stack instead of recursion. O(n). */
void inorderIterative(TreeNode* root) {
    stack<TreeNode*> st;
    TreeNode* curr = root;

    cout << "Inorder: ";
    while (curr != nullptr || !st.empty()) {
        while (curr != nullptr) {          // go as far left as possible
            st.push(curr);
            curr = curr->left;
        }
        curr = st.top();
        st.pop();
        cout << curr->val << " ";          // visit
        curr = curr->right;                // then the right subtree
    }
    cout << endl;
}

/* ===================== 4. Postorder (iterative, one stack) ===================== */

/* Left, Right, Root using a single stack. 'last' remembers the most recently
 * visited node so we know whether the right subtree is already done. O(n). */
void postorderIterative(TreeNode* root) {
    stack<TreeNode*> st;
    TreeNode* curr = root;
    TreeNode* last = nullptr;

    cout << "Postorder: ";
    while (curr != nullptr || !st.empty()) {
        if (curr != nullptr) {
            st.push(curr);
            curr = curr->left;
        }
        else {
            TreeNode* top = st.top();
            if (top->right != nullptr && last != top->right) {
                curr = top->right;         // right subtree not visited yet
            }
            else {
                cout << top->val << " ";   // both children done: visit
                last = top;
                st.pop();
            }
        }
    }
    cout << endl;
}

/* ===================== 5. Validate string (no tree built) ===================== */

bool validNode(const string& s, size_t& i);

/* A child is either empty (next char is ',' or ')') or a full node. */
bool validChild(const string& s, size_t& i) {
    if (i < s.size() && (s[i] == ',' || s[i] == ')'))
        return true;
    return validNode(s, i);
}

/* node := letter | letter '(' child ',' child ')'
 * This enforces balanced parentheses AND correct child placement:
 * exactly two child slots, separated by exactly one comma. */
bool validNode(const string& s, size_t& i) {
    if (i >= s.size() || !isalpha((unsigned char)s[i])) return false;
    i++;
    if (i < s.size() && s[i] == '(') {
        i++;
        if (!validChild(s, i)) return false;
        if (i >= s.size() || s[i] != ',') return false;
        i++;
        if (!validChild(s, i)) return false;
        if (i >= s.size() || s[i] != ')') return false;
        i++;
    }
    return true;
}

bool isValidTreeString(const string& s) {
    size_t i = 0;
    return validNode(s, i) && i == s.size();   // must consume the whole string
}

/* ===================== 6. Height and depth ===================== */

/* Height counted in edges: empty tree = -1, single node = 0. */
int height(TreeNode* root) {
    if (!root) return -1;
    int leftH = height(root->left);
    int rightH = height(root->right);
    return 1 + (leftH > rightH ? leftH : rightH);
}

/* Prints each node with its depth (root has depth 0), recursively. */
void printDepths(TreeNode* root, int depth = 0) {
    if (!root) return;
    cout << "Node " << root->val << "  depth " << depth << endl;
    printDepths(root->left, depth + 1);
    printDepths(root->right, depth + 1);
}

/* ===================== 7. BST from a list of integers ===================== */

BSTNode* insertBST(BSTNode* root, int key) {
    if (!root) return new BSTNode(key);
    if (key < root->key)
        root->left = insertBST(root->left, key);
    else if (key > root->key)
        root->right = insertBST(root->right, key);
    return root;                               // duplicates are ignored
}

/* Builds a BST; the first element of the list becomes the root. */
BSTNode* buildBST(const vector<int>& keys) {
    BSTNode* root = nullptr;
    for (int k : keys)
        root = insertBST(root, k);
    return root;
}

void inorderBST(BSTNode* root) {
    if (!root) return;
    inorderBST(root->left);
    cout << root->key << " ";
    inorderBST(root->right);
}

void freeBST(BSTNode* root) {
    if (!root) return;
    freeBST(root->left);
    freeBST(root->right);
    delete root;
}

/* ===================== main ===================== */

int main() {
    string treeStr = "a(b(d,e),c(,f(g,)))";

    cout << "Original string: " << treeStr << endl;
    bool ok = isValidTreeString(treeStr);
    cout << "Valid? " << (ok ? "YES" : "NO") << endl;

    if (ok) {
        TreeNode* root = constructFromString(treeStr);

        cout << "Preorder: " << getPreorder(root) << endl;
        inorderIterative(root);
        postorderIterative(root);

        cout << "Height: " << height(root) << endl;
        cout << "Depths of nodes:\n";
        printDepths(root);

        freeTree(root);
    }

    // Validator test cases
    cout << "\nValidator tests:\n";
    const string tests[] = {
        "a(b,c)", "a(,c)", "a(b,)", "a",                  // valid
        "a()", "a(b,c,d)", "a(b,,c)", "a,b", "a(b,c",
        "a(b,c))", "(a,b)", "a(b c)", ""                   // invalid
    };
    for (const string& t : tests)
        cout << "  \"" << t << "\" -> " << (isValidTreeString(t) ? "valid" : "invalid") << endl;

    // Part 7
    vector<int> keys = { 50, 30, 70, 20, 40, 60, 80 };
    cout << "\nBST from [50, 30, 70, 20, 40, 60, 80]:\n";
    BSTNode* bst = buildBST(keys);
    cout << "Inorder of BST: ";
    inorderBST(bst);
    cout << endl;
    freeBST(bst);

    return 0;
}