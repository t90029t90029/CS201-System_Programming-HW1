/* Your code for the "Maze Walker" program will go here. 
 *
 * Do not include a separate .h file for headers and prototypes, 
 * make sure to keep all your code within this single .c file.
 *
 */

#include <stdio.h>
#include <stdlib.h>

/* prototype */
int move_forward(char * direction,int * temp_x,int * temp_y,char * current,int * x_size,int * y_size,char ** map,char ** change);  /* move if possible */
int move_right(char * direction,int * temp_x,int * temp_y,char * current,int * x_size,int * y_size,char ** map,char ** change);	   /* check if possible */
int move_left(char * direction,int * temp_x,int * temp_y,char * current,int * x_size,int * y_size,char ** map,char ** change);	   /* move if possible */
int turn_back(char * direction);

int main()
{
	char ** map = NULL;
	int * x_size = NULL;	/*the size of the maze at x axis*/
	int * y_size = NULL;	/*the size of the maze at y axis*/
	char * delim = NULL;	/*delimiter : to get rid of the comma*/
	char * current = NULL;	/*where we are now*/
	char * end = NULL;	/*where we are now*/
	char * direction = NULL;/*which direction the person is facing*/
	/* the purpose I declare these variables below is to hold the 2nd,3rd lines data first
	so that I can read in the map and make the pointers point to that locations*/
	int * temp_x = NULL;	/*temporarily store the x-axis data of the current pointer */
	int * temp_y = NULL;	/*temporarily store the y-axis data of the current pointer */
	int * end_x = NULL;	/* the x-axis data of the end pointer */
	int * end_y = NULL;	/* the y-axis data of the end pointer */
	char ** change = NULL;	/* just to change the current pointer in functions */

	/* just for forloop, so it is not dynamic */
	int i = 0;		/* this is because we do not append std=c99 in makefile, which means I need to declare this at here.
				So, only this place breaks the rule of "any variable" should be dynamically allocated by the request from teacher. */


	FILE * fp = fopen("sample_maze.txt","r");	/* the request does not say we need to update the new map into the file. */
	if(!fp)
	{
		return 0;
	}

	/* starting to read in the first line */
	x_size = (int *)malloc(4);
	y_size = (int *)malloc(4);
	delim = (char *)malloc(1);
		
	fscanf(fp,"%d ",x_size);
	fscanf(fp,"%1c ",delim);
	fscanf(fp,"%d\n ",y_size);

	/* starting to read in the second line */
	temp_x = (int *)malloc(4);
	temp_y = (int *)malloc(4);
		
	fscanf(fp,"%d ",temp_x);
	fscanf(fp,"%1c ",delim);
	fscanf(fp,"%d\n ",temp_y);


	/* starting to read in the third line */
	end_x = (int *)malloc(4);
	end_y = (int *)malloc(4);
		
	fscanf(fp,"%d ",end_x);
	fscanf(fp,"%1c ",delim);
	fscanf(fp,"%d\n ",end_y);

	/* starting to read in the map from the file */
	map = (char**)malloc((*y_size)*sizeof(char*));
	for(i=0;i<*y_size;++i)
	{
		map[i] = (char*)malloc(*x_size+1);
		fread(map[i],*x_size+1,1,fp);
	}
	/* make the current and end pointers point to the specific place */
	current = map[*temp_y]+(*temp_x);
	change = &current;
	end = map[*end_y]+(*end_x);


	/* starting to decide the direction */
	/* e = east , s = south , w = west , n = north */	
	/* i.g. *temp_x == 0 : beginning at the west = facing at the east */

	direction = (char *)malloc(1);
	if(*temp_x == 0)
	{
		*direction = 'e';
	}
	else if(*temp_y == 0)
	{
		*direction = 's';
	}
	else if(*temp_x == (*x_size)-1)
	{
		*direction = 'w';
	}
	else if(*temp_y == (*y_size)-1)
	{
		*direction = 'n';
	}

	/* starting to write the algorithm */
	*current = 'W';		/* set the starting position to W */
	while(current != end)
	{
		if(!move_right(direction,temp_x,temp_y,current,x_size,y_size,map,change))
		{
			if(!move_forward(direction,temp_x,temp_y,current,x_size,y_size,map,change))
			{
				if(!move_left(direction,temp_x,temp_y,current,x_size,y_size,map,change))
				{
					turn_back(direction);
					move_forward(direction,temp_x,temp_y,current,x_size,y_size,map,change);
				}
			}
		}
	}

	/* starting to print the new map to the user */
	for(i=0;i<*y_size;++i)
	{
		printf("%s",map[i]);
	}

	/* starting to free the dynamic memories */
	if(change)
	{
		change = NULL;
	}

	if(current)
	{
		current = NULL;
	}

	if(end)
	{
		end = NULL;
	}

	if(map)
	{
		for(i=0;i<*y_size;++i)
		{
			if(map[i])
			{
				free(map[i]);
				map[i] = NULL;
			}
		}
		free(map);
	}
	map = NULL;

	if(x_size)
	{
		free(x_size);
	}
	x_size = NULL;

	if(y_size)
	{
		free(y_size);
	}
	y_size = NULL;

	if(delim)
	{
		free(delim);
	}
	delim = NULL;

	if(temp_x)
	{
		free(temp_x);
	}
	temp_x = NULL;

	if(temp_y)
	{
		free(temp_y);
	}
	temp_y = NULL;

	if(end_x)
	{
		free(end_x);
	}
	end_x = NULL;

	if(end_y)
	{
		free(end_y);
	}
	end_y = NULL;

	if(direction)
	{
		free(direction);
	}
	direction = NULL;

	i = 0;

	fclose(fp);

	return 0;
}


