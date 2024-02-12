/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_special_char(char c);
void	print_token(t_token *token);

static bool	is_special_char(char c)
{
	if (c == '|' || c == ';' || c == '\'' || c == '\"' || \
	c == '>' || c == '<' || c == '$' || c == '&' || \
	c == '(' || c == ')' || c == 92)
		return (true);
	return (false);
}

t_token	*special_char(char *input, t_token *token, int *i, bool quote[2])
{
	int 	begin;

	(*i) = *i - 1;
	while (input[++(*i)])
	{
		if (input[*i] == ' ')
			continue ;
		if (input[*i] == '\'')
		{
			quote[S_QUOTE] = !quote[S_QUOTE];
			begin = *i + 1;
			while (quote[S_QUOTE] == true && input[++(*i)] && input[*i] != '\'')
				;
			token->value = ft_substr(input, begin, *i - begin);
			token->next = (t_token *)malloc(sizeof(t_token));
			token->next->prev = token;
			token = token->next;
			break ;
		}
		else if (input[*i] == '\"')
		{
			quote[D_QUOTE] = !quote[D_QUOTE];
			begin = *i + 1;
			while (quote[D_QUOTE] == true && input[++(*i)] && input[*i] != '\"')
				;
			token->value = ft_substr(input, begin, *i - begin);
			token->next = (t_token *)malloc(sizeof(t_token));
			token->next->prev = token;
			token = token->next;
			break ;
		}
		else if (input[*i] == '$')
		{
			begin = *i;
			while (input[++(*i)] && input[*i] != ' ')
				;
			token->type = ENV;
			token->value = ft_substr(input, begin, *i - begin);
			token->next = (t_token *)malloc(sizeof(t_token));
			token->next->prev = token;
			token = token->next;
			break ;
		}
		else if (input[*i] == '|')
		{
			if (input[*i + 1] == '|')
			{
				token->type = OR;
				token->value = ft_strdup("||");
				token->next = (t_token *)malloc(sizeof(t_token));
				token->next->prev = token;
				token = token->next;
				(*i)++;
			}
			else
			{
			token->type = PIPE;
			token->value = ft_strdup("|");
			token->next = (t_token *)malloc(sizeof(t_token));
			token->next->prev = token;
			token = token->next;
			break ;
			}
		}
		else if (input[*i] == ';')
		{
			token->type = SEMICOLON;
			token->value = ft_strdup(";");
			token->next = (t_token *)malloc(sizeof(t_token));
			token->next->prev = token;
			token = token->next;
			break ;
		}
		else if (input[*i] == '>')
		{
			if (input[*i + 1] == '>')
			{
				token->type = REDIR_APPEND;
				(*i)++;
				while (input[++(*i)] && input[*i] == ' ')
					;
				begin = (*i);
				while (input[++(*i)] && input[*i] != ' ')
					;
				token->value = ft_substr(input, begin, *i - begin);
				token->next = (t_token *)malloc(sizeof(t_token));
				token->next->prev = token;
				token = token->next;
			}
			else
			{
				token->type = REDIR_OUT;
				while (input[++(*i)] && input[*i] == ' ')
					;
				begin = (*i);
				while (input[++(*i)] && input[*i] != ' ')
					;
				token->value = ft_substr(input, begin, *i - begin);
				token->next = (t_token *)malloc(sizeof(t_token));
				token->next->prev = token;
				token = token->next;
			}
			break ;
		}
		else if (input[*i] == '<')
		{
			if (input[*i + 1] == '<')
			{
				token->type = REDIR_HEREDOC;
				(*i)++;
				while (input[++(*i)] && input[*i] == ' ')
					;
				begin = (*i);
				while (input[++(*i)] && input[*i] != ' ')
					;
				token->value = ft_substr(input, begin, *i - begin);
				token->next = (t_token *)malloc(sizeof(t_token));
				token->next->prev = token;
				token = token->next;
			}
			else
			{
				token->type = REDIR_IN;
				while (input[++(*i)] && input[*i] == ' ')
					;
				begin = (*i);
				while (input[++(*i)] && input[*i] != ' ')
					;
				token->value = ft_substr(input, begin, *i - begin);
				token->next = (t_token *)malloc(sizeof(t_token));
				token->next->prev = token;
				token = token->next;
			}
			break ;
		}
		else if (input[*i] == '&')
		{
			if (input[*i + 1] == '&')
			{
				token->type = AND;
				token->value = ft_strdup("&&");
				token->next = (t_token *)malloc(sizeof(t_token));
				token->next->prev = token;
				token = token->next;
				(*i)++;
			}
			else
			{
				token->type = AMPERSAND;
				token->value = ft_strdup("&");
				token->next = (t_token *)malloc(sizeof(t_token));
				token->next->prev = token;
				token = token->next;
			}
			break ;
		}
		else if (input[*i] == 92)
		{
			token->type = BACKSLASH;
			token->value[0] = input[*i];
			token->value[1] = input[*i + 1];
			token->value[2] = '\0';
			(*i)++;
			token->next = (t_token *)malloc(sizeof(t_token));
			token->next->prev = token;
			token = token->next;
			break ;
		}
		else if (input[*i] == '(')
		{
			token->type = PARENTHESIS;
			token->value = ft_strdup("(");
			token->next = (t_token *)malloc(sizeof(t_token));
			token->next->prev = token;
			token = token->next;
			break ;
		}
		else if (input[*i] == ')')
		{
			token->type = PARENTHESIS;
			token->value = ft_strdup(")");
			token->next = (t_token *)malloc(sizeof(t_token));
			token->next->prev = token;
			token = token->next;
			break ;
		}
	}
	return (token);
}

int	lexical(char *input , t_shell *shell)
{
	t_token	*token;
	int		i;
	int		j;

	j = -1;
	i = -1;
	token = shell->token;
	while (input[++i])
	{
		token->quote[S_QUOTE] = false;
		token->quote[D_QUOTE] = false;
		while (input[i] == ' ')
			i++;
		if (input[i] == '\0')
			break ;
		if (is_special_char(input[i]) == true)
			token = special_char(input, token, &i, token->quote);
		else
		{
			token->type = WORD;
			token->value = ft_calloc(1, sizeof(char *));
			while (input[i] > 32 && is_special_char(input[i]) == false)
				token->value = ft_strjoin(token->value, ft_substr(input, i++, 1));
			token->next = (t_token *)malloc(sizeof(t_token));
			token->next->prev = token;
			token = token->next;
		}
	}
	print_token(shell->token);
	return (0);
}

void	print_token(t_token *token)
{
	t_token	*temp;

	temp = token;
	while (temp->next != NULL)
	{
		printf("Value: |%s|\n", temp->value);
		printf("Type: %d\n", temp->type);
		temp = temp->next;
	}
}

