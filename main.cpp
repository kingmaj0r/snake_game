#include <gtkmm.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

const int GRID_SIZE = 20;
const int GRID_WIDTH = 20;
const int GRID_HEIGHT = 20;

enum class Direction
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

class SnakeGame : public Gtk::DrawingArea
{
public:
  SnakeGame()
      : snake({{GRID_WIDTH / 2, GRID_HEIGHT / 2}}),
        food(5, 5),
        direction(Direction::RIGHT),
        score(0),
        high_score(0),
        game_over(false)
  {
    std::srand(std::time(nullptr));
    add_events(Gdk::KEY_PRESS_MASK);
    set_can_focus(true);
    grab_focus();

    Glib::signal_timeout().connect(sigc::mem_fun(*this, &SnakeGame::update_game), 100);
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &SnakeGame::refresh_screen), 16);
  }

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override
  {
    cr->set_source_rgb(0, 0, 0);
    cr->paint();

    cr->set_source_rgb(0.2, 0.2, 0.2);
    for (int i = 0; i <= GRID_WIDTH; ++i)
    {
      cr->move_to(i * GRID_SIZE, 0);
      cr->line_to(i * GRID_SIZE, GRID_HEIGHT * GRID_SIZE);
    }
    for (int i = 0; i <= GRID_HEIGHT; ++i)
    {
      cr->move_to(0, i * GRID_SIZE);
      cr->line_to(GRID_WIDTH * GRID_SIZE, i * GRID_SIZE);
    }
    cr->stroke();

    cr->set_source_rgb(0, 1, 0);
    for (const auto &segment : snake)
    {
      cr->rectangle(segment.first * GRID_SIZE, segment.second * GRID_SIZE, GRID_SIZE, GRID_SIZE);
      cr->fill();
    }

    cr->set_source_rgb(1, 0, 0);
    cr->rectangle(food.first * GRID_SIZE, food.second * GRID_SIZE, GRID_SIZE, GRID_SIZE);
    cr->fill();

    cr->set_source_rgb(1, 1, 1);
    cr->select_font_face("Sans", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
    cr->set_font_size(15);
    cr->move_to(10, 20);
    cr->show_text("Score: " + std::to_string(score));
    cr->move_to(10, 40);
    cr->show_text("High Score: " + std::to_string(high_score));

    if (game_over)
    {
      cr->set_font_size(20);
      const std::string message = "Press SPACE to replay";

      Cairo::TextExtents text_extents;
      cr->get_text_extents(message, text_extents);
      double x_center = (GRID_WIDTH * GRID_SIZE - text_extents.width) / 2.0;
      double y_center = (GRID_HEIGHT * GRID_SIZE) / 2.0;

      cr->move_to(x_center, y_center);
      cr->show_text(message);
    }

    return true;
  }

  bool on_key_press_event(GdkEventKey *event) override
  {
    switch (event->keyval)
    {
    case GDK_KEY_w:
    case GDK_KEY_W:
    case GDK_KEY_Up:
      if (direction != Direction::DOWN)
        direction = Direction::UP;
      break;
    case GDK_KEY_s:
    case GDK_KEY_S:
    case GDK_KEY_Down:
      if (direction != Direction::UP)
        direction = Direction::DOWN;
      break;
    case GDK_KEY_a:
    case GDK_KEY_A:
    case GDK_KEY_Left:
      if (direction != Direction::RIGHT)
        direction = Direction::LEFT;
      break;
    case GDK_KEY_d:
    case GDK_KEY_D:
    case GDK_KEY_Right:
      if (direction != Direction::LEFT)
        direction = Direction::RIGHT;
      break;
    case GDK_KEY_space:
      if (game_over)
        respawn();
      break;
    }
    return true;
  }

private:
  std::vector<std::pair<int, int>> snake;
  std::pair<int, int> food;
  Direction direction;
  int score;
  int high_score;
  bool game_over;

  bool update_game()
  {
    if (game_over)
      return true;

    auto head = snake.front();
    switch (direction)
    {
    case Direction::UP:
      head.second -= 1;
      break;
    case Direction::DOWN:
      head.second += 1;
      break;
    case Direction::LEFT:
      head.first -= 1;
      break;
    case Direction::RIGHT:
      head.first += 1;
      break;
    }

    if (head.first < 0 || head.first >= GRID_WIDTH || head.second < 0 || head.second >= GRID_HEIGHT ||
        std::find(snake.begin(), snake.end(), head) != snake.end())
    {
      game_over = true;
      if (score > high_score)
        high_score = score;
      return true;
    }

    snake.insert(snake.begin(), head);

    if (head == food)
    {
      spawn_food();
      score++;
    }
    else
    {
      snake.pop_back();
    }

    return true;
  }

  bool refresh_screen()
  {
    queue_draw();
    return true;
  }

  void spawn_food()
  {
    food.first = std::rand() % GRID_WIDTH;
    food.second = std::rand() % GRID_HEIGHT;
  }

  void respawn()
  {
    snake = {{GRID_WIDTH / 2, GRID_HEIGHT / 2}};
    direction = Direction::RIGHT;
    score = 0;
    game_over = false;
    spawn_food();
  }
};

class MainWindow : public Gtk::Window
{
public:
  MainWindow()
  {
    set_title("Snake Game");
    set_default_size(GRID_SIZE * GRID_WIDTH, GRID_SIZE * GRID_HEIGHT);

    set_resizable(false);

    add(game);
    game.show();
  }

private:
  SnakeGame game;
};

int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "com.kingmaj0r.snake_game");

  MainWindow window;
  return app->run(window);
}
