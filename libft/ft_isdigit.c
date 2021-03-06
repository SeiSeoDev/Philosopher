/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 15:31:55 by dasanter          #+#    #+#             */
/*   Updated: 2019/11/20 18:03:32 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isdigit(int nb)
{
	if ((nb >= '0' && nb <= '9'))
		return (1);
	return (0);
}
