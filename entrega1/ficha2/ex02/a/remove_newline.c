/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * remove_newline.c
 * 
 */


/*
 * Removes newline ('\n').
 * 
 * returns 1 if at least one newline is removed.
 */
int remove_newline(char *str)
{
	int i = 0, bool = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\n')
		{
			str[i] = '\0';
			bool = 1;
		}
		i++;
	}
	
	return bool;
}
