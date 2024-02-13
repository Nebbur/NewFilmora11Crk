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
char	*get_token_type(int type);

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
	printf("Special char\n");
	while (input[++(*i)])
	{
		if (input[*i] == ' ')
			continue ;
		if (input[*i] == '\'')
		{
			quote[S_QUOTE] = !quote[S_QUOTE];
			begin = *i + 1;
			token->type = S_QUOTE;
			while (quote[S_QUOTE] == true && input[++(*i)] && input[*i] != '\'')
				;
			token->value = ft_substr(input, begin, *i - begin);
			token = init_token(token->next);
			token->next->prev = token;
			token = token->next;
			break ;
		}
		else if (input[*i] == '\"')
		{
			quote[D_QUOTE] = !quote[D_QUOTE];
			token->type = D_QUOTE;
			begin = *i + 1;
			while (quote[D_QUOTE] == true && input[++(*i)] && input[*i] != '\"')
				;
			token->value = ft_substr(input, begin, *i - begin);
			token = init_token(token->next);
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
			token = init_token(token->next);
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
				token = init_token(token->next);
				token->next->prev = token;
				token = token->next;
				(*i)++;
			}
			else
			{
			token->type = PIPE;
			token->value = ft_strdup("|");
			token = init_token(token->next);
			token->next->prev = token;
			token = token->next;
			break ;
			}
		}
		else if (input[*i] == ';')
		{
			token->type = SEMICOLON;
			token->value = ft_strdup(";");
			token = init_token(token->next);
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
				token = init_token(token->next);
				token->value = ft_substr(input, begin, *i - begin);
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
				token = init_token(token->next);
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
				token = init_token(token->next);
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
				token = init_token(token->next);
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
				token = init_token(token->next);
				token->next->prev = token;
				token = token->next;
				(*i)++;
			}
			else
			{
				token->type = AMPERSAND;
				token->value = ft_strdup("&");
				token = init_token(token->next);
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
			token = init_token(token->next);
			token->next->prev = token;
			token = token->next;
			break ;
		}
		else if (input[*i] == '(')
		{
			token->type = PARENTHESIS;
			token->value = ft_strdup("(");
			token = init_token(token->next);
			token->next->prev = token;
			token = token->next;
			break ;
		}
		else if (input[*i] == ')')
		{
			token->type = PARENTHESIS;
			token->value = ft_strdup(")");
			token = init_token(token->next);
			token->next->prev = token;
			token = token->next;
			break ;
		}
	}
	printf("Token type %i\n", token->type);
	printf("Special char %i\n", *i);
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
		char *quote;
		if (temp->quote[S_QUOTE] == true)
			quote = "SINGLE";
		else if (temp->quote[D_QUOTE] == true)
			quote = "DOUBLE";
		else
			quote = "NONE";
		printf("%s_______________________\n", RED);
		printf("|%s Value: |%s%s%s|\n", BLUE, YELLOW, temp->value, BLUE);
		printf("%s|%s Type: %s%s\n", RED, BLUE, YELLOW, get_token_type(temp->type));
		printf("%s|%s Quote: %s%s\n", RED, BLUE, YELLOW, quote);
		printf("%s|______________________\n", RED);
		printf("%s\n", RESET);
		temp = temp->next;
	}
}

char	*get_token_type(int type)
{
	if (type == S_QUOTE)
		return ("S_QUOTE");
	else if (type == D_QUOTE)
		return ("D_QUOTE");
	else if (type == BACKSLASH)
		return ("BACKSLASH");
	else if (type == AMPERSAND)
		return ("AMPERSAND");
	else if (type == OR)
		return ("OR");
	else if (type == AND)
		return ("AND");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == SEMICOLON)
		return ("SEMICOLON");
	else if (type == REDIR_OUT)
		return ("REDIR_OUT");
	else if (type == REDIR_IN)
		return ("REDIR_IN");
	else if (type == REDIR_APPEND)
		return ("REDIR_APPEND");
	else if (type == REDIR_HEREDOC)
		return ("REDIR_HEREDOC");
	else if (type == ENV)
		return ("ENV");
	else if (type == PARENTHESIS)
		return ("PARENTHESIS");
	else if (type == WORD)
		return ("WORD");
	return ("ERROR");
}
