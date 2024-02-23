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
char	*get_token_type(int type);
int	ft_isnot_theend(char *input, int i);
int	ft_isnotspace(char c);
bool	is_same_word(char *input, int i);
void remove_last_node(t_token **head) ;

static bool	is_special_char(char c)
{
	if (c == '|' || c == ';' || c == '\'' || c == '\"' || \
	c == '>' || c == '<' || c == '$' || c == '&' || \
	c == '(' || c == ')' || c == '\\')
		return (true);
	return (false);
}

void	handle_backslash(t_token *token, char *input, int *i, char *special_char[2])
{
	int	begin;
	int	j;
	int	count_char;
	bool	trig[2];

	token->type = BACKSLASH;
	trig[1] = false;
	if (input[*i + 1] == '\0')
		token->error = 1;		// Error: Backslash isn't followed by anything
	if (*i % 2 == 0)
		trig[0] = true;
	else
		trig[0] = false;

	if (ft_strchr(*special_char, input[*i + 1]) != NULL)
	{
		begin = *i;
		count_char = 0;
		j = -1;
		while (input[++j] && input[j] > 32 && trig[1] == false)
		{
			if (input[j] == '\\')
			{
				if(j % 2 == 0 && input[j + 2] && input[j + 2] != '\\')
					trig[1] = !trig[1] ;
				j++;
				if (ft_strchr(*special_char, input[*i]) != NULL)
					count_char++;
			}
			else
				count_char++;
		}
		token->value = ft_calloc(1, sizeof(char *) * (count_char + 1));;
		trig[1] = false;
		(*i)--;
		while (input[++(*i)] && input[*i] > 32 && trig[1] == false)
		{
			if (input[*i] == '\\')
			{
				if (trig[0] == true)
					if(*i % 2 == 0 && input[*i + 2] && input[*i + 2] != '\\')
						trig[1] = !trig[1] ;
				if (trig[0] == false)
					if(*i % 2 != 0 && input[*i + 2] && input[*i + 2] != '\\')
						trig[1] = !trig[1] ;
				if (ft_strchr(*special_char, input[++(*i)]) != NULL)
					token->value = ft_strjoin(token->value, ft_substr(input, *i, 1));
				else if (*special_char[1] == '1')
					token->value = ft_strjoin(token->value, ft_substr(input, (*i), 1));
			}
			else
				token->value = ft_strjoin(token->value, ft_substr(input, *i, 1));
		}
		(*i)--;
		if (is_same_word(input, (*i)) == true)
			token->same_word = true;
	}
	else
		printf("Error: Backslash isn't followed by anything\n");
}

bool	is_same_word(char *input, int i)
{
	if (input[i] && input[i + 1] && (input[i + 1] == '\"' || input[i + 1] == '\'') || \
	ft_isnotspace(input[i + 1]) == 1)
		return (true);
	else if (input[i] && input[i + 1] && input[i + 1] == '\\' && input[i + 2] && \
	(input[i + 2] == '\'' || input[i + 2] == '\\' || input[i + 2] == '\"'))
		return (true);
	return (false);
}

int	ft_isnotspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r')
		return (0);
	return (1);
}

int	ft_isnot_theend(char *input, int i)
{
	if (input[i] && input[i + 1] && \
	input[i] == '\\' && input[i + 1] != '\"')
	{
		if (input[i + 2] && input[i + 2] == '\"')
			return (3);
		return (2);
	}
	return (1);
}

void	handle_backslah_in_quote(t_token *token, char *input, int *i_beg[2], char quote_char)
{
	char	*temp;
	char	*temp2;
	int		j;
	int		k;

	j = -1;
	while (token->value[++j])
		;
	if (token->value[j - 1] == quote_char)
		token->value[j - 1] = '\0';
	j = -1;
	while (token->value[++j] && quote_char == '\"' && token->value[j] != quote_char)
	{
		if (token->value[j + 1] && token->value[j] == '\\')
		{
			temp = ft_calloc(1, sizeof(char *) * (ft_strlen(token->value) + 1));
			temp = ft_substr(token->value, 0, j);
			k = 0;
			while (token->value[j + k] && token->value[j + k + 1] && \
			ft_isnotspace(token->value[j + k]))
				k = k + ft_isnot_theend(token->value, j + k);
			temp2 = ft_strdup(token->value + j + k);
			handle_backslash(token, token->value, &j, (char *[]){"\\\"$`", "1"});
			temp = ft_strjoin(temp, token->value);
			token->value = ft_strdup(temp);
			token->value = ft_strjoin(token->value, temp2);
			free(temp);
			free(temp2);
		}
	}
}

