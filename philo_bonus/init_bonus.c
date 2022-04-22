/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 17:21:32 by zait-sli          #+#    #+#             */
/*   Updated: 2022/04/18 22:33:23 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void init_vars(int ac, char **av,t_vars *vars)
{
	int i = 0;
	if(ac == 5 || ac == 6)
	{
		vars->nb_of_philos = atoi(av[1]);
		vars->number_of_forks = vars->nb_of_philos;
		vars->time_to_die = atoi(av[2]);
		vars->time_to_eat = ft_atoi(av[3]);
		vars->time_to_sleep = atoi(av[4]);
		vars->starting_time = get_msec_time();
		vars->tab_forks = malloc(sizeof(int) * vars->number_of_forks);
		vars->one_dead = false;
		while(i < vars->nb_of_philos)
			vars->tab_forks[i++] = 0;
		if (ac == 6)
			vars->number_of_times_each_philo_must_eat = atoi(av[5]);
	}
	else
	{
		write(2, "Wrong number of args.\n",22);
		free(vars);
		exit(1);
	}
}

void	init_philo_info(t_info *info, t_vars *vars)
{
	info->vars = vars;
	info->alive = true;
	info->philo_id = vars->i + 1;
	info->times_eaten = 0;
	info->time_to_eat = get_msec_time();
	info->forks_held = 0;
	info->present_time = get_msec_time();
}

int get_msec_time(void)
{
	struct timeval time;
	long long result;
	gettimeofday(&time,NULL);
	result  = (time.tv_usec / 1000) + (time.tv_sec * 1000);
	return(result);
}

void	detach_all(t_vars *vars)
{
	int i = 0;

	while(i < vars->nb_of_philos)
	{
		pthread_detach(vars->th[i]);
		i++;
	}
}