	#include <stdio.h>
	#include <stdint.h>
	#include <stdbool.h>
	#include <stdlib.h>
	#include <Windows.h>
	#include <conio.h>
	#include <time.h>
	#include <math.h>

	void Move(char *c);
	void PickUpLogic();
	int RandomInt(int max);
	void EnemyLogic();
	int sign(int num);
	void Render();
	void PrintColouredPx(char colour, int length);
	void Restart();

	typedef struct
	{
		unsigned char x;
		unsigned char y;
	} Entity;

	Entity Player = {0, 0};
	Entity Loot = {0, 0};
	Entity Enemy = {5, 5};

	int64_t Score = -10;
	const int fps = 30;
	int64_t Tick = 0;
	uint8_t Health = 20;

	bool Paused = false;

	int main()
	{
	#ifdef _WIN32
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut == INVALID_HANDLE_VALUE)
		{
			return 1;
		}

		DWORD dwMode = 0;
		if (!GetConsoleMode(hOut, &dwMode))
		{
			return 1;
		}

		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!SetConsoleMode(hOut, dwMode))
		{
			return 1;
		}
	#endif

		uint64_t Time = time(NULL);
		srand(Time);

		system("cls");

		Render();
		_getch();
		while (1)
		{
			char c;
			bool input = _kbhit();
			if (input)
			{
				c = _getch();
				if (c == 'p')
				{
					Paused ^= 1;
				}
			}

			if (!Paused)
			{
				if (input)
				{
					Move(&c);
				}
				PickUpLogic();
				EnemyLogic();
				Render();
				rand();
				Tick++;
			}
			printf("\033[?25h\033[0m");
			Sleep(1000 / fps);
		}
	}

	void Move(char *c)
	{
		switch (*c)
		{
		case 'w':
		{
			(Player.y)--;
			if (Player.y < 0)
			{
				Player.y = 9;
			}
			break;
		}
		case 'a':
		{
			Player.x--;
			if (Player.x < 0)
			{
				Player.x = 9;
			}
			break;
		}
		case 's':
		{
			Player.y++;
			if (Player.y > 9)
			{
				Player.y = 0;
			}
			break;
		}
		case 'd':
		{
			Player.x++;
			if (Player.x > 9)
			{
				Player.x = 0;
			}
			break;
		}
		}
	}

	void PickUpLogic()
	{
		if (Player.x != Loot.x || Player.y != Loot.y)
		{
			return;
		}
		Loot.x = RandomInt(10);
		Loot.y = RandomInt(10);
		Score += 10;
		if (Score >= 500)
		{
			Render();
			printf("\nYou've won!!!");
			Restart();
		}
	}

	int RandomInt(int max)
	{
		for (int i = 0; i < rand() % 128; i++)
		{
			rand();
		}
		return rand() % max;
	}

	void EnemyLogic()
	{
		if (Tick % 10)
		{
			return;
		}
		int XDist = Enemy.x - Player.x;
		int YDist = Enemy.y - Player.y;
		if (abs(XDist) > abs(YDist))
		{
			Enemy.x -= sign(XDist);
		}
		else
		{
			Enemy.y -= sign(YDist);
		}
		if (Enemy.x != Player.x || Enemy.y != Player.y)
		{
			return;
		}
		Score -= 5;
		Health--;
		if (Health == 0)
		{
			Render();
			printf("\nYou've lose :(");
			Restart();
		}
	}

	int sign(int num)
	{
		if (num > 0)
		{
			return 1;
		}
		if (num < 0)
		{
			return -1;
		}
		return 0;
	}

	void Render()
	{
		char Buffer[] = "++++++++++++\n"
										"+          +\n"
										"+          +\n"
										"+          +\n"
										"+          +\n"
										"+          +\n"
										"+          +\n"
										"+          +\n"
										"+          +\n"
										"+          +\n"
										"+          +\n"
										"++++++++++++";

		Buffer[(Player.x + 1) + 13 * (Player.y + 1)] = '@';
		Buffer[(Enemy.x + 1) + 13 * (Enemy.y + 1)] = '!';
		Buffer[(Loot.x + 1) + 13 * (Loot.y + 1)] = '$';

		printf("\033[?25l\033[H%lld", Score);
		int x = snprintf(NULL, 0, "%lld", Score);
		for (int i = 0; i < (4 - x); i++)
		{
			printf(" ");
		}

		PrintColouredPx('r', Health);
		PrintColouredPx('d', 20 - Health);
		printf("\n");

		int i = 0;
		while (true)
		{
			switch (Buffer[i])
			{
			case ' ':
			{
				printf("  ");
				break;
			}
			case ('+'):
			{
				PrintColouredPx('w', 2);
				break;
			}
			case ('@'):
			{
				PrintColouredPx('g', 2);
				break;
			}
			case ('$'):
			{
				PrintColouredPx('y', 2);
				break;
			}
			case ('!'):
			{
				PrintColouredPx('r', 2);
				break;
			}
			case ('\n'):
			{
				printf("\n");
				break;
			}
			case ('\0'):
			{
				printf("\033[?25h");
				return;
			}
			}
			i++;
		}
	}

	/**
	 * @param colour b for black, r for red, g for green, y for yellow, l for blue, m for magenta, c for cyan and w for white, other will be default
	 */
	void PrintColouredPx(char colour, int length)
	{
		switch (colour)
		{
		case 'b':
		{
			printf("\033[40m");
			break;
		}
		case 'r':
		{
			printf("\033[41m");
			break;
		}
		case 'g':
		{
			printf("\033[42m");
			break;
		}
		case 'y':
		{
			printf("\033[43m");
			break;
		}
		case 'l':
		{
			printf("\033[44m");
			break;
		}
		case 'm':
		{
			printf("\033[45m");
			break;
		}
		case 'c':
		{
			printf("\033[46m");
			break;
		}
		case 'w':
		{
			printf("\033[47m");
			break;
		}
		}

		for (int i = 0; i < length; i++)
		{
			printf(" ");
		}

		printf("\033[49m");
	}

	void Restart()
	{
		_getch();
		system("cls");
		Player = (Entity){0, 0};
		Loot = (Entity){0, 0};
		Enemy = (Entity){5, 5};

		Score = -10;
		Tick = 0;
		Health = 20;

		Paused = false;

		Render();
	}
