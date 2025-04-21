#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node* next;
} Node;

Node* reverse(Node* head) {
    Node *prev = NULL, *curr = head;
    while (curr != NULL) {
        Node* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

void print_and_free(Node* head) {
    Node* temp;
    while (head != NULL) {
        printf("%d ", head->value);
        temp = head;
        head = head->next;
        free(temp);
    }
    printf("\n"); 
}


int main() {
    Node* head = NULL;
    Node* tail = NULL;
    int num;

    while (1) {
        scanf("%d", &num);
        if (num == 0) break;

        Node* new_node = (Node*)malloc(sizeof(Node));
        new_node->value = num;
        new_node->next = NULL;

        if (head == NULL) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    head = reverse(head);
    print_and_free(head);

    return 0;
}

