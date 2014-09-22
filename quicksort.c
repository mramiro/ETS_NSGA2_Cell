//
//  quicksort.c
//  NSGA2
//
//  Created by Aurelio A Santiago Pineda on 22/05/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "quicksort.h"

void sort_makespan(Sol arr[], int beg, int end)
{
    if (end > beg + 1)
    {
        double piv = arr[beg].makespan;
        int l = beg + 1, r = end;
        while (l < r)
        {
            if (arr[l].makespan <= piv)
                l++;
            else
                swap_sol(&arr[l], &arr[--r]);
        }
        swap_sol(&arr[--l], &arr[beg]);
        sort_makespan(arr, beg, l);
        sort_makespan(arr, r, end);
    }
}

void sort_energy(Sol arr[], int beg, int end)
{
    if (end > beg + 1)
    {
        double piv = arr[beg].energy;
        int l = beg + 1, r = end;
        while (l < r)
        {
            if (arr[l].energy <= piv)
                l++;
            else
                swap_sol(&arr[l], &arr[--r]);
        }
        swap_sol(&arr[--l], &arr[beg]);
        sort_energy(arr, beg, l);
        sort_energy(arr, r, end);
    }
}

void sort_distance(Sol arr[], int beg, int end)
{
    if (end > beg + 1)
    {
        double piv = arr[beg].crowdist;
        int l = beg + 1, r = end;
        while (l < r)
        {
            if (arr[l].crowdist <= piv)
                l++;
            else
                swap_sol(&arr[l], &arr[--r]);
        }
        swap_sol(&arr[--l], &arr[beg]);
        sort_distance(arr, beg, l);
        sort_distance(arr, r, end);
    }
}
