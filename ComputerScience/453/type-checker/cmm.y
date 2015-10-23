%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "symtab.h"
#include "utils.h"
#include "ast.h"
#include "globals.h"
#include "cmm.tab.h"

extern char *yytext;

// Parse state information
int mylineno;
int mycolno;
bool foundError = false;

// Typing and scoping
bool funcTypeSet = false;
bool declaredExtern = false;
Type currentFunctionReturnType = VOID_TYPE;
Scope *globalScope;
Scope *scope;
Type baseDeclType;

// Helper functions
int diffComp(void *a, void *b);
bool addFunctionDeclarationToScope(FunctionDeclaration *declaration);

%}

%union {
    Expression *expression;
    Statement *statement;
    Type type;
    FunctionParameter *parameter;
    FunctionDeclaration *functionDeclaration;
    VariableDeclaration *variableDeclaration;
    char *string;
}

%type<expression> expr optional_expr expr_list
%type<statement> stmt assg optional_assign stmt_list
%type<type> type
%type<functionDeclaration> func
%type<variableDeclaration> optional_var_decl_list var_decl var_decl_list
%type<parameter> param_types param_types_list non_void_param_type

/* Language Tokens */
%token WHILE FOR
%token INT CHAR VOID
%token IF ELSE
%token EXTERN RETURN
%token COMMA SEMICOLON

/* Brackets */
%token LEFT_PAREN RIGHT_PAREN
%token LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET
%token LEFT_CURLY_BRACKET RIGHT_CURLY_BRACKET

/* Operators */
%token EQ NEQ LTE LT GT GTE NOT
%token AND OR
%token ADD MINUS MUL DIV
%token ASSIGN

/* Text tokens */
%token <expression> INTCON
%token <expression> CHARCON
%token <expression> STRINGCON
%token <string> ID
%token TEXT SPACE

/* If/Else Precedence fix */
%nonassoc WITHOUT_ELSE
%nonassoc ELSE WITH_ELSE

/* Operator Precedence specification */
%left OR
%left AND
%left EQ NEQ equality_op
%left GTE LTE GT LT relop
%left ADD MINUS add_sub
%left MUL DIV mul_div
%right UMINUS NOT

%%

prog : decl prog
     | func prog
     | epsilon

decl : type var_decl_list SEMICOLON {
        // TODO: Make this better
        // To handle global scopes, scope is flattened on a global declaration (ew)
        globalScope = flattenScope(scope);
        scope = newScope(globalScope); // Clear out the scope the declaration was made in (ew)
    }
     | type name_args_lists SEMICOLON { scope = newScope(globalScope); }
     | VOID name_args_lists SEMICOLON { scope = newScope(globalScope); }
     | extern type name_args_lists SEMICOLON { scope = newScope(globalScope); declaredExtern = false; }
     | extern VOID name_args_lists SEMICOLON { scope = newScope(globalScope); declaredExtern = false; }
     | error SEMICOLON
     ;

extern : EXTERN {
    declaredExtern = true;
}

func : type ID LEFT_PAREN param_types RIGHT_PAREN LEFT_CURLY_BRACKET optional_var_decl_list stmt_list RIGHT_CURLY_BRACKET {
        FunctionDeclaration *decl = newFunction($1, $2, $4, $7, $8);
        scope = newScope(globalScope);
        addFunctionDeclarationToScope(decl);
        funcTypeSet = false;

        $$ = decl;
    }
     | VOID ID LEFT_PAREN param_types RIGHT_PAREN LEFT_CURLY_BRACKET optional_var_decl_list  stmt_list RIGHT_CURLY_BRACKET {
        FunctionDeclaration *decl = newFunction(VOID_TYPE, $2, $4, $7, $8);
        scope = newScope(globalScope);
        addFunctionDeclarationToScope(decl);
        funcTypeSet = false;

        $$ = decl;
    }
     | error RIGHT_CURLY_BRACKET { $$ = NULL; }
     ;

