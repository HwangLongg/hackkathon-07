#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Order {
    int id;
    char customer[100];
    int status;
    float total;
} Order;

typedef struct NodeDLL {
    Order data;
    struct NodeDLL* prev;
    struct NodeDLL* next;
} NodeDLL;

typedef struct NodeSLL {
    Order data;
    struct NodeSLL* next;
} NodeSLL;

NodeDLL* headDLL = NULL;
NodeSLL* headSLL = NULL;

Order createOrder() {
    Order o;
    printf("Nhập ID: "); scanf("%d", &o.id);
    printf("Nhập tên khách hàng: "); getchar(); fgets(o.customer, sizeof(o.customer), stdin);
    o.customer[strcspn(o.customer, "\n")] = 0;
    printf("Nhập tổng tiền: "); scanf("%f", &o.total);
    o.status = 0;
    return o;
}

void addOrder() {
    Order o = createOrder();
    NodeDLL* newNode = (NodeDLL*)malloc(sizeof(NodeDLL));
    newNode->data = o;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (headDLL == NULL) {
        headDLL = newNode;
    } else {
        NodeDLL* temp = headDLL;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
        newNode->prev = temp;
    }
    printf("Thêm đơn hàng thành công!\n");
}

void displayOrders() {
    printf("=== DANH SÁCH ĐƠN HÀNG CHƯA GIAO ===\n");
    NodeDLL* temp = headDLL;
    while (temp != NULL) {
        printf("ID: %d | Khách: %s | Tổng tiền: %.2f | Trạng thái: %s\n", 
            temp->data.id, temp->data.customer, temp->data.total, "Chưa giao");
        temp = temp->next;
    }

    printf("=== LỊCH SỬ ĐƠN HÀNG ĐÃ GIAO ===\n");
    NodeSLL* temp2 = headSLL;
    while (temp2 != NULL) {
        printf("ID: %d | Khách: %s | Tổng tiền: %.2f | Trạng thái: %s\n", 
            temp2->data.id, temp2->data.customer, temp2->data.total, "Đã giao");
        temp2 = temp2->next;
    }
}

void deleteOrder(int id) {
    NodeDLL* temp = headDLL;
    while (temp != NULL && temp->data.id != id)
        temp = temp->next;

    if (temp == NULL) {
        printf("Không tìm thấy đơn hàng để xóa.\n");
        return;
    }

    if (temp->prev) temp->prev->next = temp->next;
    else headDLL = temp->next;
    if (temp->next) temp->next->prev = temp->prev;
    free(temp);
    printf("Xóa đơn hàng thành công.\n");
}

void updateOrder(int id) {
    NodeDLL* temp = headDLL;
    while (temp != NULL && temp->data.id != id)
        temp = temp->next;

    if (temp == NULL) {
        printf("Không tìm thấy đơn hàng để cập nhật.\n");
        return;
    }

    printf("Nhập tên mới: "); getchar(); fgets(temp->data.customer, sizeof(temp->data.customer), stdin);
    temp->data.customer[strcspn(temp->data.customer, "\n")] = 0;
    printf("Nhập tổng tiền mới: "); scanf("%f", &temp->data.total);
    printf("Cập nhật thành công.\n");
}

void markDelivered(int id) {
    NodeDLL* temp = headDLL;
    while (temp != NULL && temp->data.id != id)
        temp = temp->next;

    if (temp == NULL) {
        printf("Không tìm thấy đơn hàng để đánh dấu giao.\n");
        return;
    }

    Order o = temp->data;
    o.status = 1;

    NodeSLL* newNode = (NodeSLL*)malloc(sizeof(NodeSLL));
    newNode->data = o;
    newNode->next = headSLL;
    headSLL = newNode;

    if (temp->prev) temp->prev->next = temp->next;
    else headDLL = temp->next;
    if (temp->next) temp->next->prev = temp->prev;
    free(temp);

    printf("Đã đánh dấu đơn hàng là ĐÃ GIAO.\n");
}

void sortOrdersByTotal() {
    if (headDLL == NULL || headDLL->next == NULL) return;
    int swapped;
    NodeDLL *ptr1;
    do {
        swapped = 0;
        ptr1 = headDLL;
        while (ptr1->next != NULL) {
            if (ptr1->data.total > ptr1->next->data.total) {
                Order temp = ptr1->data;
                ptr1->data = ptr1->next->data;
                ptr1->next->data = temp;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
    } while (swapped);
    printf("Sắp xếp hoàn tất.\n");
}

void searchOrder(char* name) {
    int found = 0;
    printf("Tìm trong danh sách chưa giao:\n");
    NodeDLL* temp = headDLL;
    while (temp != NULL) {
        if (strstr(temp->data.customer, name)) {
            printf("ID: %d | Khách: %s | Tổng tiền: %.2f\n", temp->data.id, temp->data.customer, temp->data.total);
            found = 1;
        }
        temp = temp->next;
    }

    printf("Tìm trong danh sách đã giao:\n");
    NodeSLL* temp2 = headSLL;
    while (temp2 != NULL) {
        if (strstr(temp2->data.customer, name)) {
            printf("ID: %d | Khách: %s | Tổng tiền: %.2f\n", temp2->data.id, temp2->data.customer, temp2->data.total);
            found = 1;
        }
        temp2 = temp2->next;
    }

    if (!found) printf("Không tìm thấy đơn hàng nào.\n");
}

void menu() {
    int choice, id;
    char name[100];
    do {
        printf("\n—————————— ORDER MANAGER ——————————\n");
        printf("1. Thêm đơn hàng mới\n");
        printf("2. Hiển thị danh sách đơn hàng\n");
        printf("3. Xóa đơn hàng (theo ID)\n");
        printf("4. Cập nhật thông tin đơn hàng\n");
        printf("5. Đánh dấu đơn hàng đã được giao\n");
        printf("6. Sắp xếp đơn hàng theo tổng tiền\n");
        printf("7. Tìm kiếm đơn hàng theo tên khách\n");
        printf("8. Thoát chương trình\n");
        printf("Chọn: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: addOrder(); break;
            case 2: displayOrders(); break;
            case 3: printf("Nhập ID cần xóa: "); scanf("%d", &id); deleteOrder(id); break;
            case 4: printf("Nhập ID cần cập nhật: "); scanf("%d", &id); updateOrder(id); break;
            case 5: printf("Nhập ID cần giao: "); scanf("%d", &id); markDelivered(id); break;
            case 6: sortOrdersByTotal(); break;
            case 7: 
                printf("Nhập tên khách hàng cần tìm: "); getchar(); fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                searchOrder(name); break;
            case 8: printf("Thoát chương trình.\n"); break;
            default: printf("Lựa chọn không hợp lệ.\n");
        }
    } while (choice != 8);
}

int main() {
    menu();
    return 0;
}
