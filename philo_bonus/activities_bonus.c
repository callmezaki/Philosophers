/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 17:21:34 by zait-sli          #+#    #+#             */
/*   Updated: 2022/05/21 23:04:14 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	take_a_fork(t_info *info)
{
	int	time;

	time = get_msec_time() - info->vars->starting_time;
	{
		sem_wait(info->vars->sem_msg);
		printf("%d  %d has taken a fork\n", time, info->ph_id);
		sem_post(info->vars->sem_msg);
	}
}

void	philo_eating(t_info *info)
{
	int	time;

	time = get_msec_time() - info->vars->starting_time;
	{
		sem_wait(info->vars->sem_msg);
		printf("%d  %d is eating\n", time, info->ph_id);
		info->times_eaten++;
		sem_post(info->vars->sem_msg);
	}
	info->time_to_eat = get_msec_time();
	if (info->times_eaten == info->vars->times_phs_must_eat)
	{
		info->done_eating = TRUE;
	}
	while (1)
	{
		if (get_msec_time() >= info->time_to_eat + info->vars->time_to_eat)
			break ;
		usleep(50);
	}
}

void	philo_sleeping(t_info *info)
{
	int	time;

	time = get_msec_time() - info->vars->starting_time;
	{	
		sem_wait(info->vars->sem_msg);
		printf("%d  %d is sleeping\n", time, info->ph_id);
		sem_post(info->vars->sem_msg);
	}
	info->time_to_sleep = get_msec_time();
	while (1)
	{
		if (get_msec_time() >= info->time_to_sleep
			+ info->vars->time_to_sleep)
			break ;
		usleep(50);
	}
}

void	philo_thinking(t_info *info)
{
	int	time;

	time = get_msec_time() - info->vars->starting_time;
	{	
		sem_wait(info->vars->sem_msg);
		printf("%d  %d is thinking\n", time, info->ph_id);
		sem_post(info->vars->sem_msg);
	}
}

int	philo_is_dead_check(t_info *info)
{
	int	time;
	int	t;

	if (info->vars->done == TRUE)
		return (0);
	time = get_msec_time() - info->vars->starting_time;
	t = get_msec_time() - info->time_to_eat;
	if (t >= info->vars->time_to_die)
	{
		sem_wait(info->vars->sem_msg);
		printf("%d  %d is died\n", t, info->ph_id);
		info->vars->done = TRUE;
		return (0);
	}
	return (1);
}