stmt : IF LEFT_PAREN expr RIGHT_PAREN stmt %prec WITHOUT_ELSE   { $$ = newIfStatement(scope, $3, $5); }
     | IF LEFT_PAREN expr RIGHT_PAREN stmt ELSE stmt            { $$ = newIfElseStatement(scope, $3, $5, $7); }
     | WHILE LEFT_PAREN expr RIGHT_PAREN stmt                   { $$ = newWhileStatement(scope, $3, $5); }
     | FOR LEFT_PAREN optional_assign SEMICOLON optional_expr SEMICOLON optional_assign RIGHT_PAREN stmt
     | RETURN optional_expr SEMICOLON                           { $$ = newReturnStatement(scope, $2); }
     | assg SEMICOLON                                           { $$ = $1; }
     | ID LEFT_PAREN expr_list RIGHT_PAREN SEMICOLON
     | LEFT_CURLY_BRACKET stmt_list RIGHT_CURLY_BRACKET         { $$ = $2; }
     | SEMICOLON                                                { $$ = NULL; }
     | error SEMICOLON                                          { $$ = NULL; }
     | error RIGHT_CURLY_BRACKET                                { $$ = NULL; }
     ;

expr : MINUS expr %prec UMINUS                          { $$ = newUnaryExpression(NEG_OP, $2); }
     | NOT expr %prec UMINUS                            { $$ = newUnaryExpression(NOT_OP, $2); }
     | expr ADD expr %prec add_sub                      { $$ = newBinaryExpression(ADD_OP, $1, $3); }
     | expr MINUS expr %prec add_sub                    { $$ = newBinaryExpression(SUB_OP, $1, $3); }
     | expr AND expr                                    { $$ = newBinaryExpression(AND_OP, $1, $3); }
     | expr OR expr                                     { $$ = newBinaryExpression(OR_OP, $1, $3); }
     | expr MUL expr %prec mul_div                      { $$ = newBinaryExpression(MUL_OP, $1, $3); }
     | expr DIV expr %prec mul_div                      { $$ = newBinaryExpression(DIV_OP, $1, $3); }
     | expr EQ expr %prec equality_op                   { $$ = newBinaryExpression(EQ_OP, $1, $3); }
     | expr NEQ expr %prec equality_op                  { $$ = newBinaryExpression(NEQ_OP, $1, $3); }
     | expr GTE expr %prec relop                        { $$ = newBinaryExpression(GTE_OP, $1, $3); }
     | expr LTE expr %prec relop                        { $$ = newBinaryExpression(LTE_OP, $1, $3); }
     | expr GT expr %prec relop                         { $$ = newBinaryExpression(GT_OP, $1, $3); }
     | expr LT expr %prec relop                         { $$ = newBinaryExpression(LT_OP, $1, $3); }
     | ID LEFT_PAREN expr_list RIGHT_PAREN              { $$ = newFunctionExpression(scope, $1, $3); }
     | ID LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET { $$ = newVariableExpression(scope, $1, $3); }
     | ID                                               { $$ = newVariableExpression(scope, $1, NULL); }
     | LEFT_PAREN expr RIGHT_PAREN                      { $$ = $2; }
     | INTCON                                           { $$ = newIntConstExpression(atoi(yytext)); }
     | CHARCON                                          { $$ = newCharConstExpression(yytext[0]); }
     | STRINGCON                                        { $$ = newCharArrayConstExpression(strdup(yytext)); }
     | error                                            { $$ = NULL; }
     ;

name_args_lists : ID LEFT_PAREN param_types RIGHT_PAREN {
                    List *names = newList(diffComp);
                    List *types = newList(diffComp);
                    FunctionParameter *param = $3;
                    while(param) {
                        listInsert(names, param->identifier);
                        listInsert(types, &param->type);
                        param = param->next;
                    }
                    bool success = declareFunction(globalScope, currentFunctionReturnType, $1,
                                                    names, types, declaredExtern);

                    if(!success) {
                        fprintf(stderr, "ERROR: Attempted redeclaration of \"%s\" as function on line %d.\n",
                            $1, mylineno);
                    }
                }
                | name_args_lists COMMA ID LEFT_PAREN param_types RIGHT_PAREN
                ;

var_decl : ID { declareVar(scope, baseDeclType, $1); $$ = newVariable(baseDeclType, $1); }
         | ID LEFT_SQUARE_BRACKET INTCON RIGHT_SQUARE_BRACKET {
            if(baseDeclType == INT_TYPE) {
                declareVar(scope, INT_ARRAY_TYPE, $1);
                $$ = newVariable(INT_ARRAY_TYPE, $1);
            } else if(baseDeclType == CHAR_TYPE) {
                declareVar(scope, CHAR_ARRAY_TYPE, $1);
                $$ = newVariable(CHAR_ARRAY_TYPE, $1);
            } else {
                fprintf(stderr, "ERROR: Cannot determine type when declaring %s on line %d!\n", $1, mylineno);
                foundError = true;
            }
         }
         ;

