#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define COUNT 5

// =============================================================
//                      STRUCTURES
// =============================================================

typedef struct tree{
    int size;
    int id;
    struct tree *left;
    struct tree *right;
}tree;

typedef struct truck{
    int size;
    int id;
    struct truck *next;
}truck;


tree* create_node(int id, int size){
    tree *new_node = (tree*)malloc(sizeof(tree));
    new_node->id = id;
    new_node->size = size;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

// utility function to check if the tree is balanced or not
int height(tree *root){
    if(root == NULL)
        return 0;
    int lheight = height(root->left);
    int rheight = height(root->right);
    if(lheight > rheight)
        return lheight + 1;
    else
        return rheight + 1;
}

int balance_factor(tree *root){
    if(root == NULL)
        return 0;
    int lheight = height(root->left);
    int rheight = height(root->right);
    return lheight - rheight;
}
// end of balance utility function

// avl rotation functions
void left_rotate(tree **root){
    tree *temp = (*root)->right;
    (*root)->right = temp->left;
    temp->left = *root;
    *root = temp;
}

void right_rotate(tree **root){
    tree *temp = (*root)->left;
    (*root)->left = temp->right;
    temp->right = *root;
    *root = temp;
}
// end of rotation functions

void avl_balancing(tree **root){  // balancing the tree using avl
    int balance = balance_factor(*root);
    if(balance > 1){
        if(balance_factor((*root)->left) > 0)
            right_rotate(root);
        else{
            left_rotate(&((*root)->left));
            right_rotate(root);
        }
    }
    else if(balance < -1){
        if(balance_factor((*root)->right) < 0)
            left_rotate(root);
        else{
            right_rotate(&((*root)->right));
            left_rotate(root);
        }
    }
}

// ================================================================
//                         Insert to tree
// ================================================================
void insert_tree(tree **root, int id, int size){
    if(*root == NULL){
        *root = create_node(id, size);
        return;
    }
    if((*root)->id > id){
        insert_tree(&(*root)->left, id, size);
    }
    else if((*root)->id < id){
        insert_tree(&(*root)->right, id, size);
    }
    avl_balancing(root);
}

// void delete_tree(tree **root, int id){
//     if(*root == NULL)
//         return;
//     if((*root)->id > id)
//         delete_tree(&(*root)->left, id);
//     else if((*root)->id < id)
//         delete_tree(&(*root)->right, id);
//     else{
//         if((*root)->left == NULL && (*root)->right == NULL){
//             free(*root);
//             *root = NULL;
//         }
//         else if((*root)->left == NULL){
//             tree *temp = *root;
//             *root = (*root)->right;
//             free(temp);
//         }
//         else if((*root)->right == NULL){
//             tree *temp = *root;
//             *root = (*root)->left;
//             free(temp);
//         }
//         else{
//             tree *temp = (*root)->right;
//             while(temp->left != NULL)
//                 temp = temp->left;
//             (*root)->id = temp->id;
//             delete_tree(&(*root)->right, temp->id);
//         }
//     }
//     avl_balancing(root);
// }

// search tree for item closest lower than given size and delete it
void delete_smaller(tree **root, int size){
}



// insert to truck
void insert_truck(truck **root, int size){
    
}

// function to print the tree in 2d (delete this)
void print2DUtil(tree *root, int space)
{

    if (root == NULL)
        return;
 

    space += COUNT;

    print2DUtil(root->right, space);
 

    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", root->id);
 

    print2DUtil(root->left, space);
}
 

void print2D(tree *root)
{
   print2DUtil(root, 0);
}
// end of function to print the tree

void checkSize(tree *root, tree **temp, int size){
    (*temp) = root;
    if((*temp)->size > size && (*temp)->left != NULL) checkSize((*temp)->left, &(*temp), size);
    else if((*temp)->size < size && (*temp)->right != NULL && (size - ((*temp)->right->size) >= 0)) checkSize((*temp)->right, &(*temp), size);
    return;
}

int main(){
    
    truck truck_no[COUNT];
    //=============================================================
    //                      truck test size
    //=============================================================
    truck_no[0].size = 35;
    truck_no[1].size = 200;


    tree *root = NULL;
    tree *temp = NULL;


    insert_tree(&root, 10, 10);
    insert_tree(&root, 20, 20);
    insert_tree(&root, 30, 30);
    insert_tree(&root, 40, 40);
    insert_tree(&root, 50, 50);
    insert_tree(&root, 60, 60);
    insert_tree(&root, 45, 45);

    // delete_smaller(&root, 65);
    checkSize(root, &temp, truck_no[0].size);
    printf("temp size = %d\n========\n", temp->size);
    print2D(root);



    return 0;
}