/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 00:38:15 by zait-sli          #+#    #+#             */
/*   Updated: 2022/05/12 18:08:10 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <pthread.h>
#include <sys/time.h>
# define true 1
# define false 0

typedef struct s_vars
{
	int	nb_of_philos;
	int	number_of_forks;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int number_of_times_each_philo_must_eat;
	int	one_dead;
	pthread_mutex_t *mutex;
	int starting_time;
	int i;
	int *tab_forks;
	pthread_t *th;
}	t_vars;

typedef struct s_info
{
	int philo_id;
	int	forks_held;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int times_eaten;
	int present_time;
	int alive;
	
	t_vars *vars;
	
}	t_info;



int		ft_atoi(const char *str);
int		get_msec_time(void);
void	philo_sleeping(t_info *info);
void	philo_started_eating(t_info *info);
void	take_left_fork(t_info *info);
void	take_right_fork(t_info *info);
void	put_down_forks(t_info *info);
int		check_right_fork_aviability(t_info *info);
int		check_left_fork_aviability(t_info *info);
void	philo_is_dead_check(t_info *info);
void init_vars(int ac, char **av,t_vars *vars);
void	init_philo_info(t_info *info, t_vars *vars);
void	detach_all(t_vars *vars);
void print_think(t_info *info);

#endif