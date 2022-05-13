/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 17:21:34 by zait-sli          #+#    #+#             */
/*   Updated: 2022/05/13 21:45:23 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void philo_is_dead_check(t_info *info)
{
	int time;
	int t;

	if (info->vars->one_dead == true)
		exit(0) ;
	time = get_msec_time() - info->vars->starting_time;
	t = get_msec_time() - info->time_to_eat;
	if (get_msec_time() >= info->time_to_eat + info->vars->time_to_die)
	{
		printf("%d  philo %d is dead\n",t ,info->philo_id);
		info->vars->one_dead = true;
		exit(0);
	}
}

void print_think(t_info *info)
{
	int time;
	if (info->vars->one_dead)
			return ;
	time = get_msec_time() - info->vars->starting_time;
	printf("%d  philo %d is thinking\n",time ,info->philo_id);
}

void	philo_sleeping(t_info *info)
{
	int time;
	if (info->vars->one_dead)
			return ;
	info->time_to_sleep = get_msec_time();
	time = get_msec_time() - info->vars->starting_time;
	sem_wait(info->vars->sem_forks);
	info->forks_held = 0;
	info->vars->number_of_forks=+2;
	if(info->philo_id == info->vars->nb_of_philos)
	{
		info->vars->tab_forks[info->philo_id - 1] = 0;
		info->vars->tab_forks[0] = 0;
	}
	else
	{
		info->vars->tab_forks[info->philo_id - 1] = 0;
		info->vars->tab_forks[info->philo_id] = 0;
	}
	printf("%d  philo %d is sleeping\n",time ,info->philo_id);
	sem_post(info->vars->sem_forks);
	while(1)
	{
		if (get_msec_time()  >= info->time_to_sleep + info->vars->time_to_sleep)
			break ;
		usleep(100);
		philo_is_dead_check(info);
	}
	// philo_is_dead_check(info);
	print_think(info);
}

void record_last_eaten(t_info *info)
{
	info->time_to_eat = get_msec_time();
}

void	philo_started_eating(t_info *info)
{
	int time;
	if (info->vars->one_dead)
			return ;
	usleep(300);
	record_last_eaten(info);
	time = get_msec_time() - info->vars->starting_time;
	printf("%d  philo %d started eating\n",time ,info->philo_id);
	sem_wait(info->vars->sem_forks);
	info->times_eaten++;
	sem_post(info->vars->sem_forks);
	while(1)
	{
		if (get_msec_time()  >= info->time_to_eat + info->vars->time_to_eat)
			break ;
		usleep(100);
		philo_is_dead_check(info);
	}
	// philo_is_dead_check(info);
	philo_sleeping(info);
}