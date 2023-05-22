#ifndef _HELPERS_
#define _HELPERS_

#include <cstdio>       /* printf, sprintf */
#include <cstdlib>      /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <cstring>      /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include "json.hpp"

#define BUFLEN 4096
#define LINELEN 1000

using namespace std;
using namespace nlohmann;

// shows the current error
void error(const char *msg);

// adds a line to a string message
void compute_message(char *message, const char *line);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(const char *host_ip, int portno, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, char *message);

// receives and returns the message from a server
char *receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
char *basic_extract_json_response(char *str);

// extrage un array JSON din raspunsul serverului
char *basic_extract_json_array(char *str);

// extrage cookieul din raspunsul serverului dupa login
string extract_cookie_from_response(char *response);

//extrage tokenul JWT din raspunsul cererii enter_library
string extract_jwt_from_response(char *response);

// creaza un obiect json cu campurile username si password date, pe care
// il serializeaza dupa intr-un string
string create_auth_json(const string &username, const string &password);

// creaza un obiect json cu campuri ce contin date despre o carte, pe care
// il serializeaza dupa intr-un string
string create_book_json(const string &title, const string &author, const string &genre, 
                        int page_count, const string &publisher);

//afiseaza utilizatorului un raspuns corespunzator raspunsului serverului
//dupa cererea de inregistrare
void register_response(char *response);

//afiseaza utilizatorului un raspuns corespunzator raspunsului serverului
//dupa cererea de login
void login_response(char *response);

/*afiseaza utilizatorului un raspuns corespunzator raspunsului serverului
dupa cererea de acces la biblioteca. Returneaza:
  0 pt succes
 -1 pt eroare*/
int access_response(char *response);

/*afiseaza utilizatorului un raspuns corespunzator raspunsului serverului
dupa cererea de a afisa toate cartile
Afiseaza mesaj de eroare in caz ca utilizatorul nu este autorizat*/
void get_books_response(char *response);

/*afiseaza utilizatorului un raspuns corespunzator raspunsului serverului
dupa cererea de a afisa cartea cu id-ul cerut
Afiseaza mesaj de eroare in caz ca utilizatorul nu este autorizat sau 
daca nu exista cartea cu id-ul dat*/
void get_book_id_response(char *response);

/*afiseaza utilizatorului un raspuns corespunzator raspunsului serverului
dupa cererea de a adauga o carte
Afiseaza mesaj de eroare in caz ca utilizatorul nu este autorizat*/
void add_book_response(char *response);

/*afiseaza utilizatorului un raspuns corespunzator raspunsului serverului
dupa cererea de a sterge cartea cu id-ul cerut
Afiseaza mesaj de eroare in caz ca utilizatorul nu este autorizat sau 
daca nu exista cartea cu id-ul dat*/
void delete_book_response(char *response);

/*afiseaza utilizatorului un raspuns corespunzator raspunsului serverului
dupa cererea de logout. Returneaza:
  0 pt succes
 -1 pt eroare*/
int logout_response(char *response);

#endif
