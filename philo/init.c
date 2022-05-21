/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 17:21:32 by zait-sli          #+#    #+#             */
/*   Updated: 2022/05/21 00:08:26 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_vars(int ac, char **av, t_vars *vars)
{
	int	i;

	if (ac == 5 || ac == 6)
	{
		if (!check_isdigit(ac - 1, av) || !check_int(ac - 1, av))
			return (0);
		vars->nb_of_philos = atoi(av[1]);
		vars->number_of_forks = vars->nb_of_philos;
		vars->time_to_die = atoi(av[2]);
		vars->time_to_eat = ft_atoi(av[3]);
		vars->time_to_sleep = atoi(av[4]);
		vars->starting_time = get_msec_time();
		vars->f_mutex = malloc(sizeof(pthread_mutex_t) * vars->nb_of_philos);
		i = -1;
		while (++i < vars->nb_of_philos)
		{
			pthread_mutex_init(&vars->f_mutex[i], NULL);
		}
		vars->tab_forks = malloc(sizeof(int) * vars->number_of_forks);
		vars->one_dead = FALSE;
		vars->number_of_times_each_philo_must_eat = 100000000;
		vars->nb_of_phs_eaten = 0;
		i = -1;
		while (++i < vars->nb_of_philos)
			vars->tab_forks[i] = 0;
		if (ac == 6)
			vars->number_of_times_each_philo_must_eat = atoi(av[5]);
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
	info->philo_id = vars->i + 1;
	info->time_to_eat = get_msec_time();
	info->forks_held = 0;
	info->present_time = get_msec_time();
	info->times_eaten = 0;
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
	while (i < vars->nb_of_philos)
	{
		pthread_detach(vars->th[i]);
		i++;
	}
}