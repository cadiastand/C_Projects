#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_STRLEN 20
#define INIT_MALLOC_SIZE 64
const char T9alpha[10][4] = {
    {' ', ' ', ' ', ' '},
    {'\00', '\00', '\00', '\00'},
    {'A', 'B', 'C', '\00'},
    {'D', 'E', 'F', '\00'},
    {'G', 'H', 'I', '\00'},
    {'J', 'K', 'L', '\00'},
    {'M', 'N', 'O', '\00'},
    {'P', 'Q', 'R', 'S'},
    {'T', 'U', 'V', '\00'},
    {'W', 'X', 'Y', 'Z'}
};

const char T9alpha_lowercase[10][4] = {
    {'\xFF', '\xFF', '\xFF', '\xFF'},
    {' ', ' ', ' ', ' '},
    {'a', 'b', 'c', '\xFF'},
    {'d', 'e', 'f', '\xFF'},
    {'g', 'h', 'i', '\xFF'},
    {'j', 'k', 'l', '\xFF'},
    {'m', 'n', 'o', '\xFF'},
    {'p', 'q', 'r', 's'},
    {'t', 'u', 'v', '\xFF'},
    {'w', 'x', 'y', 'z'}
};

typedef struct {
    char* name;
    char* number; // 20 chars +1 NULL byte
} contact;

typedef struct {
    size_t allocated;
    size_t used;
    contact* mem;
} contact_alloc;

contact_alloc* contact_list_init(void) {

    contact_alloc* allocator = (contact_alloc*)malloc(sizeof(contact_alloc));
    if (allocator == NULL)
        return NULL;

    if ((allocator->mem = (contact*)malloc(INIT_MALLOC_SIZE*sizeof(contact_alloc))) == NULL)
        return NULL; //Not enough memory

    allocator->allocated = INIT_MALLOC_SIZE;
    allocator->used = 0;    

    return allocator;

}

void contact_list_destroy(contact_alloc* cts) {
    if (cts == NULL)
        return;

    if (cts->mem != NULL) {
        for (size_t i = 0; i < cts->used / sizeof(contact); i++) {
            if (cts->mem[i].name != NULL) free(cts->mem[i].name);
            if (cts->mem[i].number != NULL) free(cts->mem[i].number);
        }
        free(cts->mem);
    }
    free(cts);
}

bool starts_withT9(char* name, char* num) {

    for (int i = 0; i < NUM_STRLEN && num[i] != '\00'; i++) {

        if (name[i] == '\00')
            return false;

        int indx = num[i] - '0';
        
        bool has_matched = false;
        for (int j = 0; j < 4; j++) {
            if (T9alpha[indx][j] == name[i] || T9alpha_lowercase[indx][j] == name[i]) {
                
                has_matched = true;
                break;
            }
        }
        if (!has_matched) return false;
    }

    return true;
}

bool starts_with(char* num_ct, char* num) {

    for (int i = 0; i < NUM_STRLEN && num[i] != '\00'; i++) {

        if (num_ct[i] == '\00')
            return false;

        if (num_ct[i] != num[i])
            return false;
    }
    return true;
}

bool contact_exists(contact_alloc* cts, char* name, char* num) {

    for (size_t i = 0; i < (cts->used / sizeof(contact)); i++) {

        if (strcmp(cts->mem[i].name, name) == 0 && strcmp(cts->mem[i].number, num) == 0)
           return true;
    }
    return false;
}

int create_contact(contact_alloc* cts, char* name, char* num) {

    if (cts->used + sizeof(contact) > cts->allocated) {

        /* Do a reallocation */
        contact* realloc_mem;
        if((realloc_mem = (contact*)realloc(cts->mem, 2 * cts->allocated)) == NULL)
            return -1;

        cts->mem = realloc_mem;
        cts->allocated = 2 * cts->allocated;
    }

    char* new_num = (char*)malloc((strlen(num) + 1) * sizeof(char));
    char* new_name = (char*)malloc((strlen(name) + 1) * sizeof(char));

    strcpy(new_num, num);
    strcpy(new_name, name);

    contact ct;
    ct.name = new_name;
    ct.number = new_num;

    /* Copy out data */
    memcpy((void*)((size_t)cts->mem + cts->used), &ct, sizeof(contact));
    cts->used += sizeof(contact);

    return 0;
}

void find(contact_alloc* cts, char* num) {

    assert(num);
    assert(cts);
    contact_alloc* outs = contact_list_init();

    size_t matches = 0;
    for (size_t i = 0; i < (cts->used / sizeof(contact)); i++) {

        if (starts_with(cts->mem[i].number, num) || starts_withT9(cts->mem[i].name, num)) {
            create_contact(outs, cts->mem[i].name, cts->mem[i].number);
            matches += 1;
        }
    }

    if (matches <= 10) {
        for (size_t i = 0; i < (outs->used / sizeof(contact)); i++) 
            printf("%s %s\n", outs->mem[i].number, outs->mem[i].name);
    }

    printf("Celkem: %zu\n", matches);
    contact_list_destroy(outs);
}

