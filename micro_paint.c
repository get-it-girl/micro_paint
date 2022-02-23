#include<stdio.h>
#include<unistd.h>
#include <stdarg.h>
#include <stdlib.h>

typedef struct s_big
{
	int width;
	int height;
	char obj;
}	t_big;

typedef struct s_small
{
	char type;
	float x;
	float y;
	float width;
	float height;
	char obj;
}	t_small;

int print_map(char **map, t_big *b_ract)
{
	int i;
	
	i = 0;
	if (!map)
	{	
		write(1, "lol", 4);
		return (1);
	}
	while (i < b_ract->height)
	{
		if (map[i])
			printf("%s", map[i]);
		i++;
	}
	return (0);
}

int fill_big_ract(t_big *b_ract, char ***map)
{
	int i;
	int j;


	i = 0;
	if (b_ract->width > 300 || b_ract->width < 0 ||
		b_ract->height > 300 || b_ract->height < 0 ||
		!b_ract->obj)
		return (1);
	*map = malloc(sizeof(char *) * b_ract->height);

	while (i < b_ract->height)
	{
		(*map)[i] = malloc(sizeof(char) * (b_ract->width + 2));
		j = 0;
		while (j < b_ract->width)
		{
			(*map)[i][j] = b_ract->obj;
			j++;
		}
		(*map)[i][j++] = '\n';
		(*map)[i][j] = '\0';
		i++;
	}
	return (0);
}

int check_small(t_small *s_ract)
{
	if (s_ract->width > 0.00000000 && s_ract->height > 0.00000000 &&
			(s_ract->type == 'r' || s_ract->type == 'R'))
		return (0);
	return (1);

}

int in_ract(float x, float y, t_small *s_ract)
{
	if ((((x < s_ract->x) || (s_ract->x + s_ract->width < x)) ||
		(y < s_ract->y)) || (s_ract->y + s_ract->height < y))
			return (0);
	if (((x - s_ract->x < 1.0000000 )|| ((s_ract->x + s_ract->width) - x < 1.0000000)) ||
		((y - s_ract->y < 1.0000000 || ((s_ract->y + s_ract-> height)- y < 1.0000000))))
			return (1);
	return (2);
}

int put_small_ract(t_small *s_ract, t_big *b_ract, char ***map)
{
	int i;
	int j;
	int ch;

	i = 0;
	while (i < b_ract->height)
	{
		j = 0;
		while (j < b_ract->width)
		{
			ch = in_ract(i, j, s_ract);
			if ((ch == 1) ||
				(s_ract->type == 'R' && ch))
				(*map)[i][j] = s_ract->obj;
			j++;
		}
		i++;
	}
	return (0);
}

int get_small_ract(char ***map, t_big *b_ract, FILE *fd)
{
	int check;
	t_small s_ract;

	check = fscanf(fd, "%c %f %f %f %f %c\n", &s_ract.type, &s_ract.x, &s_ract.y, &s_ract.width, &s_ract.height, &s_ract.obj);
	while (check == 6)
	{
		if (check_small(&s_ract) || put_small_ract(&s_ract, b_ract, map))
			return(1);
		write(1, "pupa\n", 6);
		printf( "%c %f %f %f %f %c\n", s_ract.type, s_ract.x, s_ract.y, s_ract.width, s_ract.height, s_ract.obj);
		check = fscanf(fd, "%c %f %f %f %f %c\n", &s_ract.type, &s_ract.x, &s_ract.y, &s_ract.width, &s_ract.height, &s_ract.obj);
	}
	if (check == -1)
	{
		print_map(*map, b_ract);
		return (0);
	}
	return (1);
}

int main(int argc, char *argv[])
{
	FILE *fd;
	t_big b_ract;
	int check;
	char **map;
	check = 0;

	if (argc != 2)
	{
		printf("Error: argument\n");
		return (1);
	}
	else
	{
		fd = fopen(argv[1], "r");
		if (fd == NULL)
		{
			printf("Error: Operation file corrupted\n");
			return (1);
		}
		else
		{
			check = fscanf(fd, "%d %d %c\n", &b_ract.width, &b_ract.height, &b_ract.obj);
			if (check != 3)
			{
				printf("Error: Operation file corrupted\n");
				return (1);
			}
			if (fill_big_ract(&b_ract, &map) == 1)
			{
				printf("Error: Operation file corrupted\n");
				return (1);
			}
			if (get_small_ract(&map, &b_ract, fd) == 1)
			{
				printf("Error: Operation file corrupted\n");
				return (1);
			}
		}
	}
	fclose(fd);
	return (0);
}