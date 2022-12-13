#include "../libft/libft.h"
#include <fcntl.h>
#include <stdio.h>
char	*ft_find_path(char	*cmd, char **env)
{
	char *current;
	char **paths;
	char *path;
	int i = 0;

	while (ft_strnstr(env[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(env[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		current = ft_strjoin(paths[i], "/");
		path = ft_strjoin(current, cmd);
		free(current);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	i = 0;
	while(paths[i++])
		free(paths[i]);
	free(paths);
	return (0);
}

void	ft_exec(char *av, char **env)
{
	char	**cmd;
	char	*pathname;
	int	i;

	i = 0;
	cmd = ft_split(av, ' ');
	pathname = ft_find_path(cmd[0], env);
	if (!pathname)
	{
		write(2, "no command", 10);
		while (cmd[i++])
			free(cmd[i]);
		free(cmd);
		exit(127);
	}
	execve(pathname, cmd, env);
}
void	child_proc(char **av, char **env, int *fd)
{
	int	fd_in;

	if (access(av[1], F_OK) == -1)
	{
		printf("NO LA CHUGUINA FILENA OR DERICTORINA\n");
		exit (1);
	}
	fd_in = open(av[1], O_RDWR, 0777);
	dup2(fd[1], STDOUT_FILENO);
	dup2(fd_in, STDIN_FILENO);
	close(fd[0]);
	ft_exec(av[2], env);
}
void	parent_proc(char **av, char **env, int *fd)
{
	int	fd_out;

	fd_out = open(av[4], O_CREAT | O_RDWR, 0777);
	if (fd_out == -1)
	{
		printf("lapalina");
		exit(1);
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	close(fd[1]);
	ft_exec(av[3], env);
}

int main(int ac, char **av, char **env)
{
	int	fd[2];
	pid_t pid;

	if (ac == 5)
	{
		if (pipe(fd) == -1)
		{
			printf("La balina sigina hawana miss la kanssiyon\n");
			return (1);
		}
		pid = fork();
		if (pid == -1)
		{
			printf("lapalina ciguina huala mithmacantion\n");
			return (1);
		}
		if (pid == 0)
			child_proc(av, env, fd);
		waitpid(pid, NULL, 0);
		parent_proc(av, env, fd);
	}
	else
		printf("Inavalidina la palina argemantina la ciguina\n");
}