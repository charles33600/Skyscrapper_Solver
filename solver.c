/* Allowed functions: write, malloc, free */
#include <unistd.h>

/* Structure pour stocker les contraintes du puzzle */
typedef struct s_constraints
{
	int	col_up[4];
	int	col_down[4];
	int	row_left[4];
	int	row_right[4];
}	t_constraints;

/* Toutes les permutations possibles de 1 à 4 */
int	g_perms[24][4] = {
	{1, 2, 3, 4}, {1, 2, 4, 3}, {1, 3, 2, 4}, {1, 3, 4, 2},
	{1, 4, 2, 3}, {1, 4, 3, 2}, {2, 1, 3, 4}, {2, 1, 4, 3},
	{2, 3, 1, 4}, {2, 3, 4, 1}, {2, 4, 1, 3}, {2, 4, 3, 1},
	{3, 1, 2, 4}, {3, 1, 4, 2}, {3, 2, 1, 4}, {3, 2, 4, 1},
	{3, 4, 1, 2}, {3, 4, 2, 1}, {4, 1, 2, 3}, {4, 1, 3, 2},
	{4, 2, 1, 3}, {4, 2, 3, 1}, {4, 3, 1, 2}, {4, 3, 2, 1}
};

/* Variable globale pour indiquer si une solution a été trouvée */
int	g_solution_found = 0;

/* Compte le nombre de gratte-ciels visibles depuis une direction */
int	count_visible(int *line, int direction)
{
	int	max_height;
	int	visible_count;
	int	i;

	max_height = 0;
	visible_count = 0;
	if (direction == 0)
	{
		i = 0;
		while (i < 4)
		{
			if (line[i] > max_height)
			{
				max_height = line[i];
				visible_count++;
			}
			i++;
		}
	}
	else
	{
		i = 3;
		while (i >= 0)
		{
			if (line[i] > max_height)
			{
				max_height = line[i];
				visible_count++;
			}
			i--;
		}
	}
	return (visible_count);
}

/* Vérifie si une ligne respecte les contraintes gauche/droite */
int	check_row_constraints(int row, int *line, t_constraints *constr)
{
	return (count_visible(line, 0) == constr->row_left[row]
		&& count_visible(line, 1) == constr->row_right[row]);
}

/* Vérifie s'il y a un conflit de colonne (même valeur dans la colonne) */
int	has_column_conflict(int grid[4][4], int row, int *line)
{
	int	col;
	int	prev_row;

	col = 0;
	while (col < 4)
	{
		prev_row = 0;
		while (prev_row < row)
		{
			if (grid[prev_row][col] == line[col])
				return (1);
			prev_row++;
		}
		col++;
	}
	return (0);
}

/* Vérifie si toutes les colonnes respectent les contraintes haut/bas */
int	check_column_constraints(int grid[4][4], t_constraints *constr)
{
	int	col;
	int	row;
	int	column[4];

	col = 0;
	while (col < 4)
	{
		row = 0;
		while (row < 4)
		{
			column[row] = grid[row][col];
			row++;
		}
		if (count_visible(column, 0) != constr->col_up[col])
			return (0);
		if (count_visible(column, 1) != constr->col_down[col])
			return (0);
		col++;
	}
	return (1);
}

/* Affiche la grille solution */
void	print_grid(int grid[4][4])
{
	int		row;
	int		col;
	char	c;

	row = 0;
	while (row < 4)
	{
		col = 0;
		while (col < 4)
		{
			c = grid[row][col] + '0';
			write(1, &c, 1);
			if (col < 3)
				write(1, " ", 1);
			col++;
		}
		write(1, "\n", 1);
		row++;
	}
}

/* Fonction récursive de résolution par backtracking */
void	solve(int row, int grid[4][4], t_constraints *constr)
{
	int	perm_idx;
	int	col;

	if (row == 4)
	{
		if (check_column_constraints(grid, constr))
		{
			print_grid(grid);
			g_solution_found = 1;
		}
		return ;
	}
	perm_idx = 0;
	while (perm_idx < 24 && !g_solution_found)
	{
		if (!check_row_constraints(row, g_perms[perm_idx], constr))
		{
			perm_idx++;
			continue ;
		}
		if (has_column_conflict(grid, row, g_perms[perm_idx]))
		{
			perm_idx++;
			continue ;
		}
		col = 0;
		while (col < 4)
		{
			grid[row][col] = g_perms[perm_idx][col];
			col++;
		}
		solve(row + 1, grid, constr);
		col = 0;
		while (col < 4)
		{
			grid[row][col] = 0;
			col++;
		}
		perm_idx++;
	}
}

/* Convertit un caractère en entier */
int	char_to_int(char c)
{
	if (c >= '1' && c <= '4')
		return (c - '0');
	return (-1);
}

/* Parse les arguments et remplit la structure de contraintes */
int	parse_arguments(char *str, t_constraints *constr)
{
	int	i;
	int	count;
	int	value;

	i = 0;
	count = 0;
	while (str[i] && count < 16)
	{
		if (str[i] >= '1' && str[i] <= '4')
		{
			value = char_to_int(str[i]);
			if (count < 4)
				constr->col_up[count] = value;
			else if (count < 8)
				constr->col_down[count - 4] = value;
			else if (count < 12)
				constr->row_left[count - 8] = value;
			else
				constr->row_right[count - 12] = value;
			count++;
		}
		else if (str[i] != ' ')
			return (0);
		i++;
	}
	return (count == 16);
}

/* Affiche un message d'erreur */
void	print_error(void)
{
	write(1, "Error\n", 6);
}

/* Fonction principale */
int	main(int argc, char **argv)
{
	int				grid[4][4];
	t_constraints	constr;
	int				row;
	int				col;

	if (argc != 2)
	{
		print_error();
		return (1);
	}
	if (!parse_arguments(argv[1], &constr))
	{
		print_error();
		return (1);
	}
	row = 0;
	while (row < 4)
	{
		col = 0;
		while (col < 4)
		{
			grid[row][col] = 0;
			col++;
		}
		row++;
	}
	solve(0, grid, &constr);
	if (!g_solution_found)
		print_error();
	return (0);
}
