#include "libft.h"
#include <stdio.h>

void    print_map(void)
{
    char *line;
    while((line = arn_get_next_line(0)) != NULL)
        ft_putstr_fd(line, 1);
}