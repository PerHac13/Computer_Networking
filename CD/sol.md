I'll provide solutions for all the compiler design lab questions in a modular form. I'll tackle each problem with a clear structure, providing the necessary C code and explanations.

## Question 1: C program to generate three address code

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EXPR_SIZE 100
#define MAX_TEMP 20

// Structure to store the three address code
typedef struct {
    char op;           // Operator
    char arg1[10];     // Argument 1
    char arg2[10];     // Argument 2
    char result[10];   // Result
} ThreeAddressCode;

int temp_count = 0;

// Function to generate a new temporary variable
void new_temp(char *temp) {
    sprintf(temp, "t%d", temp_count++);
}

// Function to generate three address code for an expression
int generate_code(char *expr, ThreeAddressCode code[]) {
    char stack[MAX_EXPR_SIZE];
    int top = -1;
    char temp1[10], temp2[10], temp3[10];
    int i, j = 0;

    for (i = 0; expr[i] != '\0'; i++) {
        if (isalnum(expr[i])) {
            stack[++top] = expr[i];
        } else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            // Pop top two operands from stack
            temp2[0] = stack[top--];
            temp2[1] = '\0';

            temp1[0] = stack[top--];
            temp1[1] = '\0';

            // Generate three address code
            new_temp(temp3);
            code[j].op = expr[i];
            strcpy(code[j].arg1, temp1);
            strcpy(code[j].arg2, temp2);
            strcpy(code[j].result, temp3);
            j++;

            // Push the result back to stack
            stack[++top] = 't';
            stack[top+1] = '\0';
        }
    }

    return j;
}

// Function to display the three address code
void display_code(ThreeAddressCode code[], int n) {
    int i;
    printf("Three Address Code:\n");
    for (i = 0; i < n; i++) {
        printf("%s = %s %c %s\n", code[i].result, code[i].arg1, code[i].op, code[i].arg2);
    }
}

int main() {
    char expr[MAX_EXPR_SIZE];
    ThreeAddressCode code[MAX_EXPR_SIZE];
    int code_lines;

    printf("Enter a postfix expression: ");
    scanf("%s", expr);

    code_lines = generate_code(expr, code);
    display_code(code, code_lines);

    return 0;
}

```

## Question 2: C Program to implement NFAs for identifiers, constants, and operators

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to check if the given string is an identifier
int is_identifier(char *str) {
    // NFA implementation for identifiers
    // Rule: Must start with a letter or underscore, followed by letters, digits, or underscores
    int state = 0;
    int i;

    for (i = 0; str[i] != '\0'; i++) {
        switch (state) {
            case 0:
                if (isalpha(str[i]) || str[i] == '_')
                    state = 1;
                else
                    return 0;  // Invalid identifier
                break;
            case 1:
                if (isalnum(str[i]) || str[i] == '_')
                    state = 1;
                else
                    return 0;  // Invalid identifier
                break;
        }
    }

    return (state == 1);  // Final state must be 1 for a valid identifier
}

// Function to check if the given string is a constant
int is_constant(char *str) {
    // NFA implementation for constants (integer and floating-point)
    int state = 0;
    int i;

    for (i = 0; str[i] != '\0'; i++) {
        switch (state) {
            case 0:
                if (isdigit(str[i]))
                    state = 1;
                else
                    return 0;  // Invalid constant
                break;
            case 1:
                if (isdigit(str[i]))
                    state = 1;
                else if (str[i] == '.')
                    state = 2;
                else
                    return 0;  // Invalid constant
                break;
            case 2:
                if (isdigit(str[i]))
                    state = 3;
                else
                    return 0;  // Invalid constant
                break;
            case 3:
                if (isdigit(str[i]))
                    state = 3;
                else
                    return 0;  // Invalid constant
                break;
        }
    }

    return (state == 1 || state == 3);  // Final state must be 1 or 3 for a valid constant
}

// Function to check if the given string is an operator
int is_operator(char *str) {
    // NFA implementation for operators
    char operators[][5] = {"+", "-", "*", "/", "=", "==", "<", ">", "<=", ">=", "!="};
    int num_operators = 11;

    for (int i = 0; i < num_operators; i++) {
        if (strcmp(str, operators[i]) == 0)
            return 1;  // Valid operator
    }

    return 0;  // Invalid operator
}

int main() {
    char input[100];

    printf("Enter a string: ");
    scanf("%s", input);

    if (is_identifier(input))
        printf("%s is an identifier\n", input);
    else if (is_constant(input))
        printf("%s is a constant\n", input);
    else if (is_operator(input))
        printf("%s is an operator\n", input);
    else
        printf("%s is not recognized\n", input);

    return 0;
}

```

## Question 3: C Program to Scan and Count characters, words, and lines in a file

```c
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void count_file_stats(FILE *file, int *chars, int *words, int *lines) {
    int ch;
    int in_word = 0;

    *chars = 0;
    *words = 0;
    *lines = 0;

    while ((ch = fgetc(file)) != EOF) {
        (*chars)++;

        // Count lines
        if (ch == '\n')
            (*lines)++;

        // Count words
        if (isspace(ch)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            (*words)++;
        }
    }

    // If file doesn't end with a newline, increment line count
    if (*chars > 0 && ch != '\n')
        (*lines)++;
}

int main() {
    FILE *file;
    char filename[100];
    int char_count, word_count, line_count;

    printf("Enter the filename: ");
    scanf("%s", filename);

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    count_file_stats(file, &char_count, &word_count, &line_count);

    printf("Characters: %d\n", char_count);
    printf("Words: %d\n", word_count);
    printf("Lines: %d\n", line_count);

    fclose(file);
    return 0;
}

```

