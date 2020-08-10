
#include <iostream>
#include "Log.h"
#include <fstream>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <sstream>



class cTetromino 
{
	olc::Pixel color;
	int m_ShapeType;
	int m_ShapeSize = 4;	
	short m_rotation = 0;
	std::string m_piece;
public:	
	int x = 0;
	int y = 0;	
	olc::vd2d pos;	

	cTetromino()
	{
		m_rotation = 0;
		m_ShapeType = rand() % 7 +1;
		x = 8 / 2;
		y = 0;
		
		

		switch (m_ShapeType)
		{
		case 1:
			color = olc::CYAN;
			y++;
			m_piece =  " #  ";
			m_piece += " #  ";
			m_piece += " #  ";
			m_piece += " #  ";
			break;
		case 2:
			color = { 255,255,0 };

			m_piece =  "    ";
			m_piece += " ## ";
			m_piece += " ## ";
			m_piece += "    ";
			break;
		case 3:
			color = { 255,0,0 }; // red

			m_piece =  "    ";
			m_piece += " ## ";
			m_piece += "  ##";
			m_piece += "    ";
			break;
		case 4:
			color = { 0,255,0 }; //green

			m_piece =  "    ";
			m_piece += " ## ";
			m_piece += "##  ";
			m_piece += "    ";
			break;
		case 5:
			color = { 64,64,0 };
			y++;
			m_piece =  " #  ";
			m_piece += " #  ";
			m_piece += " ## ";
			m_piece += "    ";
			break;
		case 6:
			color = { 0,0,255 };
			y++;
			m_piece =  "  # ";
			m_piece += "  # ";
			m_piece += " ## ";
			m_piece += "    ";
			break;
		case 7:
			color = { 255,0,255 };
			m_piece =  "    ";
			m_piece += "### ";
			m_piece += " #  ";
			m_piece += "    ";
			break;


		default:
			m_piece = "invalid shape";
			break;
		}

	};
	~cTetromino(){ }
	
	
	int Shapetype() { return m_ShapeType; };
	void Move(int moveUnitsy, int moveUnitsx,int gridSize)
	{
		y += moveUnitsy;
		x += moveUnitsx;
		pos.y = (double)y * gridSize;
		pos.x = (double)x * gridSize;
		

	};
	void rotate() { m_rotation++; }
	char getPieceRotated(int iy,int ix,int rotation=0)
	{		
		
		switch ((m_rotation + rotation) % 4)
		{
		case 0:
			return m_piece[iy * 4 + ix];
		case 1:
			return m_piece[12 + iy - (ix * 4)];
		case 2:
			return m_piece[15 - (iy * 4) - ix];
		case 3:
			return m_piece[3 - iy + (ix * 4)];
		}
	}	

};

class cTetris : public olc::PixelGameEngine
{
public:
	EnLogger logger;
	int TetrominoCount = 1;
	std::vector<cTetromino> Tetrominos;
	olc::Pixel borderColor = olc::DARK_GREY;
	unsigned long Score = 0;
	std::string HighScore="0";
	int GridSize = 8;
	float GameTimer = 0;
	float GameSpeed = 1;
	float downMod = 1;
	float GameTikk = 0.7f;
	bool updatedSpeed = false;
	olc::vd2d pos = { 0,0 };
	olc::vd2d size = {(double)GridSize*0.9f,(double)GridSize*0.9f };	
	std::vector<int> completedLines;
	static const int rows = 22;
	static const int cols = 12;
	bool RemoveLines = false;
	short RemoveCounter = 1;
	int shapecounter[7] = { 0,0,0,0,0,0,0 }; 

	//std::string Field;	
	
	int Field[rows][cols];

