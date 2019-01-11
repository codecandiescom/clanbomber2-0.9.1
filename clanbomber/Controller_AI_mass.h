#ifndef Controller_AI_mass_h
#define Controller_AI_mass_h

#include "ClanBomber.h"
#include "Controller.h"
#include "Map.h"

#include "clanstring.h"
#include "cl_vector.h"
#include "link.h"

class Bomb;
class Controller_AI_mass;
class Explosion;

enum AIAction
{
  AI_move,
  AI_escape,
  AI_panic,
  AI_wait,
  AI_get,
  AI_put,
  AI_throw
};

class AIPathItem
{
public:
  AIPathItem (Controller_AI_mass* cai, CL_Vector* v, CL_Vector f, AIAction act);
  virtual ~AIPathItem();
  Direction follow();
  bool passing();
  bool passed();
  bool verify_action();
  bool put_bomb();
  bool get_extra();
  CL_Vector* get_map_v();
  void set_end();
private:
  bool verify_move();
  bool verify_escape();
  bool verify_panic();
  bool verify_wait();
  bool verify_get();
  bool verify_put();
  bool verify_throw();
private:
  bool was_passing;
  CL_Vector* map_v;
  Direction v_dir;
  Controller_AI_mass* ai;
  AIAction action;
  int rating;
};

class AIPath
{
public:
  AIPath (Controller_AI_mass* cai, CL_List<CL_Vector>* way, AIAction act);
  virtual ~AIPath();
  Direction follow();
  bool passed();
  bool verify();
  bool continue_path();
private:
  bool verify_step();
  bool verify_actions();
  bool continue_move(Direction dir);
private:
  CL_List<AIPathItem>* path;
  Controller_AI_mass* ai;
  Direction curr_dir;
  int curr_item;
};

class Controller_AI_mass: public Controller
{
public:
  Controller_AI_mass();
  virtual ~Controller_AI_mass();
  void reset();
  void update();
  bool is_left();
  bool is_right();
  bool is_up();
  bool is_down();
  bool is_bomb();
public:
  bool passable (CL_Vector* map_v, int min_rating);
  bool has_extra (CL_Vector* map_v);
  bool has_bomb (CL_Vector* map_v);
  int get_rating (CL_Vector* map_v);
  void set_put_bomb();
  int attack_rating (int map_x, int map_y);
private:
  void update_ratings();
  void update_ratings_passable();
  void update_ratings_explosion();
  void update_ratings_bomb (bool update);
  void update_ratings_extra (bool update);
  bool ground_changed();
  void clear_path();
  bool verify_path();
  void assign_path();
  void follow_path();
  CL_List<CL_Vector>* seek_util();
  CL_List<CL_Vector>* sure_position();
  bool place_bomb();
  CL_List<CL_Vector>* bomb_path();
  Explosion* get_explosion (int map_x, int map_y);
  bool has_extra (int map_x, int map_y);
  bool has_disease (int map_x, int map_y);
  bool enemy_bomber (int map_x, int map_y);
  bool team_bomber (int map_x, int map_y);
  bool destroys_extras (int map_x, int map_y);
  void apply_explosion (Explosion* exp);
  void apply_bomb (int map_x, int map_y, int power, int mode);
  int extra_rating (int map_x, int map_y);
  CL_List<CL_Vector>* path (CL_Vector start_v, CL_Vector dest_v, int min_path_rating, int min_end_rating);
private:
  Map* curr_map;
  Direction curr_dir;
  bool put_bomb;
  int ratings[MAP_WIDTH][MAP_HEIGHT];
  AIPath* curr_path;
  bool box_hits[MAP_WIDTH][MAP_HEIGHT];
  int explosions[MAP_WIDTH][MAP_HEIGHT];
};

#endif
