#include <stdlib.h>
#include <string.h>
#include "bst.h"

BST* createBST(int (*cmp)(void*, void*), void (*print)(void*), void (*freeData)(void*)) {
    BST *bst = safeMalloc(sizeof(BST));

    bst->root = NULL;

    bst->compare = cmp;
    bst->print = print;
    bst->freeData = freeData;

    return bst;
}

BSTNode* bstInsert(BSTNode* root, void* data, int (*cmp)(void*, void*)) {
    // base case: empty tree, insert here
    if (root == NULL) {
        root = safeMalloc(sizeof(BSTNode));
        root->data = data;
        root->left = NULL;
        root->right = NULL;
        return root;
    }

    // soft failure: if reaching a node with NULL data, insert here
    if (root->data == NULL) {
        root->data = data;
        return root;
    }

    // recursive case: traverse the tree
    int result = cmp(data, root->data);
    if (result < 0) {
        return bstInsert(root->left, data, cmp);
    } else if (result > 0) {
        return bstInsert(root->right, data, cmp);
    } else {
        // data == root->data; do not insert. NULL pointer indicates caller should free data
        return NULL;
    }     
}

void* bstFind(BSTNode* root, void* data, int (*cmp)(void*, void*)) {
    // base case 1: empty tree
    if (root == NULL) {
        return NULL;
    }

    // safe failure: node with NULL data
    if (root->data == NULL) {
        return NULL;
    }

    // recursive case: traverse the tree
    int result = cmp(data, root->data);
    if (result < 0) {
        return bstFind(root->left, data, cmp);
    } else if (result > 0) {
        return bstFind(root->right, data, cmp);
    } else {
        // base case 2: data == root->data; return data
        return root;
    }     
}

void bstInorder(BSTNode* root, void (*print)(void*)) {
    if (root == NULL) {
        return;
    }

    bstInorder(root->left, print);
    print(root->data);
    bstInorder(root->right, print);
}

void bstPreorder(BSTNode* root, void (*print)(void*)) {
    if (root == NULL) {
        return;
    }

    print(root->data);
    bstPreorder(root->left, print);
    bstPreorder(root->right, print);
}

void bstPostorder(BSTNode* root, void (*print)(void*)) {
    if (root == NULL) {
        return;
    }

    bstPostorder(root->left, print);
    bstPostorder(root->right, print);
    print(root->data);
}

void bstFree(BSTNode* root, void (*freeData)(void*)) {
    if (root == NULL) {
        return;
    }

    bstFree(root->left, freeData);
    bstFree(root->right, freeData);
    freeData(root->data);
}
