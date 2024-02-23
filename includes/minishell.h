#ifndef MINISHELL_H
# define MINISHELL_H

# include <math.h>
# include <errno.h>
# include <signal.h>
# include <dirent.h>
# include <stdarg.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/types.h>
# include "./libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>

# define CYAN "\033[0;36m"
# define BLUE "\033[0;34m"
# define YELLOW "\033[0;33m"
# define GREEN "\033[0;32m"
# define RED "\033[0;31m"
# define RESET "\033[0m"

enum	e_TYPE_TOKEN {
	S_QUOTE = 0,
	D_QUOTE,		// 1
	BACKSLASH,		// 2
	AMPERSAND,		// 3
	OR,				// 4
	AND,			// 5
	PIPE,			// 6
	SEMICOLON,		// 7
	REDIR_OUT,		// 8
	REDIR_IN,		// 9
	REDIR_APPEND,	// 10
	REDIR_HEREDOC,	// 11
	ENV,			// 12
	PARENTHESIS,	// 13
	WORD,			// 14
	EMPTY,			// 15
};

enum	e_PIPES {
	READ = 0,
	WRITE,
};

enum	e_GLOBAL {
	INITIATE_VALUE = -1,
	INDEX,
	BREAK,
};

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_redir
{
	char	*file;
	int		type;
}	t_redir;

typedef struct s_token
{
	char			*value;
	int				type;
	bool			quote[2];
	bool			same_word;
	int				error;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_pipe
{
	char			**cmds;
	int				**pipe;
	int				fd_out;
	int				fd_in;
	int				index;
}	t_pipe;

typedef struct s_cmds
{
	int		count_cmds;
	char	**cmds;
	int		*type;
	char	*input;
}	t_cmds;

typedef struct s_shell
{
	t_env	*env;
	t_env	*exp;
	t_token	*token;
	t_cmds	*cmds;
	t_pipe	*pipe;
	t_redir	*redir;
}	t_shell;

//			Lexical
int	lexical(char *input , t_shell *shell);
t_token	*special_char(char *input, t_token *token, int *i);

//			Parser
int	parser(t_shell *shell);

//			Initialize
void	init_env(t_shell *shell, char **envp);
void	init_export(t_shell *shell, char **envp);
void	init_shell(t_shell *shell, char **envp);
t_token	*init_token(t_token *token);
t_pipe	*init_pipe(t_pipe *pipe);
t_cmds	*init_cmds(t_cmds *cmds);

//			Temp Main
void	sort_list(t_env *head);
void	free_all(t_shell *shell);
void	ft_echo(char **cmds);
char	**ft_env_to_char(t_env *env);
int	ft_exec(char **cmds, t_shell *shell);
void	ft_cd(char **cmds, t_shell *shell);
void	ft_export(char **cmds, t_shell *shell);
void	ft_unset(t_env **head, char *key);
void	ft_env(char **cmds, t_shell *shell);
int		main(int ac, char **av, char **envp);

void	print_token(t_token *token);

#endif
