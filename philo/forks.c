/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 17:21:37 by zait-sli          #+#    #+#             */
/*   Updated: 2022/04/20 00:34:52 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	if (check_left_fork_aviability(info) && info->vars->number_of_forks > 0)
		take_left_fork(info);
}

void	take_left_fork(t_info *info)
{
	int time;

	if (info->vars->one_dead)
			return ;
	time = get_msec_time() - info->vars->starting_time;
	if(info->philo_id == info->vars->nb_of_philos)
		info->vars->tab_forks[0] = 0;
	else
		info->vars->tab_forks[info->philo_id] = 1;
	info->forks_held++;
	info->vars->number_of_forks--;
	printf("%d  philo %d has taken a left fork\n",time ,info->philo_id);
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