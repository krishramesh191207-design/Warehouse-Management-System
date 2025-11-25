# Warehouse Management System – Project Report

## 1. Project Description
This project implements a **warehouse inventory management system** using a **sorted singly linked list** in C. Each inventory entry (batch) includes an item name, quantity, price, and expiry date. Batches are stored in sorted order by item name and expiry date. The system supports batch addition, deletion, searching, selling/removing quantities using FIFO logic, and removing expired items.

## 2. Functionalities
- **Add Batch**: Insert a new batch into the sorted linked list. If same item and expiry exist, quantities merge.
- **Display Inventory**: Prints all batches in sorted order.
- **Remove Expired Items**: Deletes all batches older than a given date.
- **Remove Quantity (FIFO)**: Removes quantity starting from the earliest-expiry batch of that item.
- **Delete Batch**: Removes a specific item-expiry batch.
- **Update Batch**: Modify quantity and price of a given batch.
- **Search Item**: Display all batches of a given item and total quantity.
- **File Storage**: Loading from and saving to `warehouse.txt`.

## 3. Step-by-Step Implementation Summary
1. **Linked List Structure**  
   A `Node` struct stores batch data and a pointer to the next batch.
2. **Sorted Insertion**  
   `addbatch()` inserts nodes in alphabetical order of item name and then expiry date.
3. **Batch Merging**  
   If a batch with the same name and expiry exists, quantities merge.
4. **Removing Expired Items**  
   The list is scanned and old nodes freed.
5. **FIFO Removal**  
   Quantities are removed from earliest-expiry batches. Empty batches are deleted.
6. **Searching & Updating**  
   The list is traversed sequentially to locate the target item/batch.
7. **File Handling**  
   Data loads at startup and saves at exit.

Here you go — **formatted EXACTLY like the screenshot**, same bullet style, same indentation, same markdown format:

---

## **4. Contribution Breakdown**

* **Mahith Giridhar**:

  * Implemented data structures
  * Wrote major functions (add, delete, remove qty, update, search, expired removal)
  * Implemented file I/O
  * Created testing scenarios and handled debugging

* **R Krish**:

  * Assisted in designing FIFO removal logic and batch flow
  * Helped structure item/batch traversal logic, test expired-item removal and sorting accuracy
  * Tested edge cases (invalid inputs, empty lists, missing items)
  * Improved expiry-date validation and merging conditions

* **Dhanush Narayan**:

  * Contributed to project documentation and report preparation
  * Verified batch merging behavior and alphabetical order correctness
  * Assisted in debugging memory issues and verifying node deletions
  * Improved expiry-date validation and merging conditions

---


## 5. Function Summaries

### `newnode()`
Creates a new node (batch) with given item data. Allocates memory and initializes values.

### `addbatch()`
Inserts a new batch in sorted order. Merges quantities if same item and expiry already exist.

### `isvalidexpiry()`
Checks if the expiry date string follows the `YYYY-MM-DD` format.

### `createbatch()`
Prompts the user for batch details and returns a newly created node.

### `displayinventory()`
Prints the full inventory in sorted order. Shows item name, quantity, price, and expiry.

### `removeexpireditems()`
Removes all nodes whose expiry date comes before the provided current date.

### `removequantity_earliestexpiry()`
Implements FIFO-based quantity removal. Deducts from earliest expiry first and deletes empty batches.

### `deletebatch()`
Deletes a specific batch based on item name and expiry date.

### `updatebatch()`
Updates the quantity and price of a specific batch.

### `searchitem()`
Displays all batches belonging to a particular item and the total available quantity.

### `loadfromfile()`
Reads saved inventory data from `warehouse.txt` and reconstructs the linked list.

### `savetofile()`
Writes the current inventory linked list to `warehouse.txt`.

### `freelist()`
Frees all nodes in memory before program exit.
