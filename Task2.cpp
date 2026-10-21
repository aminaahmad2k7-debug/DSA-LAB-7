#include <iostream>
#include <string>
using namespace std;

struct Student {
    int studentID;
    string name;
    float GPA;
    Student* left;
    Student* right;

    Student(int id, const string& n, float g)
        : studentID(id), name(n), GPA(g), left(nullptr), right(nullptr) {
    }
};

class StudentBST {
private:
    Student* root;

    /* ---------- recursive helpers ---------- */

    /* Inserts a record; sets 'inserted' to false if the ID already exists. */
    Student* insert(Student* node, int id, const string& name, float gpa, bool& inserted) {
        if (!node) {
            inserted = true;
            return new Student(id, name, gpa);
        }
        if (id < node->studentID)
            node->left = insert(node->left, id, name, gpa, inserted);
        else if (id > node->studentID)
            node->right = insert(node->right, id, name, gpa, inserted);
        else
            inserted = false;                  // duplicate ID
        return node;
    }

    Student* search(Student* node, int id) const {
        if (!node || node->studentID == id) return node;
        return (id < node->studentID) ? search(node->left, id)
            : search(node->right, id);
    }

    Student* findMin(Student* node) const {
        while (node && node->left) node = node->left;
        return node;
    }

    /* Deletes by ID. Handles leaf, one-child and two-child cases
     * (two children: replace with inorder successor). */
    Student* remove(Student* node, int id, bool& removed) {
        if (!node) return nullptr;

        if (id < node->studentID)
            node->left = remove(node->left, id, removed);
        else if (id > node->studentID)
            node->right = remove(node->right, id, removed);
        else {
            removed = true;
            if (!node->left) {
                Student* temp = node->right;
                delete node;
                return temp;
            }
            if (!node->right) {
                Student* temp = node->left;
                delete node;
                return temp;
            }
            Student* succ = findMin(node->right);
            node->studentID = succ->studentID;
            node->name = succ->name;
            node->GPA = succ->GPA;
            bool dummy = false;
            node->right = remove(node->right, succ->studentID, dummy);
        }
        return node;
    }

    void printStudent(const Student* s) const {
        cout << "ID: " << s->studentID
            << " | Name: " << s->name
            << " | GPA: " << s->GPA << endl;
    }

    void inorder(Student* node) const {
        if (!node) return;
        inorder(node->left);
        printStudent(node);
        inorder(node->right);
    }

    int count(Student* node) const {
        if (!node) return 0;
        return 1 + count(node->left) + count(node->right);
    }

    void findExtremes(Student* node, float& maxG, float& minG) const {
        if (!node) return;
        if (node->GPA > maxG) maxG = node->GPA;
        if (node->GPA < minG) minG = node->GPA;
        findExtremes(node->left, maxG, minG);
        findExtremes(node->right, maxG, minG);
    }

    /* Prints every student whose GPA equals 'target' (handles ties). */
    void printWithGPA(Student* node, float target) const {
        if (!node) return;
        printWithGPA(node->left, target);
        if (node->GPA == target) printStudent(node);
        printWithGPA(node->right, target);
    }

    /* Prints students with GPA strictly above 'th'; returns how many. */
    int aboveThreshold(Student* node, float th) const {
        if (!node) return 0;
        int n = aboveThreshold(node->left, th);
        if (node->GPA > th) { printStudent(node); n++; }
        return n + aboveThreshold(node->right, th);
    }

    void destroy(Student* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    StudentBST() : root(nullptr) {}
    ~StudentBST() { destroy(root); }
    StudentBST(const StudentBST&) = delete;            // avoid double-free
    StudentBST& operator=(const StudentBST&) = delete;

    /* ---------- public operations (display + logic) ---------- */

    void insert(int id, const string& name, float gpa) {
        bool inserted = false;
        root = insert(root, id, name, gpa, inserted);
        if (inserted) cout << "Inserted student " << id << " (" << name << ")\n";
        else          cout << "Insert failed: ID " << id << " already exists.\n";
    }

    void search(int id) const {
        Student* s = search(root, id);
        if (s) { cout << "Found -> "; printStudent(s); }
        else   cout << "Student ID " << id << " not found.\n";
    }

    void remove(int id) {
        bool removed = false;
        root = remove(root, id, removed);
        if (removed) cout << "Deleted student ID " << id << ".\n";
        else         cout << "Delete failed: ID " << id << " not found.\n";
    }

    void displayInorder() const {
        cout << "\n--- Students sorted by ID ---\n";
        if (!root) { cout << "(no records)\n"; return; }
        inorder(root);
    }

    void totalStudents() const {
        cout << "Total students: " << count(root) << endl;
    }

    void highestLowestGPA() const {
        if (!root) { cout << "Tree is empty.\n"; return; }
        float maxG = root->GPA, minG = root->GPA;
        findExtremes(root, maxG, minG);
        cout << "Highest GPA (" << maxG << "):\n";
        printWithGPA(root, maxG);
        cout << "Lowest GPA (" << minG << "):\n";
        printWithGPA(root, minG);
    }

    void studentsAbove(float threshold) const {
        cout << "Students with GPA above " << threshold << ":\n";
        if (aboveThreshold(root, threshold) == 0)
            cout << "(none)\n";
    }
};

int main() {
    StudentBST sbst;

    sbst.insert(105, "Ali", 3.4f);
    sbst.insert(102, "Sara", 3.8f);
    sbst.insert(110, "Ahmed", 2.9f);
    sbst.insert(101, "Fatima", 3.9f);
    sbst.insert(108, "Usman", 3.1f);
    sbst.insert(103, "Ayesha", 2.7f);
    sbst.insert(105, "Duplicate", 2.0f);      // rejected: ID already exists

    sbst.displayInorder();
    sbst.totalStudents();
    sbst.highestLowestGPA();

    float threshold;
    cout << "\nEnter GPA threshold: ";
    if (!(cin >> threshold)) threshold = 3.0f;   // fall back to 3.0 on bad input
    sbst.studentsAbove(threshold);

    cout << "\nSearching ID 102:\n";
    sbst.search(102);
    sbst.search(999);

    cout << "\nDeleting ID 110 (leaf), 102 (two children), 777 (missing):\n";
    sbst.remove(110);
    sbst.remove(102);
    sbst.remove(777);
    sbst.displayInorder();
    sbst.totalStudents();

    return 0;
}