#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int HEADER_SIZE = 100;
const int DB_HEADER_SIZE = 100;
const int PAGE_HEADER_SIZE = 8;

/**
 * @brief Reads the page size from the database file and prints it.
 * 
 * @param database_file_path The path to the SQLite database file.
 */
void show_db_info(const string &database_file_path)
{
    ifstream database_file(database_file_path, ios::binary);
    if (!database_file)
    {
        cerr << "Failed to open the database file" << endl;
        return;
    }

    // Skip the first 16 bytes of the SQLite header and read the page size
    database_file.seekg(16);
    char buffer[2];
    database_file.read(buffer, 2);
    unsigned short page_size = (static_cast<unsigned char>(buffer[1]) | (static_cast<unsigned char>(buffer[0]) << 8));

    // Print the page size
    cout << "database page size: " << page_size << endl;

    // Read the number of tables (skipping to the required position)
    database_file.seekg(HEADER_SIZE + 3);
    database_file.read(buffer, 2);
    unsigned short number_of_tables = (static_cast<unsigned char>(buffer[1]) | (static_cast<unsigned char>(buffer[0]) << 8));

    // Print the number of tables
    cout << "number of tables: " << number_of_tables << endl;
}

/**
 * @brief Reads and prints the names of all tables in the database.
 * 
 * @param database_file_path The path to the SQLite database file.
 */
void list_tables(const string &database_file_path)
{
    ifstream database_file(database_file_path, ios::binary);
    if (!database_file)
    {
        cerr << "Failed to open the database file" << endl;
        return;
    }

    // Move to the position where the cell count is stored
    database_file.seekg(DB_HEADER_SIZE + 3);
    char buffer[2];
    database_file.read(buffer, 2);
    unsigned short cell_count = (static_cast<unsigned char>(buffer[1]) | (static_cast<unsigned char>(buffer[0]) << 8));

    // Loop through each cell to find and print table names
    for (int i = 0; i < cell_count; i++)
    {
        // Read each cell's location
        database_file.seekg(DB_HEADER_SIZE + PAGE_HEADER_SIZE + (i * 2));
        database_file.read(buffer, 2);
        unsigned short curr_cell_location = (static_cast<unsigned char>(buffer[1]) | (static_cast<unsigned char>(buffer[0]) << 8));

        // Move to the cell and read its data
        database_file.seekg(curr_cell_location);
        char cell_header[3];
        database_file.read(cell_header, 3);

        // Determine the length of the type and name
        unsigned short columns = static_cast<unsigned short>(cell_header[2]);
        char column_header[columns];
        database_file.read(column_header, columns - 1);
        unsigned short type_length = (static_cast<unsigned short>(column_header[0]) - 13) / 2;
        unsigned short name_length = (static_cast<unsigned short>(column_header[1]) - 13) / 2;

        // Read and print the name of the table
        char name_buffer[name_length];
        database_file.read(name_buffer, name_length);
        for (int j = 0; j < name_length; j++)
        {
            cout << name_buffer[j];
        }
        cout << " ";
    }
    cout << endl;
}

/**
 * @brief Main function that processes the command line arguments and invokes the appropriate function.
 * 
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return int 0 on success, 1 on failure.
 */
int main(int argc, char *argv[])
{
    // Flush after every std::cout / std::cerr
    cout << unitbuf;
    cerr << unitbuf;

    // Check the number of arguments
    if (argc != 3)
    {
        cerr << "Expected two arguments" << endl;
        return 1;
    }

    string database_file_path = argv[1];
    string command = argv[2];

    // Handle different commands
    if (command == ".dbinfo")
    {
        show_db_info(database_file_path);
    }
    else if (command == ".tables")
    {
        list_tables(database_file_path);
    }
    else
    {
        cerr << "Unknown command: " << command << endl;
        return 1;
    }

    return 0;
}
