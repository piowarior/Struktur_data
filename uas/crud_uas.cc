#include <iostream>
#include <mysql/mysql.h>
#include <sstream>
#include <ctime>
#include <unistd.h>
#include <termios.h>


using namespace std;

const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "123";
const char* dbname = "uas_cpp";
unsigned int port = 31235;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connect_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Connected to the database successfully." << endl;
        } else {
            cerr << "Connection failed: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init failed" << endl;
    }
    return conn;
}


string get_current_date() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[20];
    sprintf(buf, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    return string(buf);
}

string get_return_date() {
    time_t now = time(0);
    now += 5 * 24 * 60 * 60; // Add 5 days
    tm* ltm = localtime(&now);
    char buf[20];
    sprintf(buf, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    return string(buf);
}

// .......................................................................................................................

bool validate_login(const string& table, const string& username, const string& password) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "SELECT * FROM " << table << " WHERE nama_" << table << " = '" << username << "' AND password_" << table << " = '" << password << "'";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "SELECT failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return false;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        bool valid = (mysql_num_rows(res) > 0);

        mysql_free_result(res);
        mysql_close(conn);
        return valid;
    }
    return false;
}



bool book_exists(int book_id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "SELECT id_buku FROM buku WHERE id_buku = " << book_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "SELECT failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return false;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        bool exists = (mysql_num_rows(res) > 0);
        mysql_free_result(res);
        mysql_close(conn);
        return exists;
    }
    return false;
}



bool user_exists(int user_id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "SELECT id_pengguna FROM pengguna WHERE id_pengguna = " << user_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "SELECT failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return false;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        bool exists = (mysql_num_rows(res) > 0);
        mysql_free_result(res);
        mysql_close(conn);
        return exists;
    }
    return false;
}


bool admin_exists() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT COUNT(*) FROM admin")) {
            cerr << "SELECT COUNT failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return false;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        bool exists = (row && atoi(row[0]) > 0);

        mysql_free_result(res);
        mysql_close(conn);
        return exists;
    }
    return false;
}

//.........................................................................

// Borrow Book
void borrow_book(int book_id, int user_id, const string& username, const string& email) {
    if (!book_exists(book_id)) {
        cout << "Book ID not found." << endl;
        return;
    }

    if (!user_exists(user_id)) {
        cout << "User ID not found." << endl;
        return;
    }

    MYSQL* conn = connect_db();
    if (conn) {
        string tgl_peminjaman = get_current_date();
        string tgl_pengembalian = get_return_date();
        stringstream query;
        query << "INSERT INTO peminjam (id_pengguna, id_buku, nama_pengguna, email_pengguna, tgl_peminjaman, tgl_pengembalian) VALUES ("
              << user_id << ", " << book_id << ", '" << username << "', '" << email << "', '" << tgl_peminjaman << "', '" << tgl_pengembalian << "')";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "INSERT failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Book successfully borrowed." << endl;
        }
        mysql_close(conn);
    }
}

void search_books(const string& keyword) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "SELECT * FROM buku WHERE judul LIKE '%" + keyword + "%' OR penulis LIKE '%" + keyword + "%' OR penerbit LIKE '%" + keyword + "%' OR genre LIKE '%" + keyword + "%'";
        if (mysql_query(conn, query.c_str())) {
            cerr << "SELECT failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Title: " << row[1] << ", Author: " << row[2] << ", Publisher: " << row[3] << ", Genre: " << row[4] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}


// User Functions
void create_user(const string& username, const string& email, const string& password) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "INSERT INTO pengguna (nama_pengguna, email_pengguna, password_pengguna) VALUES ('" + username + "', '" + email + "', '" + password + "')";
        if (mysql_query(conn, query.c_str())) {
            cerr << "INSERT failed: " << mysql_error(conn) << endl;
        } else {
            cout << "User successfully added." << endl;
        }
        mysql_close(conn);
    }
}

void get_books() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM buku")) {
            cerr << "SELECT failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            if (row[0] && row[1] && row[2] && row[3] && row[4]) { // Ensure all columns are not null
                cout << "ID: " << row[0] << ", Title: " << row[1] << ", Author: " << row[2] << ", Publisher: " << row[3] << ", Genre: " << row[4] << endl;
            } else {
                cout << "Row has NULL values." << endl;
            }
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}


