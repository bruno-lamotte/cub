#include "cub.h"

void	add_dollar_to_start_symbol(t_slr1 *data);
void	update_follows(t_slr1 *data, t_list *current_rule, int *added);

void	get_follows(t_slr1 *data)
{
	t_list	*current_rule;
	int		added;

	added = 1;
	current_rule = data->rules;
	add_dollar_to_start_symbol(data);
	while (added)
	{
		added = 0;
		update_follows(data, current_rule, &added);
	}
}
