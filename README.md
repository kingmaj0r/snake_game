# Snake Game

A simple Snake game implemented in C++ using GTKmm. The game supports movement using both **WASD** keys and **Arrow** keys. 

---

## Prerequisites

Make sure you have the following installed on your system:
- A C++ compiler with support for C++17 or later (e.g., GCC, Clang).
- [GTKmm 3.0](https://developer.gnome.org/gtkmm-tutorial/stable/) library and development files.
- Meson build system and Ninja.

### Install dependencies on Fedora
```bash
sudo dnf install gtkmm30-devel meson ninja-build gcc-c++
```

### Install dependencies on Ubuntu/Debian
```bash
sudo apt-get install libgtkmm-3.0-dev meson ninja-build g++
```

---

## Build and Run Instructions

1. Clone this repository and navigate into its directory:
   ```bash
   git clone https://github.com/KingMaj0r/snake_game.git
   cd snake_game
   ```

2. Create a build directory and configure the project with Meson:
   ```bash
   meson setup build
   ```

3. Compile the project using Ninja:
   ```bash
   meson compile -C build
   ```

4. Run the game:
   ```bash
   ./build/snake_game
   ```

---

## How to Play
- Use **WASD** or **Arrow keys** to control the snake.
- Eat the red food squares to grow the snake and increase your score.
- Avoid colliding with the walls or yourself.
- Press **SPACE** to restart the game after a game over.

## License
This project is licensed under the MIT License.
