#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct social_Tag {
    char username[51];
    struct social_Tag* next_User;
    struct social_Tag* prev_User;
} social;

social* user_Head = NULL;
social* user_Tail = NULL;
uint32_t user_Quant = 0;


void add_User(const char* name) {
    social* new_User = (social*)malloc(sizeof(social));

    if (new_User == NULL) {
        printf("Erro ao alocar memoria para o novo usuario");
        return;
    }


    strcpy(new_User->username, name);
    new_User->next_User = NULL;
    new_User->prev_User = NULL;

    if (user_Head == NULL) {
        user_Head = new_User;
        user_Tail = new_User;
        new_User->next_User = new_User;
        user_Tail->prev_User = new_User;
    } else {
        new_User->next_User = user_Head;
        new_User->prev_User = user_Tail;
        user_Head->prev_User = new_User;
        user_Tail->next_User = new_User;
        user_Tail = new_User;
    }

    user_Quant++;
}

social* find_User(const char* user_Find) {
    if (user_Head == NULL) {
        return NULL;
    }
    social* actual_User = user_Head;

    do {
        if (strcmp(actual_User->username, user_Find) == 0) {return actual_User;}
        actual_User = actual_User->next_User;
    } while (actual_User != user_Head);
    return NULL;
}

void remove_User(const char* user_Remove) {
    social* user_Found = find_User(user_Remove);

    if (user_Found == NULL) {
        return;
    }

    social* next_Found = user_Found->next_User;
    social* prev_Found = user_Found->prev_User;

    if (user_Found->next_User == user_Found) {
        user_Head = NULL;
        user_Tail = NULL;
    } else {
        prev_Found->next_User = next_Found;
        next_Found->prev_User = prev_Found;
        if (user_Found == user_Head) {
            user_Head = next_Found;
        } else if (user_Found == user_Tail) {
            user_Tail = prev_Found;
        }
    }
    free(user_Found);
    user_Quant--;
}

int main( int argc, char* argv[] ) {
    FILE* input_File = fopen(argv[1], "r");
    FILE* output_File = fopen(argv[2], "w");

    char buffer[256];

    if (input_File == NULL){
        printf("Error opening file\n");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), input_File) != NULL) {
        char *action = strtok(buffer, " \n\r\t");
        char *username = strtok(NULL, "");

        if (action == NULL || username == NULL){
            continue;
        }
        char* registred_Action = strdup(action);
        char* registred_Username = strdup(username);

        if (registred_Action == NULL || registred_Username == NULL){
            free(registred_Action);
            free(registred_Username);
            continue;
        }

        registred_Username[strcspn(registred_Username, "\n\r")] = '\0';

        for (uint32_t i = 0; i < strlen(registred_Username); i++) {
            if (registred_Username[i] == ' ') {
                registred_Username[i] = '_';
            }
        }

        social* user_Founded = find_User(registred_Username);
        if (strcmp(registred_Action, "ADD") == 0) {
            if (user_Founded != NULL) {
                fprintf(output_File, "[FAILURE]ADD=%s\n", registred_Username);
                free(registred_Username);
                free(registred_Action);
                continue;
            }

            add_User(registred_Username);
            fprintf(output_File, "[SUCCESS]ADD=%s\n", registred_Username);
        }
        else if (strcmp(registred_Action, "REMOVE") == 0) {
            if (user_Founded != NULL) {
                remove_User(registred_Username);
                fprintf(output_File, "[SUCCESS]REMOVE=%s\n", registred_Username);
                free(registred_Username);
                free(registred_Action);
                continue;
            }

            fprintf(output_File, "[FAILURE]REMOVE=%s\n", registred_Username);
        }
        else if (strcmp(registred_Action, "SHOW") == 0) {
            if (user_Founded == NULL) {
                fprintf(output_File, "[FAILURE]SHOW=?<-%s->?\n", registred_Username);
                free(registred_Username);
                free(registred_Action);
                continue;
            }
            fprintf(output_File, "[SUCCESS]SHOW=%s<-%s->%s\n", user_Founded->prev_User->username,user_Founded->username, user_Founded->next_User->username);
        }

        free(registred_Action);
        free(registred_Username);


    }
    fclose(input_File);
    fclose(output_File);
        printf("Processamento do arquivo concluído com sucesso!\n");
    return 0;
}