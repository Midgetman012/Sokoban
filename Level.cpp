// Project Includes
#include "Level.h"
#include "Framework/AssetManager.h"
#include "Wall.h"

// Library Includes
#include <iostream>
#include <fstream>

Level::Level()
	: m_cellSize(64.0f)
	, m_currentLevel(0)
	, m_background()
	, m_contents()
{
	LoadLevel(1);
}


void Level::Draw(sf::RenderTarget& _target)
{
	// Create and update
	sf::View camera = _target.getDefaultView();
	// Adjust Camera
	camera.setCenter( m_background[0].size() * m_cellSize / 2 ,
		m_background.size() * m_cellSize / 2);

	// Draw game world to the window
	_target.setView(camera);
	for (int y = 0; y < m_background.size(); ++y)
	{
		for (int x = 0; x < m_background[y].size(); ++x)
		{
			_target.draw(m_background[y][x]);
		}
	}

	for (int y = 0; y < m_contents.size(); ++y)
	{
		for (int x = 0; x < m_contents[y].size(); ++x)
		{
			for (int z = 0; z < m_contents[y][x].size(); ++z)
			{
				m_contents[y][x][z]->Draw(_target);
			}
		}
	}

	// RESET VIEW
	_target.setView(_target.getDefaultView());
}


void Level::Update(sf::Time _frameTime)
{
	// TODO
}


void Level::LoadLevel(int _levelToLoad)
{
	// Clean up the old level

	// Delete any data already in the level
	for (int y = 0; y < m_contents.size(); ++y)
	{
		for (int x = 0; x < m_contents[y].size(); ++x)
		{
			for (int z = 0; z < m_contents[y][x].size(); ++z)
			{
				delete m_contents[y][x][z];
			}
		}
	}

	// Clear out our lists
	m_background.clear();
	m_contents.clear();

	// Set the current level
	m_currentLevel = _levelToLoad;


	// Set up the new level

	// Open our file for reading
	std::ifstream inFile;
	std::string fileName = "levels/Level" + std::to_string(m_currentLevel) + ".txt";
	inFile.open(fileName);

	// Make sure the file was opened
	if (!inFile) {
		std::cerr << "Unable to open file " + fileName;
		exit(1); // Call system to stop program with error
	}

	// Set the starting x and y coordinates used to position level objects
	int x = 0;
	int y = 0;

	//Create a first row in our grid
	m_background.push_back(std::vector<sf::Sprite>());
	m_contents.push_back(std::vector<std::vector<GridObject*> >());

	// Create the player first as other objects will need to reference it
	
	// Read each character one by one from the file...
	char ch;
	// Each time, try to read the next character
	// If successful, execute body of loop
	// the "noskipws" means our input from the file will include 
	// the white space (spaces and new lines)
	while (inFile >> std::noskipws >> ch)
	{
		// Perform actions based on what character was read in

		if (ch == ' ')
		{
			++x;
		}
		else if (ch == '\n')
		{
			++y;
			x = 0;

			//Create a new row in our grid
			m_background.push_back(std::vector<sf::Sprite>());
			m_contents.push_back(std::vector<std::vector<GridObject*> >());
		}
		else
		{
			// Create background sprite
			//It will be some object or empty space so we need a background
			m_background[y].push_back(sf::Sprite(AssetManager::GetTexture("graphics/ground.png")));
			m_background[y][x].setPosition(x*m_cellSize, y*m_cellSize);

			// Create an empty vector for our grid contents in this cell
			m_contents[y].push_back(std::vector<GridObject*>());

			if (ch == '-')
			{
				// Do nothing - empty space
			}
			else if (ch == 'W')
			{
				Wall* wall = new Wall();
				wall->SetLevel(this);
				wall->SetGridPosition(x, y);
				m_contents[y][x].push_back(wall);
			}
			else
			{
				std::cerr << "Unrecognised character in level file: " << ch;
			}
		}
	}

	// Close the file now that we are done with it
	inFile.close();

}

void Level::ReloadLevel()
{
	LoadLevel(m_currentLevel);
}

void Level::LoadNextLevel()
{
	LoadLevel(m_currentLevel + 1);
}

float Level::GetCellSize()
{
	return m_cellSize;
}