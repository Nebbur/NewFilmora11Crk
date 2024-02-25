# To-Do

### Exemplo de entrada:
```bash
ls|wc -l > redir_out >> 'append';(cd srcs/ && ls -a)&&"e""c"'h'o "ok"||'echo' '"ko"';echo "'end'"; cat append && echo "$$$?"
```

```c
typedef struct s_token {
    char *value;
    int type; 
    int delimiter; // NONE, SIMPLE, DOUBLE, PARENTHESIS
	bool same_word;
    struct s_token *next;
} t_token;
```
Type:
- WORD
- PIPE (|)
- OR (||)
- AND (&&)
- SEMICOLON (;)
- PARENTHESIS (())
- BACKSLASH (\\)
- APPEND (>>)
- HEREDOC (<<)
- REDIRECT_IN (<)
- REDIRECT_OUT (>)
- ENV ($)

## 1. Criar a lista de tokens

| Value             | Type            | Delimiter   | same_word   |
|-------------------|-----------------|-------------|-------------|
| ls                | WORD            | NONE        | FALSE		  |
| \|                | PIPE            | NONE        | FALSE		  |
| wc                | WORD            | NONE        | FALSE		  |
| -l                | WORD            | NONE        | FALSE		  |
| >					| REDIR_OUT 	  | NONE 		| FALSE		  |
| redir_out			| WORD		 	  | NONE 		| FALSE		  |
| >>				| APPEND	 	  | NONE 		| FALSE		  |
| append			| WORD		 	  | SIMPLE 		| FALSE		  |
| ;                 | SEMICOLON       | NONE        | FALSE		  |
| (cd srcs/ && ls -a) | WORD       | PARENTHESIS | FALSE		  |
| &&                | AND             | NONE        | FALSE		  |
| e			        | WORD            | DOUBLE      | TRUE		  |
| c			        | WORD            | DOUBLE      | TRUE		  |
| h			        | WORD            | SIMPLE      | TRUE		  |
| o			        | WORD            | NONE        | FALSE		  |
| ok                | WORD            | DOUBLE      | FALSE       |
| \|\|              | OR              | NONE        | FALSE		  |
| echo              | WORD            | SIMPLE      | FALSE		  |
| ko                | WORD            | DOUBLE      | FALSE		  |
| ;                 | SEMICOLON       | NONE        | FALSE		  |
| echo              | WORD            | NONE        | FALSE		  |
| end	            | WORD            | SIMPLE      | FALSE		  |
| ;                 | SEMICOLON       | NONE        | FALSE		  |
| cat	            | WORD            | NONE	    | FALSE		  |
| append			| WORD		 	  | NONE 		| FALSE		  |
| &&                | AND             | NONE        | FALSE		  |
| echo              | WORD            | NONE        | FALSE		  |
| $$$? 	            | ENV             | DOUBLE      | FALSE		  |

## 2. Processar os Tokens

Expansões e unir tokens 

| Value             | Type            | Delimiter   |
|-------------------|-----------------|-------------|
| ls                | WORD            | NONE        |
| \|                | PIPE            | NONE        |
| wc                | WORD            | NONE        |
| -l                | WORD            | NONE        |
| redir_out			| REDIR_OUT 	  | NONE 		|
| append			| APPEND	 	  | NONE 		|
| ;                 | SEMICOLON       | NONE        |
| (cd srcs/ && ls -a) | WORD       | PARENTHESIS |
| &&                | AND             | NONE        |
| echo		        | WORD            | NONE        |
| ok                | WORD            | DOUBLE      |
| \|\|              | OR              | NONE        |
| echo              | WORD            | SIMPLE      |
| ko                | WORD            | DOUBLE      |
| ;                 | SEMICOLON       | NONE        |
| echo              | WORD            | NONE        |
| end	            | WORD            | SIMPLE      |
| ;                 | SEMICOLON       | NONE        |
| cat	            | WORD            | NONE	    |
| append			| WORD		 	  | NONE 		|
| &&                | AND             | NONE        |
| echo              | WORD            | NONE        |
| (process number)	| ENV             | DOUBLE      |
| (exit status)		| ENV             | DOUBLE      |

## 3. Criar a linkelist dos processos, validar syntax

A linkedlist será separada por AND, OR e SEMICOLON

### Tabela de Nós:

| Value             | Type            | Delimiter   |
|-------------------|-----------------|-------------|
|**-------------------**|**NODE #1**		  |**-------------------**|
| ls                | WORD            | NONE        |
| \|                | PIPE            | NONE        |
| wc                | WORD            | NONE        |
| -l                | WORD            | NONE        |
| redir_out         | REDIR_OUT       | NONE        |
| append            | APPEND          | NONE        |
| ;                 | SEMICOLON       | NONE        |
|**-------------------**|**NODE #2**		  |**-------------------**|
| (cd srcs/ && ls -a) | WORD       | PARENTHESIS |
| &&                | AND             | NONE        |
|**-------------------**|**NODE #3**		  |**-------------------**|
| echo              | WORD            | NONE        |
| ok                | WORD            | DOUBLE      |
| \|\|              | OR              | NONE        |
| echo              | WORD            | SIMPLE      |
| ko                | WORD            | DOUBLE      |
| ;                 | SEMICOLON       | NONE        |
|**-------------------**|**NODE #4**		  |**-------------------**|
| echo              | WORD            | NONE        |
| end               | WORD            | SIMPLE      |
| ;                 | SEMICOLON       | NONE        |
|**-------------------**|**NODE #5**		  |**-------------------**|
| cat               | WORD            | NONE        |
| append            | WORD            | NONE        |
| &&                | AND             | NONE        |
|**-------------------**|**NODE #6**		  |**-------------------**|
| echo              | WORD            | NONE        |
| (process number)  | ENV             | DOUBLE      |
| (exit status)     | ENV             | DOUBLE      |
