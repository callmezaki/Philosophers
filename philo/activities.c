/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zait-sli <zait-sli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 17:21:34 by zait-sli          #+#    #+#             */
/*   Updated: 2022/05/20 22:00:04 by zait-sli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int philo_is_dead_check(t_info *info)
{
	int time;
	int t;
	

	if (info->vars->one_dead == TRUE)
		return(0);
	t = get_msec_time() - info->time_to_eat;
	time = get_msec_time() - info->vars->starting_time;
	if (t >= info->vars->time_to_die)
	{
		printf("%d  philo %d is dead\n",t ,info->philo_id);
		info->vars->one_dead = TRUE;
		return(0);
	}
	return(1);
}
