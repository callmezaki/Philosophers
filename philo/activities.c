/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 17:21:34 by zait-sli          #+#    #+#             */
/*   Updated: 2022/05/21 22:59:22 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_is_dead_check(t_info *info)
{
	int	time;
	int	t;

	if (info->vars->done == TRUE)
		return (0);
	t = get_msec_time() - info->time_to_eat;
	time = get_msec_time() - info->vars->starting_time;
	if (t >= info->vars->time_to_die)
	{
		pthread_mutex_lock(info->vars->mutex);
		printf("%d  %d died\n", t, info->ph_id);
		info->vars->done = TRUE;
		return (0);
	}
	return (1);
}

void	take_a_fork(t_info *info)
{
	int	time;

	{
		time = get_msec_time() - info->vars->starting_time;
		pthread_mutex_lock(info->vars->mutex);
		printf("%d  %d has taken a fork\n", time, info->ph_id);
		pthread_mutex_unlock(info->vars->mutex);
	}
}

void	start_eating(t_info *info)
{
	int	time;

	{
		time = get_msec_time() - info->vars->starting_time;
		pthread_mutex_lock(info->vars->mutex);
		info->times_eaten++;
		printf("%d  %d is eating\n", time, info->ph_id);
		pthread_mutex_unlock(info->vars->mutex);
	}
	usleep(250);
	info->time_to_eat = get_msec_time();
	if (info->times_eaten == info->vars->times_phs_must_eat)
	{
		info->vars->nb_of_phs_eaten++;
		usleep(100);
	}
	while (1)
	{
		if (get_msec_time() >= info->time_to_eat + info->vars->time_to_eat)
			break ;
		usleep(100);
	}
}

void	philo_sleeping(t_info *info)
{
	int	time;

	{
		time = get_msec_time() - info->vars->starting_time;
		pthread_mutex_lock(info->vars->mutex);
		printf("%d  %d is sleeping\n", time, info->ph_id);
		pthread_mutex_unlock(info->vars->mutex);
		info->time_to_sleep = get_msec_time();
	}
	{
		pthread_mutex_unlock(&info->vars->f_mutex[info->ph_id
			% info->vars->nb_phs]);
		pthread_mutex_unlock(&info->vars->f_mutex[info->ph_id - 1]);
	}
	while (1)
	{
		if (get_msec_time() >= info->time_to_sleep
			+ info->vars->time_to_sleep)
			break ;
		usleep(100);
	}
}

void	philo_thinking(t_info *info)
{
	int	time;

	time = get_msec_time() - info->vars->starting_time;
	pthread_mutex_lock(info->vars->mutex);
	printf("%d  %d is thinking\n", time, info->ph_id);
	pthread_mutex_unlock(info->vars->mutex);
}