	void CheckCompletedLines(int y, int x) 
	{		
		olc::vd2d dpos;
		for (int rw = y; rw < y + 4; rw++) 
		{
			if (rw >= rows) break;
			for (int col = 1; col < (cols-1); col++) 
			{
				if (Field[rw][col] == 0 || Field[rw][col]==8 || Field[rw][col] == 9) break;
				if (col == (cols - 2)) 
				{
					completedLines.push_back(rw);
					RemoveLines = true;
				}
				
			}
		}
		for (auto rw : completedLines) 
		{
			for (int col = 1; col < (cols-1) ; col++)
			{
				Field[rw][col] = 8;
				dpos.y = (double)rw * GridSize;
				dpos.x = (double)col * GridSize;
				dRect(dpos,8);				
			}

		}

	}
	bool shapeFits(int y,int x, int rotationoffset=0)
	{		
		
		for (int index=0;index< 16; index ++)		
			if (Tetrominos[0].getPieceRotated( index / 4,index % 4,rotationoffset) == '#')
				if (Field[y + index / 4][x + index % 4] != 0)
				{					
					//std::cout << "R: " << y + index / 4 << '\t' << " C: " + x + index % 4 << '\t' << " Rw/Col Field Value: " << +Field[y + index / 4][x + index % 4] << std::endl;					
					return false;
				};
		return true;
	}
	void updateField(int n=0)	{

		//insert shape
		if (n > 0 && n< 8)
		{
			for (int index = 0; index < 16; index++)
				if (Tetrominos[0].getPieceRotated(index / 4, index % 4 )== '#')
					Field[Tetrominos[0].y + (index / 4)][Tetrominos[0].x + (index % 4)] = n;

			CheckCompletedLines(Tetrominos[0].y, Tetrominos[0].x);
		}
		
		if (n == 0)
		{
			for (int i = 0; i < rows; i++) //blank field with borders
				for (int j = 0; j < cols; j++)
				{					
					if (i == 0 || i == rows - 1) n = 9;
					if (j == 0 || j == cols - 1) n = 9;
					Field[i][j] = n;
					n = 0;
				};
		}
		if (n == 8)
		{
			switch (completedLines.size())
			{
			case 1:
				updateInfo(100);
				break;
			case 2:
				updateInfo(400);
				break;
			case 3:
				updateInfo(1500);
				break;
			case 4:
				updateInfo(5000);
				break;
			}
			for (auto rw : completedLines)
			{
				for (int col = 1; col < (cols - 1); col++)
					Field[rw][col] = 0;

				for (int i = rw; i > 1; i--)
					for (int j = 1; j < (cols - 1); j++)
					{
						Field[i][j] = Field[i - 1][j];
					}
				completedLines.pop_back();
			}
			drawField();
		}
		
	}
	void drawField()
	{
		for (int i = 0; i < rows; i++)
		{
			pos.y = (double)i * GridSize;
			for (int j = 0; j < cols; j++)
			{
				pos.x = (double)j * GridSize;
				dRect(pos, Field[i][j]);
				
			};
		};
	};
	void updateInfo(int AddScore)
	{
		olc::vd2d infoPos = { (double)(cols + 1) * GridSize,(double)0 };
		olc::vd2d infoSize = { (double)9 * GridSize,(double)rows * GridSize };
		
		FillRect(infoPos,infoSize, olc::BLACK);		
		Score = Score + AddScore;
		
		DrawString(infoPos, "Score:" );
		infoPos.y +=(double)2 * GridSize;
		DrawString(infoPos , std::to_string(Score));
		infoPos.y +=(double)2 * GridSize;
		DrawString(infoPos, "Highscore:");
		infoPos.y += (double)2 * GridSize;
		DrawString(infoPos, HighScore);
		infoPos.y += (double)2 * GridSize;
		DrawString(infoPos, "Next:");
		infoPos.y += (double)2 * GridSize;
		drawTetromino(&Tetrominos[1] ,infoPos);
	}
	void drawTetromino(cTetromino* t, olc::vd2d DrawPos)
	{	
		olc::vd2d OffsetPos;
		for (int index = 0; index < 16; index++)			
			if (t->getPieceRotated(index / 4, index % 4) == '#')
			{				
				OffsetPos.x =DrawPos.x + ((double)(index % 4) *GridSize);
				OffsetPos.y =DrawPos.y + ((double)(index / 4) *GridSize);
				dRect(OffsetPos,t->Shapetype());				
			};		
		
	}
	void dRect(olc::vd2d p, int color)
	{
		switch (color)
		{
		case 1:
			FillRect(p, size, olc::CYAN);
			DrawRect(p, size,  olc::DARK_GREY);
			break;
		case 2:
			FillRect(p, size, olc::YELLOW);
			DrawRect(p, size, olc::DARK_GREY);
			break;
		case 3:
			FillRect(p, size, olc::RED);
			DrawRect(p, size, olc::DARK_GREY);
			break;
		case 4:
			FillRect(p, size, olc::GREEN);
			DrawRect(p, size, olc::DARK_GREY);
			break;
		case 5:
			FillRect(p, size, olc::DARK_YELLOW);
			DrawRect(p, size, olc::DARK_GREY);
			break;
		case 6:
			FillRect(p, size, olc::BLUE);
			DrawRect(p, size, olc::DARK_GREY);
			break;
		case 7:
			FillRect(p, size, olc::DARK_MAGENTA);
			DrawRect(p, size, olc::DARK_GREY);
			break;
		case 8:
			FillRect(p, size, olc::WHITE);
			break;
		case 9:
			FillRect(p, size, olc::GREY);
			DrawRect(p, size, olc::DARK_GREY);
			break;
		default:
			FillRect(p, size, olc::BLACK);
			DrawRect(p, size, olc::BLACK);
			
			break;
		}
	}


public:
	cTetris()
	{
		sAppName = "Tetris";
	}

public:
	bool OnUserCreate() override
	{
		while (Tetrominos.size() < 2)
		{
			cTetromino Tetromino;
			Tetrominos.push_back(Tetromino);
		}
		std::fstream myfile("highscore.dat", std::ios::in);		
		if (myfile.is_open())		{
			
			myfile >> HighScore;
			myfile.close();
		}
		else
		{
			/*
			std::fstream myfile("highscore.dat", std::ios::out);
			myfile << 0;
			myfile.close();
			*/
			std::cout << "File Not Found" << std::endl;
			
			
		}
		updateField();
		drawField();
		updateInfo(0);
		Tetrominos[0].Move(0,0,GridSize);
		drawTetromino(&Tetrominos[0], Tetrominos[0].pos);
		
		// Called once at the start, so create things here	

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{			
		
		//get input
		if (GetKey(olc::UP).bPressed && shapeFits(Tetrominos[0].y, Tetrominos[0].x, 1))		
			Tetrominos[0].rotate();		

		if (GetKey(olc::LEFT).bPressed && shapeFits(Tetrominos[0].y, Tetrominos[0].x - 1, 0))			
				Tetrominos[0].Move(0,-1,GridSize);

		if (GetKey(olc::RIGHT).bPressed && shapeFits(Tetrominos[0].y, Tetrominos[0].x + 1, 0))
			Tetrominos[0].Move(0, 1, GridSize);
		
		if (GetKey(olc::DOWN).bHeld)
			downMod = 7;
		else downMod = 1;

		if (TetrominoCount % 10 == 0 && (!updatedSpeed))
		{
			std::cout << "Updated Speed " << TetrominoCount << std::endl;
			updatedSpeed = true;
			//EnLogger::info << "Updated Speed";
			GameSpeed += 0.1;
		}
		GameTimer += fElapsedTime*(GameSpeed*downMod);
				
		///GAMETICK 
		
		if ( GameTimer >= GameTikk)
		{
			GameTimer = 0;

			if (RemoveLines)
				if (RemoveCounter==0)
				{
				RemoveLines = false;
				RemoveCounter = 1;
				updateField(8);
				}
				else
					RemoveCounter--;
							
		
			if (shapeFits(Tetrominos[0].y+1, Tetrominos[0].x,0))				
				Tetrominos[0].Move(1, 0, GridSize);
			else
			{
				updateField(Tetrominos[0].Shapetype());				
				updatedSpeed = false;
				shapecounter[Tetrominos[0].Shapetype()-1]++;
				TetrominoCount ++;				
				Tetrominos[0] = Tetrominos[1];
				Tetrominos[0].Move(0, 0, GridSize);
				Tetrominos.pop_back();
				cTetromino t;
				Tetrominos.push_back (t);
				updateInfo(15);
				
				if (!shapeFits(Tetrominos[0].y, Tetrominos[0].x,0))
				{
					DrawString(10, 10, "GAME OVER", olc::RED,2 );
					if (Score > (long)std::stoi(HighScore))
					{
						std::fstream myfile("highscore.dat",std::ios::out);
						myfile << Score;
						myfile.close();
					}
					for (auto n : shapecounter)
						std::cout << n << '\t';
					return false;
				}

			}
			
		}
		//screen update

		//draw field
		drawField();

		// tetromino
		drawTetromino(&Tetrominos[0], Tetrominos[0].pos);

		return true;
	}
};


int main()
{	
	srand(time(0));
	cTetris Tetris;
	if (Tetris.Construct(192, 176, 4, 4))
		Tetris.Start();
	
	return 0;
}

