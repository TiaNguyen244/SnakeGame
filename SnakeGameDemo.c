#include <ncurses.h>
#include <unistd.h>

#define SNAKE_LENGTH 5

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point body[SNAKE_LENGTH];
    int length;
    int dx,dy; //Direction of movement
} Snake;

void drawSnakePitBorder(int width, int height) {
    // Draw top border
    for (int i = 0; i < width; i++) {
        mvaddch(0, i, '#');
    }
    // Draw bottom border
    for (int i = 0; i < width; i++) {
        mvaddch(height - 1, i, '#');
    }
    // Draw left border
    for (int i = 0; i < height; i++) {
        mvaddch(i, 0, '#');
    }
    // Draw right border
    for (int i = 0; i < height; i++) {
        mvaddch(i, width - 1, '#');
    }
}

//Move the snake
// {Code here}

//CHange direction of the snake
// {Code here}



int main() {
    initscr(); // Initialize ncurses
    cbreak(); // Line buffering disabled
    noecho(); // Don't echo characters to the screen
    curs_set(0); // Hide the cursor
    keypad(stdscr,TRUE);

    int width, height;
    getmaxyx(stdscr, height, width); // Get terminal dimensions

    drawSnakePitBorder(width, height);

    // Initialize the snake
    Snake snake;
    snake.length = SNAKE_LENGTH;
    for (int i = 0; i < SNAKE_LENGTH; i++) {
        snake.body[i].x = width / 2 + i;
        snake.body[i].y = height / 2;
        mvaddch(snake.body[i].y, snake.body[i].x, 'O'); // Draw snake body
    }

    //Main game loop


    refresh(); // Refresh the screen
    getch(); // Wait for a key press to exit
    endwin(); // End ncurses
    return 0;
}
