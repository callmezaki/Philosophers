/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 14:56:48 by zait-sli          #+#    #+#             */
/*   Updated: 2022/05/21 00:55:03 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_check_death(t_info *info)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < info[0].vars->nb_of_philos)
		{
			if (philo_is_dead_check(&info[i]) == 0
				|| info->vars->nb_of_phs_eaten == info[0].vars->nb_of_philos)
			{
				detach_all(info[0].vars);
				return (NULL);
			}
		}
	}
	return (NULL);
}

void	*ft_test(t_info *info)
{
	int	time;

	if (info->philo_id % 2 == 0)
		usleep(60);
	while (!info->vars->one_dead)
	{
		pthread_mutex_lock(&info->vars->f_mutex[info->philo_id - 1]);
		time = get_msec_time() - info->vars->starting_time;
		{
			pthread_mutex_lock(info->vars->mutex);
			printf("%d  philo %d has taken a right fork\n", time, info->philo_id);
			pthread_mutex_unlock(info->vars->mutex);
			pthread_mutex_lock(&info->vars->f_mutex[info->philo_id % info->vars->nb_of_philos]);
		}
		time = get_msec_time() - info->vars->starting_time;
		{
			pthread_mutex_lock(info->vars->mutex);
			printf("%d  philo %d has taken a left fork\n", time, info->philo_id);
			pthread_mutex_unlock(info->vars->mutex);
		}
		time = get_msec_time() - info->vars->starting_time;
		{
			pthread_mutex_lock(info->vars->mutex);
			info->times_eaten++;
			printf("%d  philo %d start eating\n", time, info->philo_id);
			pthread_mutex_unlock(info->vars->mutex);
		}
		info->time_to_eat = get_msec_time();
		if (info->times_eaten == info->vars->number_of_times_each_philo_must_eat)
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
		{
			pthread_mutex_unlock(&info->vars->f_mutex[info->philo_id % info->vars->nb_of_philos]);
			pthread_mutex_unlock(&info->vars->f_mutex[info->philo_id - 1]);
		}
		info->time_to_sleep = get_msec_time();
		time = get_msec_time() - info->vars->starting_time;
		{
			pthread_mutex_lock(info->vars->mutex);
			printf("%d  philo %d start sleeping\n", time, info->philo_id);
			pthread_mutex_unlock(info->vars->mutex);
		}
		while (1)
		{
			if (get_msec_time() >= info->time_to_sleep + info->vars->time_to_sleep)
				break ;
			usleep(100);
		}
		time = get_msec_time() - info->vars->starting_time;
		pthread_mutex_lock(info->vars->mutex);
		printf("%d  philo %d thinking\n", time, info->philo_id);
		pthread_mutex_unlock(info->vars->mutex);
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_info	*info;
	t_vars	*vars;

	vars = malloc(sizeof(t_vars));
	vars->mutex = malloc(sizeof(pthread_mutex_t));
	if (!init_vars(ac, av, vars))
		return (0);
	info = malloc(sizeof(t_info) * vars->nb_of_philos);
	vars->th = malloc(sizeof(pthread_t) * vars->nb_of_philos);
	pthread_mutex_init(vars->mutex, NULL);
	vars->i = -1;
	while (++(vars->i) < vars->nb_of_philos)
	{
		init_philo_info(&info[vars->i],vars);
		pthread_create(&vars->th[vars->i], NULL, (void *)ft_test, &info[vars->i]);
	}
	pthread_create(&vars->death, NULL, (void *)ft_check_death, info);
	pthread_join(vars->death, NULL);
	pthread_mutex_destroy(vars->mutex);
	while (vars->i < vars->nb_of_philos)
	{
		pthread_mutex_destroy(&vars->f_mutex[vars->i]);
		vars->i++;
	}
	vars->i = 0;
	free(info);
	free(vars->th);
	free(vars->mutex);
	free(vars);
	// while(1);
}
