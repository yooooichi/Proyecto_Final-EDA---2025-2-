#include <iostream>
using namespace std;

const int T = 3; // Grado mínimo del árbol B

class BTree {

private:
    class BTreeNode { //ahora BtreeNode es una clase anidada dentro de BTree
    public:
        int *keys;
        BTreeNode **C;
        int n;
        bool leaf;

        BTreeNode(bool _leaf);
        ~BTreeNode();

        //metodos
        void traverse();
        void insertNonFull(int k);
        void splitChild(int i, BTreeNode *y);
        void remove(int k);
        int findKey(int k);
        void removeFromLeaf(int idx);
        void removeFromNonLeaf(int idx);
        int getPred(int idx);
        int getSucc(int idx);
        void fill(int idx);
        void borrowFromPrev(int idx);
        void borrowFromNext(int idx);
        void merge(int idx);

        //friend class BTree;
    };
    BTreeNode *root;
    int t;

public:
    BTree() {
        root = nullptr;
        t = T;
    }

    // destructor de BTree
    ~BTree() {
        if(root != nullptr)
            delete root; // inicia la destrucción recursiva
    }

    void traverse() {
        if (root != nullptr) root->traverse();
    }

    void insert(int k);
    void remove(int k);
};

// ------------------ Métodos de BTreeNode ------------------
// se agrega el prefijo 'BTree::' para indicar que BTreeNode es una clase anidada dentro de BTree
BTree::BTreeNode::BTreeNode(bool _leaf) {
    leaf = _leaf;
    keys = new int[2 * T - 1];
    C = new BTreeNode *[2 * T];
    n = 0;
}

// destructor de BTreeNode
BTree::BTreeNode::~BTreeNode() {
    // se libera la memoria de los nodos hijos recursivamente
    if (!leaf) {
        for (int i = 0; i <= n; i++) {
            // C[i] es un puntero a otro BTreeNode
            // al hacer 'delete', se llamará a su propio destructor.
            delete C[i];
        }
    }
    // liberar la memoria de los arrays de este nodo
    delete[] keys;
    delete[] C;
}

void BTree::BTreeNode::traverse() {
    for (int i = 0; i < n; i++) {
        if (!leaf)
            C[i]->traverse();
        cout << " " << keys[i];
    }
    if (!leaf)
        C[n]->traverse();
}

void BTree::insert(int k) {
    if (root == nullptr) {
        root = new BTreeNode(true);
        root->keys[0] = k;
        root->n = 1;
    } else {
        if (root->n == 2 * t - 1) {
            BTreeNode *s = new BTreeNode(false);
            s->C[0] = root;
            s->splitChild(0, root);

            int i = 0;
            if (s->keys[0] < k)
                i++;
            s->C[i]->insertNonFull(k);

            root = s;
        } else
            root->insertNonFull(k);
    }
}

void BTree::BTreeNode::insertNonFull(int k) {
    int i = n - 1;
    if (leaf) {
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = k;
        n++;
    } else {
        while (i >= 0 && keys[i] > k)
            i--;
        if (C[i + 1]->n == 2 * T - 1) {
            splitChild(i + 1, C[i + 1]);
            if (keys[i + 1] < k)
                i++;
        }
        C[i + 1]->insertNonFull(k);
    }
}

void BTree::BTreeNode::splitChild(int i, BTreeNode *y) {
    BTreeNode *z = new BTreeNode(y->leaf);
    z->n = T - 1;

    for (int j = 0; j < T - 1; j++)
        z->keys[j] = y->keys[j + T];

    if (!y->leaf) {
        for (int j = 0; j < T; j++)
            z->C[j] = y->C[j + T];
    }

    y->n = T - 1;

    for (int j = n; j >= i + 1; j--)
        C[j + 1] = C[j];

    C[i + 1] = z;

    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    keys[i] = y->keys[T - 1];
    n++;
}

// ------------------ Eliminación ------------------

void BTree::remove(int k) {
    if (!root) {
        cout << "Árbol vacío\n";
        return;
    }

    root->remove(k);

    if (root->n == 0) {
        BTreeNode *tmp = root;
        if (root->leaf)
            root = nullptr;
        else
            root = root->C[0];

        // se 'desconecta' al hijo del padre antes de borrar 'tmp'.
        // ya que sino, el destructor de 'tmp' borrará C[0], el cual es el nuevo root.
        if (!tmp->leaf)
            tmp->C[0] = nullptr;

        delete tmp; //antes no habia problemas ya que no existía un destructor recursivo
    }
}

