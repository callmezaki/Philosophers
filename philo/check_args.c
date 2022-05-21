/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 03:43:21 by zait-sli          #+#    #+#             */
/*   Updated: 2022/05/21 23:09:50 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	else
		return (0);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	check_isdigit(int len, char **args)
{
	int	i;
	int	j;

	i = 0;
	while (++i <= len)
	{
		j = 0;
		if (!ft_strlen(args[i]))
		{
			write(1, "Error\n", 6);
			return (0);
		}
		while (args[i][j])
		{
			if ((args[i][j] == '-' || args[i][j] == '+') && j == 0)
				j++;
			if (!ft_isdigit(args[i][j]))
			{
				write(1, "Error\n", 6);
				return (0);
			}
			j++;
		}
	}
	return (1);
}

int	check_int(int len, char **args)
{
	int	i;

	i = 1;
	while (i <= len)
	{
		if (ft_atoi(args[i]) > 2147483647 || ft_atoi(args[i]) <= 0)
		{
			write(1, "Error\n", 6);
			return (0);
		}
		i++;
	}
	return (1);
}
