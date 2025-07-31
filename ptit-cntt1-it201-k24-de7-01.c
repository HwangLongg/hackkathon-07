#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Song {
    char title[100];
    char artist[50];
    float duration;
    struct Song* left;
    struct Song* right;
} Song;

Song* createSong(char* title, char* artist, float duration) {
    Song* newNode = (Song*)malloc(sizeof(Song));
    strcpy(newNode->title, title);
    strcpy(newNode->artist, artist);
    newNode->duration = duration;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Song* insertSong(Song* root, char* title, char* artist, float duration) {
    if (root == NULL) return createSong(title, artist, duration);
    int cmp = strcmp(title, root->title);
    if (cmp == 0) {
        printf("Bai hat da ton tai. Khong them.\n");
        return root;
    }
    if (cmp < 0) root->left = insertSong(root->left, title, artist, duration);
    else root->right = insertSong(root->right, title, artist, duration);
    return root;
}

void inOrderDisplay(Song* root) {
    if (root == NULL) return;
    inOrderDisplay(root->left);
    printf("Ten bai hat: %s\n", root->title);
    printf("Ca si: %s\n", root->artist);
    printf("Thoi luong: %.2f phut\n", root->duration);
    printf("------------------------\n");
    inOrderDisplay(root->right);
}

Song* searchSong(Song* root, char* title) {
    if (root == NULL) return NULL;
    int cmp = strcmp(title, root->title);
    if (cmp == 0) return root;
    if (cmp < 0) return searchSong(root->left, title);
    else return searchSong(root->right, title);
}

Song* findMin(Song* root) {
    while (root && root->left != NULL)
        root = root->left;
    return root;
}

Song* deleteSong(Song* root, char* title) {
    if (root == NULL) return NULL;
    int cmp = strcmp(title, root->title);
    if (cmp < 0) root->left = deleteSong(root->left, title);
    else if (cmp > 0) root->right = deleteSong(root->right, title);
    else {
        if (root->left == NULL) {
            Song* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Song* temp = root->left;
            free(root);
            return temp;
        } else {
            Song* temp = findMin(root->right);
            strcpy(root->title, temp->title);
            strcpy(root->artist, temp->artist);
            root->duration = temp->duration;
            root->right = deleteSong(root->right, temp->title);
        }
    }
    return root;
}

float sumDuration(Song* root) {
    if (root == NULL) return 0;
    return root->duration + sumDuration(root->left) + sumDuration(root->right);
}

void freeTree(Song* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    Song* root = NULL;
    int choice;
    char title[100], artist[50];
    float duration;

    do {
        printf("\n---------------- FAVORITE SONG MANAGER ----------------\n");
        printf("1. THEM BAI HAT MOI\n");
        printf("2. HIEN THI DANH SACH TANG DAN THEO TEN\n");
        printf("3. TIM BAI HAT THEO TEN\n");
        printf("4. XOA BAI HAT\n");
        printf("5. TINH TONG THOI LUONG TAT CA BAI HAT\n");
        printf("6. THOAT\n");
        printf("Chon chuc nang: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Nhap ten bai hat: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0';
                printf("Nhap ten ca si: ");
                fgets(artist, sizeof(artist), stdin);
                artist[strcspn(artist, "\n")] = '\0';
                printf("Nhap thoi luong (phut): ");
                scanf("%f", &duration);
                root = insertSong(root, title, artist, duration);
                break;
            case 2:
                printf("Danh sach bai hat:\n");
                inOrderDisplay(root);
                break;
            case 3:
                printf("Nhap ten bai hat can tim: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0';
                Song* found = searchSong(root, title);
                if (found) {
                    printf("Tim thay:\n");
                    printf("Ten: %s\n", found->title);
                    printf("Ca si: %s\n", found->artist);
                    printf("Thoi luong: %.2f phut\n", found->duration);
                } else {
                    printf("Khong tim thay bai hat.\n");
                }
                break;
            case 4:
                printf("Nhap ten bai hat can xoa: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0';
                root = deleteSong(root, title);
                printf("Da xoa bai hat (neu ton tai).\n");
                break;
            case 5:
                printf("Tong thoi luong tat ca bai hat: %.2f phut\n", sumDuration(root));
                break;
            case 6:
                freeTree(root);
                printf("Ket thuc chuong trinh.\n");
                break;
            default:
                printf("Lua chon khong hop le.\n");
        }

    } while (choice != 6);

    return 0;
}