void BTree::BTreeNode::remove(int k) {
    int idx = findKey(k);

    if (idx < n && keys[idx] == k) {
        if (leaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    } else {
        if (leaf) {
            cout << "La clave " << k << " no existe\n";
            return;
        }

        bool flag = ((idx == n) ? true : false);

        if (C[idx]->n < T)
            fill(idx);

        if (flag && idx > n)
            C[idx - 1]->remove(k);
        else
            C[idx]->remove(k);
    }
}

int BTree::BTreeNode::findKey(int k) {
    int idx = 0;
    while (idx < n && keys[idx] < k)
        ++idx;
    return idx;
}

void BTree::BTreeNode::removeFromLeaf(int idx) {
    for (int i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];
    n--;
}

void BTree::BTreeNode::removeFromNonLeaf(int idx) {
    int k = keys[idx];

    if (C[idx]->n >= T) {
        int pred = getPred(idx);
        keys[idx] = pred;
        C[idx]->remove(pred);
    } else if (C[idx + 1]->n >= T) {
        int succ = getSucc(idx);
        keys[idx] = succ;
        C[idx + 1]->remove(succ);
    } else {
        merge(idx);
        C[idx]->remove(k);
    }
}

int BTree::BTreeNode::getPred(int idx) {
    BTreeNode *cur = C[idx];
    while (!cur->leaf)
        cur = cur->C[cur->n];
    return cur->keys[cur->n - 1];
}

int BTree::BTreeNode::getSucc(int idx) {
    BTreeNode *cur = C[idx + 1];
    while (!cur->leaf)
        cur = cur->C[0];
    return cur->keys[0];
}

void BTree::BTreeNode::fill(int idx) {
    if (idx != 0 && C[idx - 1]->n >= T)
        borrowFromPrev(idx);
    else if (idx != n && C[idx + 1]->n >= T)
        borrowFromNext(idx);
    else {
        if (idx != n)
            merge(idx);
        else
            merge(idx - 1);
    }
}

void BTree::BTreeNode::borrowFromPrev(int idx) {
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx - 1];

    for (int i = child->n - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];

    if (!child->leaf) {
        for (int i = child->n; i >= 0; --i)
            child->C[i + 1] = child->C[i];
    }

    child->keys[0] = keys[idx - 1];

    if (!child->leaf)
        child->C[0] = sibling->C[sibling->n];

    keys[idx - 1] = sibling->keys[sibling->n - 1];

    child->n += 1;
    sibling->n -= 1;
}

void BTree::BTreeNode::borrowFromNext(int idx) {
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx + 1];

    child->keys[child->n] = keys[idx];

    if (!(child->leaf))
        child->C[child->n + 1] = sibling->C[0];

    keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->n; ++i)
        sibling->keys[i - 1] = sibling->keys[i];

    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; ++i)
            sibling->C[i - 1] = sibling->C[i];
    }

    child->n += 1;
    sibling->n -= 1;
}

void BTree::BTreeNode::merge(int idx) {
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx + 1];

    child->keys[T - 1] = keys[idx];

    for (int i = 0; i < sibling->n; ++i)
        child->keys[i + T] = sibling->keys[i];

    if (!child->leaf) {
        for (int i = 0; i <= sibling->n; ++i){
            child->C[i + T] = sibling->C[i]; //'child' adopta al hijo C[i] de 'sibling'

            // se 'desconecta' al hijo de 'sibling'
            // para que el destructor de 'sibling' no lo borre
            sibling->C[i] = nullptr; 
        }
    }

    for (int i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];

    for (int i = idx + 2; i <= n; ++i)
        C[i - 1] = C[i];

    child->n += sibling->n + 1;
    n--;

    delete sibling;
}

int main(){
    BTree w;

    w.insert(30);
    w.insert(60);
    w.insert(10);
    w.insert(20);
    w.insert(40);
    w.insert(50);
    w.insert(70);
    w.insert(80);
    w.insert(1);
    w.insert(5);
    w.insert(11);
    w.insert(15);
    w.insert(21);
    w.insert(25);
    w.insert(31);
    w.insert(35);
    w.insert(41);
    w.insert(45);
    w.insert(51);
    w.insert(55);
    w.insert(61);
    w.insert(65);
    w.insert(71);
    w.insert(75);
    w.insert(81);
    w.insert(85);
    w.remove(10);
    
    w.traverse();
}

