/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 14:56:48 by zait-sli          #+#    #+#             */
/*   Updated: 2022/04/20 00:34:35 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void	*ft_test(t_info *info)
// {
// 	while(1)
// 	{
// 		if (info->vars->one_dead)
// 			break ;
// 		pthread_mutex_lock(info->vars->mutex);
// 		else if (check_right_fork_aviability(info) && check_left_fork_aviability(info) && info->vars->number_of_forks >= 2)
// 			take_right_fork(info);
// 		else if (check_right_fork_aviability(info) && info->forks_held == 0 && info->vars->number_of_forks > 0)
// 			take_right_fork(info);
// 		pthread_mutex_unlock(info->vars->mutex);
// 		// printf("------| %d |-----\n", info->vars->number_of_forks);
// 		// printf("------|%d has %d forks|-----\n", info->philo_id,info->forks_held);
// 		if (info->forks_held == 2)
// 			philo_started_eating(info);
// 		else if (info->forks_held == 1)
// 			philo_is_dead_check(info);
// 	}
// 	return (NULL);
// }

void	*ft_test(t_info *info)
{
	while(!info->vars->one_dead)
	{
		// philo_is_dead_check(info);
		if (info->vars->one_dead)
			break ;
		pthread_mutex_lock(info->vars->mutex);
		if (check_left_fork_aviability(info) && info->forks_held == 1 && info->vars->number_of_forks > 0)
			take_left_fork(info);
		// if (check_left_fork_aviability(info) && info->forks_held == 1 && info->vars->number_of_forks > 0)
		// 	take_left_fork(info);
		else if (check_right_fork_aviability(info) && check_left_fork_aviability(info) && info->vars->number_of_forks > 0)
			take_right_fork(info);
		else if (check_right_fork_aviability(info) && info->vars->number_of_forks == 1 && info->vars->number_of_forks > 0)
			take_right_fork(info);
		pthread_mutex_unlock(info->vars->mutex);
		if (info->forks_held == 2)
			philo_started_eating(info);
		else if (info->forks_held == 1)
			philo_is_dead_check(info);
		// else if (info->forks_held == 0 && info->times_eaten == 0)
		// 	philo_is_dead_check(info);
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
