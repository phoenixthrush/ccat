#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <unistd.h>
#include <sys/stat.h>

#define URL_FORMAT "https://api.thecatapi.com/v1/images/search?mime_types=jpg,png,gif&limit=1"
#define MAX_URL_LENGTH 512

void progress_bar(double progress) {
    int bar_width = 50;
    int pos = bar_width * progress;
    printf("< ");
    for (int i = 0; i < bar_width; ++i) {
        if (i < pos) printf("🐈");
        else printf(" ");
    }
    printf(" > %3.0f%%\r", progress * 100);
    fflush(stdout);
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

int progress_func(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    if (dltotal > 0) {
        double progress = (double)dlnow / (double)dltotal;
        progress_bar(progress);
    }
    return 0;
}

struct memory {
    char *response;
    size_t size;
};

size_t write_json_response(void *ptr, size_t size, size_t nmemb, void *data) {
    size_t real_size = size * nmemb;
    struct memory *mem = (struct memory *)data;

    char *temp = realloc(mem->response, mem->size + real_size + 1);
    if (temp == NULL) {
        fprintf(stderr, "[ERROR] Failed to allocate memory for JSON response.\n");
        return 0;
    }
    mem->response = temp;

    memcpy(&(mem->response[mem->size]), ptr, real_size);
    mem->size += real_size;
    mem->response[mem->size] = '\0';

    return real_size;
}

int download_cat_image(const char *url, const char *output_path) {
    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        fp = fopen(output_path, "wb");
        if (!fp) {
            perror("[ERROR] Error opening file");
            return 1;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_func);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, NULL);
        
        printf("\n🐱 Pirating: %s\n", output_path);

        res = curl_easy_perform(curl);
        fclose(fp);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            return 1;
        }

        curl_easy_cleanup(curl);
        printf("\n");
        return 0;
    }
    return 1;
}

int get_cat_image_url(char *image_url) {
    CURL *curl; CURLcode res;
    struct memory json_data; struct json_object *parsed_json, *url_json; struct json_object *first_element;
    json_data.response = malloc(1); json_data.size = 0;
    curl = curl_easy_init();
    
    if (!curl) return 1;
    curl_easy_setopt(curl, CURLOPT_URL, URL_FORMAT); curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_json_response);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&json_data); curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    res = curl_easy_perform(curl); curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        free(json_data.response);
        return 1;
    }

    parsed_json = json_tokener_parse(json_data.response);
    if (!parsed_json) {
        fprintf(stderr, "Failed to parse JSON response.\n");
        free(json_data.response);
        return 1;
    }

    if (json_object_is_type(parsed_json, json_type_array)) {
        first_element = json_object_array_get_idx(parsed_json, 0);
        json_object_object_get_ex(first_element, "url", &url_json);
        strcpy(image_url, json_object_get_string(url_json));
    } else {
        fprintf(stderr, "Unexpected JSON format.\n");
        free(json_data.response);
        return 1;
    }

    free(json_data.response);  // Free memory
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("[Pro Tip] Usage: %s <number_of_images> <output_directory>\n", argv[0]);
        return 1;
    }

    int num_images = atoi(argv[1]);
    char *output_dir = argv[2];
    char image_url[MAX_URL_LENGTH];
    char output_file[MAX_URL_LENGTH];

    if (access(output_dir, F_OK) != 0) {
        if (mkdir(output_dir, 0755) != 0) {
            perror("[ERROR] Error creating directory");
            return 1;
        }
    }

    CURLcode curl_global_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (curl_global_res != CURLE_OK) {
        fprintf(stderr, "curl_global_init() failed: %s\n", curl_easy_strerror(curl_global_res));
        return 1;
    }

    for (int i = 1; i <= num_images; i++) {
        if (get_cat_image_url(image_url)) {
            fprintf(stderr, "[ERROR] Oh no! Failed to fetch the cat image URL\n");
            continue;
        }

        snprintf(output_file, sizeof(output_file), "%s/meow_%d.jpg", output_dir, i);

        if (download_cat_image(image_url, output_file)) {
            fprintf(stderr, "Failed to pirate image: %s\n", output_file);
        }
    }

    printf("\n   |\\      _,,,---,,_\n   /,`.-'`'    -.  ;-;;,_\n  |,4-  ) )-,_..;\\ (  `'-'\n '---''(_/--'  `-'\\_)  \n\n");

    curl_global_cleanup();
    return 0;
}
