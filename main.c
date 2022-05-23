#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#define COUNT 5

// =============================================================
//                      STRUCTURES
// =============================================================

typedef struct tree{
    int size;
    int id;
    char name[26];
    char category[26];
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

void delete_tree(tree **root, int id){
    if(*root == NULL)
        return;
    if((*root)->id > id)
        delete_tree(&(*root)->left, id);
    else if((*root)->id < id)
        delete_tree(&(*root)->right, id);
    else{
        if((*root)->left == NULL && (*root)->right == NULL){
            free(*root);
            *root = NULL;
        }
        else if((*root)->left == NULL){
            tree *temp = *root;
            *root = (*root)->right;
            free(temp);
        }
        else if((*root)->right == NULL){
            tree *temp = *root;
            *root = (*root)->left;
            free(temp);
        }
        else{
            tree *temp = (*root)->right;
            while(temp->left != NULL)
                temp = temp->left;
            (*root)->id = temp->id;
            delete_tree(&(*root)->right, temp->id);
        }
    }
    avl_balancing(root);
}

void checkSize(tree *root, tree **temp, int size){
    (*temp) = root;
    if((*temp)->size > size && (*temp)->left != NULL) checkSize((*temp)->left, &(*temp), size);
    else if((*temp)->size < size && (*temp)->right != NULL && (size - ((*temp)->right->size) >= 0)) checkSize((*temp)->right, &(*temp), size);
    return;
}

// insert to truck
int insert_truck(truck **truck_no, tree **root){
    tree *temp = NULL;
    for(int i = 0; i < 5; i++){
        checkSize((*root), &temp, truck_no[i]->size);
        truck *newPackage = (truck *) malloc(sizeof(truck));
        newPackage->id = temp->id;
        newPackage->next = NULL;
        if(truck_no[i]->size >= temp->size){
            truck *ptr = truck_no[i];
            newPackage->size = truck_no[i]->size;
            if(ptr->id == -1) truck_no[i] = newPackage;
            else{
                while(ptr->next != NULL){
                    ptr = ptr->next;
                }
                ptr->next = newPackage;
            }
            truck_no[i]->size -= temp->size;
            delete_tree(&(*root), temp->id);
            return 1;
        }
    }
    return 0;
}

//INTERFACE FUNCTIONS
void mainMenu(char *input){
    system("cls");
    printf("====================\n    Project UAS\n====================\n");
    printf("1. Admin\n2. Kirim Barang\n3. Histori Data Pengiriman\n0. Exit\nPilihan: ");
    scanf(" %c", input);
}

void module1(tree **root){
    char inputModule1;
    int lebar, panjang, tinggi;
    char namaPengirim[26], namaPenerima[26], tipe[10];
    while(1){
        system("cls");
        printf("===============\n     Admin\n===============\n");
        printf("1. Input Data Pengiriman\n2. List Pengiriman\n0. Kembali ke Menu Utama\nPilihan: ");
        scanf(" %c", &inputModule1);
        switch(inputModule1){
            case '1':
                system("cls");
                printf("====================\n    Input Data Pengiriman\n====================\n");
                printf("Masukkan Nama Pengirim: "); scanf(" %25[^\n]", namaPengirim);
                printf("Masukkan Nama Penerima: "); scanf(" %25[^\n]", namaPenerima);
                printf("Masukkan Dimensi Barang [panjang lebar tinggi](cm): "); scanf(" %d %d %d", &panjang, &lebar, &tinggi);
                FILE *data = fopen("data.txt", "a");
                fprintf(data, "Nama Pengirim: %s\nNama Penerima: %s\nDimensi (panjangxlebarxtinggi): %dx%dx%d\n", namaPengirim, namaPenerima, panjang, lebar, tinggi);
                fclose(data);
                break;
            case '2':
                system("cls");
                printf("=======================\n    List Pengiriman\n=======================\n");
                FILE *list = fopen("data.txt", "r");
                while(!feof(list)){
                    fscanf(list, "Nama Pengirim: %s\nNama Penerima: %s\nDimensi (panjangxlebarxtinggi): %dx%dx%d\n", namaPengirim, namaPenerima, &panjang, &lebar, &tinggi);
                    printf("Nama Pengirim: %s\nNama Penerima: %s\nDimensi (panjangxlebarxtinggi): %dx%dx%d\n", namaPengirim, namaPenerima, panjang, lebar, tinggi);
                    printf("=======================\n");
                }
                fclose(list);
                getch();
                break;
            case '0':
                return;
            default:
                printf("Invalid Input\nPress any key to continue...");
                getch();
                break;
        }
    }
}
//END OF INTERFACE FUNCTIONS

int main(){
    truck *truck_no[COUNT];
    tree *root = NULL;
    for (int i = 0; i < COUNT; i++){
        truck_no[i] = (truck *) malloc(sizeof(truck));
        truck_no[i]->id = -1;
        truck_no[i]->size = 200;
        truck_no[i]->next = NULL;
    }
    
    FILE *start = fopen("data.txt", "r");
    insert_tree(&root, 10, 10);
    insert_tree(&root, 20, 20);
    insert_tree(&root, 30, 30);
    insert_tree(&root, 40, 40);
    insert_tree(&root, 50, 50);
    insert_tree(&root, 60, 60);
    insert_tree(&root, 45, 45);

    system("COLOR 9");
    printf("                 /_/    /_/ /_/_/_/ /_/       /_/         /_/_/       \n");
    printf("                /_/    /_/ /_/     /_/       /_/       /_/    _/     \n");
    printf("               /_/_/_ /_/ /_/_/_/ /_/       /_/       /_/    _/     \n");
    printf("              /_/    /_/ /_/     /_/       /_/       /_/    _/     \n");
    printf("             /_/    /_/ /_/_/_/ /_/_/_/_/ /_/_/_/_/   /_/_/       \n\n");

    printf("/_/           /_/  /_/_/_/  /_/        /_/_/_/    /_/_/    /_/_/     /_//_/ /_/_/_/\n");
    printf("/_/          /_/  /_/      /_/       /_/       /_/    /_/ /_/  _/ _/   /_/ /_/    \n");
    printf("/_/   _/    /_/  /_/_/_/  /_/       /_/       /_/    /_/ /_/   _/     /_/ /_/_/_/\n");
    printf(" /_/ _/ _/ /_/  /_/      /_/       /_/       /_/    /_/ /_/          /_/ /_/    \n");
    printf("  /_/    /_/   /_/_/_/  /_/_/_/_/   /_/_/_/   /_/_/    /_/          /_/ /_/_/_/\n");
    getch();

    char input;
    while(1){
        mainMenu(&input);
        switch(input){
            case '1':
                module1(&root);
                break;
            case '2':
                printf("Kirim Barang\n");
                int status = 1;
                while(status != 0 && root != NULL) {
                    status = insert_truck(truck_no, &root);
                    printf("status: %d\n", status);
                }
                //print truck contents
                for(int i = 0; i < COUNT; i++){
                    truck *ptr = truck_no[i];
                    printf("Truck %d: ", i);
                    while(ptr != NULL){
                        printf("[%d] -> ", ptr->id);
                        ptr = ptr->next;
                    }
                    printf("NULL\n");
                }
                getch();
                break;
            case '3':
                printf("Histori Data Pengiriman\n");
                getch();
                break;
            case '0':
                printf("Exit\n");
                return 0;
            default:
                printf("Invalid Input\nPress any key to continue...");
                getch();
                break;
        }
    }
}