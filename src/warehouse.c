#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "warehouse.txt"

typedef struct node {
    char itemname[50];
    int quantity;
    float price;
    char expirydate[20];
    struct node *next;
} Node;

Node* newnode(char *name, int qty, float price, char *expiry) {
    Node *n = (Node*)malloc(sizeof(Node));
    strcpy(n->itemname, name);
    n->quantity = qty;
    n->price = price;
    strcpy(n->expirydate, expiry);
    n->next = NULL;
    return n;
}

/* ------------------- ADD BATCH ------------------- */

Node* addbatch(Node *head, Node *temp) {
    if (head == NULL) {
        temp->next = NULL;
        return temp;
    }

    if (strcmp(temp->itemname, head->itemname) == 0 &&
        strcmp(temp->expirydate, head->expirydate) == 0) {
        head->quantity += temp->quantity;
        head->price = temp->price;
        free(temp);
        return head;
    }

    if (strcmp(temp->itemname, head->itemname) < 0 ||
       (strcmp(temp->itemname, head->itemname) == 0 &&
        strcmp(temp->expirydate, head->expirydate) < 0)) {
        temp->next = head;
        return temp;
    }

    Node *cur = head;
    while (cur->next != NULL) {
        if (strcmp(temp->itemname, cur->next->itemname) == 0 &&
            strcmp(temp->expirydate, cur->next->expirydate) == 0) {
            cur->next->quantity += temp->quantity;
            cur->next->price = temp->price;
            free(temp);
            return head;
        }

        if (strcmp(temp->itemname, cur->next->itemname) < 0 ||
           (strcmp(temp->itemname, cur->next->itemname) == 0 &&
            strcmp(temp->expirydate, cur->next->expirydate) < 0))
            break;

        cur = cur->next;
    }

    temp->next = cur->next;
    cur->next = temp;
    return head;
}

/* ------------------- VALID EXPIRY ------------------- */

int isvalidexpiry(char *exp) {
    if (strlen(exp) != 10) return 0;
    if (exp[4] != '-' || exp[7] != '-') return 0;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (exp[i] < '0' || exp[i] > '9') return 0;
    }
    return 1;
}

/* ------------------- CREATE BATCH ------------------- */

Node* createbatch() {
    char name[50], expiry[20];
    int qty;
    float price;

    printf("enter item name: ");
    scanf("%s", name);

    printf("enter quantity: ");
    scanf("%d", &qty);

    printf("enter price: ");
    scanf("%f", &price);

    do {
        printf("enter expiry (yyyy-mm-dd): ");
        scanf("%s", expiry);
        if (!isvalidexpiry(expiry))
            printf("invalid format, re-enter.\n");
    } while (!isvalidexpiry(expiry));

    return newnode(name, qty, price, expiry);
}

/* ------------------- DISPLAY INVENTORY ------------------- */

void displayinventory(Node *head) {
    if (head == NULL) {
        printf("\ninventory is empty.\n");
        return;
    }

    Node *cur = head;
    printf("\nwarehouse items (sorted):\n");
    while (cur != NULL) {
        printf("%s | qty: %d | price: %.2f | exp: %s\n",
               cur->itemname, cur->quantity, cur->price, cur->expirydate);
        cur = cur->next;
    }
}

/* ------------------- REMOVE EXPIRED ------------------- */

Node* removeexpireditems(Node *head) {
    char curdate[20];
    do {
        printf("enter current date (yyyy-mm-dd): ");
        scanf("%s", curdate);
        if (!isvalidexpiry(curdate))
            printf("invalid format! re-enter.\n");
    } while (!isvalidexpiry(curdate));

    while (head != NULL && strcmp(head->expirydate, curdate) < 0) {
        printf("removing expired batch: %s %s\n", head->itemname, head->expirydate);
        Node *tmp = head;
        head = head->next;
        free(tmp);
    }

    if (head == NULL) {
        printf("all expired items removed.\n");
        return NULL;
    }

    Node *cur = head;
    while (cur->next != NULL) {
        if (strcmp(cur->next->expirydate, curdate) < 0) {
            printf("removing expired batch: %s %s\n",
                   cur->next->itemname, cur->next->expirydate);

            Node *tmp = cur->next;
            cur->next = cur->next->next;
            free(tmp);
        } else cur = cur->next;
    }

    return head;
}

/* ------------------- REMOVE QUANTITY ------------------- */

Node* removequantity_earliestexpiry(Node *head) {
    char name[50];
    int req_qty;

    printf("enter item name to remove from: ");
    scanf("%s", name);

    printf("enter quantity to remove: ");
    scanf("%d", &req_qty);

    if (req_qty <= 0) {
        printf("quantity must be positive.\n");
        return head;
    }

    Node *cur = head, *prev = NULL;
    int found = 0;
    int removed_any = 0;

    while (cur != NULL && req_qty > 0) {
        if (strcmp(cur->itemname, name) == 0) {
            found = 1;

            if (cur->quantity > req_qty) {
                printf("removed %d from %s (exp %s)\n",
                       req_qty, cur->itemname, cur->expirydate);

                cur->quantity -= req_qty;
                req_qty = 0;
                removed_any = 1;
                break;
            }
            else {
                printf("removing entire batch: %s qty %d (exp %s)\n",
                       cur->itemname, cur->quantity, cur->expirydate);

                req_qty -= cur->quantity;
                removed_any = 1;

                Node *tmp = cur;
                cur = cur->next;

                if (prev == NULL)
                    head = cur;
                else
                    prev->next = cur;

                free(tmp);
                continue;
            }
        }

        prev = cur;
        cur = cur->next;
    }

    if (!found)
        printf("item '%s' not found in inventory.\n", name);

    if (found && !removed_any)
        printf("item '%s' found, but no quantity removed.\n", name);

    if (req_qty > 0 && found)
        printf("warning: not enough stock to remove entire requested quantity.\n");

    return head;
}

