#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>


typedef struct big_circl
{
	int width;
	int height;
	char bgr;
}	background;

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

int b_cir(background *b_cir)
{
	if (b_cir->width <= 0 || b_cir->height > 300 ||
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
	args = fscanf(file, "%d %d %c\n", b_cir->width, b_cir->height, b_cir->bgr);
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



int main(int argc, char **argv)
{
	FILE *file;
	char **map;
	background b_cir;

	if (argc != 2)
		return(print_error("Error: argument\n"));
	if ((file = fopen(argv[1], "r")) == NULL || fill_map(&map, &b_cir, file))
		return(print_error("Error: Oparaation file corrupted\n"));
	print_map(map); // написать функцию 
	return (0);

}