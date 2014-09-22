/*
 * genetic.c
 *
 *  Created on: 18/02/2013
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#include <randoms.h>

#include "defs.h"
#include "eval.h"

#include "genetic.h"

void tournament_selection(Sol *pool, int *selected_parents, int poolsize)
{
    int i, a, b, winner;
    for(i=0;i<poolsize;i++)
    {
        do
        {
			a=random_int(0,poolsize);//rand()%poolsize;
			b=random_int(0,poolsize);//rand()%poolsize;
        }
        while(a==b);
        winner=-1;
        if(pool[a].rank<pool[b].rank)
            winner=a;
        else if(pool[b].rank<pool[a].rank)
            winner=b;
        else if(pool[a].crowdist>pool[b].crowdist)
            winner=a;
        else if(pool[b].crowdist>pool[a].crowdist)
            winner=b;
        else
        {
            if(random_int(0,2) == 0)
                winner=a;
            else
                winner=b;
        }
        selected_parents[i]=winner;
    }
}

void point_crossover(Sol *parent_a, Sol *parent_b, Sol *son_a, Sol *son_b)
{
	int i, point;
	point = random_int(0,n);//rand()%n;
	for (i = 0; i < point; i++)
	{
		son_a->conf[machine][i] = parent_a->conf[machine][i];
		son_a->conf[vlevel][i] = parent_a->conf[vlevel][i];
		son_b->conf[machine][i] = parent_b->conf[machine][i];
		son_b->conf[vlevel][i] = parent_b->conf[vlevel][i];
	}
	for (i = point; i < n; i++)
	{
		son_a->conf[machine][i] = parent_b->conf[machine][i];
		son_a->conf[vlevel][i] = parent_b->conf[vlevel][i];
		son_b->conf[machine][i] = parent_a->conf[machine][i];
		son_b->conf[vlevel][i] = parent_a->conf[vlevel][i];
	}
}

void uniform_mutation(Sol *s)
{
	int i, j, mach, volt;
	for(i=0; i < n; i++)
	{
		//if((rand()%101)<=5)
		if (random_double() <= 0.05)
		{
			mach = random_int(0,m);//rand()%m;
			for (j = 0; j < n_vlevels && vspairs[j][mach<<1] != 0; j++); //j--;
			volt = random_int(0,j);//rand()%j;
			s->conf[0][i] = mach;
			s->conf[1][i] = volt;
		}
	}
}

void biased_mutation(Sol *s, int inverse)
{
	int i, j, mach, volt, n_vl;
	for(i=0; i < n; i++)
	{
		//if((rand()%101)<=5)
		if (random_double() <= 0.05)
		{
			mach = random_int(0,m);//rand()%m;
			for (n_vl = 0; n_vl < n_vlevels && vspairs[n_vl][mach<<1] != 0; n_vl++);
			// Asignar pesos a los v-levels
			double p[n_vl], ifactor, w, r;
			r = random_double();
			ifactor = n_vl*n_vl/2.0 + n_vl/2.0;
			if (inverse)
			{
				for (j = n_vl-1; j >= 0; j--)
				{
					w = (n_vl-j)/ifactor;
					p[j] = j == n_vl-1 ? w : w + p[j+1];
				}
				for (volt = n_vl-1; volt >= 0; volt--)
					if (p[volt] > r) break;
			}
			else
			{
				for (j = 0; j < n_vl; j++)
				{
					w = (j+1)/ifactor;
					p[j] = j == 0 ? w : w + p[j-1];
				}
				for (volt = 0; volt < n_vl; volt++)
					if (p[volt] > r) break;
			}
			s->conf[0][i] = mach;
			s->conf[1][i] = volt;
		}
	}
}