## Question 4: C Program to implement DFAs for identifiers, constants, and operators

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to check if the given string is an identifier using DFA
int is_identifier_dfa(char *str) {
    // DFA states:
    // 0: Start state
    // 1: Valid identifier state
    // 2: Dead state (invalid)
    int state = 0;
    int i = 0;

    while (str[i] != '\0') {
        char ch = str[i];

        switch (state) {
            case 0:
                if (isalpha(ch) || ch == '_')
                    state = 1;
                else
                    state = 2;
                break;
            case 1:
                if (isalnum(ch) || ch == '_')
                    state = 1;
                else
                    state = 2;
                break;
            case 2:
                // Once in dead state, stay there
                break;
        }

        if (state == 2)
            break;  // No need to continue if already in dead state

        i++;
    }

    return (state == 1);  // Accept if in final state 1
}

// Function to check if the given string is a constant (integer or float) using DFA
int is_constant_dfa(char *str) {
    // DFA states:
    // 0: Start state
    // 1: Valid integer state
    // 2: Decimal point seen
    // 3: Valid float state (after decimal digits)
    // 4: Dead state (invalid)
    int state = 0;
    int i = 0;

    while (str[i] != '\0') {
        char ch = str[i];

        switch (state) {
            case 0:
                if (isdigit(ch))
                    state = 1;
                else
                    state = 4;
                break;
            case 1:
                if (isdigit(ch))
                    state = 1;
                else if (ch == '.')
                    state = 2;
                else
                    state = 4;
                break;
            case 2:
                if (isdigit(ch))
                    state = 3;
                else
                    state = 4;
                break;
            case 3:
                if (isdigit(ch))
                    state = 3;
                else
                    state = 4;
                break;
            case 4:
                // Once in dead state, stay there
                break;
        }

        if (state == 4)
            break;  // No need to continue if already in dead state

        i++;
    }

    return (state == 1 || state == 3);  // Accept if in final states 1 or 3
}

// Function to check if the given string is an operator using DFA
int is_operator_dfa(char *str) {
    // Simple DFA for operators
    // Valid operators: +, -, *, /, =, ==, <, >, <=, >=, !=
    if (strcmp(str, "+") == 0 || strcmp(str, "-") == 0 ||
        strcmp(str, "*") == 0 || strcmp(str, "/") == 0 ||
        strcmp(str, "=") == 0)
        return 1;

    if (strcmp(str, "==") == 0 || strcmp(str, "<") == 0 ||
        strcmp(str, ">") == 0 || strcmp(str, "<=") == 0 ||
        strcmp(str, ">=") == 0 || strcmp(str, "!=") == 0)
        return 1;

    return 0;
}

int main() {
    char input[100];

    printf("Enter a string: ");
    scanf("%s", input);

    if (is_identifier_dfa(input))
        printf("%s is an identifier\n", input);
    else if (is_constant_dfa(input))
        printf("%s is a constant\n", input);
    else if (is_operator_dfa(input))
        printf("%s is an operator\n", input);
    else
        printf("%s is not recognized\n", input);

    return 0;
}

```

## Question 5: Program for recognizing a valid variable

```c
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Function to check if a variable name is valid
// Valid variables start with a letter followed by any number of letters/digits
int is_valid_variable(char *var) {
    int i = 0;

    // Check if first character is a letter
    if (!isalpha(var[0]))
        return 0;  // Invalid - first character must be a letter

    // Check the rest of the characters (must be letters or digits)
    for (i = 1; var[i] != '\0'; i++) {
        if (!isalnum(var[i]))
            return 0;  // Invalid - contains non-alphanumeric character
    }

    return 1;  // Valid variable
}

int main() {
    char variable[100];

    printf("Enter a variable name: ");
    scanf("%s", variable);

    if (is_valid_variable(variable))
        printf("%s is a valid variable name.\n", variable);
    else
        printf("%s is not a valid variable name.\n", variable);

    return 0;
}

