// Tetris_final.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <utility>
#include <functional>

using namespace std;

#define LEFT 'a'
#define RIGHT 'd'
#define DOWN 's'

#define FieldWidth 14
#define FieldHeight 30

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
	static Figure* randFigure()
	{
		//int random = rand() % 7;
		//int random = 1;
		//
		//if (random == 1)
		//{
		//	Figure *a = new I;
		//	return a;
		//}
	}

	void SetCurrentCoords(int x, int y)
	{
		currentCoords.X = x;
		currentCoords.Y = y;
	}

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

	void figureElementsInit(int figurePosition=0)
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

	vector<COORD> GetFigureElements()
	{
		return figureElement;
	}
};


class Field
{
private:
	array<array<bool, FieldWidth>, FieldWidth> field = {0};

public: 
	void printField()
	{
		cout << endl;
		for (auto it_y : field)
		{
			cout << "  ";
			for (auto it_x : it_y)
			{
				cout << it_x;
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

	void figureMoove(vector<COORD> figureElements, COORD& currentMatrixCoords, char side)
	{
		// It helps me with finding needed elements
		array<array<bool, 4>, 4> tmpFigureMatrix = {0};
		for (int i = 0; i < figureElements.size(); i++)
		{
			tmpFigureMatrix[figureElements[i].Y][figureElements[i].X] = true;
		}

		

		if (side == LEFT)
		{
			vector<COORD> leftElements; // needed elements coords vector

			int vectEl_counter = 0;
			// Finding needed elements
			for (int y = FigureHeight - 1; y >= 0; y--)
			{
				for (int x = FigureWidth - 1; x >= 0; x--)
				{
					if (tmpFigureMatrix[y][x] == true)
					{
						COORD tmp;
						tmp.X = x;
						tmp.Y = y;
						leftElements.push_back(tmp);
					}
				}
				vectEl_counter++;
			} //------------------------
			

			clearFigure(figureElements, currentMatrixCoords);

			bool isPossible = true;
			for (int i = 0; i < leftElements.size(); i++)
			{
				if (field[leftElements[i].Y][leftElements[i].X - 1] == true || field[leftElements[i].Y][leftElements[i].X - 1] <= 0)
				{
					isPossible = false;
				}
			}

			if (isPossible == true)
			{
				currentMatrixCoords.X -= 1;
				SetFigureIn(figureElements, currentMatrixCoords);
			}
			else
			{
				SetFigureIn(figureElements, currentMatrixCoords);
			}
		}
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


public:
	static Game& GetInstance()
	{
		static Game Tetris;
		return Tetris;
	}

	void TetrisGame()
	{
		//FIXME:
		//Figure *figure = Figure::randFigure();
		int counter = 0;
		while (1)
		{
			I figure;
			figure.figureElementsInit();
			if (counter == 0)
			{
				figure.SetCurrentCoords(rand() % 11, 0);
				counter++;
			}

			Field glass;
			glass.SetFigureIn(figure.GetFigureElements(), figure.GetCurrentCoords());
			glass.figureMoove(figure.GetFigureElements(), figure.GetCurrentCoords(), LEFT);
			glass.printField();
			Sleep(700);
			system("cls");
		}
	}
};

int main()
{
	srand(time(0));
	Game& Tetris = Game::GetInstance();
	Tetris.TetrisGame();


	//figure.PrintFigure();
	//figure.PrintFigureElements();


    return 0;
}

