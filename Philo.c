/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 14:56:48 by zait-sli          #+#    #+#             */
/*   Updated: 2022/04/13 23:46:36 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

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

void	take_right_fork(t_info *info)
{
	int time;
	pthread_mutex_lock(info->vars->mutex);
	if (info->vars->number_of_forks >=1 && info->forks_held < 2)
	{
		time = get_msec_time() - info->vars->starting_time;
		info->forks_held++;
		info->vars->number_of_forks--;
		printf("%d  philo %d has taken a right fork\n",time ,info->philo_id);
		if(info->philo_id == info->vars->nb_of_philos)
			info->vars->tab_forks[0] = 1;
		else
			info->vars->tab_forks[info->philo_id] = 1;
	}
	pthread_mutex_unlock(info->vars->mutex);
	if (check_left_fork_aviability(info))
		take_left_fork(info);
}

void	take_left_fork(t_info *info)
{
	int time;
	pthread_mutex_lock(info->vars->mutex);
	if (info->vars->number_of_forks >=1 && info->forks_held < 2)
	{
		time = get_msec_time() - info->vars->starting_time;
		info->forks_held++;
		info->vars->number_of_forks--;
		if(info->philo_id == info->vars->nb_of_philos)
			info->vars->tab_forks[info->philo_id - 1] = 1;
		else
			info->vars->tab_forks[info->philo_id - 1] = 1;
		printf("%d  philo %d has taken a left fork\n",time ,info->philo_id);
	}
	pthread_mutex_unlock(info->vars->mutex);
}

void	philo_sleeping(t_info *info)
{
	int time;
	time = get_msec_time() - info->vars->starting_time;
	pthread_mutex_lock(info->vars->mutex);
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
	pthread_mutex_unlock(info->vars->mutex);
	usleep(info->vars->time_to_sleep * 1000);
}

void	philo_started_eating(t_info *info)
{
	int time;
	time = get_msec_time() - info->vars->starting_time;
	printf("%d  philo %d started eating\n",time ,info->philo_id);
	usleep(info->vars->time_to_eat * 1000);
	philo_sleeping(info);
}

int	check_right_fork_aviability(t_info *info)
{
	int i = 0;
	pthread_mutex_lock(info->vars->mutex);
	if(info->philo_id == info->vars->nb_of_philos)
	{
		if (info->vars->tab_forks[0] == 0)
			i = 1;
	}
	else
	{
		if(info->vars->tab_forks[info->philo_id] == 0)
			i = 1;
	}
	pthread_mutex_unlock(info->vars->mutex);
	return(i);
}

int	check_left_fork_aviability(t_info *info)
{
	int i = 0;
	pthread_mutex_lock(info->vars->mutex);
	if (info->vars->tab_forks[info->philo_id - 1] == 0)
		i = 1;
	pthread_mutex_unlock(info->vars->mutex);
	return(i);
}

void	*ft_test(t_info *info)
{
	while(!info->vars->one_dead)
	{
		if (check_left_fork_aviability(info) && info->forks_held == 1)
			take_left_fork(info);
		if (check_right_fork_aviability(info) && check_left_fork_aviability(info))
			take_right_fork(info);
		if (check_right_fork_aviability(info))
			take_right_fork(info);
		if (info->forks_held == 2)
			philo_started_eating(info);
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