```

## Question 6: Program for Code Optimization Technique

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CODE_LINES 100
#define MAX_LINE_LENGTH 200

typedef struct {
    char op;
    char arg1[20];
    char arg2[20];
    char result[20];
} ThreeAddressCode;

// Parse a line of three address code
int parse_three_address_code(char *line, ThreeAddressCode *code) {
    char *result, *arg1, *arg2;
    char *op_pos;

    // Format: result = arg1 op arg2
    // or: result = op arg1 (unary)

    // Get result
    result = strtok(line, "=");
    if (result == NULL) return 0;

    // Remove leading/trailing spaces
    while (isspace(*result)) result++;
    strcpy(code->result, result);

    // Get the rest
    char *rest = strtok(NULL, "");
    if (rest == NULL) return 0;

    // Check for operation
    if (strstr(rest, "+")) {
        code->op = '+';
        op_pos = strstr(rest, "+");
    } else if (strstr(rest, "-")) {
        code->op = '-';
        op_pos = strstr(rest, "-");
    } else if (strstr(rest, "*")) {
        code->op = '*';
        op_pos = strstr(rest, "*");
    } else if (strstr(rest, "/")) {
        code->op = '/';
        op_pos = strstr(rest, "/");
    } else {
        // Handle assignments or unary operations
        char *temp = rest;
        while (isspace(*temp)) temp++;
        strcpy(code->arg1, temp);
        code->op = '=';
        code->arg2[0] = '\0';
        return 1;
    }

    // Get arg1
    arg1 = rest;
    while (isspace(*arg1)) arg1++;

    // Get arg2
    arg2 = op_pos + 1;
    while (isspace(*arg2)) arg2++;

    // Null-terminate arg1
    *op_pos = '\0';
    op_pos--;
    while (isspace(*op_pos) && op_pos >= arg1) {
        *op_pos = '\0';
        op_pos--;
    }

    strcpy(code->arg1, arg1);
    strcpy(code->arg2, arg2);

    return 1;
}

// Constant folding optimization
void constant_folding(ThreeAddressCode *code) {
    // Check if both operands are constants
    if (isdigit(code->arg1[0]) && isdigit(code->arg2[0])) {
        int val1 = atoi(code->arg1);
        int val2 = atoi(code->arg2);
        int result;

        // Perform operation
        switch (code->op) {
            case '+': result = val1 + val2; break;
            case '-': result = val1 - val2; break;
            case '*': result = val1 * val2; break;
            case '/':
                if (val2 != 0)
                    result = val1 / val2;
                else
                    return; // Avoid division by zero
                break;
            default: return; // Unsupported operation
        }

        // Update code to direct assignment
        code->op = '=';
        sprintf(code->arg1, "%d", result);
        code->arg2[0] = '\0';
    }
}

// Common subexpression elimination
int common_subexpression_elimination(ThreeAddressCode *codes, int n) {
    int i, j;
    int eliminated = 0;

    for (i = 0; i < n; i++) {
        if (codes[i].op == '=') continue; // Skip simple assignments

        for (j = i + 1; j < n; j++) {
            if (codes[j].op == codes[i].op &&
                strcmp(codes[j].arg1, codes[i].arg1) == 0 &&
                strcmp(codes[j].arg2, codes[i].arg2) == 0) {
                // Found common subexpression
                codes[j].op = '=';
                strcpy(codes[j].arg1, codes[i].result);
                codes[j].arg2[0] = '\0';
                eliminated++;
            }
        }
    }

    return eliminated;
}

// Print the three address code
void print_three_address_code(ThreeAddressCode *code) {
    if (code->op == '=') {
        printf("%s = %s\n", code->result, code->arg1);
    } else {
        printf("%s = %s %c %s\n", code->result, code->arg1, code->op, code->arg2);
    }
}

int main() {
    char input_lines[MAX_CODE_LINES][MAX_LINE_LENGTH];
    ThreeAddressCode codes[MAX_CODE_LINES];
    int num_lines = 0;

    printf("Enter three address code (empty line to end):\n");

    // Read input
    while (1) {
        char line[MAX_LINE_LENGTH];
        fgets(line, MAX_LINE_LENGTH, stdin);

        // Remove newline
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) break;

        strcpy(input_lines[num_lines], line);
        num_lines++;

        if (num_lines >= MAX_CODE_LINES) break;
    }

    // Parse three address code
    for (int i = 0; i < num_lines; i++) {
        parse_three_address_code(input_lines[i], &codes[i]);
    }

    printf("\nOriginal code:\n");
    for (int i = 0; i < num_lines; i++) {
        print_three_address_code(&codes[i]);
    }

    // Apply constant folding
    for (int i = 0; i < num_lines; i++) {
        constant_folding(&codes[i]);
    }

    // Apply common subexpression elimination
    int eliminated = common_subexpression_elimination(codes, num_lines);

    printf("\nOptimized code:\n");
    for (int i = 0; i < num_lines; i++) {
        print_three_address_code(&codes[i]);
    }

    printf("\nOptimizations performed:\n");
    printf("- Constant folding\n");
    printf("- Common subexpression elimination (%d eliminated)\n", eliminated);

    return 0;
}

```

## Question 7: Backend compiler for 8086 assembly from three-address code

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CODE_LINES 100
#define MAX_LINE_LENGTH 200
#define MAX_SYMBOLS 100

typedef struct {
    char op;            // Operator (+, -, *, /, =)
    char arg1[20];      // First operand
    char arg2[20];      // Second operand
    char result[20];    // Result
} ThreeAddressCode;

typedef struct {
    char name[20];      // Symbol name
    int address;        // Memory address (offset from BP)
} Symbol;

Symbol symbol_table[MAX_SYMBOLS];
int symbol_count = 0;

// Find symbol in the symbol table or add it if not found
int get_symbol_address(char *name) {
    int i;

    // Search for the symbol
    for (i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return symbol_table[i].address;
        }
    }

    // Add new symbol
    strcpy(symbol_table[symbol_count].name, name);
    symbol_table[symbol_count].address = -2 * (symbol_count + 1);  // -2, -4, -6, ...
    symbol_count++;

    return symbol_table[symbol_count - 1].address;
}

// Parse a line of three address code
int parse_three_address_code(char *line, ThreeAddressCode *code) {
    char *result, *arg1, *arg2;
    char *op_pos;

    // Format: result = arg1 op arg2
    // or: result = op arg1 (unary)
    // or: result = arg1 (assignment)

    // Get result
    result = strtok(line, "=");
    if (result == NULL) return 0;

    // Remove leading/trailing spaces
    while (isspace(*result)) result++;
    strcpy(code->result, result);

    // Get the rest
    char *rest = strtok(NULL, "");
    if (rest == NULL) return 0;

    // Check for operation
    if (strstr(rest, "+")) {
        code->op = '+';
        op_pos = strstr(rest, "+");
    } else if (strstr(rest, "-")) {
        code->op = '-';
        op_pos = strstr(rest, "-");
    } else if (strstr(rest, "*")) {
        code->op = '*';
        op_pos = strstr(rest, "*");
    } else if (strstr(rest, "/")) {
        code->op = '/';
        op_pos = strstr(rest, "/");
    } else {
        // Handle assignments
        code->op = '=';
        while (isspace(*rest)) rest++;
        strcpy(code->arg1, rest);
        code->arg2[0] = '\0';
        return 1;
    }

    // Get arg1
    arg1 = rest;
    while (isspace(*arg1)) arg1++;

    // Get arg2
    arg2 = op_pos + 1;
    while (isspace(*arg2)) arg2++;

    // Null-terminate arg1
    *op_pos = '\0';
    op_pos--;
    while (isspace(*op_pos) && op_pos >= arg1) {
        *op_pos = '\0';
        op_pos--;
    }

    strcpy(code->arg1, arg1);
    strcpy(code->arg2, arg2);

    return 1;
}