/* ------------------- DELETE BATCH ------------------- */

Node* deletebatch(Node *head) {
    char name[50], expiry[20];

    printf("enter item name to delete: ");
    scanf("%s", name);

    printf("enter expiry of batch to delete (yyyy-mm-dd): ");
    scanf("%s", expiry);

    Node *cur = head, *prev = NULL;
    int item_found = 0;

    while (cur != NULL) {

        if (strcmp(cur->itemname, name) == 0) {
            item_found = 1;

            if (strcmp(cur->expirydate, expiry) == 0) {

                if (prev == NULL) head = cur->next;
                else prev->next = cur->next;

                printf("batch deleted: %s (exp %s)\n", name, expiry);
                free(cur);
                return head;
            }
        }

        prev = cur;
        cur = cur->next;
    }

    if (!item_found)
        printf("item '%s' not found in inventory.\n", name);
    else
        printf("item found, but no batch with expiry '%s'.\n", expiry);

    return head;
}

/* ------------------- UPDATE BATCH ------------------- */

Node* updatebatch(Node *head) {
    char name[50], expiry[20];
    printf("enter item name to update: ");
    scanf("%s", name);

    printf("enter expiry of batch to update (yyyy-mm-dd): ");
    scanf("%s", expiry);

    Node *cur = head;

    while (cur != NULL) {
        if (strcmp(cur->itemname, name) == 0 &&
            strcmp(cur->expirydate, expiry) == 0) {

            printf("enter new quantity: ");
            scanf("%d", &cur->quantity);

            printf("enter new price: ");
            scanf("%f", &cur->price);

            printf("batch updated.\n");
            return head;
        }
        cur = cur->next;
    }

    printf("batch with item '%s' and expiry '%s' not found.\n", name, expiry);
    return head;
}

/* ------------------- SEARCH ITEM ------------------- */

void searchitem(Node *head) {
    char name[50];
    printf("enter item name to search: ");
    scanf("%s", name);

    Node *cur = head;
    int found = 0, totalqty = 0;

    while (cur != NULL) {
        if (strcmp(cur->itemname, name) == 0) {
            if (!found)
                printf("item | qty | price | expiry\n");

            found = 1;
            printf("%s | %d | %.2f | %s\n",
                   cur->itemname, cur->quantity,
                   cur->price, cur->expirydate);

            totalqty += cur->quantity;
        }
        cur = cur->next;
    }

    if (found)
        printf("total quantity: %d\n", totalqty);
    else
        printf("item '%s' not found.\n", name);
}

/* ------------------- FILE I/O ------------------- */

Node* loadfromfile(Node *head) {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) return head;

    char name[50], expiry[20];
    int qty;
    float price;

    while (fscanf(fp, "%49s %d %f %19s", name, &qty, &price, expiry) == 4) {
        Node *n = newnode(name, qty, price, expiry);
        head = addbatch(head, n);
    }

    fclose(fp);
    return head;
}

void savetofile(Node *head) {
    FILE *fp = fopen(FILENAME, "w");
    if (!fp) return;

    Node *cur = head;
    while (cur != NULL) {
        fprintf(fp, "%s %d %.2f %s\n",
                cur->itemname, cur->quantity,
                cur->price, cur->expirydate);
        cur = cur->next;
    }

    fclose(fp);
}

/* ------------------- FREE LIST ------------------- */

void freelist(Node *head) {
    Node *cur = head;
    while (cur != NULL) {
        Node *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

/* ------------------- MAIN ------------------- */

int main() {
    Node *head = NULL;
    int a;

    head = loadfromfile(head);

    while (1) {
        printf("\n==== warehouse menu ====\n");
        printf("1. add batch\n");
        printf("2. display inventory\n");
        printf("3. remove expired items\n");
        printf("4. remove quantity\n");
        printf("5. delete batch\n");
        printf("6. update batch\n");
        printf("7. search item\n");
        printf("enter choice: ");

        if (scanf("%d", &a) != 1) break;

        if (a == 1) head = addbatch(head, createbatch());
        else if (a == 2) displayinventory(head);
        else if (a == 3) head = removeexpireditems(head);
        else if (a == 4) head = removequantity_earliestexpiry(head);
        else if (a == 5) head = deletebatch(head);
        else if (a == 6) head = updatebatch(head);
        else if (a == 7) searchitem(head);
        else break;
    }

    savetofile(head);
    freelist(head);
    return 0;
}
