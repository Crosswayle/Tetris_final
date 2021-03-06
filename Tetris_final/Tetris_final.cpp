
#include "stdafx.h"
#include <utility>
#include <functional>

using namespace std;

HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

#define LEFT 'a'
#define RIGHT 'd'
#define DOWN 's'

#define FieldWidth 14
#define FieldHeight 20

#define PositionsCount 4 // count of possible rotate positions
#define FigureWidth 4
#define FigureHeight 4

#define FiguresCount 7	 

class Figure
{
private:
	COORD currentCoords; // Coords of figure matrix in field(glass) matrix


public:
	//FIXME:
	void virtual figureElementsInit(int figurePosition = 0) = 0;
	virtual vector<COORD>& GetFigureElements() = 0;

	//void SetCurrentCoords(int x, int y)
	//{
	//	currentCoords.X = x;
	//	currentCoords.Y = y;
	//}

	COORD& GetCurrentCoords()
	{
		return currentCoords;
	}


};

class I : public Figure
{
private:
	array<array<array<bool, FigureWidth>, FigureHeight>, PositionsCount> figureMatrix;
	vector<COORD> figureElement; // Coords in figure matrix
public:
	I()
	{
		//Vertical
		figureMatrix[0][0][1] = 1;
		figureMatrix[0][1][1] = 1;
		figureMatrix[0][2][1] = 1;
		figureMatrix[0][3][1] = 1;

		figureMatrix[2][0][1] = 1;
		figureMatrix[2][1][1] = 1;
		figureMatrix[2][2][1] = 1;
		figureMatrix[2][3][1] = 1;

		//Horisontal
		figureMatrix[1][2][0] = 1;
		figureMatrix[1][2][1] = 1;
		figureMatrix[1][2][2] = 1;
		figureMatrix[1][2][3] = 1;

		figureMatrix[3][2][0] = 1;
		figureMatrix[3][2][1] = 1;
		figureMatrix[3][2][2] = 1;
		figureMatrix[3][2][3] = 1;
	}

	void figureElementsInit(int figurePosition = 0)
	{
		COORD tmp;
		for (int y = 0; y < FigureHeight; y++)
		{
			for (int x = 0; x < FigureWidth; x++)
			{
				if (figureMatrix[figurePosition][y][x] == 1)
				{
					tmp.X = x;
					tmp.Y = y;
					figureElement.push_back(tmp);
				}
			}
		}
	}

	//Tetsing methods:
	void PrintFigure(int FigurePos = 0)
	{
		for (auto it_y : figureMatrix[FigurePos])
		{
			for (auto it_x : it_y)
			{
				cout << it_x;
				/*if (it_x == 0)
				{
				cout << ' ';
				}

				else
				{ cout << it_x; }
				*/
			}
			cout << endl;
		}
	}
	void PrintFigureElements()
	{
		for (auto it_coord : figureElement)
		{
			// It prints (Y,X) 
			cout << "[" << it_coord.Y << "," << it_coord.X << "]" << endl;
		}
	}

	vector<COORD>& GetFigureElements()
	{
		return figureElement;
	}
};


