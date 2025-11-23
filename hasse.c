#include <malloc.h>

#include "hasse.h"

void removeTransitiveLinks(t_link_array *p_link_array)
{
    int i = 0;
    while (i < p_link_array->count)
    {
        t_link link1 = p_link_array->links[i];
        int j = 0;
        int to_remove = 0;
        while (j < p_link_array->count && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->links[j];
                if (link1.start_class == link2.start_class)
                {
                    // look for a link from link2.end_class to link1.end_class
                    int k = 0;
                    while (k < p_link_array->count && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];
                            if ((link3.start_class == link2.end_class) && (link3.end_class == link1.end_class))
                            {
                                to_remove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }
        if (to_remove)
        {
            // remove link1 by replacing it with the last link
            p_link_array->links[i] = p_link_array->links[p_link_array->count - 1];
            p_link_array->count--;
        }
        else
        {
            i++;
        }
    }
}