// Generate 8086 assembly code for a three address code instruction
void generate_assembly_code(ThreeAddressCode *code, FILE *output) {
    int addr_result, addr_arg1, addr_arg2;

    // Get addresses for variables
    addr_result = get_symbol_address(code->result);

    // Handle different operations
    switch (code->op) {
        case '=':  // Assignment
            if (isdigit(code->arg1[0])) {
                // Assign a constant
                fprintf(output, "    MOV AX, %s\n", code->arg1);
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            } else {
                // Copy a variable
                addr_arg1 = get_symbol_address(code->arg1);
                fprintf(output, "    MOV AX, [BP%+d]\n", addr_arg1);
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            }
            break;

        case '+':  // Addition
            if (isdigit(code->arg1[0]) && isdigit(code->arg2[0])) {
                // Both are constants
                int val = atoi(code->arg1) + atoi(code->arg2);
                fprintf(output, "    MOV AX, %d\n", val);
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            } else if (isdigit(code->arg1[0])) {
                // arg1 is constant
                addr_arg2 = get_symbol_address(code->arg2);
                fprintf(output, "    MOV AX, [BP%+d]\n", addr_arg2);
                fprintf(output, "    ADD AX, %s\n", code->arg1);
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            } else if (isdigit(code->arg2[0])) {
                // arg2 is constant
                addr_arg1 = get_symbol_address(code->arg1);
                fprintf(output, "    MOV AX, [BP%+d]\n", addr_arg1);
                fprintf(output, "    ADD AX, %s\n", code->arg2);
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            } else {
                // Both are variables
                addr_arg1 = get_symbol_address(code->arg1);
                addr_arg2 = get_symbol_address(code->arg2);
                fprintf(output, "    MOV AX, [BP%+d]\n", addr_arg1);
                fprintf(output, "    ADD AX, [BP%+d]\n", addr_arg2);
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            }
            break;

        case '-':  // Subtraction
            if (isdigit(code->arg1[0]) && isdigit(code->arg2[0])) {
                // Both are constants
                int val = atoi(code->arg1) - atoi(code->arg2);
                fprintf(output, "    MOV AX, %d\n", val);
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            } else if (isdigit(code->arg1[0])) {
                // arg1 is constant
                addr_arg2 = get_symbol_address(code->arg2);
                fprintf(output, "    MOV AX, %s\n", code->arg1);
                fprintf(output, "    SUB AX, [BP%+d]\n", addr_arg2);
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            } else if (isdigit(code->arg2[0])) {
                // arg2 is constant
                addr_arg1 = get_symbol_address(code->arg1);
                fprintf(output, "    MOV AX, [BP%+d]\n", addr_arg1);
                fprintf(output, "    SUB AX, %s\n", code->arg2);
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            } else {
                // Both are variables
                addr_arg1 = get_symbol_address(code->arg1);
                addr_arg2 = get_symbol_address(code->arg2);
                fprintf(output, "    MOV AX, [BP%+d]\n", addr_arg1);
                fprintf(output, "    SUB AX, [BP%+d]\n", addr_arg2);
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            }
            break;

        case '*':  // Multiplication
            addr_arg1 = get_symbol_address(code->arg1);
            addr_arg2 = get_symbol_address(code->arg2);

            if (isdigit(code->arg1[0]) && isdigit(code->arg2[0])) {
                // Both are constants
                int val = atoi(code->arg1) * atoi(code->arg2);
                fprintf(output, "    MOV AX, %d\n", val);
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            } else if (isdigit(code->arg1[0])) {
                // arg1 is constant
                fprintf(output, "    MOV AX, [BP%+d]\n", addr_arg2);
                fprintf(output, "    MOV BX, %s\n", code->arg1);
                fprintf(output, "    IMUL BX\n");
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            } else if (isdigit(code->arg2[0])) {
                // arg2 is constant
                fprintf(output, "    MOV AX, [BP%+d]\n", addr_arg1);
                fprintf(output, "    MOV BX, %s\n", code->arg2);
                fprintf(output, "    IMUL BX\n");
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            } else {
                // Both are variables
                fprintf(output, "    MOV AX
```

Let me continue providing the solutions for the compiler design lab questions.

```c
// Both are variables
                fprintf(output, "    MOV AX, [BP%+d]\n", addr_arg1);
                fprintf(output, "    MOV BX, [BP%+d]\n", addr_arg2);
                fprintf(output, "    IMUL BX\n");
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            }
            break;

        case '/':  // Division
            addr_arg1 = get_symbol_address(code->arg1);
            addr_arg2 = get_symbol_address(code->arg2);

            if (isdigit(code->arg1[0]) && isdigit(code->arg2[0])) {
                // Both are constants
                int val = atoi(code->arg1) / atoi(code->arg2);
                fprintf(output, "    MOV AX, %d\n", val);
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            } else if (isdigit(code->arg1[0])) {
                // arg1 is constant
                fprintf(output, "    MOV AX, %s\n", code->arg1);
                fprintf(output, "    CWD\n");  // Sign extend AX into DX:AX
                fprintf(output, "    MOV BX, [BP%+d]\n", addr_arg2);
                fprintf(output, "    IDIV BX\n");
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            } else if (isdigit(code->arg2[0])) {
                // arg2 is constant
                fprintf(output, "    MOV AX, [BP%+d]\n", addr_arg1);
                fprintf(output, "    CWD\n");  // Sign extend AX into DX:AX
                fprintf(output, "    MOV BX, %s\n", code->arg2);
                fprintf(output, "    IDIV BX\n");
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            } else {
                // Both are variables
                fprintf(output, "    MOV AX, [BP%+d]\n", addr_arg1);
                fprintf(output, "    CWD\n");  // Sign extend AX into DX:AX
                fprintf(output, "    MOV BX, [BP%+d]\n", addr_arg2);
                fprintf(output, "    IDIV BX\n");
                fprintf(output, "    MOV [BP%+d], AX\n", addr_result);
            }
            break;
    }
}

int main() {
    char input_lines[MAX_CODE_LINES][MAX_LINE_LENGTH];
    ThreeAddressCode codes[MAX_CODE_LINES];
    int num_lines = 0;
    FILE *output_file;

    printf("Enter three address code (empty line to end):\n");

    // Read input
    while (1) {
        char line[MAX_LINE_LENGTH];
        fgets(line, MAX_LINE_LENGTH, stdin);

        // Remove newline
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) break;

        strcpy(input_lines[num_lines], line);
        num_lines++;

        if (num_lines >= MAX_CODE_LINES) break;
    }

    // Parse three address code
    for (int i = 0; i < num_lines; i++) {
        parse_three_address_code(input_lines[i], &codes[i]);
    }

    // Open output file
    output_file = fopen("output.asm", "w");
    if (output_file == NULL) {
        printf("Error: Unable to create output file.\n");
        return 1;
    }

    // Generate assembly code header
    fprintf(output_file, ".MODEL SMALL\n");
    fprintf(output_file, ".STACK 100h\n");
    fprintf(output_file, ".DATA\n");
    fprintf(output_file, ".CODE\n");
    fprintf(output_file, "MAIN PROC\n");
    fprintf(output_file, "    MOV AX, @DATA\n");
    fprintf(output_file, "    MOV DS, AX\n");
    fprintf(output_file, "    MOV BP, SP\n");
    fprintf(output_file, "    SUB SP, %d\n", 2 * symbol_count);  // Allocate space for variables

    // Generate code for each three address code instruction
    for (int i = 0; i < num_lines; i++) {
        generate_assembly_code(&codes[i], output_file);
    }

    // Generate assembly code footer
    fprintf(output_file, "    MOV AH, 4Ch\n");
    fprintf(output_file, "    INT 21h\n");
    fprintf(output_file, "MAIN ENDP\n");
    fprintf(output_file, "END MAIN\n");

    fclose(output_file);

    printf("Assembly code has been generated and saved to 'output.asm'\n");

    return 0;
}

