#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ORDER_HISTORY 100

// Structure for menu items
struct MenuItem {
    int id;
    char name[50];
    float price;
    struct MenuItem *next;
};

// Structure for customer orders
struct OrderItem {
    int itemId;
    char itemName[50];
    float itemPrice;
    struct OrderItem *next;
};

// Structure for each order
struct Order {
    int orderId;
    struct OrderItem *items;
    struct Order *next;
};

// Linked List for menu items
struct MenuItem *menu = NULL;

// Linked List for customer orders (queue)
struct Order *orderQueueFront = NULL;
struct Order *orderQueueRear = NULL;

// Linked List for order history (stack)
struct Order *orderHistoryTop = NULL;

// Function to add a menu item
void addMenuItem() {
    struct MenuItem *newItem = (struct MenuItem *)malloc(sizeof(struct MenuItem));

    newItem->id = rand() % 1000 + 1;  // Random ID for the item
    printf("Enter item name: ");
    scanf(" %[^\n]", newItem->name);
    printf("Enter item price: ");
    scanf("%f", &newItem->price);

    newItem->next = menu;
    menu = newItem;

    printf("Item added successfully! Item ID: %d\n", newItem->id);
}

// Function to display menu items
void displayMenu() {
    struct MenuItem *current = menu;
    if (current == NULL) {
        printf("Menu is empty.\n");
        return;
    }

    printf("Menu Items:\n");
    printf("ID\tName\t\tPrice\n");
    while (current != NULL) {
        printf("%d\t%s\t%.2f\n", current->id, current->name, current->price);
        current = current->next;
    }
}

// Function to search for a menu item by ID
void searchMenuItem() {
    int id;
    printf("Enter item ID to search: ");
    scanf("%d", &id);

    struct MenuItem *current = menu;
    while (current != NULL) {
        if (current->id == id) {
            printf("Found item: %d\t%s\t%.2f\n", current->id, current->name, current->price);
            return;
        }
        current = current->next;
    }
    printf("Item not found.\n");
}

// Function to delete a menu item by ID
void deleteMenuItem() {
    int id;
    printf("Enter item ID to delete: ");
    scanf("%d", &id);

    struct MenuItem *current = menu, *prev = NULL;

    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                menu = current->next;  // Deleting the first item
            } else {
                prev->next = current->next;  // Bypassing the current item
            }
            free(current);
            printf("Item deleted successfully.\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Item not found.\n");
}

// Function to add an order
void addOrder() {
    int orderId = rand() % 1000 + 1;  // Random order ID
    struct Order *newOrder = (struct Order *)malloc(sizeof(struct Order));

    newOrder->orderId = orderId;
    newOrder->items = NULL;
    newOrder->next = NULL;

    struct MenuItem *currentItem;
    char addMoreItems = 'y';

    while (addMoreItems == 'y' || addMoreItems == 'Y') {
        int menuItemId;
        printf("Enter menu item ID to add to the order: ");
        scanf("%d", &menuItemId);

        struct MenuItem *item = menu;
        while (item != NULL) {
            if (item->id == menuItemId) {
                struct OrderItem *newOrderItem = (struct OrderItem *)malloc(sizeof(struct OrderItem));
                newOrderItem->itemId = item->id;
                strcpy(newOrderItem->itemName, item->name);
                newOrderItem->itemPrice = item->price;
                newOrderItem->next = newOrder->items;
                newOrder->items = newOrderItem;
                printf("Item added to order.\n");
                break;
            }
            item = item->next;
        }

        printf("Add more items? (y/n): ");
        scanf(" %c", &addMoreItems);
    }

    if (orderQueueRear == NULL) {
        orderQueueFront = orderQueueRear = newOrder;
    } else {
        orderQueueRear->next = newOrder;
        orderQueueRear = newOrder;
    }

    printf("Order ID %d placed successfully!\n", orderId);
}

// Function to process customer orders (FIFO queue)
void processOrders() {
    if (orderQueueFront == NULL) {
        printf("No orders to process.\n");
        return;
    }

    struct Order *currentOrder = orderQueueFront;
    printf("Processing Order ID: %d\n", currentOrder->orderId);

    struct OrderItem *currentItem = currentOrder->items;
    printf("Items in order:\n");
    while (currentItem != NULL) {
        printf("%s\t%.2f\n", currentItem->itemName, currentItem->itemPrice);
        currentItem = currentItem->next;
    }

    // Remove order from queue (FIFO)
    orderQueueFront = orderQueueFront->next;
    if (orderQueueFront == NULL) {
        orderQueueRear = NULL;
    }

    // Push order to order history (stack)
    currentOrder->next = orderHistoryTop;
    orderHistoryTop = currentOrder;

    printf("Order ID %d processed and moved to history.\n", currentOrder->orderId);
}

// Function to display order history (stack)
void displayOrderHistory() {
    if (orderHistoryTop == NULL) {
        printf("No order history.\n");
        return;
    }

    printf("Order History:\n");
    struct Order *currentOrder = orderHistoryTop;
    while (currentOrder != NULL) {
        printf("Order ID: %d\n", currentOrder->orderId);

        struct OrderItem *currentItem = currentOrder->items;
        while (currentItem != NULL) {
            printf("%s\t%.2f\n", currentItem->itemName, currentItem->itemPrice);
            currentItem = currentItem->next;
        }
        printf("\n");

        currentOrder = currentOrder->next;
    }
}


int main() {
    int choice;

    while (1) {
        printf("\nCafeteria Management System\n");
        printf("1. Add Menu Item\n");
        printf("2. Display Menu\n");
        printf("3. Search Menu Item\n");
        printf("4. Delete Menu Item\n");
        printf("5. Place Order\n");
        printf("6. Process Orders\n");
        printf("7. View Order History\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addMenuItem(); break;
            case 2: displayMenu(); break;
            case 3: searchMenuItem(); break;
            case 4: deleteMenuItem(); break;
            case 5: addOrder(); break;
            case 6: processOrders(); break;
            case 7: displayOrderHistory(); break;
            case 8: exit(0); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
