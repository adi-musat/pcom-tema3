#include "helpers.hpp"
#include "requests.hpp"

#define SERV_ADDR "34.241.4.235"
#define PORT 8080

#define URL_REGISTER "/api/v1/tema/auth/register"
#define URL_LOGIN "/api/v1/tema/auth/login"
#define URL_ACCESS "/api/v1/tema/library/access"
#define URL_ALL_BOOKS "/api/v1/tema/library/books"
#define URL_LOGOUT "/api/v1/tema/auth/logout"
#define CONTENT_TYPE "application/json"

int main(int argc, char *argv[]) {
    int sockfd;
    char *message, *response; 
    string command, cookies, jwt;
        
    while (true) {
        //citire comanda
        getline(cin, command);

        if (command == "exit") {
            break;
        } else if (command == "register") {
            string username, password, json_string;
            bool ok = false;
            //citire username si parola si validare input
            do {
                cout << "username=";
                getline(cin, username);
                if (username.find(' ') == string::npos) {
                    ok = true;
                } else {
                    cout << "Username must not contain spaces!\n";
                }
            } while (!ok);
            ok = false;
            do {
                cout << "password=";
                getline(cin, password);
                if (password.find(' ') == string::npos) {
                    ok = true;
                } else {
                    cout << "Password must not contain spaces!\n";
                }
            } while (!ok);
            
            json_string = create_auth_json(username, password);

            //deschidere conexiune catre server
            sockfd = open_connection(SERV_ADDR, PORT, AF_INET, SOCK_STREAM, 0);

            //creare cerere POST pt inregistrare
            message = compute_post_request(SERV_ADDR, URL_REGISTER, CONTENT_TYPE, json_string, "", "");

            //trimitere cerere la server
            send_to_server(sockfd, message);
            //primire raspuns de la server si afisare
            response = receive_from_server(sockfd);
            register_response(response);

            //inchidere conexiune la server si eliberare memorie mesaj
            close_connection(sockfd);
            free(message);
        } else if (command == "login") {
            //nu ne putem loga daca suntem deja logati
            if(!cookies.empty()) {
                cout << "You are already logged in!\n";
                continue;
            }
            
            string username, password, json_string;
            
            bool ok = false;
            //citire username si parola si validare input
            do {
                cout << "username=";
                getline(cin, username);
                if (username.find(' ') == string::npos) {
                    ok = true;
                } else {
                    cout << "Username must not contain spaces!\n";
                }
            } while (!ok);
            ok = false;
            do {
                cout << "password=";
                getline(cin, password);
                if (password.find(' ') == string::npos) {
                    ok = true;
                } else {
                    cout << "Password must not contain spaces!\n";
                }
            } while (!ok);

            json_string = create_auth_json(username, password);

            //deschidere conexiune catre server
            sockfd = open_connection(SERV_ADDR, PORT, AF_INET, SOCK_STREAM, 0);

            //creare cerere POST pt login
            message = compute_post_request(SERV_ADDR, URL_LOGIN, CONTENT_TYPE, json_string, "", "");

            //trimitere cerere la server
            send_to_server(sockfd, message);
            //primire raspuns de la server, salvare cookie si afisare
            response = receive_from_server(sockfd);
            cookies = extract_cookie_from_response(response);
            login_response(response);

            //inchidere conexiune la server si eliberare memorie mesaj
            close_connection(sockfd);
            free(message);
        } else if (command == "enter_library") {
            //deschidere conexiune catre server
            sockfd = open_connection(SERV_ADDR, PORT, AF_INET, SOCK_STREAM, 0);

            //creare cerere GET pentru acces
            message = compute_get_request(SERV_ADDR, URL_ACCESS, nullptr, cookies, "");

            //trimitere cerere la server
            send_to_server(sockfd, message);
            //primire raspuns de la server, salvare jwt si afisare
            response = receive_from_server(sockfd);
            if (access_response(response) == 0) {
                jwt = extract_jwt_from_response(response);
            }

            //inchidere conexiune la server si eliberare memorie mesaj
            close_connection(sockfd);
            free(message);
        } else if (command == "get_books") {
            //deschidere conexiune catre server
            sockfd = open_connection(SERV_ADDR, PORT, AF_INET, SOCK_STREAM, 0);

            //creare cerere GET pentru afisare carti
            message = compute_get_request(SERV_ADDR, URL_ALL_BOOKS, nullptr, "", jwt);

            //trimitere cerere la server
            send_to_server(sockfd, message);
            //primire raspuns de la server si afisare
            response = receive_from_server(sockfd);
            get_books_response(response);
            
            //inchidere conexiune la server si eliberare memorie mesaj
            close_connection(sockfd);
            free(message);
        } else if (command == "get_book") {
            string id;
            bool ok = false;
            //citire id carte si validare input
            do {
                try {
                    cout << "id=";
                    cin >> id;
                    stoi(id);
                    ok = true;
                } catch (exception &e) {
                    cout << "ID must be a number!\n";
                }
            } while (!ok);
            //eliberare buffer cin
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            //creare url cu id-ul cartii cerute
            string url = "/api/v1/tema/library/books/" + id;

            //deschidere conexiune catre server
            sockfd = open_connection(SERV_ADDR, PORT, AF_INET, SOCK_STREAM, 0);

            //creare cerere GET pentru afisare carte
            message = compute_get_request(SERV_ADDR, url.c_str(), nullptr, "", jwt);

            //trimitere cerere la server
            send_to_server(sockfd, message);
            //primire raspuns de la server si afisare
            response = receive_from_server(sockfd);
            get_book_id_response(response);

            //inchidere conexiune la server si eliberare memorie mesaj
            close_connection(sockfd);
            free(message);
        } else if (command == "add_book") {
            string title, author, genre, page_count, publisher, json_string;
            //citire detalii despre carte
            cout << "title=";
            getline(cin, title);
            cout << "author=";
            getline(cin, author);
            cout << "genre=";
            getline(cin, genre);
            cout << "publisher=";
            getline(cin, publisher);
            //validare input nr de pagini
            bool ok = false;
            do {
                try {
                    cout << "page_count=";
                    cin >> page_count;
                    stoi(page_count);
                    ok = true;
                } catch (exception &e) {
                    cout << "Page count must be a number!\n";
                }
            } while (!ok);
            //eliberare buffer cin
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            json_string = create_book_json(title, author, genre, stoi(page_count), publisher);

            //deschidere conexiune catre server
            sockfd = open_connection(SERV_ADDR, PORT, AF_INET, SOCK_STREAM, 0);

            //creare cerere POST pentru adaugare carte
            message = compute_post_request(SERV_ADDR, URL_ALL_BOOKS, CONTENT_TYPE, json_string, "", jwt);

            //trimitere cerere la server
            send_to_server(sockfd, message);
            //primire raspuns de la server si afisare
            response = receive_from_server(sockfd);
            add_book_response(response);

            //inchidere conexiune la server si eliberare memorie mesaj
            close_connection(sockfd);
            free(message);
        } else if (command == "delete_book") {
            string id;
            bool ok = false;
            //citire id carte si validare input
            do {
                try {
                    cout << "id=";
                    cin >> id;
                    stoi(id);
                    ok = true;
                } catch (exception &e) {
                    cout << "ID must be a number!\n";
                }
            } while (!ok);
            //eliberare buffer cin
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            //creare url cu id-ul cartii cerute
            string url = "/api/v1/tema/library/books/" + id;

            //deschidere conexiune catre server
            sockfd = open_connection(SERV_ADDR, PORT, AF_INET, SOCK_STREAM, 0);

            //creare cerere DELETE pentru stergere carte
            message = compute_delete_request(SERV_ADDR, url.c_str(), nullptr, "", jwt);

            //trimitere cerere la server
            send_to_server(sockfd, message);
            //primire raspuns de la server si afisare
            response = receive_from_server(sockfd);
            delete_book_response(response);

            //inchidere conexiune la server si eliberare memorie mesaj
            close_connection(sockfd);
            free(message);
        } else if (command == "logout") {
            //deschidere conexiune catre server
            sockfd = open_connection(SERV_ADDR, PORT, AF_INET, SOCK_STREAM, 0);

            //creare cerere GET pentru delogare
            message = compute_get_request(SERV_ADDR, URL_LOGOUT, nullptr, cookies, "");

            //trimitere cerere la server
            send_to_server(sockfd, message);
            //primire raspuns de la server si afisare
            response = receive_from_server(sockfd);
            if (logout_response(response) == 0) {
                //daca a avut loc delogarea se sterg cookies si tokenul JWT
                cookies.clear();
                jwt.clear();
            }

            //inchidere conexiune la server si eliberare memorie mesaj
            close_connection(sockfd);
            free(message);
        } else {
            cout << "Unknown command " << command << ". Available commands:\n";
            cout << "register\nlogin\nenter_library\nget_books\nget_book\nadd_book\ndelete_book\nlogout\nexit\n";
        }
    }

    return 0;
}
