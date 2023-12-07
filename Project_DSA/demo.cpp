#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
    int key;
    int depth;
    Node* parent;
    Node* left;
    Node* right;
};

vector<Node> eulerTour;
vector<int> firstOccurrence;
vector<int> depth;
vector<vector<int>> adjacencyMatrix;

Node* buildEulerTourTree(int start, int end) {
    if (start > end) return nullptr;

    int minDepth = eulerTour[start].depth;
    int minIndex = start;

    for (int i = start + 1; i <= end; ++i) {
        if (eulerTour[i].depth < minDepth) {
            minDepth = eulerTour[i].depth;
            minIndex = i;
        }
    }

    Node* root = new Node{ eulerTour[minIndex].key, minDepth, nullptr, nullptr, nullptr };

    root->left = buildEulerTourTree(start, minIndex - 1);
    root->right = buildEulerTourTree(minIndex + 1, end);

    if (root->left) root->left->parent = root;
    if (root->right) root->right->parent = root;

    return root;
}

Node* findRoot(Node* node) {
    while (node->parent != nullptr) {
        node = node->parent;
    }
    return node;
}

void splay(Node* x) {
    while (x->parent != nullptr) {
        Node* p = x->parent;
        Node* g = p->parent;

        if (g == nullptr) {
            if (x == p->left) {
                // Zig rotation
                Node* b = x->right;
                x->right = p;
                p->left = b;
                if (b) b->parent = p;
                p->parent = x;
                x->parent = nullptr;
            }
            else {
                // Zag rotation
                Node* b = x->left;
                x->left = p;
                p->right = b;
                if (b) b->parent = p;
                p->parent = x;
                x->parent = nullptr;
            }
        }
        else {
            if (x == p->left && p == g->left) {
                // Zig-zig rotation
                Node* b = x->right;
                Node* c = p->right;
                x->right = p;
                p->left = b;
                p->right = g;
                g->left = c;
                if (b) b->parent = p;
                if (c) c->parent = g;
                p->parent = x;
                g->parent = p->parent;
                x->parent = nullptr;
            }
            else if (x == p->right && p == g->right) {
                // Zag-zag rotation
                Node* b = x->left;
                Node* c = p->left;
                x->left = p;
                p->right = b;
                p->left = g;
                g->right = c;
                if (b) b->parent = p;
                if (c) c->parent = g;
                p->parent = x;
                g->parent = p->parent;
                x->parent = nullptr;
            }
            else {
                // Zig-zag or Zag-zig rotation
                Node* b, * c;
                if (x == p->left && p == g->right) {
                    // Zig-zag
                    b = x->right;
                    c = x->left;
                }
                else {
                    // Zag-zig
                    b = x->left;
                    c = x->right;
                }
                x->left = p;
                x->right = g;
                p->right = c;
                g->left = b;
                if (c) c->parent = p;
                if (b) b->parent = g;
                p->parent = x;
                g->parent = x;
                x->parent = nullptr;
            }
        }
    }
}

void reroot(Node* x) {
    splay(x);
    x->left = nullptr;
}
/* 0 -1 0 */
void eulerTourDFS(int current, int parent, int currentDepth) {
    eulerTour.push_back({ current, currentDepth, nullptr, nullptr, nullptr });
    depth.push_back(currentDepth);
    firstOccurrence[current] = eulerTour.size() - 1;

    for (int child = 0; child < adjacencyMatrix[current].size(); ++child) {
        if (adjacencyMatrix[current][child] && child != parent) {
            eulerTourDFS(child, current, currentDepth + 1);
            eulerTour.push_back({ current, currentDepth, nullptr, nullptr, nullptr });
            depth.push_back(currentDepth);
        }
    }
}

Node* findNode(int key) {
    int index = firstOccurrence[key];
    return &eulerTour[index];
}

void unionNodes(int u, int v) {
    Node* nodeU = findNode(u);
    Node* nodeV = findNode(v);

    int firstOccurU = firstOccurrence[u];
    int firstOccurV = firstOccurrence[v];
    if (firstOccurU > firstOccurV) swap(firstOccurU, firstOccurV);

    int index = min_element(depth.begin() + firstOccurU, depth.begin() + firstOccurV + 1) - depth.begin();
    Node* lca = &eulerTour[index];

    splay(nodeU);
    reroot(nodeV);

    // Implement your Union logic here
    // For example, you can connect the corresponding subtrees in the Euler Tour Tree.
    cout << "Union " << u << " and " << v << " at LCA " << lca->key << endl;
}

bool isConnected(int u, int v) {
    Node* nodeU = findNode(u);
    Node* nodeV = findNode(v);

    // Implement your Find logic here
    // For example, you can check if the two nodes belong to the same subtree in the Euler Tour Tree.
    return (findRoot(nodeU) == findRoot(nodeV));
}

void cutLink(int u, int v) {
    Node* nodeU = findNode(u);
    Node* nodeV = findNode(v);

    // Implement your Cut Link logic here
    // For example, you can remove the corresponding edges in the adjacency matrix.
    adjacencyMatrix[u][v] = adjacencyMatrix[v][u] = 0;

    splay(nodeU);
    reroot(nodeV);

    cout << "Cutting the link between " << u << " and " << v << endl;
}

void link(int u, int v) {
    Node* nodeU = findNode(u);
    Node* nodeV = findNode(v);

    // Implement your Link logic here
    // For example, you can add the corresponding edges in the adjacency matrix.
    adjacencyMatrix[u][v] = adjacencyMatrix[v][u] = 1;

    splay(nodeU);
    reroot(nodeV);

    cout << "Linking " << u << " and " << v << endl;
}

