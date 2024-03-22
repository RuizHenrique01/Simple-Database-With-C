#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    StatementType type;
} Statement;

typedef struct
{
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

InputBuffer *new_input_buffer()
{
    InputBuffer *inputBuffer = (InputBuffer *)malloc(sizeof(InputBuffer));
    inputBuffer->buffer = NULL;
    inputBuffer->buffer_length = 0;
    inputBuffer->input_length = 0;
    return inputBuffer;
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer *input_buffer)
{
    ssize_t bytes_read =
        getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if (bytes_read <= 0)
    {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer *inputBuffer)
{
    free(inputBuffer->buffer);
    free(inputBuffer);
}

MetaCommandResult do_meta_command(InputBuffer* inputBuffer){
    if(strcmp(inputBuffer->buffer, ".exit") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* Statement){
    if(strncmp(input_buffer->buffer, "insert", 6) == 0) {
        Statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if(strcmp(input_buffer->buffer, "select") == 0){
        Statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* Statement){
    switch (Statement->type)
    {
    case (STATEMENT_INSERT):
        printf("Inserting data into the database!\n");
        break;
    case (STATEMENT_SELECT):
        printf("Selecting data from the database!\n");
    
    default:
        break;
    }
}

int main()
{
    InputBuffer *input_buffer = new_input_buffer();
    while (1)
    {
        print_prompt();
        read_input(input_buffer);

        if(input_buffer->buffer[0] == '.'){
            switch(do_meta_command(input_buffer)){
                case(META_COMMAND_SUCCESS):
                    continue;
                case(META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }

        Statement statement;
        switch(prepare_statement(input_buffer, &statement)){
            case(PREPARE_SUCCESS):
                break;
            case(PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
                continue;
        }

        execute_statement(&statement);
        printf("Executed.\n");
    }
}