```

## Question 8: Lex program to count vowels and consonants

```c
%{
    // Count vowels and consonants in a string
    #include <stdio.h>
    int vowels = 0;
    int consonants = 0;
%}

%%
[aAeEiIoOuU]    { vowels++; }
[a-zA-Z]        { consonants++; }
.               { /* Ignore other characters */ }
\n              { /* Ignore newlines */ }
%%

int yywrap() {
    return 1;
}

int main() {
    printf("Enter input string (Ctrl+D to end):\n");
    yylex();
    printf("Number of vowels: %d\n", vowels);
    printf("Number of consonants: %d\n", consonants - vowels);
    return 0;
}

```

## Question 9: C program to identify tokens in a program

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 100

// Token types
enum TokenType {
    IDENTIFIER,
    KEYWORD,
    INTEGER,
    FLOAT,
    OPERATOR,
    DELIMITER,
    STRING,
    UNKNOWN
};

// Keywords in C
char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

int is_keyword(char *str) {
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if a character is a delimiter
int is_delimiter(char ch) {
    return (ch == ' ' || ch == '\t' || ch == '\n' || ch == ',' || ch == ';' ||
            ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}');
}

// Check if a character is an operator
int is_operator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' ||
            ch == '=' || ch == '<' || ch == '>' || ch == '!' || ch == '&' ||
            ch == '|' || ch == '^' || ch == '~' || ch == '?');
}

// Function to identify token type
enum TokenType identify_token(char *token) {
    int i, len = strlen(token);
    int has_decimal = 0;

    // Check for empty token
    if (len == 0) return UNKNOWN;

    // Check if token is a keyword
    if (is_keyword(token)) return KEYWORD;

    // Check if token is an identifier
    if (isalpha(token[0]) || token[0] == '_') {
        for (i = 1; i < len; i++) {
            if (!isalnum(token[i]) && token[i] != '_') {
                return UNKNOWN;
            }
        }
        return IDENTIFIER;
    }

    // Check if token is a number (integer or float)
    if (isdigit(token[0]) || (token[0] == '.' && len > 1)) {
        for (i = 0; i < len; i++) {
            if (token[i] == '.') {
                if (has_decimal) return UNKNOWN; // More than one decimal point
                has_decimal = 1;
            } else if (!isdigit(token[i])) {
                return UNKNOWN;
            }
        }
        return has_decimal ? FLOAT : INTEGER;
    }

    // Check if token is a string literal
    if (token[0] == '"' && token[len-1] == '"') {
        return STRING;
    }

    // Check if token is an operator
    if (len == 1 && is_operator(token[0])) {
        return OPERATOR;
    } else if (len == 2 && is_operator(token[0]) && is_operator(token[1])) {
        // Handle two-character operators like +=, -=, ==, etc.
        return OPERATOR;
    }

    // Check if token is a delimiter
    if (len == 1 && is_delimiter(token[0])) {
        return DELIMITER;
    }

    return UNKNOWN;
}

// Function to tokenize a program
void tokenize_program(FILE *fp) {
    char c, buffer[MAX_TOKEN_LEN];
    int buffer_pos = 0;
    enum TokenType token_type;

    int in_string = 0;      // Flag for string literals
    int in_comment = 0;     // Flag for comments
    int in_multiline = 0;   // Flag for multi-line comments

    while ((c = fgetc(fp)) != EOF) {
        // Handle comments
        if (in_comment) {
            if (c == '\n') in_comment = 0;
            continue;
        }
        if (in_multiline) {
            if (c == '*' && (c = fgetc(fp)) == '/') in_multiline = 0;
            continue;
        }
        if (c == '/' && (c = fgetc(fp)) == '/') {
            in_comment = 1;
            continue;
        }
        if (c == '/' && (c = fgetc(fp)) == '*') {
            in_multiline = 1;
            continue;
        }

        // Handle string literals
        if (c == '"') {
            if (in_string) {
                buffer[buffer_pos++] = c;
                buffer[buffer_pos] = '\0';
                printf("String: %s\n", buffer);
                buffer_pos = 0;
                in_string = 0;
            } else {
                if (buffer_pos > 0) {
                    buffer[buffer_pos] = '\0';
                    token_type = identify_token(buffer);
                    switch (token_type) {
                        case IDENTIFIER: printf("Identifier: %s\n", buffer); break;
                        case KEYWORD: printf("Keyword: %s\n", buffer); break;
                        case INTEGER: printf("Integer: %s\n", buffer); break;
                        case FLOAT: printf("Float: %s\n", buffer); break;
                        case OPERATOR: printf("Operator: %s\n", buffer); break;
                        case DELIMITER: printf("Delimiter: %s\n", buffer); break;
                        case STRING: printf("String: %s\n", buffer); break;
                        case UNKNOWN: printf("Unknown: %s\n", buffer); break;
                    }
                    buffer_pos = 0;
                }
                buffer[buffer_pos++] = c;
                in_string = 1;
            }
            continue;
        }

        if (in_string) {
            buffer[buffer_pos++] = c;
            continue;
        }

        // Handle normal tokens
        if (is_delimiter(c)) {
            if (buffer_pos > 0) {
                buffer[buffer_pos] = '\0';
                token_type = identify_token(buffer);
                switch (token_type) {
                    case IDENTIFIER: printf("Identifier: %s\n", buffer); break;
                    case KEYWORD: printf("Keyword: %s\n", buffer); break;
                    case INTEGER: printf("Integer: %s\n", buffer); break;
                    case FLOAT: printf("Float: %s\n", buffer); break;
                    case OPERATOR: printf("Operator: %s\n", buffer); break;
                    case DELIMITER: printf("Delimiter: %s\n", buffer); break;
                    case STRING: printf("String: %s\n", buffer); break;
                    case UNKNOWN: printf("Unknown: %s\n", buffer); break;
                }
                buffer_pos = 0;
            }

            // Print delimiters only if they're not whitespace
            if (c != ' ' && c != '\t' && c != '\n') {
                buffer[0] = c;
                buffer[1] = '\0';
                printf("Delimiter: %s\n", buffer);
            }
        } else if (is_operator(c)) {
            if (buffer_pos > 0) {
                buffer[buffer_pos] = '\0';
                token_type = identify_token(buffer);
                switch (token_type) {
                    case IDENTIFIER: printf("Identifier: %s\n", buffer); break;
                    case KEYWORD: printf("Keyword: %s\n", buffer); break;
                    case INTEGER: printf("Integer: %s\n", buffer); break;
                    case FLOAT: printf("Float: %s\n", buffer); break;
                    case OPERATOR: printf("Operator: %s\n", buffer); break;
                    case DELIMITER: printf("Delimiter: %s\n", buffer); break;
                    case STRING: printf("String: %s\n", buffer); break;
                    case UNKNOWN: printf("Unknown: %s\n", buffer); break;
                }
                buffer_pos = 0;
            }

            buffer[0] = c;
            // Check for two-character operators
            char next_c = fgetc(fp);
            if (is_operator(next_c)) {
                buffer[1] = next_c;
                buffer[2] = '\0';
            } else {
                buffer[1] = '\0';
                ungetc(next_c, fp);  // Put back the character
            }
            printf("Operator: %s\n", buffer);
            buffer_pos = 0;
        } else {
            buffer[buffer_pos++] = c;
            if (buffer_pos >= MAX_TOKEN_LEN - 1) {
                buffer[buffer_pos] = '\0';
                printf("Error: Token too long: %s\n", buffer);
                buffer_pos = 0;
            }
        }
    }

    // Process the last token
    if (buffer_pos > 0) {
        buffer[buffer_pos] = '\0';
        token_type = identify_token(buffer);
        switch (token_type) {
            case IDENTIFIER: printf("Identifier: %s\n", buffer); break;
            case KEYWORD: printf("Keyword: %s\n", buffer); break;
            case INTEGER: printf("Integer: %s\n", buffer); break;
            case FLOAT: printf("Float: %s\n", buffer); break;
            case OPERATOR: printf("Operator: %s\n", buffer); break;
            case DELIMITER: printf("Delimiter: %s\n", buffer); break;
            case STRING: printf("String: %s\n", buffer); break;
            case UNKNOWN: printf("Unknown: %s\n", buffer); break;
        }
    }
}

int main() {
    FILE *fp;
    char filename[100];

    printf("Enter the source code filename: ");
    scanf("%s", filename);

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        return 1;
    }

    printf("Tokens found in the program:\n");
    tokenize_program(fp);

    fclose(fp);
    return 0;
}

```