void handle_quote(int quote_type, int *i, char *input, t_token *token)
{
	char	quote_char;
	int		begin;
	int		j;
	char	*temp;
	int		len;

	len = 0;
	token->type = quote_type;
	begin = *i + 1;
	quote_char = input[*i];
	if (input[*i + 1] && input[*i + 1] == quote_char && input[*i] == quote_char)
	{
		token->type = EMPTY;
		(*i) = *i + 2;
		return ;
	}
	if (quote_char == '\'' && input[*i] == quote_char)
	{
		while (input[++(*i)] && input[*i] != quote_char)
			;
		token->value = ft_substr(input, begin, *i - begin);
		if (is_same_word(input, (*i)) == true)
			token->same_word = true;
		return ;
	}
	while (input[++(*i)])
	{
		if (quote_char == '\"' && input[*i + 1] && \
		input[*i] != '\\' && input[*i + 1] == quote_char)
		{
			len = 1;
			break;
		}
		else if (input[*i] && input[*i + 1] && input[*i + 2] && \
		input[*i] == '\\' && input[*i + 1] == '\\' && input[*i + 2] == quote_char)
		{
			len = 3;
			break ;
		}
	}
	token->value = ft_substr(input, begin, (*i  + len) - begin);
	if (quote_char == '\"')
	{
		handle_backslah_in_quote(token, token->value, (int *[]){i, &begin}, quote_char);
		(*i) = *i + len;
	}
	if (is_same_word(input, (*i)) == true)
		token->same_word = true;
	(*i)--;
}

void	delete_node(t_token *token)
{
	t_token	*temp;

	temp = token;
	if (temp != NULL)
	{
		temp->quote[S_QUOTE] = false;
		temp->quote[D_QUOTE] = false;
		free(temp->value);
		free(temp);
		temp = NULL;
	}
}

t_token	*special_char(char *input, t_token *token, int *i)
{
	int	begin;

	if (input[*i] == ' ')
		(*i)++;
	if (input[*i] == '\'' || input[*i] == '\"')
	{
		if (input[*i] == '\'')
		{
			token->quote[S_QUOTE] = !token->quote[S_QUOTE];
			if (token->quote[S_QUOTE] == false)
			{
				if (input[*i + 1] == '\0')
						token = token->prev;
				return (token);
			}
			handle_quote(S_QUOTE, i, input, token);
		}
		else
		{
			token->quote[D_QUOTE] = !token->quote[D_QUOTE];
			if (token->quote[D_QUOTE] == false)
			{
				if (token->value == NULL)
					token = token->prev;
				return (token);
			}
			handle_quote(D_QUOTE, i, input, token);
		}
	}
	else if (input[*i] == '$')
	{
		begin = *i;
		while (input[++(*i)] && input[*i] != ' ')
			;
		token->type = ENV;
		token->value = ft_substr(input, begin, *i - begin);
	}
	else if (input[*i] == '|')
	{
		if (input[*i + 1] == '|')
		{
			token->type = OR;
			token->value = ft_strdup("||");
			(*i)++;
		}
		else
		{
		token->type = PIPE;
		token->value = ft_strdup("|");
		}
	}
	else if (input[*i] == ';')
	{
		token->type = SEMICOLON;
		token->value = ft_strdup(";");
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
		}
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
		}
	}
	else if (input[*i] == '&')
	{
		if (input[*i + 1] == '&')
		{
			token->type = AND;
			token->value = ft_strdup("&&");
			(*i)++;
		}
		else
		{
			token->type = AMPERSAND;
			token->value = ft_strdup("&");
		}
	}
	else if (input[*i] == '\\')
		handle_backslash(token, input, i, (char *[]){"\\\"\'$&|<>();?*!`", "0"});
	else if (input[*i] == '(')
	{
		token->type = PARENTHESIS;
		token->value = ft_strdup("(");
	}
	else if (input[*i] == ')')
	{
		token->type = PARENTHESIS;
		token->value = ft_strdup(")");
	}
	return (token);
}

void remove_last_node(t_token **head)
{
	t_token	*current;

	if (*head == NULL)
		return;
	current = *head;
	if (current->next == NULL)
	{
		free(current->value);
		free(current);
		*head = NULL;
	}
	else
	{
		while (current->next != NULL)
			current = current->next;
		current->prev->next = NULL;
		free(current->value);
		free(current);
	}
}

int	lexical(char *input , t_shell *shell)
{
	t_token	*token;
	int		i;
	int		j;

	j = -1;
	i = -1;
	token = shell->token;
	token->quote[S_QUOTE] = false;
	token->quote[D_QUOTE] = false;
	while (input[++i])
	{
		while (input[i] == ' ')
			i++;
		if (input[i] == '\0')
			break ;
		if (is_special_char(input[i]) == true)
		{
			token = special_char(input, token, &i);
			token->next = (t_token *)malloc(sizeof(t_token));
			token->next->prev = token;
			token->next->quote[S_QUOTE] = token->quote[S_QUOTE];
			token->next->quote[D_QUOTE] = token->quote[D_QUOTE];
			token = token->next;
		}
		else
		{
			token->type = WORD;
			token->value = ft_calloc(1, sizeof(char *));
			while (input[i] && input[i] > 32 && is_special_char(input[i]) == false)
				token->value = ft_strjoin(token->value, ft_substr(input, i++, 1));
			i--;
			if (is_same_word(input, i) == true)
				token->same_word = true;
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
	while (temp->next)
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
		printf("%s|%s Same word: %s%s\n", RED, BLUE, YELLOW, (temp->same_word == true) ? "TRUE" : "FALSE");
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