class Field
{
private:
	array<array<bool, FieldWidth>, FieldHeight> field = { 0 };

public:
	void printField(int score)
	{
		cout << "  SCORE: " << score << endl;
		for (int i = 0;i<FieldHeight;i++)
		{
			cout << "  ";
			for (int k=0;k<FieldWidth;k++)
			{	
				if (field[i][k] == 0)
				{
					SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_GREEN | 4);
					cout << char(177);
				}
				else
				{
				SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_GREEN | 8);
				cout << char(176);
				}
				
			}
			cout << endl;
		}
	}

	void SetFigureIn(vector<COORD> figureElements, COORD currentMatrixCoords)
	{
		for (auto it_coords : figureElements)
		{
			field[it_coords.Y + currentMatrixCoords.Y][it_coords.X + currentMatrixCoords.X] = true;
		}
	}

	void clearFigure(vector<COORD> figureElements, COORD currentMatrixCoords)
	{
		for (auto it_coords : figureElements)
		{
			field[it_coords.Y + currentMatrixCoords.Y][it_coords.X + currentMatrixCoords.X] = false;
		}
	}

	void figureMoove(Figure *a, char side, bool& IsFallen)
	{
		// It helps me with finding needed elements
		array<array<bool, 4>, 4> tmpFigureMatrix = { 0 };
		for (int i = 0; i < a->GetFigureElements().size(); i++)
		{
			tmpFigureMatrix[a->GetFigureElements()[i].Y][a->GetFigureElements()[i].X] = true;
		}


		if (side == LEFT)
		{
			vector<COORD> leftElements; // needed elements coords vector
			COORD tmp;
			int vectEl_counter = 0;
			// Finding needed elements
			for (int y = FigureHeight - 1; y >= 0; y--)
			{
				bool refresh = false;
				for (int x = FigureWidth - 1; x >= 0; x--)
				{
					if (tmpFigureMatrix[y][x] == true)
					{
						tmp.X = x;
						tmp.Y = y;
						refresh = true;
					}
				}
				if (refresh == true)
				{
					leftElements.push_back(tmp);
					vectEl_counter++;
				}
			} //------------------------

			bool isPossible = true;
			for (int i = 0; i < leftElements.size(); i++)
			{
				COORD abs_coords;
				abs_coords.X = a->GetCurrentCoords().X + leftElements[i].X;
				abs_coords.Y = a->GetCurrentCoords().Y + leftElements[i].Y;
				if (abs_coords.X - 1 < 0 ||  field[abs_coords.Y][abs_coords.X - 1] == true  )
				{
					isPossible = false;
				}
			}
			clearFigure(a->GetFigureElements(), a->GetCurrentCoords());
			if (isPossible == true)
			{
				a->GetCurrentCoords().X -= 1;
				SetFigureIn(a->GetFigureElements(), a->GetCurrentCoords());
			}
			else
			{
				SetFigureIn(a->GetFigureElements(), a->GetCurrentCoords());
			}
		}

		if (side == RIGHT)
		{
			vector<COORD> rightElements; // needed elements coords vector

			int vectEl_counter = 0;
			COORD tmp;
			// Finding needed elements
			for (int y = 0; y < FigureHeight; y++)
			{
				bool refresh = false;
				for (int x = 0; x < FigureWidth; x++)
				{

					if (tmpFigureMatrix[y][x] == true)
					{
						tmp.X = x;
						tmp.Y = y;
						refresh = true;
					}

				}
				if (refresh == true)
				{
					rightElements.push_back(tmp);
					vectEl_counter++;
				}
			} //------------------------

			bool isPossible = true;
			for (int i = 0; i < rightElements.size(); i++)
			{
				COORD abs_coords;
				abs_coords.X = a->GetCurrentCoords().X + rightElements[i].X;
				abs_coords.Y = a->GetCurrentCoords().Y + rightElements[i].Y;
				if (abs_coords.X + 1 >= FieldWidth  || field[abs_coords.Y][abs_coords.X + 1] == true)
				{
					isPossible = false;
				}
			}
			clearFigure(a->GetFigureElements(), a->GetCurrentCoords());

			if (isPossible == true)
			{
				a->GetCurrentCoords().X += 1;
				SetFigureIn(a->GetFigureElements(), a->GetCurrentCoords());
			}
			else
			{
				SetFigureIn(a->GetFigureElements(), a->GetCurrentCoords());
			}
		}

		if (side == DOWN)
		{
			vector<COORD> bottomElements; // needed elements coords vector
			COORD tmp;
			int vectEl_counter = 0;
			// Finding needed elements
			for (int x = 0; x < FigureWidth; x++)
			{
				bool refresh = false;
				for (int y = 0; y <FigureHeight; y++)
				{

					if (tmpFigureMatrix[y][x] == true)
					{
						tmp.X = x;
						tmp.Y = y;
						refresh = true;
					}
				}
				if (refresh == true)
				{
					bottomElements.push_back(tmp);
					vectEl_counter++;
				}
			} //------------------------

			bool isPossible = true;
			for (int i = 0; i < bottomElements.size(); i++)
			{
				COORD abs_coords;
				abs_coords.X = a->GetCurrentCoords().X + bottomElements[i].X;
				abs_coords.Y = a->GetCurrentCoords().Y + bottomElements[i].Y;
				if (abs_coords.Y + 1 >= FieldHeight || field[abs_coords.Y+1][abs_coords.X] == true)
				{
					isPossible = false;
				}
			}
			clearFigure(a->GetFigureElements(), a->GetCurrentCoords());
			if (isPossible == true)
			{
				a->GetCurrentCoords().Y += 1;
				SetFigureIn(a->GetFigureElements(), a->GetCurrentCoords());
			}
			else
			{
				SetFigureIn(a->GetFigureElements(), a->GetCurrentCoords());
				IsFallen = true;
			}
		}
	}

	auto GetField()
	{
		return field;
	}
};


