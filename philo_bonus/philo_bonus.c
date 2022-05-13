/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 14:56:48 by zait-sli          #+#    #+#             */
/*   Updated: 2022/05/13 23:37:35 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// void	*ft_test(t_info *info)
// {
// 	if (info->vars->fork_id[info->philo_id - 1] == 0)
// 	{
// 	while(!info->vars->one_dead)
// 	{
// 		// philo_is_dead_check(info);
// 		if (info->vars->one_dead)
// 			exit(1) ;
// 		pthread_mutex_lock(info->vars->mutex);
// 		if (check_left_fork_aviability(info) && info->forks_held == 1)
// 			take_left_fork(info);
// 		else if (check_right_fork_aviability(info) && check_left_fork_aviability(info))
// 			take_right_fork(info);
// 		else if (check_right_fork_aviability(info) && info->vars->number_of_forks == 1)
// 			take_right_fork(info);
// 		pthread_mutex_unlock(info->vars->mutex);
// 		if (info->forks_held == 2)
// 			philo_started_eating(info);
// 		else if (info->forks_held == 0 && info->times_eaten == 0)
// 			philo_is_dead_check(info);
// 		else if (info->forks_held == 1)
// 			philo_is_dead_check(info);
// 	}
// 	}
// 	return (NULL);
// }

void	*ft_test(t_info *info)
{
	while(!info->vars->one_dead)
	{
		if (info->vars->one_dead)
			exit(0) ;
		sem_wait(info->vars->sem_forks);
		if (check_left_fork_aviability(info) && info->forks_held == 1  && info->vars->number_of_forks >= 1)
			take_left_fork(info);
		else if (check_right_fork_aviability(info) && check_left_fork_aviability(info) && info->vars->number_of_forks >= 2)
			take_right_fork(info);
		else if (check_right_fork_aviability(info) && info->vars->number_of_forks == 1 && info->vars->number_of_forks >= 1)
			take_right_fork(info);
		sem_post(info->vars->sem_forks);
		if (info->forks_held == 2)
			philo_started_eating(info);
		philo_is_dead_check(info);
	}
	return (NULL);
}

int main (int ac, char **av)
{
	t_info **info;
	t_vars *vars;


	vars = malloc(sizeof(t_vars)); 
	// vars->mutex = malloc(sizeof(pthread_mutex_t));
	// vars->sem_msg = malloc(sizeof(sem_t));
	vars->sem_forks = malloc(sizeof(sem_t));
	init_vars(ac, av, vars);
	info = malloc(sizeof(t_info *));
	vars->th = malloc(sizeof(pthread_t) * vars->nb_of_philos);
	vars->fork_id = malloc(sizeof(int) * vars->nb_of_philos);
	// pthread_mutex_init(vars->mutex,NULL);
	// sem_unlink("sem1");
	vars->sem_forks = sem_open("forks", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
	// vars->sem_msg = sem_open("msg", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
	vars->i = -1;
	while(vars->i + 1 < vars->nb_of_philos)
	{
		vars->i += 1;
		vars->fork_id[vars->i] = fork(); 
		if (vars->fork_id[vars->i] == 0)
		{
			//function
			info[vars->i] = malloc(sizeof(t_info));
			init_philo_info(info[vars->i],vars);
			ft_test(info[vars->i]);
			// pthread_create(&vars->th[vars->i], NULL, (void *)ft_test, info[vars->i]);
		}
	}
	vars->i = 0;
	while(vars->i < vars->nb_of_philos)
	{
		if (vars->fork_id[vars->i] == 0)
		{
			pthread_join(vars->th[vars->i], NULL);
		}
		vars->i++;
	}
	// while(1)
	// {
	// 	if (vars->one_dead)
	// 	{
	// 		detach_all(vars);
	// 		exit(0);
	// 		break ;
	// 	}
	// }
	// sem_unlink("msg");
	sem_unlink("forks");
	waitpid(-1, NULL, 0);
}