// User Menu
void user_menu() {
    int choice;
    while (true) {
        cout << "\nUser Menu:\n";
        cout << "1. Register\n";
        cout << "2. Show All Books\n";
        cout << "3. Search Books\n";
        cout << "4. Borrow Book\n";
        cout << "5. Back to Main Menu\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string username, email, password;
            cout << "Enter username: ";
            cin.ignore();
            getline(cin, username);
            cout << "Enter email: ";
            getline(cin, email);
            cout << "Enter password: ";
            getline(cin, password);
            create_user(username, email, password);
        } else if (choice == 2) {
            get_books();
        } else if (choice == 3) {
            string keyword;
            cout << "Enter keyword to search: ";
            cin.ignore();
            getline(cin, keyword);
            search_books(keyword);
        } else if (choice == 4) {
            int book_id, user_id;
            string username, email;
            cout << "Enter book ID to borrow: ";
            cin >> book_id;
            cout << "Enter user ID: ";
            cin >> user_id;
            cin.ignore();
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter email: ";
            getline(cin, email);
            borrow_book(book_id, user_id, username, email);
        } else if (choice == 5) {
            break;
        } else if (choice == 6) {
            exit(0);
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

void get_users() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM pengguna")) {
            cerr << "SELECT failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            if (row[0] && row[1] && row[2] && row[3]) { // Ensure all columns are not null
                cout << "ID: " << row[0] << ", Username: " << row[1] << ", Email: " << row[2] << ", Password: " << row[3] << endl;
            } else {
                cout << "Row has NULL values." << endl;
            }
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void delete_book(int book_id) {
    if (!book_exists(book_id)) {
        cout << "Maaf, data yang dicari tidak ditemukan." << endl;
        return;
    }

    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM buku WHERE id_buku = " << book_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "DELETE failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Book successfully deleted." << endl;
        }
        mysql_close(conn);
    }
}


void update_book(int book_id, const string& title, const string& author, const string& publisher, const string& genre) {
    if (!book_exists(book_id)) {
        cout << "Maaf, data yang dicari tidak ditemukan." << endl;
        return;
    }

    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE buku SET judul = '" << title << "', penulis = '" << author << "', penerbit = '" << publisher << "', genre = '" << genre << "' WHERE id_buku = " << book_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "UPDATE failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Book successfully updated." << endl;
        }
        mysql_close(conn);
    }
}


// Book Functions
void create_book(const string& title, const string& author, const string& publisher, const string& genre) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "INSERT INTO buku (judul, penulis, penerbit, genre) VALUES ('" + title + "', '" + author + "', '" + publisher + "', '" + genre + "')";
        if (mysql_query(conn, query.c_str())) {
            cerr << "INSERT failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Book successfully added." << endl;
        }
        mysql_close(conn);
    }
}

void press_any_key() {
    cout << "Press any key to continue..." << endl;

    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char ch;
    read(STDIN_FILENO, &ch, 1);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

// Admin Menu
void admin_menu() {
    int choice;
    while (true) {
        cout << "\nAdmin Menu:\n";
        cout << "1. Add Book\n";
        cout << "2. Show All Books\n";
        cout << "3. Update Book\n";
        cout << "4. Delete Book\n";
        cout << "5. Show All Users\n";
        cout << "6. Back to Main Menu\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string title, author, publisher, genre;
            cout << "Enter title: ";
            cin.ignore();
            getline(cin, title);
            cout << "Enter author: ";
            getline(cin, author);
            cout << "Enter publisher: ";
            getline(cin, publisher);
            cout << "Enter genre: ";
            getline(cin, genre);
            create_book(title, author, publisher, genre);
        } else if (choice == 2) {
            get_books();
        } else if (choice == 3) {
            int book_id;
            string title, author, publisher, genre;
            cout << "Enter book ID to update: ";
            cin >> book_id;
            cin.ignore();
            cout << "Enter new title: ";
            getline(cin, title);
            cout << "Enter new author: ";
            getline(cin, author);
            cout << "Enter new publisher: ";
            getline(cin, publisher);
            cout << "Enter new genre: ";
            getline(cin, genre);
            update_book(book_id, title, author, publisher, genre);
        } else if (choice == 4) {
            int book_id;
            cout << "Enter book ID to delete: ";
            cin >> book_id;
            delete_book(book_id);
        } else if (choice == 5) {
            get_users();
        } else if (choice == 6) {
            break;
        } else if (choice == 7) {
            exit(0);
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

// Admin Functions
void create_admin(const string& name, const string& email, const string& password) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "INSERT INTO admin (nama_admin, email_admin, password_admin) VALUES ('" + name + "', '" + email + "', '" + password + "')";
        if (mysql_query(conn, query.c_str())) {
            cerr << "INSERT failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Admin successfully added." << endl;
        }
        mysql_close(conn);
    }
}


// Main Menu
void main_menu() {
    int choice;
    while (true) {
        cout << "\nMain Menu:\n";
        cout << "1. Login as Admin\n";
        cout << "2. Login as User\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            if (!admin_exists()) {
                cout << "No admin found. Please register an admin.\n";
                string name, email, password;
                cout << "Enter admin name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter admin email: ";
                getline(cin, email);
                cout << "Enter admin password: ";
                getline(cin, password);
                create_admin(name, email, password);
            } else {
                int attempts = 0;
                while (attempts < 3) {
                    string name, password;
                    cout << "Enter admin name: ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "Enter password: ";
                    getline(cin, password);
                    if (validate_login("admin", name, password)) {
                        admin_menu();
                        break;
                    } else {
                        cout << "Invalid admin credentials. Try again." << endl;
                        attempts++;
                    }
                }
                if (attempts == 3) {
                    cout << "Too many failed attempts. Returning to main menu." << endl;
                    press_any_key();
                }
            }
        } else if (choice == 2) {
            string username, password;
            user_menu();
        } else if (choice == 3) {
            exit(0);
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

int main() {
    main_menu();
    return 0;
}