char* str_readln(void) {

    size_t alloc = INIT_MALLOC_SIZE;
    size_t str_len = 0;

    char* str  = NULL;
    str = (char*)malloc(INIT_MALLOC_SIZE * sizeof(char));
    memset(str, 0x00, INIT_MALLOC_SIZE * sizeof(char));

    if (str == NULL)
        return NULL;
 
    while (true) {

        char c;
        if (scanf("%c", &c) < 1)
            break;

        str_len++;
        if (c == '\n') 
            break; 

        str[str_len - 1] = c;
        
        if (str_len == alloc - 1) { //So we'll always have at least one byte left

            alloc = alloc * 2;
            
            char* new_str = (char*)realloc(str, alloc * sizeof(char));
            assert(new_str);
            memset((void*)((size_t)new_str + str_len), 0x00, alloc-str_len);

            str = new_str;
        }
    }
    
    if (str_len == 0) {
        free(str);
        return NULL;
    }

    str[str_len] = '\00'; //No need to increase len, as the rest of the shifting algo doesn't care about null byte
    return str;
}

char* parse_num(char* line, size_t offset, size_t len) {

    char* num_start = (char*)((size_t)line + offset);
    char* num = (char*)malloc((len + 1) * sizeof(char));
    assert(num);

    memset(num, 0x00, (len + 1) * sizeof(char));

    if (len == 0) { 
        free(num); 
        return NULL;
    }

    //Alphabet check
    for (size_t i = 0; i < len; i++) {

        if (num_start[i] < '0' || num_start[i] > '9') {
            free(num);
            return NULL;
        }

        num[i] = num_start[i];
    }

    return num;
}

char* parse_name(char* line, size_t offset, size_t len) {

    char* str_start = (char*)((size_t)line + offset);

    char* str = (char*)malloc((len + 1) * sizeof(char));
    assert(str);

    memset(str, 0x00, (len + 1) * sizeof(char));

    if (len == 0) { 
        free(str); 
        return NULL;
    }

    //Handle spaces
    if (str_start[0] == ' ' || str_start[len - 1] == ' ') {
        free(str);
        return NULL;
    }

    //Alphabet check
    for (size_t i = 0; i < len; i++) {

        if (!(str_start[i] == ' ' || (str_start[i] >= 'A' && str_start[i] <= 'Z') || (str_start[i] >= 'a' && str_start[i] <= 'z'))) {
            free(str);
            return NULL;
        }

        str[i] = str_start[i];
    }

    return str;
}

int str_readcmd(char* cmd, char** str, char** num) {
    
    assert(cmd);
    assert(str);
    assert(num);

    *cmd = '\00';
    *str = NULL;
    *num = NULL;

    char* line = str_readln();
    if (line == NULL) return 1;
    *cmd = *line;

    size_t num_len = 0, name_len = 0;
    size_t num_offset = 0, name_offset = 0;
    size_t counter = 0;

    //Get lens
    for (size_t i = 0; i < strlen(line); i++) {

        if (line[i] == '\n' || line[i] == EOF || line[i] == '\00')
            break;

        if (line[i] == ' ') {
            counter += 1;

            if (counter == 1) num_offset = i + 1;
            if (counter == 2) name_offset = i + 1;
            if (counter > 2) name_len += 1;
            continue;
        }

        if (counter == 1) num_len += 1;
        if (counter >= 2) name_len += 1;
    }

    if (strlen(line) < 1) {
        free(line);
        return 1;
    }

    *num = parse_num(line, num_offset, num_len);
    *str = parse_name(line, name_offset, name_len);
    free(line);

    if (num_len == 0 && name_len == 0)
        return -1;

    if (*cmd == '?' && counter >= 2) return -5;

    if (*cmd != '?' && num_len > 20) return -3; 

    if (*num == NULL) return -2;
    if (*cmd != '?' && *str == NULL) return -4;
    if (*cmd == '?' && *str != NULL) return -1;

    return 0;
}

int main(void) {

    char c;
    char* str = NULL;
    char* num = NULL;

    
    contact_alloc* cts = contact_list_init();
    
    while(true) {
        int res = str_readcmd(&c, &str, &num);
        if (res < 0) {
            printf("Nespravny vstup.\n");
           

            if (str != NULL) free(str);
            if (num != NULL) free(num);
            continue;
        }

        if (res > 0)
            break; //Reached EOF

        if (c == '+') {
            if (!contact_exists(cts, str, num)) {
                create_contact(cts, str, num);
                printf("OK\n");
            }
            else
                printf("Kontakt jiz existuje.\n");
        }
        else if (c == '?') {
           find(cts, num);
        }
        else {
            printf("Nespravny vstup.\n");
        }
        
        if (str != NULL) free(str);
        if (num != NULL) free(num);
    }

    contact_list_destroy(cts);
    return 0;
}
