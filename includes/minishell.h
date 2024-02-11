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
	D_QUOTE,		// 2
	BACKSLASH,		// 3
	AMPERSAND,		// 4
	OR,				// 5
	AND,			// 6
	PIPE,			// 7
	SEMICOLON,		// 8
	REDIR_OUT,		// 9
	REDIR_IN,		// 10
	REDIR_APPEND,	// 11
	REDIR_HEREDOC,	// 12
	ENV,			// 13
	PARENTHESIS,	// 14
	WORD,			// 15
};

enum	e_GLOBAL {
	INDEX,
};

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	char			*value;
	int				type;
	bool			quote[2];
	int				error;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_shell
{
	t_env	*env;
	t_env	*exp;
	t_token	*token;

	char **cmds;
	char *input;
}	t_shell;

//			Lexical
int	lexical(char *input , t_shell *shell);
t_token	*special_char(char *input, t_token *token, int *i, bool quote[2]);

//			Parser
int	parser(t_shell *shell);

//			Initialize
void	init_env(t_shell *shell, char **envp);
void	init_export(t_shell *shell, char **envp);
void	init_shell(t_shell *shell, char **envp);
void	init_token(t_shell *shell);

//			Temp Main
void	sort_list(t_env *head);
void	free_all(t_shell *shell);
void	ft_echo(char **cmds);
char	**ft_env_to_char(t_env *env);
void	ft_exec(char **cmds, t_shell *shell);
void	ft_cd(char **cmds, t_shell *shell);
void	ft_export(char **cmds, t_shell *shell);
void	ft_unset(t_env **head, char *key);
void	ft_env(char **cmds, t_shell *shell);
int		main(int ac, char **av, char **envp);
#endif