## Question 10: C program for Brute Force Top-down Parsing

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_GRAMMAR_RULES 20
#define MAX_RULE_LENGTH 50
#define MAX_INPUT_LENGTH 100

// Structure to store a grammar rule
typedef struct {
    char lhs;                  // Left-hand side (non-terminal)
    char rhs[MAX_RULE_LENGTH]; // Right-hand side
} GrammarRule;

// Global variables
GrammarRule grammar[MAX_GRAMMAR_RULES];
int num_rules = 0;
char start_symbol;
char input[MAX_INPUT_LENGTH];
int input_len;

// Function to add a grammar rule
void add_rule(char lhs, char *rhs) {
    if (num_rules < MAX_GRAMMAR_RULES) {
        grammar[num_rules].lhs = lhs;
        strcpy(grammar[num_rules].rhs, rhs);
        num_rules++;
    } else {
        printf("Grammar rule limit reached.\n");
    }
}

// Function to check if a character is a non-terminal
int is_non_terminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

// Function to check if a string can be derived from a non-terminal
// using the brute force approach
int brute_force_parse(char current_string[], int pos, char derivation[], int d_pos) {
    // If we've derived the entire input string, return success
    if (pos == input_len && strlen(current_string) == 0) {
        derivation[d_pos] = '\0';
        printf("Successful parse! Derivation: %s\n", derivation);
        return 1;
    }

    // If there are no more characters in the current string, but input remains,
    // or if the current string is longer than the remaining input, fail
    if (strlen(current_string) == 0 || strlen(current_string) > (input_len - pos)) {
        return 0;
    }

    // If the next character in the current string is a terminal
    if (!is_non_terminal(current_string[0])) {
        // If it matches the input, continue parsing
        if (current_string[0] == input[pos]) {
            char new_string[MAX_RULE_LENGTH];
            strcpy(new_string, current_string + 1);  // Remove the matched terminal
            return brute_force_parse(new_string, pos + 1, derivation, d_pos);
        } else {
            return 0;  // Terminal mismatch
        }
    }

    // If the next character is a non-terminal, try all possible derivations
    char non_terminal = current_string[0];
    char new_string[MAX_RULE_LENGTH];

    // Try each rule for this non-terminal
    for (int i = 0; i < num_rules; i++) {
        if (grammar[i].lhs == non_terminal) {
            // Create a new string by replacing the non-terminal
            strcpy(new_string, grammar[i].rhs);
            strcat(new_string, current_string + 1);

            // Update the derivation trace
            derivation[d_pos] = grammar[i].lhs;
            derivation[d_pos + 1] = '-';
            derivation[d_pos + 2] = '>';
            strcpy(derivation + d_pos + 3, grammar[i].rhs);
            int new_d_pos = d_pos + 3 + strlen(grammar[i].rhs);
            derivation[new_d_pos++] = ' ';

            // Try this derivation
            if (brute_force_parse(new_string, pos, derivation, new_d_pos)) {
                return 1;
            }
        }
    }

    return 0;  // No successful derivation found
}

