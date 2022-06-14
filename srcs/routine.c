/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 14:35:10 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/08 15:02:12 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	give_fork(t_philo *philo, int nb)
{
	int	i;

	if (nb == 1)
		i = pair_one(philo);
	else
		i = not_pair_one(philo);
	return (i);
}

int	rout_part(t_philo *philo)
{
	while (!give_fork(philo, 1) && !check_death(philo))
		usleep(10);
	print(philo->id, "has taken a fork", philo);
	while (!give_fork(philo, 2) && !check_death(philo))
		usleep(10);
	print(philo->id, "has taken a fork", philo);
	if (!check_death(philo))
	{
		a_table(philo);
		philo->eat_c += 1;
		if ((philo->eat_c >= philo->arg->c_eat) && philo->arg->c_eat != -1)
		{
			pthread_mutex_lock(&(philo->full.mutex));
			philo->full.data = 1;
			pthread_mutex_unlock(&(philo->full.mutex));
		}
		time_sleep(philo);
	}
	return (1);
}

void	custom_exit(t_philo *philo, t_env *env, int err)
{
	if (env)
	{
		if (env->arg)
		{
			free(env->arg);
		}
		free(env);
	}
	if (philo)
		free(philo);
	if (err == -10)
		printf("send positive values and non null value for philo nbrs\n");
	exit(err);
}
