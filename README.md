# My_sqlite
Building my own sqlite from scratch

# SQLite Database Information Reader

This C++ program reads information from an SQLite database file, specifically displaying the page size and listing the names of the tables in the database.

## Features

- **Display Database Page Size**: Outputs the page size used by the SQLite database.
- **List Tables**: Lists all the table names present in the SQLite database.

## Prerequisites

Before running the program, make sure you have the following:

- A C++ compiler (e.g., `g++` or `clang++`)
- A valid SQLite database file

## Usage

### Compilation

To compile the program, use the following command:

```bash
bg++ -o sqlite_reader sqlite_reader.cpp
```

### Running the Program
- Once compiled, you can run the program using one of the supported commands:

```bash
./sqlite_reader <database_file_path> <command>
```
- **<database_file_path>**: Path to the SQLite database file you want to inspect.
- **command** : One of the following:
- **.dbinfo**: Displays the page size and the number of tables in the database.
- **.tables**: Lists the names of the tables in the database.

Example
To show the database page size and number of tables:
```bash
./sqlite_reader my_database.db .dbinfo
```
**Sample Output:**
```yaml
database page size: 4096
number of tables: 3
```
- To list the tables in the database:
```bash
./sqlite_reader my_database.db .tables
```
Sample Output:
```bash
users orders products
```
**Code Overview**
- ``show_db_info()``: Reads and prints the database page size and number of tables.
- ``list_tables()``: Lists all the table names in the database by reading the required data from the file.
- ``main()``: Processes command-line arguments and invokes the appropriate function based on the command.
- **Error Handling**
The program checks whether the database file can be opened. If the file cannot be opened, it will print an error message.

**Limitations :**
- The program assumes that the database file follows the SQLite structure and that tables exist at expected locations in the file.
- The table names are extracted directly from the database without verifying the schema integrity.