int main() {
    char lhs, rhs[MAX_RULE_LENGTH];
    char derivation[MAX_INPUT_LENGTH * 10];  // For storing the derivation steps

    printf("Enter the start symbol: ");
    scanf(" %c", &start_symbol);

    printf("Enter grammar rules (format: A->abc, end with 'end'):\n");
    while (1) {
        char rule[MAX_RULE_LENGTH];
        scanf("%s", rule);

        if (strcmp(rule, "end") == 0) {
            break;
        }

        // Parse the rule
        if (strlen(rule) >= 4 && rule[1] == '-' && rule[2] == '>') {
            lhs = rule[0];
            strcpy(rhs, rule + 3);
            add_rule(lhs, rhs);
        } else {
            printf("Invalid rule format. Use A->abc\n");
        }
    }

    printf("Enter the input string: ");
    scanf("%s", input);
    input_len = strlen(input);

    // Start with the start symbol
    char initial_string[2];
    initial_string[0] = start_symbol;
    initial_string[1] = '\0';

    printf("Parsing using brute force top-down approach...\n");
    if (!brute_force_parse(initial_string, 0, derivation, 0)) {
        printf("Input string is not accepted by the grammar.\n");
    }

    return 0;
}

```

## Question 11: C program to implement Recursive Descent Parser

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global variables
char *input;
int position = 0;
char lookahead;

// Function declarations
void error();
void match(char expected);
void E();
void Eprime();
void T();
void Tprime();
void F();

// Error function
void error() {
    printf("Syntax error at position %d: unexpected token '%c'\n", position, lookahead);
    exit(1);
}

// Function to match the expected token
void match(char expected) {
    if (lookahead == expected) {
        lookahead = input[++position];
    } else {
        error();
    }
}

// Productions for the following grammar:
// E -> T E'
// E' -> + T E' | ε
// T -> F T'
// T' -> * F T' | ε
// F -> ( E ) | id

void E() {
    printf("E -> T E'\n");
    T();
    Eprime();
}

void Eprime() {
    if (lookahead == '+') {
        printf("E' -> + T E'\n");
        match('+');
        T();
        Eprime();
    } else {
        printf("E' -> ε\n");
        // epsilon production, do nothing
    }
}

void T() {
    printf("T -> F T'\n");
    F();
    Tprime();
}

void Tprime() {
    if (lookahead == '*') {
        printf("T' -> * F T'\n");
        match('*');
        F();
        Tprime();
    } else {
        printf("T' -> ε\n");
        // epsilon production, do nothing
    }
}

void F() {
    if (lookahead == '(') {
        printf("F -> ( E )\n");
        match('(');
        E();
        match(')');
    } else if (isalnum(lookahead)) {
        printf("F -> id\n");
        match(lookahead); // treat any alphanumeric as an identifier
    } else {
        error();
    }
}

int main() {
    char buffer[100];

    printf("Enter an arithmetic expression (using +, *, (, ), and identifiers): ");
    scanf("%s", buffer);

    input = buffer;
    lookahead = input[position];

    printf("Parsing using recursive descent...\n");
    E();

    if (lookahead == '\0') {
        printf("Parsing completed successfully!\n");
    } else {
        printf("Syntax error: unexpected token '%c' at end of input\n", lookahead);
    }

    return 0;
}

```

