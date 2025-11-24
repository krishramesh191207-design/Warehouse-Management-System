# Warehouse Inventory Management System

A C program that manages batches of items in a warehouse using a **sorted singly linked list**.

## Features
- Add new item batches (sorted by name + expiry)
- Merge duplicate batches
- Display complete inventory
- Remove expired batches
- Sell/remove quantity using FIFO logic
- Delete specific batches
- Update existing batches
- Search by item name
- Data persistence via `warehouse.txt`

## How It Works
- Data is stored in a sorted linked list.
- Each node stores item name, quantity, price, and expiry.
- Expiry is validated using `YYYY-MM-DD` format.
- FIFO removal ensures earliest expiry is used first.
- Expired batches are removed automatically when requested.
- The inventory is saved on exit and loaded on start.

## File Structure
```
src/
   warehouse.c
warehouse.txt (auto-created)
REPORT.md
README.md
```

## Running the Program
### Compile:
```
gcc warehouse.c -o warehouse
```

### Run:
```
./warehouse
```

## Project Report
See `REPORT.md` for a full explanation of functions and implementation.
