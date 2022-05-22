/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 14:56:48 by zait-sli          #+#    #+#             */
/*   Updated: 2022/05/22 02:43:01 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_check_death(t_info *info)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < info[0].vars->nb_phs)
		{
			if (philo_is_dead_check(&info[i]) == 0
				|| info->vars->nb_of_phs_eaten == info[0].vars->nb_phs)
			{
				info->vars->done = TRUE;
				return (NULL);
			}
		}
	}
	return (NULL);
}

void	*ft_routine(t_info *info)
{
	if (info->ph_id % 2 == 0)
		usleep(100);
	while (1)
	{
		pthread_mutex_lock(&info->vars->f_mutex[info->ph_id - 1]);
		take_a_fork(info);
		pthread_mutex_lock(&info->vars->f_mutex[info->ph_id
			% info->vars->nb_phs]);
		take_a_fork(info);
		start_eating(info);
		philo_sleeping(info);
		philo_thinking(info);
	}
	return (NULL);
}

void	ft_funeral(t_vars *vars, t_info *info)
{
	usleep(1000);
	vars->i = 0;
	detach_all(vars);
	pthread_mutex_destroy(vars->mutex);
	while (vars->i < vars->nb_phs)
	{
		pthread_mutex_destroy(&vars->f_mutex[vars->i]);
		vars->i++;
	}
	free(info);
	free(vars->th);
	free(vars->mutex);
	free(vars->f_mutex);
	pthread_detach(vars->death);
	free(vars);
}

int	main(int ac, char **av)
{
	t_info	*info;
	t_vars	*vars;

	vars = malloc(sizeof(t_vars));
	if (!vars || !init_vars(ac, av, vars))
		return (0);
	vars->mutex = malloc(sizeof(pthread_mutex_t));
	info = malloc(sizeof(t_info) * vars->nb_phs);
	vars->th = malloc(sizeof(pthread_t) * vars->nb_phs);
	if (!vars->mutex || !info || !vars->th)
		return (0);
	pthread_mutex_init(vars->mutex, NULL);
	vars->i = -1;
	while (++(vars->i) < vars->nb_phs)
	{
		init_philo_info(&info[vars->i], vars);
		pthread_create(&vars->th[vars->i], NULL,
			(void *)ft_routine, &info[vars->i]);
	}
	pthread_create(&vars->death, NULL, (void *)ft_check_death, info);
	pthread_join(vars->death, NULL);
	if (info->vars->done == TRUE)
		ft_funeral(vars, info);
}
