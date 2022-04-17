/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 14:56:48 by zait-sli          #+#    #+#             */
/*   Updated: 2022/04/17 16:57:32 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void philo_is_dead_check(t_info *info)
{
	int time;
	int t;

	if (info->vars->one_dead == true)
		exit(0) ;
	t = get_msec_time() - info->time_to_eat;
	time = get_msec_time() - info->vars->starting_time;
	if (t >= info->vars->time_to_die)
	{
		printf("%d  philo %d is dead\n",t ,info->philo_id);
		info->vars->one_dead = true;
		// detach_all(vars);
		exit(0);
	}
}

void	take_right_fork(t_info *info)
{
	int time;
	if (info->vars->one_dead)
			return ;
	time = get_msec_time() - info->vars->starting_time;
	info->forks_held++;
	info->vars->number_of_forks--;
	printf("%d  philo %d has taken a right fork\n",time ,info->philo_id);
	info->vars->tab_forks[info->philo_id - 1] = 1;
	if (check_left_fork_aviability(info))
		take_left_fork(info);
}

void	take_left_fork(t_info *info)
{
	int time;

	if (info->vars->one_dead)
			return ;
	time = get_msec_time() - info->vars->starting_time;
	info->forks_held++;
	info->vars->number_of_forks--;
	if(info->philo_id == info->vars->nb_of_philos)
		info->vars->tab_forks[0] = 0;
	else
		info->vars->tab_forks[info->philo_id] = 1;
	printf("%d  philo %d has taken a left fork\n",time ,info->philo_id);
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


void	philo_started_eating(t_info *info)
{
	int time;
	if (info->vars->one_dead)
			return ;
	time = get_msec_time() - info->vars->starting_time;
	printf("%d  philo %d started eating\n",time ,info->philo_id);
	info->time_to_eat = get_msec_time();
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

int	check_right_fork_aviability(t_info *info)
{
	int i = 0;
	if(info->vars->tab_forks[info->philo_id - 1] == 0)
			i = 1;
	return(i);
}

int	check_left_fork_aviability(t_info *info)
{
	int i = 0;
	if (info->philo_id == info->vars->nb_of_philos)
	{
		if (info->vars->tab_forks[0] == 0)
			i = 1;
	}
	else
	{
		if (info->vars->tab_forks[info->philo_id] == 0)
			i = 1;
	}
	return(i);
}

void	*ft_test(t_info *info)
{
	while(!info->vars->one_dead)
	{
		// philo_is_dead_check(info);
		if (info->vars->one_dead)
			break ;
		pthread_mutex_lock(info->vars->mutex);
		if (check_left_fork_aviability(info) && info->forks_held == 1)
			take_left_fork(info);
		else if (check_right_fork_aviability(info) && check_left_fork_aviability(info))
			take_right_fork(info);
		else if (check_right_fork_aviability(info) && info->vars->number_of_forks == 1)
			take_right_fork(info);
		pthread_mutex_unlock(info->vars->mutex);
		if (info->forks_held == 2)
			philo_started_eating(info);
		else if (info->forks_held == 1)
			philo_is_dead_check(info);
	}
	return (NULL);
}

int main (int ac, char **av)
{
	t_info **info;
	t_vars *vars;


	vars = malloc(sizeof(t_vars)); 
	vars->mutex = malloc(sizeof(pthread_mutex_t));
	init_vars(ac, av, vars);
	info = malloc(sizeof(t_info *));
	vars->th = malloc(sizeof(pthread_t) * vars->nb_of_philos);
	pthread_mutex_init(vars->mutex,NULL);
	vars->i = -1;
	while(vars->i + 1 < vars->nb_of_philos)
	{
		vars->i += 1;
		info[vars->i] = malloc(sizeof(t_info) * vars->nb_of_philos);
		init_philo_info(info[vars->i],vars);
		pthread_create(&vars->th[vars->i], NULL, (void *)ft_test, info[vars->i]);
	}
	vars->i = 0;
	while(vars->i < vars->nb_of_philos)
	{
		pthread_join(vars->th[vars->i], NULL);
		vars->i++;
	}
	while(1)
	{
		if (vars->one_dead)
		{
			detach_all(vars);
			break ;
		}
	}
	pthread_mutex_destroy(vars->mutex);
}
