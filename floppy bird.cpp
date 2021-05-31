#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<ciso646>

void no_cursor()
{
	void *handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cp;
	GetConsoleCursorInfo(handle, &cp);
	cp.bVisible = false;
	SetConsoleCursorInfo(handle, &cp);
}

typedef struct Bird
{
	char symbol = 'Q';
	int x;
	int y;

	void constant_fall()
	{
		x++;
	}

	void movement()
	{
		if (_kbhit())
		{
			char button = _getch();

			switch (button)
			{
			case ' ':
			{	
				x -= 2;
				break;
			}
			}
		}
	}

	void collision(int field_size, int func)
	{
		if (x > field_size || x < 0)
		{
			std::cout << "\n\nYou hit the wall, brother!!!\n";
			std::cout << "Your final score -> "<< func << std::endl;
			exit(1);
		}
	}

}bird;

typedef struct Obstacle
{
	int row;
	int col;
	int col_2;
	int all_rows[20];///size

	void start_pipe(int size)
	{
		row = size/3 + rand() % (size/2);
		col = size*2 - 3;
		col_2 = col + 2;
	}

	void free_line(int size)
	{
		for (int i = 0; i < size; i++)
		{
			all_rows[i] = -1;
		}
	}

	int obstacle_movement(int size)
	{
		static int count = 0;//will be initialized once and then change with every call for the function 
		col--;
		col_2--;

		if (col <= 0 || col_2 <= 0)
		{
			free_line(size);
			start_pipe(size);
			count++;
		}

		return count;
	}

	void lines_down(int x, int size)
	{
		for (int i = x; i < size; i++)
		{
			all_rows[i] = i;
		}
		
	}

	bool shape(int i, int j, char* line)
	{
		bool flag = false;

		if (i == row and j == col or i == row and j == col_2 or i == row and j == col+1)
		{
			*line = '-';
			flag = true;
		}
		else if(all_rows[i] == i and j == col || all_rows[i] == i and j == col_2)
		{
			*line = '|';
			flag = true;
		}

		return flag;
	}

	bool check_birdvsobst(int bird_row, int size)
	{
		bool flag = false;

		for (int i = 0; i < size; i++)
		{
			if (bird_row == all_rows[i])
			{
				flag = true;
			}
		}

		return flag;
	}

	void obstacle_bird_collision(int bird_row, int bird_col, int score, int size)
	{
		if (check_birdvsobst(bird_row, size) and bird_col == col || check_birdvsobst(bird_row, size) and bird_col == col_2)
		{
			std::cout << "\n\n You hit the obstacle my friend \n";
			std::cout << "You final score " << score <<std::endl;
			exit(2);
			system("pause");
		}
	}

}obstacle;


typedef struct Space
{
	int size = 20;

	bool border(int i, int j, char *line)
	{
		bool flag = false;
		if(i == 0 || i == size - 1)
		{
			*line = '-';
			flag = true;
		}
		else if (j==0 || j == size*2-1)
		{
			*line = '|';
			flag = true;
		}
		return flag;
	}

	void write_space(bird player, obstacle rect, int score)
	{
		char border_line;
		char obstacle_line;

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size*2; j++)
			{
				rect.obstacle_bird_collision(player.x, player.y, score, size);

				if (player.x == i and player.y == j)
				{
					std::cout << player.symbol;
				}
				else if (border(i,j, &border_line))
				{
					std::cout << border_line;
				}
				else if(rect.shape(i, j , &obstacle_line))
				{
  					std::cout << obstacle_line;
				}
				else
				{
					std::cout << " ";
				}

			}
			std::cout << std::endl;
		}
	}

}space;



int main(int argc, char* argv[])
{
	srand(time(0));

	no_cursor();

	space field;

	bird player_1;

	obstacle pipe;
	
	player_1.x = (field.size) / 2;
	player_1.y = (field.size)-5;
	
	pipe.start_pipe(field.size);

	int score;

	while (true)
	{
		system("CLS");

		player_1.constant_fall();

		pipe.lines_down(pipe.row, field.size);

		score = pipe.obstacle_movement(field.size);

		field.write_space(player_1, pipe, score);  

		player_1.movement();

		player_1.collision(field.size, pipe.obstacle_movement(field.size));

		std::cout << "Score " << score << std::endl;

		Sleep(10);
		
	}

	return EXIT_SUCCESS;
}




