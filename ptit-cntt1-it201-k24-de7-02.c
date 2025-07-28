#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Operation {
    char url[200];
    char timeStamp[50];
} Operation;

typedef struct Node {
    Operation data;
    struct Node* next;
} Node;

Node* backStack = NULL;
Node* forwardStack = NULL;

char* getCurrentTime() {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    static char buffer[50];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
    return buffer;
}

void push(Node** stack, Operation data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = *stack;
    *stack = newNode;
}

Operation pop(Node** stack) {
    Operation dummy = {"", ""};
    if (*stack == NULL) return dummy;
    Node* temp = *stack;
    Operation data = temp->data;
    *stack = (*stack)->next;
    free(temp);
    return data;
}

Operation peek(Node* stack) {
    if (stack == NULL) {
        Operation dummy = {"", ""};
        return dummy;
    }
    return stack->data;
}

int isEmpty(Node* stack) {
    return stack == NULL;
}

void visitWebsite() {
    char inputURL[200];
    printf("Nhập URL trang web: ");
    getchar();
    fgets(inputURL, sizeof(inputURL), stdin);
    inputURL[strcspn(inputURL, "\n")] = 0;

    Operation current = { "", "" };
    if (!isEmpty(backStack)) {
        current = pop(&backStack);
        push(&forwardStack, current);
    }

    Operation newVisit;
    strcpy(newVisit.url, inputURL);
    strcpy(newVisit.timeStamp, getCurrentTime());
    push(&backStack, newVisit);

    
    while (!isEmpty(forwardStack)) pop(&forwardStack);

    printf("Truy cập thành công: %s\n", newVisit.url);
}

void goBack() {
    if (backStack == NULL || backStack->next == NULL) {
        printf("Không thể quay lại. Đây là trang đầu tiên.\n");
        return;
    }
    Operation current = pop(&backStack);
    push(&forwardStack, current);
    Operation now = peek(backStack);
    printf("Quay lại: %s\n", now.url);
}

void goForward() {
    if (isEmpty(forwardStack)) {
        printf("Không có trang nào để tiến tới.\n");
        return;
    }
    Operation next = pop(&forwardStack);
    push(&backStack, next);
    printf("Tiến tới: %s\n", next.url);
}

void currentPage() {
    if (isEmpty(backStack)) {
        printf("Chưa có trang web nào được truy cập.\n");
        return;
    }
    Operation current = peek(backStack);
    printf("Trang hiện tại: %s (Truy cập lúc %s)\n", current.url, current.timeStamp);
}

void menu() {
    int choice;
    do {
        printf("\n—————— WEB HISTORY ——————\n");
        printf("1. VISIT url\n");
        printf("2. BACKWARD\n");
        printf("3. FORWARD\n");
        printf("4. CURRENT\n");
        printf("5. THOÁT\n");
        printf("Chọn chức năng: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: visitWebsite(); break;
            case 2: goBack(); break;
            case 3: goForward(); break;
            case 4: currentPage(); break;
            case 5: printf("Thoát chương trình.\n"); break;
            default: printf("Lựa chọn không hợp lệ.\n");
        }
    } while (choice != 5);
}

int main() {
    menu();
    return 0;
}
