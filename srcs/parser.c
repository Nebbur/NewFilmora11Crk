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
	t_token	*temp;
	char	*input;

	while (1)
	{
		input = readline("> ");

	}
}

int	validation(t_shell *shell)
{
	int		i;
	t_token	*token;

	token = shell->token;
	i = 0;
	while (token)
	{
		if (token->type == BACKSLASH)
		{
			if (token->next == NULL)
			{
				printf("Error: Backslash not followed by any character\n");
				return (1);
			}
		}
		if (token->type == PIPE || token->type == AND || token->type == OR)
		{
			if (token->next == NULL)
			{
				new_input(shell);
			}
			if (token->next->type == PIPE || token->next->type == AND || token->next->type == OR)
			{
				printf("Error: Operator not followed by any command\n");
				return (1);
			}
		}
		if (token->type == REDIR_OUT || token->type == REDIR_IN || token->type == REDIR_APPEND || token->type == REDIR_HEREDOC)
		{
			if (token->next == NULL)
			{
				printf("Error: Redirection not followed by any file\n");
				return (1);
			}
			if (token->next->type == PIPE || token->next->type == AND || token->next->type == OR)
			{
				printf("Error: Redirection not followed by any file\n");
				return (1);
			}
		}
		if (token->type == SEMICOLON)
		{
			if (token->next == NULL)
			{
				printf("Error: Semicolon not followed by any command\n");
				return (1);
			}
		}
	}
	return (0);
}

int	parsing(t_shell *shell)
{
	t_token	*temp;
	int		i;

	temp = shell->token;
	i = 0;
	printf("Parsing\n");
	while (temp)
	{
		printf("Token: %s\n", temp->value);
		/* if (temp->type == S_QUOTE || temp->type == D_QUOTE)
			temp->type = WORD; */
		if (temp->type == WORD)
		{
			shell->cmds[i] = ft_strdup(temp->value);
			i++;
		}
		temp = temp->next;
	}
	shell->cmds[i] = NULL;
	return (0);
}

int	parser(t_shell *shell)
{
	if (lexical(shell->input , shell) == 1)
		return (1);
	printf("Saiu do lexical\n");
	if (validation(shell) == 1)
		return (1);
	if (parsing(shell) == 1)
		return (1);
	return (0);
}