/* implementation */
int move_forward(char * direction,int * temp_x,int * temp_y,char * current,int * x_size,int * y_size,char ** map,char ** change)
{
	if(!temp_x || !temp_y ||!direction)
	{
		return 0;
	}
	if(*direction == 'e' && *temp_x != *x_size-1)		/* facing east and not at the most east position */
	{
		char * forward = map[*temp_y]+(*temp_x+1);
		if(*forward == '0' || *forward == 'W')
		{
			*change = forward;	/* this changes the real current */
			++(*temp_x);
			*(*change) = 'W';
			return 1;
		}
	}
	else if(*direction == 's' && *temp_y != *y_size-1)	/* facing south and not at the most south position */
	{
		char * forward = map[*temp_y+1]+(*temp_x);
		if(*forward == '0' || *forward == 'W')
		{
			*change = forward;
			++(*temp_y);
			*(*change) = 'W';
			return 1;
		}
	}
	else if(*direction == 'w' && *temp_x != 0)		/* facing west and not at the most west position */
	{
		char * forward = map[*temp_y]+(*temp_x-1);
		if(*forward == '0' || *forward == 'W')
		{
			*change = forward;
			--(*temp_x);
			*(*change) = 'W';
			return 1;
		}
	}
	else if(*direction == 'n' && *temp_y != 0)		/* facing north and not at the most north position */
	{
		char * forward = map[*temp_y-1]+(*temp_x);
		if(*forward == '0' || *forward == 'W')
		{
			*change = forward;
			--(*temp_y);
			*(*change) = 'W';
			return 1;
		}
	}
	return 0;
}
	
int move_right(char * direction,int * temp_x,int * temp_y,char * current,int * x_size,int * y_size,char ** map,char ** change)
{
	if(!temp_x || !temp_y ||!direction)
	{
		return 0;
	}
	if(*direction == 'e' && *temp_y != *y_size-1)		/* facing east and not at the most south position */
	{
		char * right = map[*temp_y+1]+(*temp_x);
		if(*right == '0' || *right == 'W')
		{
			*change = right;
			++(*temp_y);
			*(*change) = 'W';
			*direction = 's';
			return 1;
		}
	}
	else if(*direction == 's' && *temp_x != 0)		/* facing south and not at the most west position */
	{
		char * right = map[*temp_y]+(*temp_x-1);
		if(*right == '0' || *right == 'W')
		{
			*change = right;
			--(*temp_x);
			*(*change) = 'W';
			*direction = 'w';
			return 1;
		}
	}
	else if(*direction == 'w' && *temp_y != 0)		/* facing west and not at the most north position */
	{
		char * right = map[*temp_y-1]+(*temp_x);
		if(*right == '0' || *right == 'W')
		{
			*change = right;
			--(*temp_y);
			*(*change) = 'W';
			*direction = 'n';
			return 1;
		}
	}
	else if(*direction == 'n' && *temp_x != *x_size-1)	/* facing north and not at the most east position */
	{
		char * right = map[*temp_y]+(*temp_x+1);
		if(*right == '0' || *right == 'W')
		{
			*change = right;
			++(*temp_x);
			*(*change) = 'W';
			*direction = 'e';
			return 1;
		}
	}
	return 0;
}

int move_left(char * direction,int * temp_x,int * temp_y,char * current,int * x_size,int * y_size,char ** map,char ** change)
{
	if(!temp_x || !temp_y ||!direction)
	{
		return 0;
	}
	if(*direction == 'e' && *temp_y != 0)			/* facing east and not at the most north position */
	{
		char * left = map[*temp_y-1]+(*temp_x);
		if(*left == '0' || *left == 'W')
		{
			*change = left;
			--(*temp_y);
			*(*change) = 'W';
			*direction = 'n';
			return 1;
		}
	}
	else if(*direction == 's' && *temp_x != *x_size-1)	/* facing south and not at the most east position */
	{
		char * left = map[*temp_y]+(*temp_x+1);
		if(*left == '0' || *left == 'W')
		{
			*change = left;
			++(*temp_x);
			*(*change) = 'W';
			*direction = 'e';
			return 1;
		}
	}
	else if(*direction == 'w' && *temp_y != *y_size-1)	/* facing west and not at the most south position */
	{
		char * left = map[*temp_y+1]+(*temp_x);
		if(*left == '0' || *left == 'W')
		{
			*change = left;
			++(*temp_y);
			*(*change) = 'W';
			*direction = 's';
			return 1;
		}
	}
	else if(*direction == 'n' && *temp_x != 0)		/* facing north and not at the most west position */
	{
		char * left = map[*temp_y]+(*temp_x-1);
		if(*left == '0' || *left == 'W')
		{
			*change = left;
			--(*temp_x);
			*(*change) = 'W';
			*direction = 'w';
			return 1;
		}
	}
	return 0;
}

int turn_back(char * direction)
{
	if(!direction)
	{
		return 0;
	}
	if(*direction == 'e')	/* if facing east */
	{
		*direction = 'w';
		return 1;
	}		
	else if(*direction == 's')	/* if facing south */
	{
		*direction = 'n';
		return 1;
	}
	else if(*direction == 'w')	/* if facing west */
	{
		*direction = 'e';
		return 1;
	}
	else if(*direction == 'n')	/* if facing north */
	{
		*direction = 's';
		return 1;
	}
	return 0;	/* for some reasons direction has no meaning */
}

