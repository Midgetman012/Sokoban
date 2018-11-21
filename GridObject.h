#pragma once

//Project includes
#include "Framework/SpriteObject.h"
#include "Level.h"

// Forward declaration
// only works with pointers/reference
class Level:

class GridObject : public SpriteObject
{

public:

	GridObject();

	void SetGridPosition(sf::Vector2i _newPosition);
	void SetGridPosition(int _x, int _y);

	void SetLevel(Level* _newLevel);

private:

	sf::Vector2i m_gridPosition;
	Level* m_level;
};
