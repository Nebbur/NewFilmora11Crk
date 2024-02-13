/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	new_input(t_shell *shell)
{
	t_token	*token;
	char	*input;

	/* while (1)
	{
		input = readline("> ");
		break;
	} */
}

int	validation(t_shell *shell)
{
	t_token	*token;

	token = shell->token;
	while (token)
	{
		if (token->type == S_QUOTE || token->type == D_QUOTE)
			token->type = WORD;
		if (token->type == BACKSLASH)
			if (token->next == NULL)
				new_input(shell);
		if (token->type == PIPE || token->type == AND || token->type == OR)
		{
			if (token->next == NULL)
				new_input(shell);
			if (token->next->type == PIPE || token->next->type == AND || token->next->type == OR)
			{
				printf("Error: Operator not followed by any command\n");
				return (1);
			}
		}
		if (token->type == REDIR_OUT || token->type == REDIR_IN || token->type == REDIR_APPEND || token->type == REDIR_HEREDOC)
		{
			if (token->next == NULL || token->next->type == PIPE || token->next->type == AND || token->next->type == OR)
			{
				printf("Error: Redirection not followed by any file\n");
				return (1);
			}
		}
		token = token->next;
	}
	return (0);
}

int	parsing(t_shell *shell)
{
	t_token	*temp;
	int		i;

	temp = shell->token;
	i = 0;

	while (temp != NULL)
	{
		if (temp->type == WORD)
			i++;
		temp = temp->next;
	}
	shell->cmds = (char **)malloc(sizeof(char *) * (i + 2));
	temp = shell->token;
	i = 0;
	while (temp != NULL)
	{
		//printf("Token: %s\n", temp->value);
		if (temp->type == WORD)
		{
			shell->cmds[i] = ft_strdup(temp->value);
		//	printf("Shell cmds[%d]: %s\n", i, shell->cmds[i]);
			i++;
		}
		if (!temp->next->next)
			break ;
		temp = temp->next;
	}
	shell->cmds[i] = NULL;
	return (0);
}

int	process_tokens(t_shell *shell)
{
	t_token	*token;

	token = shell->token;
	while (token->next != NULL)
	{
		if (token->type == S_QUOTE || token->type == D_QUOTE)
		{
			if ((token->next && token->same_word == true) && \
			(token->next->type == S_QUOTE || token->next->type == D_QUOTE))
			{
				token->value = ft_strjoin(token->value, token->next->value);
				token->next = token->next->next;
				token->next->prev = token;
			}
		}
		if (token->type == BACKSLASH)
		{
			if ((token->next && token->same_word == true))
			{
				token->value = ft_strjoin(token->value, token->next->value);
				token->type = WORD;
				token->same_word = false;
				token->next = token->next->next;
				token->next->prev = token;
			}
			/* if ((token->next && token->same_word == true) && \
			(token->next->type == S_QUOTE || token->next->type == D_QUOTE || token->next->type == BACKSLASH))
			{
				token->value = ft_strjoin(token->value, token->next->value);
				token->next = token->next->next;
				token->next->prev = token;
			} */
		}
		token = token->next;
	}
}

void	print_cmds(char **cmds)
{
	int		i;

	i = 0;
	while (cmds[i])
	{
		printf("Cmds[%d]: %s\n", i, cmds[i]);
		i++;
	}
}

int	parser(t_shell *shell)
{
	if (lexical(shell->input , shell) == 1)
		return (1);
	if (process_tokens(shell) == 1)
		return (1);
	if (validation(shell) == 1)
		return (1);
	if (parsing(shell) == 1)
		return (1);
	print_token(shell->token);
	print_cmds(shell->cmds);
	return (0);
}