void printInOrderTraversal(Node* root) {
    if (root) {
        printInOrderTraversal(root->left);
        cout << root->key << " ";
        printInOrderTraversal(root->right);
    }
}

void printEulerTourTree() {
    Node* root = buildEulerTourTree(0, eulerTour.size() - 1);
    cout << "In-order traversal of Euler Tour Tree: ";
    printInOrderTraversal(root);
    cout << endl;
}

int main() {
    // Example input: Adjacency matrix for a tree
    adjacencyMatrix = {
        {0, 1, 1, 0, 0, 0},
        {1, 0, 0, 1, 1, 0},
        {1, 0, 0, 0, 0, 1},
        {0, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0}
    };
    /* resize */
    firstOccurrence.resize(adjacencyMatrix.size());

    eulerTourDFS(0, -1, 0);

    printEulerTourTree();

    cout << "Is connected (2, 3): " << isConnected(2, 3) << endl; // Expected output: 1 (true)
    cout << "Is connected (1, 5): " << isConnected(1, 5) << endl; // Expected output: 0 (false)

    unionNodes(2, 5);

    cout << "Is connected (1, 5): " << isConnected(1, 5) << endl; // Expected output: 1 (true)

    cutLink(2, 4);

    printEulerTourTree();

    link(2, 4);

    printEulerTourTree();

    return 0;
}

    /*Dưới đây là mô tả chức năng của mỗi hàm trong mã nguồn C++ đã được mở rộng :

1. * *`buildEulerTourTree` Function : **
-**Chức năng : **Xây dựng cây nhị phân từ dãy Euler Tour.
- **Input : **Tham số `start` và `end` là chỉ số của các đỉnh trong dãy Euler Tour cần xử lý.
- **Output:**Trả về con trỏ tới nút gốc của cây nhị phân được xây dựng từ dãy Euler Tour.

2. * *`findRoot` Function : **
-**Chức năng : **Tìm nút gốc của cây nhị phân chứa nút đang xét.
- **Input : **Nút cần tìm nút gốc.
- **Output : **Trả về con trỏ tới nút gốc.

3. * *`splay` Function : **
-**Chức năng : **Thực hiện phép splay cho nút đang xét.
- **Input : **Nút cần thực hiện phép splay.
- **Output : **Cây nhị phân sau khi thực hiện phép splay.

4. * *`reroot` Function : **
-**Chức năng : **Thực hiện phép reroot cho nút đang xét.
- **Input : **Nút cần thực hiện phép reroot.
- **Output : **Cây nhị phân sau khi thực hiện phép reroot.

5. * *`eulerTourDFS` Function : **
-**Chức năng : **Thực hiện duyệt đồ thị bằng thuật toán Euler Tour để xây dựng Euler Tour Tree.
- **Input : **Tham số `current` là đỉnh hiện tại, `parent` là đỉnh cha của `current`, và `currentDepth` là độ sâu hiện tại.
- **Output:**Euler Tour Tree được lưu trong các biến toàn cục `eulerTour`, `depth`, và `firstOccurrence`.

6. * *`buildEulerTourTreeWrapper` Function:**
-**Chức năng : **Gọi hàm `eulerTourDFS` để xây dựng Euler Tour Tree từ đồ thị.

7. * *`findNode` Function:**
-**Chức năng : **Tìm nút tương ứng với một đỉnh trong Euler Tour Tree.
- **Input : **Khóa(key) của đỉnh cần tìm.
- **Output : **Trả về con trỏ tới nút chứa đỉnh cần tìm.

8. * *`unionNodes` Function : **
-**Chức năng : **Kết hợp hai cây con tương ứng với hai đỉnh trong Euler Tour Tree.
- **Input : **Hai đỉnh cần kết hợp.
- **Output : **Hiển thị thông điệp thể hiện phép kết hợp(có thể làm thay đổi cây nhị phân Euler Tour).

9. * *`isConnected` Function : **
-**Chức năng : **Kiểm tra xem hai đỉnh có thuộc cùng một cây con trong Euler Tour Tree hay không.
- **Input : **Hai đỉnh cần kiểm tra.
- **Output : **Trả về `true` nếu hai đỉnh thuộc cùng một cây con, ngược lại trả về `false`.

10. * *`cutLink` Function:**
-**Chức năng : **Cắt cạnh giữa hai đỉnh trong Euler Tour Tree.
- **Input : **Hai đỉnh cần cắt cạnh.
- **Output : **Hiển thị thông điệp thể hiện phép cắt cạnh(có thể làm thay đổi cây nhị phân Euler Tour).

11. * *`link` Function : **
-**Chức năng : **Thêm cạnh giữa hai đỉnh trong Euler Tour Tree.
- **Input : **Hai đỉnh cần thêm cạnh.
- **Output : **Hiển thị thông điệp thể hiện phép thêm cạnh(có thể làm thay đổi cây nhị phân Euler Tour).

12. * *`printInOrderTraversal` Function : **
-**Chức năng : **In ra các đỉnh của cây nhị phân theo thứ tự trung tâm.
- **Input : **Nút gốc của cây nhị phân.
- **Output : **Hiển thị các đỉnh theo thứ tự trung tâm.

13. * *`printEulerTourTree` Function : **
-**Chức năng : **In ra cây nhị phân Euler Tour theo thứ tự trung tâm.
- **Input : **Không có.
- **Output : **Hiển thị các đỉnh theo thứ tự tr

ung tâm của cây nhị phân Euler Tour.

14. * *`main` Function : **
-**Chức năng : **Chứa ví dụ về cách sử dụng các chức năng trên một đồ thị và hiển thị kết quả.*/
