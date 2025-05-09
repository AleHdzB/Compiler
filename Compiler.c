#include <stdio.h>
#include <ctype.h>
#include <string.h>

const char a = 160;       // á
const char e = 130;       // é
const char o = 162;       // ó
const char i = 161;       // í

// Tipos de tokens
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_SYMBOL,
    TOKEN_UNKNOWN
} TokenType;

// Estructura para almacenar tokens
typedef struct {
    TokenType type;
    char lexeme[100];
} Token;

// Tabla de símbolos para análisis semántico
typedef struct {
    char nombre[100];
    char tipo[20];
} Simbolo;

Simbolo tablaSimbolos[100];
int contadorSimbolos = 0;

// Prototipos de funciones
int isKeyword(const char* str);
void lexer(const char* code, Token* tokens, int* numTokens);
void parser(Token* tokens, int numTokens);
void agregarSimbolo(const char* nombre, const char* tipo);
int verificarSimbolo(const char* nombre);

// Función principal
int main() {
    const char* code = "void main(){\n   int a,b,c;\n   a = 1;\n   b=2;\n   c=a+b;\n }";
    Token tokens[100];
    int numTokens = 0;
    
    lexer(code, tokens, &numTokens);
    parser(tokens, numTokens);
    
    return 0;
}

// Verificación de palabras clave
int isKeyword(const char* str) {
    const char* keywords[] = {"void", "main", "int", "return"};
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);
    
    for(int i = 0; i < numKeywords; i++) {
        if(strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

// Analizador léxico modificado
void lexer(const char* code, Token* tokens, int* numTokens) {
    printf("\n=========== An%clisis l%cxico ===========\n\n",a,e);
    
    int i = 0;
    *numTokens = 0;
    
    while(code[i] != '\0') {
        if(isspace(code[i])) { i++; continue; }
        
        // Identificar palabras clave e identificadores
        if(isalpha(code[i])) {
            char buffer[100] = {0};
            int j = 0;
            
            while(isalnum(code[i])) {
                buffer[j++] = code[i++];
            }
            
            if(isKeyword(buffer)) {
                printf("TOKEN_KEYWORD: %s\n", buffer);
                tokens[*numTokens].type = TOKEN_KEYWORD;
            } else {
                printf("TOKEN_IDENTIFIER: %s\n", buffer);
                tokens[*numTokens].type = TOKEN_IDENTIFIER;
            }
            strcpy(tokens[(*numTokens)++].lexeme, buffer);
            continue;
        }
        
        // Números
        if(isdigit(code[i])) {
            char buffer[100] = {0};
            int j = 0;
            
            while(isdigit(code[i])) {
                buffer[j++] = code[i++];
            }
            
            printf("TOKEN_NUMBER: %s\n", buffer);
            tokens[*numTokens].type = TOKEN_NUMBER;
            strcpy(tokens[(*numTokens)++].lexeme, buffer);
            continue;
        }
        
        // Operadores
        if(strchr("+-*/=", code[i])) {
            printf("TOKEN_OPERATOR: %c\n", code[i]);
            tokens[*numTokens].type = TOKEN_OPERATOR;
            tokens[*numTokens].lexeme[0] = code[i];
            tokens[(*numTokens)++].lexeme[1] = '\0';
            i++;
            continue;
        }
        
        // Símbolos
        if(strchr("{}();,", code[i])) {
            printf("TOKEN_SYMBOL: %c\n", code[i]);
            tokens[*numTokens].type = TOKEN_SYMBOL;
            tokens[*numTokens].lexeme[0] = code[i];
            tokens[(*numTokens)++].lexeme[1] = '\0';
            i++;
            continue;
        }
        
        // Carácter desconocido
        printf("TOKEN_UNKNOWN: %c\n", code[i]);
        tokens[*numTokens].type = TOKEN_UNKNOWN;
        tokens[*numTokens].lexeme[0] = code[i];
        tokens[(*numTokens)++].lexeme[1] = '\0';
        i++;
    }
}

// Funciones para análisis semántico
void agregarSimbolo(const char* nombre, const char* tipo) {
    // Verificar duplicados
    for(int i = 0; i < contadorSimbolos; i++) {
        if(strcmp(tablaSimbolos[i].nombre, nombre) == 0) {
            printf("\nError sem%cntico: Variable '%s' ya declarada\n", a, nombre);
            return;
        }
    }
    
    if(contadorSimbolos < 100) {
        strcpy(tablaSimbolos[contadorSimbolos].nombre, nombre);
        strcpy(tablaSimbolos[contadorSimbolos].tipo, tipo);
        contadorSimbolos++;
    } else {
        printf("\nError sem%cntico: Tabla de símbolos llena\n", a);
    }
}

int verificarSimbolo(const char* nombre) {
    for(int i = 0; i < contadorSimbolos; i++) {
        if(strcmp(tablaSimbolos[i].nombre, nombre) == 0) {
            return 1;
        }
    }
    printf("\nError sem%cntico: Variable '%s' no declarada\n", a, nombre);
    return 0;
}

// Funciones del parser modificadas con análisis semántico
void parseFactor(Token* tokens, int numTokens, int* pos);
void parseTerm(Token* tokens, int numTokens, int* pos);
void parseExpression(Token* tokens, int numTokens, int* pos);

void parseFactor(Token* tokens, int numTokens, int* pos) {
    if(*pos >= numTokens) {
        printf("Error: Factor inesperado\n");
        return;
    }
    
    if(tokens[*pos].type == TOKEN_IDENTIFIER) {
        if(verificarSimbolo(tokens[*pos].lexeme)) {
            printf("Procesando factor: %s\n", tokens[*pos].lexeme);
        }
        (*pos)++;
    } else if(tokens[*pos].type == TOKEN_NUMBER) {
        printf("Procesando factor: %s\n", tokens[*pos].lexeme);
        (*pos)++;
    } else if(tokens[*pos].type == TOKEN_SYMBOL && strcmp(tokens[*pos].lexeme, "(") == 0) {
        (*pos)++;
        printf("Procesando paréntesis abierto: %s\n", tokens[*pos-1].lexeme);
        parseExpression(tokens, numTokens, pos);
        if(*pos < numTokens && strcmp(tokens[*pos].lexeme, ")") == 0) {
            printf("Procesando paréntesis cerrado: %s\n", tokens[*pos].lexeme);
            (*pos)++;
        } else {
            printf("Error: Se esperaba ')'\n");
        }
    } else {
        printf("Error: Factor inv%clido '%s'\n",a, tokens[*pos].lexeme);
    }
}

void parseTerm(Token* tokens, int numTokens, int* pos) {
    parseFactor(tokens, numTokens, pos);
    
    while(*pos < numTokens && tokens[*pos].type == TOKEN_OPERATOR && 
          (strchr("*/", tokens[*pos].lexeme[0]))) {
        printf("Procesando operador: %s\n", tokens[*pos].lexeme);
        (*pos)++;
        parseFactor(tokens, numTokens, pos);
    }
}

void parseExpression(Token* tokens, int numTokens, int* pos) {
    parseTerm(tokens, numTokens, pos);
    
    while(*pos < numTokens && tokens[*pos].type == TOKEN_OPERATOR && 
          (strchr("+-", tokens[*pos].lexeme[0]))) {
        printf("Procesando operador: %s\n", tokens[*pos].lexeme);
        (*pos)++;
        parseTerm(tokens, numTokens, pos);
    }
}

void parseAssignment(Token* tokens, int numTokens, int* pos) {
    if(*pos >= numTokens || tokens[*pos].type != TOKEN_IDENTIFIER) {
        printf("Error: Identificador esperado\n");
        return;
    }
    
    if(!verificarSimbolo(tokens[*pos].lexeme)) {
        printf("Error: Asignación a variable no declarada '%s'\n", tokens[*pos].lexeme);
    }
    
    printf("Asignando a: %s\n", tokens[*pos].lexeme);
    (*pos)++;
    
    if(*pos >= numTokens || tokens[*pos].type != TOKEN_OPERATOR || 
       strcmp(tokens[*pos].lexeme, "=") != 0) {
        printf("Error: Se esperaba '='\n");
        return;
    }
    printf("Procesando operador de asignaci%cn: %s\n", o,tokens[*pos].lexeme);
    (*pos)++;
    
    parseExpression(tokens, numTokens, pos);
    
    if(*pos >= numTokens || tokens[*pos].type != TOKEN_SYMBOL || 
       strcmp(tokens[*pos].lexeme, ";") != 0) {
        printf("Error: Se esperaba ';'\n");
        return;
    }
    printf("Procesando s%cmbolo de fin de declaraci%cn: %s\n",i,o, tokens[*pos].lexeme);
    (*pos)++;
}

void parseDeclarations(Token* tokens, int numTokens, int* pos) {
    if(*pos >= numTokens || strcmp(tokens[*pos].lexeme, "int") != 0) return;
    
    printf("Procesando declaraci%cn de tipo: %s\n",o, tokens[*pos].lexeme);
    (*pos)++;
    if(*pos >= numTokens || tokens[*pos].type != TOKEN_IDENTIFIER) {
        printf("Error: Identificador esperado\n");
        return;
    }
    
    while(1) {
        printf("Procesando declaraci%cn de identificador: %s\n",o, tokens[*pos].lexeme);
        agregarSimbolo(tokens[*pos].lexeme, "int");
        (*pos)++;
        
        if(*pos >= numTokens || strcmp(tokens[*pos].lexeme, ",") != 0) break;
        
        printf("Procesando coma: %s\n", tokens[*pos].lexeme);
        (*pos)++;
    }
    
    if(*pos >= numTokens || strcmp(tokens[*pos].lexeme, ";") != 0) {
        printf("Error: Se esperaba ';'\n");
        return;
    }
    printf("Procesando s%cmbolo de fin de declaraci%cn: %s\n",i, o,tokens[*pos].lexeme);
    (*pos)++;
}

void parseBlock(Token* tokens, int numTokens, int* pos) {
    parseDeclarations(tokens, numTokens, pos);
    
    while(*pos < numTokens && tokens[*pos].type == TOKEN_IDENTIFIER) {
        parseAssignment(tokens, numTokens, pos);
    }
}

// Analizador sintáctico principal
void parser(Token* tokens, int numTokens) {
    printf("\n=========== An%clisis sint%cctico ===========\n\n",a,a);
    
    int pos = 0;
    
    // Verificar estructura principal
    if(strcmp(tokens[pos++].lexeme, "void") != 0) {
        printf("Error: Se esperaba 'void'\n");
        return;
    }
    if(strcmp(tokens[pos++].lexeme, "main") != 0) {
        printf("Error: Se esperaba 'main'\n");
        return;
    }
    if(strcmp(tokens[pos++].lexeme, "(") != 0) {
        printf("Error: Se esperaba '('\n");
        return;
    }
    if(strcmp(tokens[pos++].lexeme, ")") != 0) {
        printf("Error: Se esperaba ')'\n");
        return;
    }
    if(strcmp(tokens[pos++].lexeme, "{") != 0) {
        printf("Error: Se esperaba '{'\n");
        return;
    }
    
    parseBlock(tokens, numTokens, &pos);
    
    if(pos >= numTokens || strcmp(tokens[pos].lexeme, "}") != 0) {
        printf("Error: Se esperaba '}'\n");
        return;
    }
    printf("Procesando cierre de bloque: %s\n", tokens[pos].lexeme);
    pos++;
    
    if(pos != numTokens) {
        printf("Error: C%cdigo adicional después del cierre\n",o);
        return;
    }
    
    printf("\n=========== An%clisis sem%cntico ===========\n", a, a);
    printf("No se encontraron errores sem%canticos\n", a);
    printf("An%clisis completado exitosamente!\n",160);
    printf("\n==========================================\n");
}