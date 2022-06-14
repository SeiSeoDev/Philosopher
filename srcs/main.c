/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:59:34 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/08 14:46:33 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//valgrind --tool=helgrind
// fsanitize=thread

t_env	*init(t_env *env, char **av, int ac)
{
	env = malloc(sizeof(t_env));
	env->arg = malloc(sizeof(t_arg));
	if (ac == 6)
	{
		env->arg->c_eat = ft_atoi(av[5]);
		if (env->arg->c_eat <= 0)
			custom_exit(NULL, env, EXIT_FAILURE);
	}
	else
		env->arg->c_eat = -1;
	env->arg->t_eat = ft_atoi(av[3]);
	env->arg->t_sleep = ft_atoi(av[4]);
	env->arg->t_die = ft_atoi(av[2]);
	env->count = ft_atoi(av[1]);
	env->philo = malloc(sizeof(t_philo) * env->count);
	if (env->arg->t_eat < 0 || env->arg->t_sleep < 0 || env->arg->t_die < 0
		|| env->count <= 0)
		custom_exit(env->philo, env, -10);
	env->arg->time = actual_time();
	pthread_mutex_init(&(env->arg->dead.mutex), NULL);
	pthread_mutex_init(&(env->arg->print.mutex), NULL);
	env->arg->dead.data = 0;
	env->arg->print.data = 1;
	return (env);
}

int	dead_thread(t_env *env, int i)
{
	pthread_mutex_unlock(&env->philo[i].last_eat.mutex);
	if (!check_all_full(env))
		print(env->philo[i].id, "died", &env->philo[i]);
	pthread_mutex_lock(&env->arg->dead.mutex);
	env->arg->dead.data = 1;
	pthread_mutex_unlock(&env->arg->dead.mutex);
	custom_close(env);
	custom_exit(env->philo, env, EXIT_FAILURE);
	return (1);
}

int	main_thread(t_env *env)
{
	int			i;
	long int	time;

	while (1)
	{
		i = 0;
		while (i < env->count && env->philo[i].thread)
		{
			time = actual_time();
			pthread_mutex_lock(&env->philo[i].last_eat.mutex);
			if ((((time - env->philo[i].last_eat.data) > env->arg->t_die)
					&& !check_full(&env->philo[i]))
				|| check_all_full(env))
			{
				dead_thread(env, i);
			}
			pthread_mutex_unlock(&env->philo[i].last_eat.mutex);
			i++;
		}
	}
}

int	main(int ac, char **av)
{
	t_env	*env;
	int		i;

	i = 0;
	if (ac < 5 || ac > 6)
	{
		printf("USAGE: ./philo number_of_philosophers time_to_die time_to_eat\
 time_to_sleep number_of_times_each_philosopher_must_eat]\n");
		return (0);
	}
	env = NULL;
	env = init(env, av, ac);
	init_philo(env);
	main_thread(env);
	free(env->philo);
	free(env);
	return (0);
}
