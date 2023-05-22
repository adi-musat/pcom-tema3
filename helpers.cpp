#include "helpers.hpp"
#include "buffer.hpp"

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

void register_response(char *response) {
    if (strstr(response, "HTTP/1.1 201 Created")) {
        cout << "Account successfully created!\n";
    } else if (strstr(response, "HTTP/1.1 400 Bad Request")) {
        char *error_msg = basic_extract_json_response(response);
        json json_msg = json::parse(error_msg);
        cout << "Error! " << json_msg["error"].get<string>() << "\n";
    }
}

void login_response(char *response) {
    if (strstr(response, "HTTP/1.1 200 OK")) {
        cout << "Login successful!\n";
    } else if (strstr(response, "HTTP/1.1 400 Bad Request")) {
        char *error_msg = basic_extract_json_response(response);
        json json_msg = json::parse(error_msg);
        cout << "Error! " << json_msg["error"].get<string>() << "\n";
    }
}

int access_response(char *response) {
    if (strstr(response, "HTTP/1.1 200 OK")) {
        cout << "Access granted!\n";
        return 0;
    } else if (strstr(response, "HTTP/1.1 401 Unauthorized")) {
        char *error_msg = basic_extract_json_response(response);
        json json_msg = json::parse(error_msg);
        cout << "Error! " << json_msg["error"].get<string>() << "\n";
    }
    return -1;
}

void get_books_response(char *response) {
    if (strstr(response, "HTTP/1.1 200 OK")) {
        cout << "List of all books:\n";
        char *json_resp = basic_extract_json_array(response);
        if (json_resp == nullptr) { //nu exista carti in biblioteca
            cout << "None\n";
            return;
        }
        json json_array = json::parse(json_resp);

        for (const auto &json_entry : json_array) {
            cout << "ID: " << json_entry["id"].get<int>() << 
            " Title: " << json_entry["title"].get<string>() << "\n";
        }
    } else if (strstr(response, "HTTP/1.1 403 Forbidden")) {
        char *error_msg = basic_extract_json_response(response);
        json json_msg = json::parse(error_msg);
        cout << "Error! " << json_msg["error"].get<string>() << "\n";
    }
}

void get_book_id_response(char *response) {
    if (strstr(response, "HTTP/1.1 200 OK")) {
        char *json_resp = basic_extract_json_response(response);
        json_resp[strlen(json_resp) - 1] = '\0'; //eliminare ']' de la sfarsit
        json json_obj = json::parse(json_resp);

        cout << "Title: " << json_obj["title"].get<string>() << "\n";
        cout << "Author: " << json_obj["author"].get<string>() << "\n";
        cout << "Publisher: " << json_obj["publisher"].get<string>() << "\n";
        cout << "Genre: " << json_obj["genre"].get<string>() << "\n";
        cout << "Page count: " << json_obj["page_count"].get<int>() << "\n";
    } else if (strstr(response, "HTTP/1.1 403 Forbidden") || 
                strstr(response, "HTTP/1.1 404 Not Found")) 
    {
        char *error_msg = basic_extract_json_response(response);
        json json_msg = json::parse(error_msg);
        cout << "Error! " << json_msg["error"].get<string>() << "\n";
    }
}

void add_book_response(char *response) {
    if (strstr(response, "HTTP/1.1 200 OK")) {
        cout << "Book successfully added!\n";
    } else if (strstr(response, "HTTP/1.1 403 Forbidden")) {
        char *error_msg = basic_extract_json_response(response);
        json json_msg = json::parse(error_msg);
        cout << "Error! " << json_msg["error"].get<string>() << "\n";
    }
}

void delete_book_response(char *response) {
    if (strstr(response, "HTTP/1.1 200 OK")) {
        cout << "Book successfully deleted!\n";
    } else if (strstr(response, "HTTP/1.1 403 Forbidden") || 
                strstr(response, "HTTP/1.1 404 Not Found")) 
    {
        char *error_msg = basic_extract_json_response(response);
        json json_msg = json::parse(error_msg);
        cout << "Error! " << json_msg["error"].get<string>() << "\n";
    }
}

