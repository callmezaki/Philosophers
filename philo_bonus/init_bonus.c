/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 17:21:32 by zait-sli          #+#    #+#             */
/*   Updated: 2022/05/22 02:49:02 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	init_args(char **av, t_vars *vars)
{
	vars->nb_phs = atoi(av[1]);
	vars->number_of_forks = vars->nb_phs;
	vars->time_to_die = atoi(av[2]);
	vars->time_to_eat = ft_atoi(av[3]);
	vars->time_to_sleep = atoi(av[4]);
	vars->starting_time = get_msec_time();
	vars->times_phs_must_eat = 1000000;
}

int	init_vars(int ac, char **av, t_vars *vars)
{
	int	i;

	i = 0;
	if (ac == 5 || ac == 6)
	{
		if (!check_isdigit(ac - 1, av) || !check_int(ac - 1, av))
		{
			free(vars);
			exit(0);
		}
		init_args(av, vars);
		if (ac == 6)
			vars->times_phs_must_eat = atoi(av[5]);
	}
	else
	{
		write(2, "Wrong number of args.\n", 22);
		free(vars);
		return (0);
	}
	return (1);
}

void	init_philo_info(t_info *info, t_vars *vars)
{
	info->vars = vars;
	info->alive = TRUE;
	info->ph_id = vars->i + 1;
	info->times_eaten = 0;
	info->time_to_eat = get_msec_time();
	info->forks_held = 0;
	info->done_eating = FALSE;
	info->present_time = get_msec_time();
}

int	get_msec_time(void)
{
	struct timeval	time;
	long long		result;

	gettimeofday(&time, NULL);
	result = (time.tv_usec / 1000) + (time.tv_sec * 1000);
	return (result);
}

void	detach_all(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->nb_phs)
	{
		pthread_detach(vars->th[i]);
		i++;
	}
}