var_decl_list : var_decl                        { $$ = $1; }
              | var_decl_list COMMA var_decl    { $3->next = $1; $$ = $3; }
              | epsilon                         { $$ = NULL; }

type : CHAR {
        if(!funcTypeSet) {
            currentFunctionReturnType = CHAR_TYPE;
            funcTypeSet = true;
        }
        baseDeclType = CHAR_TYPE; $$ = CHAR_TYPE;
     }
     | INT {
        if(!funcTypeSet) {
            currentFunctionReturnType = INT_TYPE;
            funcTypeSet = true;
        }
        baseDeclType = INT_TYPE; $$ = INT_TYPE;
     }
     ;

param_types : VOID                                              { $$ = NULL; }
            | non_void_param_type                               { $$ = $1; }
            | param_types_list COMMA non_void_param_type        { $1->next = $3; $$ = $1; }
            ;

non_void_param_type : type ID {
                        declareVar(scope, baseDeclType, $2);
                        $$ = newFunctionParameter(baseDeclType, $2);
                    }
                    | type ID LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET {
                        if(baseDeclType == INT_TYPE) {
                            declareVar(scope, INT_ARRAY_TYPE, $2);
                            $$ = newFunctionParameter(INT_ARRAY_TYPE, $2);
                        } else if(baseDeclType == CHAR_TYPE) {
                            declareVar(scope, CHAR_ARRAY_TYPE, $2);
                            $$ = newFunctionParameter(CHAR_ARRAY_TYPE, $2);
                        } else {
                            fprintf(stderr, "Type error, on line %d: Arrays of type %s are not supported.\n",
                                    mylineno, typeName(baseDeclType));
                            foundError = true;
                        }
                    }
                    ;

param_types_list : non_void_param_type                          { $$ = $1; }
                 | param_types_list COMMA non_void_param_type   { $1->next = $3; $$ = $1; }
                 | epsilon                                      { $$ = NULL; }
                 ;

optional_var_decl_list : type var_decl_list SEMICOLON optional_var_decl_list
                       { baseDeclType = $1; $2->next = $4; $$ = $2; }
                       | epsilon { $$ = NULL; }

optional_assign: assg                                               { $$ = $1; }
               | error                                              { $$ = NULL; }
               | epsilon                                            { $$ = NULL; }
               ;

optional_expr : expr                                                { $$ = $1; }
              | epsilon                                             { $$ = NULL; }
              ;

stmt_list : stmt stmt_list {
            if($1) {
                $1->next = $2;
                $$ = $1;
            } else {
                $$ = NULL;
            }
        }
          | epsilon                                                 { $$ = NULL; }
          ;

assg : ID ASSIGN expr                                               { $$ = newAssignmentStatement(scope, $1, NULL, $3); }
     | ID LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET ASSIGN expr { $$ = newAssignmentStatement(scope, $1, $3, $6); }
     ;

expr_list : optional_expr { $$ = $1; }
          | expr_list COMMA expr { $3->next = $1; $$ = $3; }

epsilon:
       ;

%%
int diffComp(void *a, void *b) {
    return -1;
}

bool addFunctionDeclarationToScope(FunctionDeclaration *declaration) {
    FunctionParameter *param = declaration->parameters;
    List *names = newList(diffComp);
    List *types = newList(diffComp);

    while(param) {
        listInsert(names, param->identifier);
        listInsert(types, &param->type);
        param = param->next;
    }

    bool success = declareFunction(globalScope, declaration->returnType, declaration->functionName,
                                names, types, declaredExtern);

    // Mark the function as implemented
    if(success) {
        ScopeElement *elem = findScopeElement(globalScope, declaration->functionName);
        ScopeFunction *func = elem->function;
        func->implemented = true;
    }

    return success;
}

int main(int argc, char **argv){
#ifdef DEBUG
    setDebuggingLevel(E_ALL);
#endif
    globalScope = newScope(NULL);
    scope = newScope(globalScope);
    yyparse();

    if(foundError) {
        return 1;
    } else {
        return 0;
    }
}

int yyerror() {
    foundError = true;

    if(yytext[0] == 0) {
        fprintf(stderr, "Encountered unexpected EOF while parsing \"%s\" starting on line %d.\n",
                yytext, mylineno);
    } else {
        fprintf(stderr, "%d:%d - Encountered error while parsing: \"%s\"\n", mylineno, mycolno,
                yytext);
    }

    return 1;
}
