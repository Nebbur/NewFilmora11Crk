/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_until_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 20:17:05 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/05 20:17:05 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char*	ft_until_char(const char* str, char caractere) 
{
	size_t	comprimentoSubstring;
	char	*substring;
	const char	*posicaoCaractere;

	if (str == NULL)
		return NULL;
	posicaoCaractere = ft_strchr(str, caractere);
	if (posicaoCaractere != NULL)
		comprimentoSubstring = (size_t)(posicaoCaractere - str);
	else
		comprimentoSubstring = ft_strlen(str);
	substring = (char*)malloc(comprimentoSubstring + 1);
	ft_strncpy(substring, str, comprimentoSubstring);
	substring[comprimentoSubstring] = '\0';

	return (substring);
}