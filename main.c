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
    char destination[26];
    struct tree *left;
    struct tree *right;
}tree;

typedef struct truck{
    int size;
    int id;
    char name[26];
    char destination[26];
    struct truck *next;
}truck;


tree* create_node(int id, int size, char name[], char destination[]){
    //ID: %d\nNama Pengirim: %s\nNama Penerima: %s\nDimensi (panjangxlebarxtinggi):
    tree *new_node = (tree*)malloc(sizeof(tree));
    new_node->id = id;
    strcpy(new_node->name, name);
    strcpy(new_node->destination, destination);
    
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
void insert_tree(tree **root, int id, int size, char name[], char destination[]){
    if(*root == NULL){
        *root = create_node(id, size, name, destination);
        return;
    }
    if((*root)->id > id){
        insert_tree(&(*root)->left, id, size, name, destination);
    }
    else if((*root)->id < id){
        insert_tree(&(*root)->right, id, size, name, destination);
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
        newPackage->size = temp->size;
        strcpy(newPackage->name, temp->name);
        strcpy(newPackage->destination, temp->destination);
        
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

// partition function for quick sort based on id
int partition(truck **truck_no, int low, int high){
    int pivot = truck_no[high]->id;
    int i = (low - 1);
    for(int j = low; j <= high - 1; j++){
        if(truck_no[j]->id <= pivot){
            i++;
            truck *temp = truck_no[i];
            truck_no[i] = truck_no[j];
            truck_no[j] = temp;
        }
    }
    truck *temp = truck_no[i + 1];
    truck_no[i + 1] = truck_no[high];
    truck_no[high] = temp;
    return (i + 1);
}
// quick sort function based on id
void quickSort(truck **node, int low, int high){
    if(low < high){
        int pi = partition(node, low, high);
        quickSort(node, low, pi-1);
        quickSort(node, pi+1, high);
    }
}

// file updating function
void updateFile(int id){
    int id, lebar, panjang, tinggi;
    char namaPengirim[26], namaPenerima[26];

    FILE *awal = fopen("data.txt", "r");
    FILE *akhir = fopen("dataNew.txt", "w");
    while(!feof(awal)){
        
    }
}

//INTERFACE FUNCTIONS
void mainMenu(char *input){
    system("cls");
    printf("====================\n    Project UAS\n====================\n");
    printf("1. Admin\n2. Kirim Barang\n3. Histori Data Pengiriman\n0. Exit\nPilihan: ");
    scanf(" %c", input);
}

int loginAuth(){
    static const char username[]="admin", password[]="admin";
    char id[8], p[6];
    int n=1, x, y;
    do{
        system("cls");
        printf("====================\n       Login\n====================\n");
        printf("Username: ");
        scanf("%s", &id);
        fflush(stdout);

        printf("Password: ");
        scanf("%s", &p);
        fflush(stdout);

        x=strcmp(id, username);
        y=strcmp(p, password);

        if(x==0 && y==0){
            printf("\nBerhasil Masuk");
            
            return 1;
        }else {
            printf("\nPassword salah, silahkan dicoba kembali", 5-n);
            getchar();
            n++;}

        if(n>5){
            printf("\nAccess Denied");
            getchar();
        }

    }while (n<=5);
    return 0;
}

void module1(tree **root, int *jumlah){
    char inputModule1;
    int id, lebar, panjang, tinggi;
    char namaPengirim[26], namaPenerima[26], tipe[10];
    while(1){
        system("cls");
        printf("=================\n    Data Base\n=================\n");
        printf("1. Input Data Pengiriman\n2. List Pengiriman\n0. Kembali ke Menu Utama\nPilihan: ");
        scanf(" %c", &inputModule1);
        switch(inputModule1){
            case '1':
                system("cls");
                printf("====================\n    Input Data Pengiriman\n====================\n");
                printf("Masukkan Nama Pengirim: "); scanf(" %25[^\n]", namaPengirim);
                printf("Masukkan Nama Penerima: "); scanf(" %25[^\n]", namaPenerima);
                printf("Masukkan Dimensi Barang [panjang lebar tinggi](cm): "); scanf(" %d %d %d", &panjang, &lebar, &tinggi);
                (*jumlah)++;
                insert_tree(&(*root), (*jumlah), (panjang*lebar*tinggi), namaPengirim, namaPenerima);
                FILE *data = fopen("data.txt", "a");
                fprintf(data, "ID: %d\nNama Pengirim: %s\nNama Penerima: %s\nDimensi (panjangxlebarxtinggi): %dx%dx%d\n", (*jumlah), namaPengirim, namaPenerima, panjang, lebar, tinggi);
                fclose(data);
                break;
            case '2':
                system("cls");
                printf("=======================\n    List Pengiriman\n=======================\n");
                FILE *list = fopen("data.txt", "r");
                while(!feof(list)){
                    fscanf(list, "ID: %d\nNama Pengirim: %s\nNama Penerima: %s\nDimensi (panjangxlebarxtinggi): %dx%dx%d\n", &id, namaPengirim, namaPenerima, &panjang, &lebar, &tinggi);
                    printf("ID: %d\nNama Pengirim: %s\nNama Penerima: %s\nDimensi (panjangxlebarxtinggi): %dx%dx%d\n", id, namaPengirim, namaPenerima, panjang, lebar, tinggi);
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
    int jumlah = 0;
    int id, panjang, lebar, tinggi, size;
    char namaPengirim[26], namaPenerima[26];

    for (int i = 0; i < COUNT; i++){
        truck_no[i] = (truck *) malloc(sizeof(truck));
        truck_no[i]->id = -1;
        truck_no[i]->size = 200;
        truck_no[i]->next = NULL;
    }
    
    FILE *start = fopen("data.txt", "r");
    while(!feof(start)){
        fscanf(start, "ID: %d\nNama Pengirim: %s\nNama Penerima: %s\nDimensi (panjangxlebarxtinggi): %dx%dx%d\n", &id, namaPengirim, namaPenerima, &panjang, &lebar, &tinggi);
        jumlah++;
        insert_tree(&root, id, (panjang*lebar*tinggi), namaPengirim, namaPenerima);
    }
    fclose(start);

    insert_tree(&root, 10, 10, "test1", "destination 1");
    insert_tree(&root, 20, 20, "test2", "destination 2");
    insert_tree(&root, 30, 30, "test3", "destination 3");
    insert_tree(&root, 40, 40, "test4", "destination 4");
    insert_tree(&root, 50, 50, "test5", "destination 5");
    insert_tree(&root, 60, 60, "test6", "destination 6");
    insert_tree(&root, 45, 45, "test7", "destination 7");

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
                if(loginAuth() == 1) module1(&root, &jumlah);
                break;
            case '2':
                printf("Kirim Barang\n");
                int status = 1;
                while(status != 0 && root != NULL) {
                    status = insert_truck(truck_no, &root);
                    printf("status: %d\n", status);
                }
                //print truck contents + write to histori.txt
                FILE *histori = fopen("histori.txt", "a");
                for(int i = 0; i < COUNT; i++){
                    truck *ptr = truck_no[i];
                    printf("Truck %d: ", i);
                    while(ptr != NULL){
                        fprintf(histori, "ID: %d\nNama Pengirim: %s\nNama Penerima: %s\nVolume: %d\n", ptr->id, ptr->name, ptr->destination, ptr->size);
                        printf("[%d] -> ", ptr->id);
                        ptr = ptr->next;
                    }
                    printf("NULL\n");
                }
                getch();
                break;
            case '3':
                printf("Histori Data Pengiriman\n");
                FILE *histori = fopen("histori.txt", "r");
                while(!feof(histori)){
                    fscanf(histori, "ID: %d\nNama Pengirim: %s\nNama Penerima: %s\nVolume: %d\n", &id, namaPengirim, namaPenerima, &size);
                    printf("ID: %d\nNama Pengirim: %s\nNama Penerima: %s\nVolume: %d\n=================\n", id, namaPengirim, namaPenerima, size);
                }
                fclose(histori);
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