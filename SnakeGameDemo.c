#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h> 

#define SNAKE_LENGTH 5   //Initial length of the snake

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point body[SNAKE_LENGTH];
    int length;
    int dx, dy; // Direction of movement
} Snake;


void endGame() {
    clear();
    refresh();
    endwin();
    exit(0);
}

//Function that draws the border on the screen
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

void moveSnake(Snake *snake) {
    // Move the body segments of the snake
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }
    // Move the head of the snake based on the current direction
    snake->body[0].x += snake->dx;
    snake->body[0].y += snake->dy;
}

void changeSnakeDirection(Snake *snake, int key, int width, int height) {
    switch (key) {
        case KEY_UP:
            if (snake->dy == 1 || snake->body[0].y == 1 || snake->body[0].y == height - 2) {   // prevent moving in opposite direction or out of bounds
                endGame();
            }
            snake->dx = 0;
            snake->dy = -1;
            break;
        case KEY_DOWN:
            if (snake->dy == -1 || snake->body[0].y == 1 || snake->body[0].y == height - 2) {
                endGame();
            }
            snake->dx = 0;
            snake->dy = 1;
            break;
        case KEY_LEFT:
            if (snake->dx == 1 || snake->body[0].x == 1 || snake->body[0].x == width - 2) {
                endGame();
            }
            snake->dx = -1;
            snake->dy = 0;
            break;
        case KEY_RIGHT:
            if (snake->dx == -1 || snake->body[0].x == 1 || snake->body[0].x == width - 2) {
                endGame();
            }
            snake->dx = 1;
            snake->dy = 0;
            break;
    }
}


int main() {
    initscr(); // Initialize ncurses
    cbreak(); // Line buffering disabled
    noecho(); // Don't echo characters to the screen
    curs_set(0); // Hide the cursor
    keypad(stdscr, TRUE); // Enable special keys to be captured
    nodelay(stdscr, TRUE); // Do not block on getch()

    int width, height;
    getmaxyx(stdscr, height, width); // Get terminal dimensions

    drawSnakePitBorder(width, height);

    // Initialize the snake
    Snake snake;
    snake.length = SNAKE_LENGTH;
    //Set initial position of head to face the right
    snake.dx = 1;
    snake.dy = 0;
    
    for (int i = 0; i < SNAKE_LENGTH; i++) {
        snake.body[i].x = (width / 2) - i;
        snake.body[i].y = height / 2;
    }

    int key;
    while (1) {
        key = getch();    // Get user input (arrow key or 'q' for quit)
        if (key != ERR) { // Process input if there is any
            if (key == 'q') {
                endGame(); // Quit the program
            }
            changeSnakeDirection(&snake, key, width, height); // Update direction based on key
        }

        moveSnake(&snake);

        if (snake.body[0].x == 0 || snake.body[0].x == width - 1 || snake.body[0].y == 0 || snake.body[0].y == height - 1) {
            endGame(); // Exit if the snake hits the border
        }

        clear();
        drawSnakePitBorder(width, height);
        for (int i = 0; i < snake.length; i++) {
            mvaddch(snake.body[i].y, snake.body[i].x, 'O');
        }
        refresh();
        usleep(220000); // Control speed of the snake
    }

    endwin();   // End ncurses
    return 0;
}
