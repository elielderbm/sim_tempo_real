#include "weather.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(1);
    if (s->ptr) s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) return 0;
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;
    return size * nmemb;
}

float obter_temperatura_sao_paulo() {
    CURL *curl = curl_easy_init();
    struct string response;
    float temperatura = -100.0;

    if (curl) {
        init_string(&response);
        const char *API_KEY = "YOUR_API_KEY";  // <-- Coloque sua API aqui
        const char *URL_FMT = "https://api.openweathermap.org/data/2.5/weather?q=Sao%20Paulo,BR&appid=%s&units=metric";

        char url[512];
        snprintf(url, sizeof(url), URL_FMT, API_KEY);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            char *temp_str = strstr(response.ptr, "\"temp\":");
            if (temp_str) {
                sscanf(temp_str, "\"temp\":%f", &temperatura);
            }
        }

        free(response.ptr);
        curl_easy_cleanup(curl);
    }

    return temperatura;
}
