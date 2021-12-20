/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 14:30:49 by dasanter          #+#    #+#             */
/*   Updated: 2021/12/20 18:24:34 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

int		print(int id, char *msg, t_philo *philo)
{
	long int	time;

	time = actual_time();
	if (time < 0)
		return (0);
	printf("%ld : philo %d %s\n", time - philo->arg->time, id, msg);
	return (1);
}

int		check_death(t_philo *philo)
{
	int ret;

	ret = 0;
	pthread_mutex_lock(&(philo->arg->dead.mutex));
	if (philo->arg->dead.data == 1)
		ret = 1;
	pthread_mutex_unlock(&(philo->arg->dead.mutex));
	return (ret);
}

int		time_sleep(t_philo *philo)
{
	long int time;
	time = actual_time();
	print(philo->id, "start sleep !", philo);
	while ((actual_time() - time < philo->arg->t_sleep) && !check_death(philo))
	{
		usleep(1);
	}
	return (1);
}

int		time_death(t_philo *philo)
{
	long int time;

	time = actual_time();
	if (time - philo->last_eat > philo->arg->t_die)
		printf("AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH\n");
	return (1);
}

int		a_table(t_philo *philo)
{
	long int time;

	int test;

	test = 0;
	philo->last_eat = actual_time();
	time = actual_time();
	print(philo->id, "est en train de manger!!", philo);
	while ((actual_time() - time) < (philo->arg->t_eat))
	{
		//printf("%ld / %d\n",(actual_time() - time), philo->arg->t_eat);
		test++;
	}
    print(philo->id, "a finis de manger", philo);
	pthread_mutex_lock(&(philo->next_fork->mutex));
	philo->next_fork->data = 1;
	pthread_mutex_unlock(&(philo->next_fork->mutex));
	pthread_mutex_lock(&(philo->next_fork->mutex));
	philo->fork.data = 1;
	pthread_mutex_unlock(&(philo->next_fork->mutex));
	return (1);
}