## Question 12: C program to compute First and Follow Sets

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NON_TERMINALS 10
#define MAX_TERMINALS 20
#define MAX_RULES 50
#define MAX_RHS_LENGTH 20

// Structure to represent a grammar rule
typedef struct {
    char lhs;                  // Left-hand side (non-terminal)
    char rhs[MAX_RHS_LENGTH];  // Right-hand side
} Rule;

// Global variables
Rule grammar[MAX_RULES];
int num_rules = 0;
char non_terminals[MAX_NON_TERMINALS];
int num_non_terminals = 0;
char terminals[MAX_TERMINALS];
int num_terminals = 0;
char start_symbol;

// Sets for FIRST and FOLLOW
char first[MAX_NON_TERMINALS][MAX_TERMINALS];
char follow[MAX_NON_TERMINALS][MAX_TERMINALS];
int first_count[MAX_NON_TERMINALS] = {0};
int follow_count[MAX_NON_TERMINALS] = {0};

// Function to check if a character is a non-terminal
int is_non_terminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

// Function to check if a character is a terminal
int is_terminal(char c) {
    return !is_non_terminal(c) && c != 'e';  // 'e' represents epsilon
}

// Function to add a character to a set
void add_to_set(char set[], int *count, char c) {
    for (int i = 0; i < *count; i++) {
        if (set[i] == c) {
            return;  // Already in the set
        }
    }
    set[(*count)++] = c;
}

// Function to add a rule to the grammar
void add_rule(char lhs, char *rhs) {
    if (num_rules < MAX_RULES) {
        grammar[num_rules].lhs = lhs;
        strcpy(grammar[num_rules].rhs, rhs);
        num_rules++;
    } else {
        printf("Grammar rule limit reached.\n");
    }
}

// Function to add a non-terminal
void add_non_terminal(char nt) {
    for (int i = 0; i < num_non_terminals; i++) {
        if (non_terminals[i] == nt) {
            return;
        }
    }
    non_terminals[num_non_terminals++] = nt;
}

// Function to add a terminal
void add_terminal(char t) {
    for (int i = 0; i < num_terminals; i++) {
        if (terminals[i] == t) {
            return;
        }
    }
    terminals[num_terminals++] = t;
}

// Function to find the index of a non-terminal
int find_non_terminal_index(char nt) {
    for (int i = 0; i < num_non_terminals; i++) {
        if (non_terminals[i] == nt) {
            return i;
        }
    }
    return -1;
}

// Function to compute FIRST sets
void compute_first_sets() {
    int i, j, k;
    int done;

    // Initialize FIRST sets
    for (i = 0; i < num_non_terminals; i++) {
        for (j = 0; j < num_rules; j++) {
            if (grammar[j].lhs == non_terminals[i]) {
                // If the first symbol is a terminal or epsilon
                if (is_terminal(grammar[j].rhs[0]) || grammar[j].rhs[0] == 'e') {
                    add_to_set(first[i], &first_count[i], grammar[j].rhs[0]);
                }
            }
        }
    }

    // Iterate until no more changes
    do {
        done = 1;

        for (i = 0; i < num_rules; i++) {
            int lhs_index = find_non_terminal_index(grammar[i].lhs);
            int rhs_index = 0;
            int old_first_count = first_count[lhs_index];

            // Process each symbol in the RHS
            while (rhs_index < strlen(grammar[i].rhs)) {
                char current = grammar[i].rhs[rhs_index];

                if (is_terminal(current)) {
                    add_to_set(first[lhs_index], &first_count[lhs_index], current);
                    break;
                } else if (current == 'e') {
                    add_to_set(first[lhs_index], &first_count[lhs_index], 'e');
                    break;
                } else if (is_non_terminal(current)) {
                    int nt_index = find_non_terminal_index(current);
                    int has_epsilon = 0;

                    // Add all symbols from FIRST(current) to FIRST(lhs) except epsilon
                    for (j = 0; j < first_count[nt_index]; j++) {
                        if (first[nt_index][j] != 'e') {
                            add_to_set(first[lhs_index], &first_count[lhs_index], first[nt_index][j]);
                        } else {
                            has_epsilon = 1;
                        }
                    }

                    // If current doesn't have epsilon, stop here
                    if (!has_epsilon) {

```