int logout_response(char *response) {
    if (strstr(response, "HTTP/1.1 200 OK")) {
        cout << "Logged out!\n";
        return 0;
    } else if (strstr(response, "HTTP/1.1 400 Bad Request")) {
        char *error_msg = basic_extract_json_response(response);
        json json_msg = json::parse(error_msg);
        cout << "Error! " << json_msg["error"].get<string>() << "\n";
    }
    return -1;
}

string extract_cookie_from_response(char *response) {
    string resp(response);
    size_t cookie_start, cookie_end;

    cookie_start = resp.find("Set-Cookie: ");
    if (cookie_start == string::npos) {
        //nu exista cookie in raspuns
        return "";
    }

    cookie_start += strlen("Set-Cookie: ");
    cookie_end = resp.find(';', cookie_start);

    return resp.substr(cookie_start, cookie_end - cookie_start);
}

string extract_jwt_from_response(char *response) {
    char *json_content = basic_extract_json_response(response);
    json json_msg = json::parse(json_content);

    return json_msg["token"].get<string>();
}

string create_auth_json(const string &username, const string &password) {
    json json_obj;
    json_obj["username"] = username;
    json_obj["password"] = password;

    return json_obj.dump(4);
}

string create_book_json(const string &title, const string &author, const string &genre, 
                        int page_count, const string &publisher)
{
    json json_obj;
    json_obj["title"] = title;
    json_obj["author"] = author;
    json_obj["genre"] = genre;
    json_obj["page_count"] = page_count;
    json_obj["publisher"] = publisher;

    return json_obj.dump(4);
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void compute_message(char *message, const char *line)
{
    strcat(message, line);
    strcat(message, "\r\n");
}

int open_connection(const char *host_ip, int portno, int ip_type, int socket_type, int flag)
{
    struct sockaddr_in serv_addr;
    int sockfd = socket(ip_type, socket_type, flag);
    if (sockfd < 0)
        error("ERROR opening socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(portno);
    inet_aton(host_ip, &serv_addr.sin_addr);

    /* connect the socket */
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    return sockfd;
}

void close_connection(int sockfd)
{
    close(sockfd);
}

void send_to_server(int sockfd, char *message)
{
    int bytes, sent = 0;
    int total = strlen(message);

    do
    {
        bytes = write(sockfd, message + sent, total - sent);
        if (bytes < 0) {
            error("ERROR writing message to socket");
        }

        if (bytes == 0) {
            break;
        }

        sent += bytes;
    } while (sent < total);
}

char *receive_from_server(int sockfd)
{
    char response[BUFLEN];
    buffer buffer = buffer_init();
    int header_end = 0;
    int content_length = 0;

    do {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0){
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
        
        header_end = buffer_find(&buffer, HEADER_TERMINATOR, HEADER_TERMINATOR_SIZE);

        if (header_end >= 0) {
            header_end += HEADER_TERMINATOR_SIZE;
            
            int content_length_start = buffer_find_insensitive(&buffer, CONTENT_LENGTH, CONTENT_LENGTH_SIZE);
            
            if (content_length_start < 0) {
                continue;           
            }

            content_length_start += CONTENT_LENGTH_SIZE;
            content_length = strtol(buffer.data + content_length_start, NULL, 10);
            break;
        }
    } while (1);
    size_t total = content_length + (size_t) header_end;
    
    while (buffer.size < total) {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0) {
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
    }
    buffer_add(&buffer, "", 1);
    return buffer.data;
}

char *basic_extract_json_response(char *str)
{
    return strstr(str, "{\"");
}

char *basic_extract_json_array(char *str) {
    return strstr(str, "[{\"");
}
