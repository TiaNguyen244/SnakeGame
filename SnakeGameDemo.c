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
void moveSnake(Snake *snake, int width, int height) {
    // Move the body segments of the snake
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i].x = snake->body[i - 1].x;
        snake->body[i].y = snake->body[i - 1].y;
    }

    // Move the head of the snake based on current direction
    snake->body[0].x += snake->dx;
    snake->body[0].y += snake->dy;
}


//Change direction of the snake
// {Code here}
void changeSnakeDirection(Snake *snake, int key) {
    switch (key) {
        case KEY_UP:
            if (snake->dy != 1) { // prevent moving in opposite direction
                snake->dx = 0;
                snake->dy = -1;
            }
            break;
        case KEY_DOWN:
            if (snake->dy != -1) {
                snake->dx = 0;
                snake->dy = 1;
            }
            break;
        case KEY_LEFT:
            if (snake->dx != 1) {
                snake->dx = -1;
                snake->dy = 0;
            }
            break;
        case KEY_RIGHT:
            if (snake->dx != -1) {
                snake->dx = 1;
                snake->dy = 0;
            }
            break;
        default:
            break;
    }
}


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

    //Set initial position of head to face the right
    snake.dx = 1;
    snake.dy = 0;
    
    for (int i = 0; i < SNAKE_LENGTH; i++) {
        snake.body[i].x = width / 2 + i;
        snake.body[i].y = height / 2;
        mvaddch(snake.body[i].y, snake.body[i].x, 'O'); // Draw snake body
    }

    //Main game loop
    while (1) { // Infinite loop until game over condition

        int key = getch(); // Get user input (arrow key or 'q' for quit)
        if (key == 'q') // If 'q' is pressed, exit the game
            break;
        changeDirection(&snake, key); // Update direction based on key
        
        moveSnake(&snake, width, height); // Move the snake

        // Check if the snake hits the border
        if (snake.body[0].x == 0 || snake.body[0].x == width - 1 || snake.body[0].y == 0 || snake.body[0].y == height - 1)
            break; // Exit the game if the snake hits the border
       
        // Draw the snake
        clear(); // Clear the screen
        drawSnakePitBorder(width, height);
        for (int i = 0; i < snake.length; i++) {
            mvaddch(snake.body[i].y, snake.body[i].x, 'O');
        }
        refresh(); // Refresh the screen
        usleep(100000); // Delay to control the speed of the game
    }

    endwin(); // End ncurses
    return 0;
}
