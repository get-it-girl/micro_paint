#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

typedef struct big_ract
{
	int width;
	int height;
	char bgr;
}	background;

typedef struct small_ract
{
	char type;
	float x;
	float y;
	float width;
	float height;
	char sym;
}	s_ract;

void print_map(char **map)
{
	int i;
	int j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
			write(1, &map[i][j++], 1);
		write(1, "\n", 1);
		i++;
	}
}

int print_error(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
	return (1);
}

int check_args(background *b_ract)
{
	if (b_ract->height <= 0 || b_ract->height > 300 ||
		b_ract->width <= 0 || b_ract->width > 300)
		return (1);
	return (0);
}

int	fill_map(char ***map, background *b_ract, FILE *file)
{
	int args;
	int i;
	int j;

	i = 0;
	args = fscanf(file, "%d %d %c\n", &b_ract->width, &b_ract->height, &b_ract->bgr);
	if (args != 3 || check_args(b_ract))
		return(1);
	(*map) = malloc(sizeof(char *) * (b_ract->height + 1));
	if (!(*map))
		return (1);
	(*map)[b_ract->height] = NULL;
	while (i < b_ract->height)
	{
		j = 0;
		(*map)[i] = malloc(sizeof(char) * (b_ract->width + 1));
		if (!(*map)[i])
			return (1);
		while (j < b_ract->width)
			(*map)[i][j++] = b_ract->bgr;
		(*map)[i][j] = '\0';
		i++;
	}
	return (0);
}

int in_ract(float x, float y, s_ract *ract)
{
	if ((((x < ract->x) || (ract->x + ract->width < x)) || (y < ract->y)) || (ract->y + ract->height < y))
		return (0);
	if (((x - ract->x < 1.00000000) || ((ract->x + ract->width) - x < 1.00000000)) ||
		((y - ract->y < 1.00000000 || ((ract->y + ract->height) - y < 1.00000000))))
		return (2); // Border
	return (1);		// Inside
}

int read_other_maps(char ***map, background *b_ract, FILE *file)
{
	int args;
	int i;
	int j;
	s_ract ract;
	int in;

	args = fscanf(file, "%c %f %f %f %f %c\n", &ract.type, &ract.x, &ract.y, &ract.width, &ract.height, &ract.sym);
	while (args == 6)
	{
		i = 0;
		while (i < b_ract->height)
		{
			if (ract.width <= 0.00000000 || ract.height <= 0.00000000 || (ract.type != 'R' && ract.type != 'r'))
			{
				return (1);
			}
			j = 0;
			while (j < b_ract->width)
			{
				in  = in_ract(j, i, &ract);
				if (in == 2 || (in == 1 && ract.type == 'R'))
					(*map)[i][j] = ract.sym;
				j++;		
			}
			i++;
		}
		// 	printf("\n%c\n", ract.type);
		// printf("%c %f %f %f %f %c\n", ract.type, ract.x, ract.y, ract.width, ract.height, ract.sym);
		args = fscanf(file, "%c %f %f %f %f %c\n", &ract.type, &ract.x, &ract.y, &ract.width, &ract.height, &ract.sym);
	}
	if (args == -1)
	{
		print_map((*map));
		return (0);
	}
	else
		return (1);
}

int main(int argc, char **argv)
{
	FILE *file;
	char  **map;
	background b_ract;

	if (argc != 2)
		return(print_error("Error: argument\n"));
	if ((file = fopen(argv[1], "r")) == NULL || fill_map(&map, &b_ract, file))
		return(print_error("Error: Operation file corrupted\n"));
	if (read_other_maps(&map, &b_ract, file))
		return(print_error("Error: Operation file corrupted\n"));
	// print_map(map);
	
	return (0);
}