/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 17:21:34 by zait-sli          #+#    #+#             */
/*   Updated: 2022/05/18 17:19:13 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void philo_is_dead_check(t_info *info)
{
	int time;
	int t;

	if (info->vars->one_dead == true)
		return ;
	usleep(60);
	t = get_msec_time() - info->time_to_eat;
	time = get_msec_time() - info->vars->starting_time;
	if (t >= info->vars->time_to_die)
	{
		printf("%d  philo %d is dead\n",t ,info->philo_id);
		info->vars->one_dead = true;
		return ;
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
	pthread_mutex_lock(info->vars->mutex);
	info->forks_held = 0;
	if(info->philo_id == info->vars->nb_of_philos)
	{
		info->vars->tab_forks[info->philo_id - 1] = 0;
		info->vars->tab_forks[0] = 0;
		pthread_mutex_unlock(info->vars->f_mutex[info->philo_id - 1]);
 		pthread_mutex_unlock(info->vars->f_mutex[0]);
	}
	else
	{
		info->vars->tab_forks[info->philo_id - 1] = 0;
		info->vars->tab_forks[info->philo_id] = 0;
		pthread_mutex_unlock(info->vars->f_mutex[info->philo_id - 1]);
		pthread_mutex_unlock(info->vars->f_mutex[info->philo_id]);
	}
	info->vars->number_of_forks++;
	info->vars->number_of_forks++;
	printf("%d  philo %d is sleeping\n",time ,info->philo_id);
	pthread_mutex_unlock(info->vars->mutex);
	while(1)
	{
		if (get_msec_time()  >= info->time_to_sleep + info->vars->time_to_sleep)
			break ;
		usleep(100);
		philo_is_dead_check(info);
	}
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
	record_last_eaten(info);
	time = get_msec_time() - info->vars->starting_time;
	printf("%d  philo %d started eating\n",time ,info->philo_id);
	pthread_mutex_lock(info->vars->mutex);
	info->times_eaten++;
	pthread_mutex_unlock(info->vars->mutex);
	usleep(200);
	while(1)
	{
		if (get_msec_time()  >= info->time_to_eat + info->vars->time_to_eat)
			break ;
		usleep(100);
		philo_is_dead_check(info);
	}
	philo_sleeping(info);
}