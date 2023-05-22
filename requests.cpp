#include "helpers.hpp"

char *compute_get_request(const char *host, const char *url, char *query_params,
                            const string &cookies, const string &jwt)
{
    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line = (char *) calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (!jwt.empty()) {
        memset(line, 0, LINELEN);
        sprintf(line, "Authorization: Bearer %s", jwt.c_str());
        compute_message(message, line);
    }

    if (!cookies.empty()) {
        memset(line, 0, LINELEN);
        sprintf(line, "Cookie: %s", cookies.c_str());
        compute_message(message, line);
    }

    // Step 4: add final new line
    compute_message(message, "");

    free(line);

    return message;
}

char *compute_post_request(const char *host, const char *url, const char *content_type, 
                            const string &body_data, const string &cookies, const string &jwt)
{
    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line = (char *) calloc(LINELEN, sizeof(char));
    char *body_data_buffer = (char *) calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    if(!body_data.empty()) {
        memset(line, 0, LINELEN);
        sprintf(line, "Content-Type: %s", content_type);
        compute_message(message, line);

        memset(line, 0, LINELEN);
        sprintf(line, "Content-Length: %ld", body_data.length());
        compute_message(message, line);

        strcat(body_data_buffer, body_data.c_str());
    }

    //adaugare jwt daca exista in headerul Authorization
    if (!jwt.empty()) {
        memset(line, 0, LINELEN);
        sprintf(line, "Authorization: Bearer %s", jwt.c_str());
        compute_message(message, line);
    }

    // Step 4 (optional): add cookies
    if (!cookies.empty()) {
        memset(line, 0, LINELEN);
        sprintf(line, "Cookie: %s", cookies.c_str());
        compute_message(message, line);
    }
    // Step 5: add new line at end of header
    compute_message(message, "");

    // Step 6: add the actual payload data
    compute_message(message, body_data_buffer);

    free(line);
    free(body_data_buffer);
    return message;
}

char *compute_delete_request(const char *host, const char *url, char *query_params,
                            const string &cookies, const string &jwt)
{
    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line = (char *) calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL) {
        sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (!jwt.empty()) {
        memset(line, 0, LINELEN);
        sprintf(line, "Authorization: Bearer %s", jwt.c_str());
        compute_message(message, line);
    }

    if (!cookies.empty()) {
        memset(line, 0, LINELEN);
        sprintf(line, "Cookie: %s", cookies.c_str());
        compute_message(message, line);
    }

    // Step 4: add final new line
    compute_message(message, "");

    free(line);

    return message;
}