class Game
{
private:
	Game()
	{

	}
	~Game()
	{

	}
	Game(const Game&)
	{

	}
	Game operator=(const Game&)
	{

	}
	//----------------------------------

	Figure* randFigure()
	{
		int random = rand() % 7;
		random = 1;

		if (random == 1)
		{
			return new I();
		}
	}

	int score = 0 - 1;

public:
	void SetScore(int k = 1)
	{
		score += k;
	}

	static Game& GetInstance()
	{
		static Game Tetris;
		return Tetris;
	}

	int GetScore()
	{
		return score;
	}

	bool GameOver(Field &a)
	{
		for (int i = 0; i < FieldWidth; i++)
		{
			if (a.GetField()[0][i] == true)
			{
				return false;	
			}
		}
		return true;
	}

	void GameOverPrint()
	{
		system("mode con:cols=60 lines=10");
		SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED | 0);
		cout << "  _____   ___  ___  ___ _____   _____  _   _ ___________" << endl;
		cout << " |  __ \\ / _ \\ |  \\/  ||  ___| |  _  || | | |  ___| ___ \\" << endl;
		cout << " | |  \\// /_\\ \\| .  . || |__   | | | || | | | |__ | |_/ /" << endl;
		cout << " | |_\\ \\| | | || |\\/| || |___  \\ \\_/ /\\ \\_/ / |___| |\\ \\ " << endl;
		cout << "  \\____/\\_| |_/\\_|  |_/\\____/   \\___/  \\___/\\____/\\_| \\_|" << endl;
		cout << endl;
		SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_GREEN | 4);
		cout << "                    YOUR SCORE: ";
		cout << GetScore() << endl;
		SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_GREEN | 7);
		cout << endl << endl;
	}

	void TetrisGame()
	{
		
		bool IsFallen = false;
		bool IsStart = true;
		Field glass;
		Figure *figure = randFigure(); 
		while (GameOver(glass)!=true)
		{
			char side;
			if (IsFallen == true || IsStart == true)
			{
				SetScore();
				IsStart = false;
				IsFallen = false;
				figure->figureElementsInit();
				figure->GetCurrentCoords().X = rand() % 11;
				figure->GetCurrentCoords().Y = 0;
			}

			
			glass.SetFigureIn(figure->GetFigureElements(), figure->GetCurrentCoords());

			glass.printField(GetScore());
			
			Sleep(10);
			// if (_kbhit() == true)
			// {
			// 	side = _getch();
			// 	glass.figureMoove(figure, side, IsFallen);
			// }
			
			system("cls");

			glass.figureMoove(figure, DOWN, IsFallen);
		}
		
		GameOverPrint();
	}
};

int main()
{
	system("mode con:cols=18 lines=22");
	system("color 0F");
	srand(time(0));
	Game& Tetris = Game::GetInstance();
	Tetris.TetrisGame();


	//figure.PrintFigure();
	//figure.PrintFigureElements();


	return 0;
}