/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 14:56:48 by zait-sli          #+#    #+#             */
/*   Updated: 2022/04/13 00:32:57 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

void init_vars(int ac, char **av,t_vars *vars)
{
	if(ac == 5 || ac == 6)
	{
		vars->nb_of_philos = atoi(av[1]);
		vars->number_of_forks = vars->nb_of_philos;
		vars->time_to_die = atoi(av[2]);
		vars->time_to_eat = ft_atoi(av[3]);
		vars->time_to_sleep = atoi(av[4]);
		vars->starting_time = get_msec_time();
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
	info->alive = true;
	info->philo_id = vars->i + 1;
	info->forks_held = 0;
	info->present_time = get_msec_time();
	info->vars = vars;
}

int get_msec_time(void)
{
	struct timeval time;
	long long result;
	gettimeofday(&time,NULL);
	result  = (time.tv_usec / 1000) + (time.tv_sec * 1000);
	return(result);
}

void	take_a_fork(t_info *info)
{
	int time;
	if (info->vars->number_of_forks)
	{
		time = get_msec_time() - info->vars->starting_time;
		pthread_mutex_lock(info->vars->mutex);
		info->forks_held++;
		info->vars->number_of_forks--;	
		printf("%d  philo %d has taken a fork\n",time ,info->philo_id);
		pthread_mutex_unlock(info->vars->mutex);
	}
}

void	philo_started_eating(t_info *info)
{
	int time;
	time = get_msec_time() - info->vars->starting_time;
	pthread_mutex_lock(info->vars->mutex);
	info->forks_held++;
	printf("%d  philo %d started eating\n",time ,info->philo_id);
	pthread_mutex_unlock(info->vars->mutex);
	usleep(info->vars->time_to_eat * 1000);
	time = get_msec_time() - info->vars->starting_time;
	pthread_mutex_lock(info->vars->mutex);
	info->forks_held = 0;
	info->vars->number_of_forks=+2;
	printf("%d  philo %d is sleeping\n",time ,info->philo_id);
	pthread_mutex_unlock(info->vars->mutex);
	usleep(info->vars->time_to_sleep * 1000);
}

void	*ft_test(t_info *info)
{
	while(1)
	{
	take_a_fork(info);
	if (info->forks_held == 2)
		philo_started_eating(info);
	// pthread_mutex_lock(info->vars->mutex);
	// pthread_mutex_unlock(info->vars->mutex);
	}
	return (NULL);
}

int main (int ac, char **av)
{
	t_info **info;
	t_vars *vars;
	pthread_t *th;

	vars = malloc(sizeof(t_vars)); 
	vars->mutex = malloc(sizeof(pthread_mutex_t));
	init_vars(ac, av, vars);
	info = malloc(sizeof(t_info *));
	th = malloc(sizeof(pthread_t) * vars->nb_of_philos);
	pthread_mutex_init(vars->mutex,NULL);
	vars->i = -1;
	while(vars->i + 1 < vars->nb_of_philos)
	{
		vars->i += 1;
		info[vars->i] = malloc(sizeof(t_info) * vars->nb_of_philos);
		init_philo_info(info[vars->i],vars);
		pthread_create(&th[vars->i], NULL, (void *)ft_test, info[vars->i]);
	}
	vars->i = 0;
	while(vars->i < vars->nb_of_philos)
	{
		pthread_join(th[vars->i], NULL);
		vars->i++;
	}
	pthread_mutex_destroy(vars->mutex);
}
