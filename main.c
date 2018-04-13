/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acauchy <acauchy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/11 11:55:40 by acauchy           #+#    #+#             */
/*   Updated: 2018/04/13 10:17:26 by acauchy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void exit_error(char *errmsg)
{
	fprintf(stderr, "errmsg\n");
	exit(1);
}

static int	start_process(char *path, int inputfd, int outputfd)
{
	pid_t	pid;
	char	*args[2] = {strdup(path), NULL};

	if ((pid = fork()) == -1)
		exit_error("fork() error");
	if (pid == 0)
	{
		if (inputfd != 0)
		{
			if (close(0) == -1)
				exit_error("close(stdin) error");
			dup(inputfd);
		}
		else if (outputfd != 1)
		{
			if (close(1) == -1)
				exit_error("close(stdout) error");
			dup(outputfd);
		}
		execve(path, args, NULL);
	}
	return (pid);
}

int			main(void)
{
	int		pipefd[2];
	pid_t	ls_pid;
	pid_t	cat_pid;

	if (pipe(pipefd) == -1)
		exit_error("pipe() error");
	printf("read end : %d, write end : %d\n", pipefd[0], pipefd[1]);
	ls_pid = start_process("/bin/ls", 0, pipefd[1]);
	cat_pid = start_process("/bin/cat", pipefd[0], 1);
	return (0);
}
