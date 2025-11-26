grammar Calc;

// parser

program : statement* ;

statement
    : expr                   # statExpr
    | ID '=' expr            # statAssign
    ;

expr
    : expr ('*'|'/') expr    # exprMulDiv
    | expr ('+'|'-') expr    # exprAddSub
    | '(' expr ')'           # exprPar
    | '-'? NUM               # exprNum
    | ID                     # exprId
    ;

// scanner

ADD  : '+' ;
SUB  : '-' ;
MUL  : '*' ;
DIV  : '/' ;
LPAR : '(' ;
RPAR : ')' ;
NUM  : [0-9]+('.'[0-9]+)? ;
ID   : [a-zA-Z_][a-zA-Z_0-9]* ;

WS   : [ \t\n\r]+ -> skip ;

