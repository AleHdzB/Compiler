#include <stdio.h>
#include <ctype.h>
#include <string.h>


// Definición de los tipos de tokens
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_SYMBOL,
    TOKEN_UNKNOWN
} TokenType;

// Función para verificar si una cadena es una palabra clave
int isKeyword(const char* str) {
    const char* keywords[] = {"void", "main", "int", "return"};
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < numKeywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Función para analizar el código y generar tokens
void lexer(const char* code) {

    char a = 160;       // á
    char e = 130;       // é
    char o = 162;       // ó
    

    printf("=========== An%clisis l%cxico del c%cdigo ===========\n\n", a, e, o);

    int i = 0;
    while (code[i] != '\0') {
        // Ignorar espacios en blanco y saltos de línea
        if (isspace(code[i])) {
            i++;
            continue;
        }

        // Identificar palabras clave e identificadores
        if (isalpha(code[i])) {
            char buffer[100];
            int j = 0;
            while (isalnum(code[i])) {
                buffer[j++] = code[i++];
            }
            buffer[j] = '\0';

            if (isKeyword(buffer)) {
                printf("[TOKEN_KEYWORD]     -> %s\n", buffer);
            } else {
                printf("[TOKEN_IDENTIFIER]  -> %s\n", buffer);
            }
            continue;
        }

        // Identificar números
        if (isdigit(code[i])) {
            char buffer[100];
            int j = 0;
            while (isdigit(code[i])) {
                buffer[j++] = code[i++];
            }
            buffer[j] = '\0';
            printf("[TOKEN_NUMBER]      -> %s\n", buffer);
            continue;
        }

        // Identificar operadores
        if (code[i] == '+' || code[i] == '-' || code[i] == '*' || code[i] == '/' || code[i] == '=') {
            printf("[TOKEN_OPERATOR]    -> %c\n", code[i]);
            i++;
            continue;
        }

        // Identificar símbolos especiales
        if (code[i] == '{' || code[i] == '}' || code[i] == '(' || code[i] == ')' || code[i] == ';'|| code[i] == ',') {
            printf("[TOKEN_SYMBOL]      -> %c\n", code[i]);
            i++;
            continue;
        }

        // Si no se reconoce el carácter, se marca como desconocido
        printf("[TOKEN_UNKNOWN]     -> %c\n", code[i]);
        i++;
    }

    printf("\n=================================================\n");
}

int main() {
    const char* code = "void main(){\n   int a,b,c;\n   a = 1;\n   b=2;\n   c=a+b;\n }";
    lexer(code);
    return 0;
}
