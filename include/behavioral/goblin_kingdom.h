#pragma once
#include <vector>

using namespace std;

struct Creature;
class Goblin;
class GoblinKing;

// ============================================================================
// Game - Container for all creatures
// ============================================================================
struct Game
{
  vector<Creature*> creatures;
};

// ============================================================================
// Creature - Base class for all creatures
// ============================================================================
struct Creature
{
protected:
  Game& game;
  int base_attack, base_defense;

public:
  Creature(Game& game, int base_attack, int base_defense)
    : game(game), base_attack(base_attack), base_defense(base_defense)
  {
  }

  virtual int get_attack() = 0;
  virtual int get_defense() = 0;
  virtual ~Creature() = default;
};

// ============================================================================
// Goblin - Regular goblin with stats based on nearby GoblinKings
// ============================================================================
class Goblin : public Creature
{
public:
  Goblin(Game& game, int base_attack, int base_defense)
    : Creature(game, base_attack, base_defense)
  {
  }

  Goblin(Game& game)
    : Creature(game, 1, 1)
  {
  }

  int get_attack() override;
  int get_defense() override;
};

// ============================================================================
// GoblinKing - Powerful goblin, increases attack of nearby goblins
// ============================================================================
class GoblinKing : public Goblin
{
public:
  GoblinKing(Game& game)
    : Goblin(game, 3, 3)
  {
  }

  int get_attack() override;
  int get_defense() override;
};

// ============================================================================
// METHOD IMPLEMENTATIONS (defined outside class bodies)
// ============================================================================
// This allows Goblin methods to reference the complete GoblinKing type

inline int Goblin::get_attack()
{
  int attack = base_attack;
  for (auto c : game.creatures)
  {
    if (c != this)
    {
      // Check if this creature is a GoblinKing
      if (dynamic_cast<GoblinKing*>(c))
        attack += 1;
    }
  }
  return attack;
}

inline int Goblin::get_defense()
{
  int defense = base_defense;
  for (auto c : game.creatures)
  {
    if (c != this)
    {
      // Check if this creature is a Goblin (includes GoblinKing since it inherits)
      if (dynamic_cast<Goblin*>(c))
        defense += 1;
    }
  }
  return defense;
}

inline int GoblinKing::get_attack()
{
  int attack = base_attack;
  for (auto c : game.creatures)
  {
    if (c != this)
    {
      // GoblinKing's attack bonus comes from other GoblinKings
      if (dynamic_cast<GoblinKing*>(c))
        attack += 1;
    }
  }
  return attack;
}

inline int GoblinKing::get_defense()
{
  int defense = base_defense;
  for (auto c : game.creatures)
  {
    if (c != this)
    {
      // GoblinKing's defense bonus comes from other Goblins
      if (dynamic_cast<Goblin*>(c))
        defense += 1;
    }
  }
  return defense;
}
