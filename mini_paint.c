#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <math.h>

typedef struct big_circl
{
	int width;
	int height;
	char bgr;
}	background;

typedef struct small_cirlc
{
	char type;
	float x;
	float y;
	float radius;
	char obj;
}	smalls;

int	print_error(char *str)
{
	int i;

	i =0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
	return (1);
}

int check_big(background *b_cir)
{
	if (b_cir->width <= 0 || b_cir->width > 300 ||
		b_cir->height <= 0 || b_cir-> height > 300)
		return (1);
	return (0);
}

int fill_map(char ***map, background *b_cir, FILE *file)
{
	int args;
	int i;
	int j;
	
	i = 0;
	args = fscanf(file, "%d %d %c\n", &b_cir->width, &b_cir->height, &b_cir->bgr);
	if (args != 3 || check_big(b_cir))
		return (1);
	(*map) = malloc (sizeof(char *) * (b_cir->height + 1));
	if (!(*map))
		return (1);
	(*map)[b_cir->height] = NULL;
	while (i < b_cir->height)
	{
		(*map)[i] = malloc(sizeof(char) * (b_cir->width + 1));
		if (!(*map)[i])
			return (1);
		j = 0;
		while (j < b_cir->width)
			(*map)[i][j++] = b_cir->bgr;
		(*map)[i][j] = '\0';
		i++;
	}
	return (0);
}

void print_map(char **map)
{
	int i;
	int j;

	i = 0;
	while (map && map[i])
	{
		j = 0;
		while (map[i][j] != '\0')
		{
			write(1, &map[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}

int is_cirle(float x, float y, smalls *cirl)
{
	float dist;

	dist = sqrtf(powf((x - cirl->x), 2.) + powf((y - cirl->y), 2.));
	if ((dist - cirl->radius) <= 0.0000000)
	{
		// write(1, "pupa\n", 5);
		if ((dist - cirl->radius) <= -1.0000000)
			return (1);
		else
			return (2);
	}
	return (0);
}

int draw_circl(char ***map, background *b_cir, FILE *file)
{
	int ch;
	smalls cirl;
	int in_cirl;
	int i;
	int j;

	ch = fscanf(file, "%c %f %f %f %c\n", &cirl.type, &cirl.x, &cirl.y, &cirl.radius, &cirl.obj);
	
	while (ch == 5)
	{
		// printf( "%c %f %f %f %c\n", cirl.type, cirl.x, cirl.y, cirl.radius, cirl.obj);
		if (cirl.radius <= 0.0000000 || (cirl.type != 'c' && cirl.type != 'C'))
			return (1);
		i = 0;
		while (i < b_cir->height)
		{
			j = 0;
			while (j < b_cir->width)
			{
				in_cirl = is_cirle(j, i, &cirl);
				// printf("%d\n", in_cirl);
				if ( in_cirl == 2 || (cirl.type == 'C' && in_cirl == 1))
				{
					(*map)[i][j] = cirl.obj;
				}
				j++;
			}
			i++;
		}
		ch = fscanf(file, "%c %f %f %f %c\n", &cirl.type, &cirl.x, &cirl.y, &cirl.radius, &cirl.obj);
	}
	if (ch == -1)
	{
		print_map(*map);
		return (0);
	}
	return (1);
}

int main(int argc, char **argv)
{
	FILE *file;
	char **map;
	background b_cir;

	if (argc != 2)
		return(print_error("Error: argument\n"));
	if ((file = fopen(argv[1], "r")) == NULL || fill_map(&map, &b_cir, file))
		return(print_error("Error: Operation file corrupted\n"));
	if (draw_circl(&map, &b_cir, file))
		return(print_error("Error: Operation file corrupted\n"));
	// print_map(map); // написать функцию 
	fclose(file);
	return (0);

}