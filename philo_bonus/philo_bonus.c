/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 14:56:48 by zait-sli          #+#    #+#             */
/*   Updated: 2022/05/17 20:03:34 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_test(t_info *info)
{
	int time;
	if(info->philo_id % 2 == 0)
		usleep(1000);
	while(1)
	{
		sem_wait(info->vars->sem_forks);
		time = get_msec_time() - info->vars->starting_time;
		printf("%d philo %d has taken a fork\n",time,info->philo_id);
		sem_wait(info->vars->sem_forks);
		time = get_msec_time() - info->vars->starting_time;
		printf("%d philo %d has taken a fork\n",time,info->philo_id);
		time = get_msec_time() - info->vars->starting_time;
		printf("%d philo %d start eating\n",time,info->philo_id);
		info->time_to_eat = get_msec_time();
		while(1)
		{
			if (get_msec_time()  >= info->time_to_eat + info->vars->time_to_eat)
				break ;
			usleep(100);
		}
		info->vars->test++;
		sem_post(info->vars->sem_forks);
		sem_post(info->vars->sem_forks);
		time = get_msec_time() - info->vars->starting_time;
		printf("%d philo %d start sleping\n",time ,info->philo_id);
		info->time_to_sleep = get_msec_time();
		while(1)
		{
			if (get_msec_time()  >= info->time_to_sleep + info->vars->time_to_sleep)
				break ;
			usleep(100);
		}
		time = get_msec_time() - info->vars->starting_time;
		printf("%d philo %d thinking\n",time ,info->philo_id);
	}
	return (NULL);
}

void *ft_death(t_info *info)
{
	int t;
	int time;
	while(1)
	{
		if (info->vars->one_dead == true)
			exit(0);
		t = get_msec_time() - info->time_to_eat;
		time = get_msec_time() - info->vars->starting_time;
		if (t >= info->vars->time_to_die)
		{
			printf("%d  philo %d is dead\n",t ,info->philo_id);
			info->vars->one_dead = true;
			exit(0);
		}
	}
}

int main (int ac, char **av)
{
	t_info *info;
	t_vars *vars;
	int  j = 0;


	vars = malloc(sizeof(t_vars)); 
	// vars->mutex = malloc(sizeof(pthread_mutex_t));
	// vars->sem_msg = malloc(sizeof(sem_t));
	// vars->sem_forks = malloc(sizeof(sem_t));
	init_vars(ac, av, vars);
	info = malloc(sizeof(t_info) * vars->nb_of_philos);
	vars->th = malloc(sizeof(pthread_t) * vars->nb_of_philos);
	vars->fork_id = malloc(sizeof(int) * vars->nb_of_philos);
	// pthread_mutex_init(vars->mutex,NULL);
	// sem_unlink("msg");
	// sem_unlink("forks");
	vars->sem_forks = sem_open("forks", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, vars->nb_of_philos);
	vars->sem_msg = sem_open("msg", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
	vars->i = -1;
	vars->test = 0;
	while(vars->i + 1 < vars->nb_of_philos)
	{
		vars->i += 1;
		vars->fork_id[vars->i] = fork();
		init_philo_info(&info[vars->i],vars);
		if (vars->fork_id[vars->i] == 0)
		{
			pthread_create(&vars->th[vars->i], NULL, (void *)ft_death, &info[vars->i]);
			ft_test(&info[vars->i]);
			pthread_join(vars->th[vars->i], NULL);
		}
	}
	waitpid(-1, NULL, 0);
	while(j < vars->nb_of_philos)
	{
		kill(vars->fork_id[j],SIGKILL);
		j++;
	}
	sem_unlink("msg");
	sem_unlink("forks");
	detach_all(vars);
	// while(1);
}
