/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 14:56:48 by zait-sli          #+#    #+#             */
/*   Updated: 2022/05/21 00:58:11 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_test(t_info *info)
{
	int	time;

	if (info->philo_id % 2 == 0)
		usleep(100);
	while (1)
	{
		sem_wait(info->vars->sem_forks);
		time = get_msec_time() - info->vars->starting_time;
		{
			sem_wait(info->vars->sem_msg);
			printf("%d philo %d has taken a fork\n", time, info->philo_id);
			sem_post(info->vars->sem_msg);
		}
		sem_wait(info->vars->sem_forks);
		time = get_msec_time() - info->vars->starting_time;
		{	
			sem_wait(info->vars->sem_msg);
			printf("%d philo %d has taken a fork\n", time, info->philo_id);
			sem_post(info->vars->sem_msg);
		}
		time = get_msec_time() - info->vars->starting_time;
			sem_wait(info->vars->sem_msg);
			printf("%d philo %d start eating\n",time,info->philo_id);
			info->times_eaten++;
			sem_post(info->vars->sem_msg);
		info->time_to_eat = get_msec_time();
		if (info->times_eaten == info->vars->number_of_times_each_philo_must_eat)
		{
			info->vars->nb_of_phs_eaten++;
			// printf("%d this one is done\n",info->philo_id);
			info->done_eating = TRUE;
		}
		while(1)
		{
			if (get_msec_time() >= info->time_to_eat + info->vars->time_to_eat)
				break ;
			usleep(100);
		}
		sem_post(info->vars->sem_forks);
		sem_post(info->vars->sem_forks);
		time = get_msec_time() - info->vars->starting_time;
			sem_wait(info->vars->sem_msg);
			printf("%d philo %d start sleeping\n",time ,info->philo_id);
			sem_post(info->vars->sem_msg);
		info->time_to_sleep = get_msec_time();
		while(1)
		{
			if (get_msec_time()  >= info->time_to_sleep + info->vars->time_to_sleep)
				break ;
			usleep(100);
		}
		time = get_msec_time() - info->vars->starting_time;
			sem_wait(info->vars->sem_msg);
			printf("%d philo %d thinking\n",time ,info->philo_id);
			sem_post(info->vars->sem_msg);
	}
	return (NULL);
}

int philo_is_dead_check(t_info *info)
{
	int time;
	int t;
	

	if (info->vars->one_dead == TRUE)
		return(0);
	time = get_msec_time() - info->vars->starting_time;
	t = get_msec_time() - info->time_to_eat;
	if (t >= info->vars->time_to_die)
	{
		printf("%d  philo %d is dead\n",t ,info->philo_id);
		info->vars->one_dead = TRUE;
		return(0);
	}
	return(1);
}

void	*ft_check_death(t_info *info)
{
	while(1)
	{
		if(philo_is_dead_check(info) == 0 || (info->done_eating == TRUE && info->philo_id == info->vars->nb_of_philos))
			exit(0);
	}
	return(NULL);
}


int main (int ac, char **av)
{
	t_info *info;
	t_vars *vars;
	int j = 0;


	vars = malloc(sizeof(t_vars)); 
	vars->sem_msg = malloc(sizeof(sem_t));
	vars->sem_forks = malloc(sizeof(sem_t));
	if (!init_vars(ac, av, vars))
		return(0);
	info = malloc(sizeof(t_info) * vars->nb_of_philos);
	vars->th = malloc(sizeof(pthread_t) * vars->nb_of_philos);
	vars->fork_id = malloc(sizeof(int) * vars->nb_of_philos);
	sem_unlink("msg");
	sem_unlink("forks");
	vars->sem_forks = sem_open("forks", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, vars->nb_of_philos);
	vars->sem_msg = sem_open("msg", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
	vars->i = -1;
	while(vars->i + 1 < vars->nb_of_philos)
	{
		vars->i += 1;
		vars->fork_id[vars->i] = fork();
		init_philo_info(&info[vars->i],vars);
		if (vars->fork_id[vars->i] == 0)
		{
			pthread_create(&vars->th[vars->i], NULL, (void *)ft_check_death, &info[vars->i]);
			ft_test(&info[vars->i]);
			pthread_join(vars->th[vars->i], NULL);
		}
	}
	waitpid(-1, NULL, 0);
	while(j < info[0].vars->nb_of_philos)
	{
		kill(info[0].vars->fork_id[j],SIGKILL);
		j++;
	}
	detach_all(vars);
	sem_unlink("msg");
	sem_unlink("forks");
	// while(1);
}
