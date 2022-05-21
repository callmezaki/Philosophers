/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 14:56:48 by zait-sli          #+#    #+#             */
/*   Updated: 2022/05/21 20:17:15 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_routine(t_info *info)
{
	if (info->ph_id % 2 == 0)
		usleep(100);
	while (1)
	{
		sem_wait(info->vars->sem_forks);
		take_a_fork(info);
		sem_wait(info->vars->sem_forks);
		take_a_fork(info);
		philo_eating(info);
		sem_post(info->vars->sem_forks);
		sem_post(info->vars->sem_forks);
		philo_sleeping(info);
		philo_thinking(info);
	}
	return (NULL);
}

void	*ft_check_death(t_info *info)
{
	while (1)
	{
		if (philo_is_dead_check(info) == 0 || (info->done_eating == TRUE
				&& info->ph_id == info->vars->nb_phs))
			exit(0);
	}
	return (NULL);
}

int	global_init(int ac, char **av, t_vars *vars)
{
	if (!init_vars(ac, av, vars))
		return (0);
	vars->th = malloc(sizeof(pthread_t) * vars->nb_phs);
	vars->fork_id = malloc(sizeof(int) * vars->nb_phs);
	sem_unlink("msg");
	sem_unlink("forks");
	vars->sem_forks = sem_open("forks", O_CREAT | O_EXCL,
			S_IRUSR | S_IWUSR, vars->nb_phs);
	vars->sem_msg = sem_open("msg", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
	vars->i = -1;
	return (1);
}

void	ft_funeral(t_vars *vars, t_info *info)
{
	int		j;

	j = 0;
	while (j < vars->nb_phs)
	{
		kill(vars->fork_id[j], SIGKILL);
		j++;
	}
	detach_all(vars);
	sem_unlink("msg");
	sem_unlink("forks");
	free(vars->th);
	free(vars->fork_id);
	free(info);
	free(vars);
}

int	main(int ac, char **av)
{
	t_info	*info;
	t_vars	*vars;

	vars = malloc(sizeof(t_vars));
	if (global_init(ac, av, vars) == 0)
		return (0);
	info = malloc(sizeof(t_info) * vars->nb_phs);
	while (vars->i + 1 < vars->nb_phs)
	{
		vars->i += 1;
		vars->fork_id[vars->i] = fork();
		init_philo_info(&info[vars->i], vars);
		if (vars->fork_id[vars->i] == 0)
		{
			pthread_create(&vars->th[vars->i], NULL,
				(void *)ft_check_death, &info[vars->i]);
			ft_routine(&info[vars->i]);
			pthread_join(vars->th[vars->i], NULL);
		}
	}
	waitpid(-1, NULL, 0);
	ft_funeral(vars, info);
}
