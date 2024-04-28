#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SNAKE_LENGTH 5
#define TROPHY_EXPIRATION_INTERVAL 10 // in seconds
#define TARGET_LENGTH (2 * (width + height) - 8) / 2 // Half the perimeter of the border
typedef struct {
    int x, y;
} Point;

typedef struct {
    Point *body; // Dynamic allocation for snake body
    int length;
    int dx, dy;
} Snake;

typedef struct {
    Point position;
    int value;
    int active;
} Trophy;

void endGame() {
    clear();
    mvprintw(10, 10, "Game Over!");
    refresh();
    sleep(2);
    endwin();
    exit(0);
}

void winGame() {
    clear();
    mvprintw(10, 10, "Congratulations! You've won the game!");
    refresh();
    sleep(3);
    endwin();
    exit(0);
}

// Function that draws the border on the screen
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
    Point nextHead = { snake->body[0].x + snake->dx, snake->body[0].y + snake->dy };

    // Check for self-collision
    for (int i = 1; i < snake->length; i++) {
        if (nextHead.x == snake->body[i].x && nextHead.y == snake->body[i].y) {
            endGame();
        }
    }

    // Move the body segments of the snake
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }
    snake->body[0] = nextHead; // Move the head
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

void drawTrophy(Trophy *trophy) {
    if (trophy->active) {
        mvprintw(trophy->position.y, trophy->position.x, "%d", trophy->value);
    }
}

void generateTrophy(Trophy *trophy, int width, int height) {
    trophy->position.x = rand() % (width - 2) + 1;
    trophy->position.y = rand() % (height - 2) + 1;
    trophy->value = rand() % 9 + 1; // Random value between 1 and 9
    trophy->active = 1;
}

void expireTrophy(Trophy *trophy) {
    trophy->active = 0;
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

    snake.body = malloc(snake.length * sizeof(Point)); // Allocate memory for snake body
    srand(time(NULL));
    int dir = rand() % 4; // Choose a random direction at the start
    switch (dir) {
        case 0:
            snake.dx = 0; snake.dy = -1; break;
        case 1:
            snake.dx = 0; snake.dy = 1; break;
        case 2:
            snake.dx = -1; snake.dy = 0; break;
        case 3:
            snake.dx = 1; snake.dy = 0; break;
    }

    for (int i = 0; i < snake.length; i++) {
        snake.body[i].x = (width / 2) - i * snake.dx;
        snake.body[i].y = (height / 2) - i * snake.dy;
    }

    // Initialize trophy
    Trophy trophy;
    int trophyActive = 0;
    time_t trophyExpirationTime;

    int key, speedDelay = 220000;
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

        // Trophy logic
        if (!trophyActive) {
            generateTrophy(&trophy, width, height);
            trophyActive = 1;
            trophyExpirationTime = time(NULL) + (rand() % TROPHY_EXPIRATION_INTERVAL) + 1;
        }

        if (time(NULL) >= trophyExpirationTime) {
            expireTrophy(&trophy);
            trophyActive = 0;
        }

        // Check if the snake eats the trophy
        if (trophyActive && snake.body[0].x == trophy.position.x && snake.body[0].y == trophy.position.y) {
            // Increase snake length
            snake.length += trophy.value;
            snake.body = realloc(snake.body, snake.length * sizeof(Point)); // Resize body array
            // Set new segment positions
            for (int i = snake.length - trophy.value; i < snake.length; i++) {
                snake.body[i].x = snake.body[i - 1].x;
                snake.body[i].y = snake.body[i - 1].y;
            }
            trophyActive = 0; // Trophy is consumed
                    // Check if the player has won
        if (snake.length >= TARGET_LENGTH) {
            winGame(); // Player has won
        }
        }

        // Draw trophy if active
        clear(); // Clear the screen before drawing
        drawSnakePitBorder(width, height);
        if (trophyActive) {
            drawTrophy(&trophy);
        }

        // Draw snake
        for (int i = 0; i < snake.length; i++) {
            mvaddch(snake.body[i].y, snake.body[i].x, 'O');
        }

        refresh(); // Refresh the screen after drawing
        usleep(speedDelay); // Control speed of the snake

        // Adjust speed based on snake length
        speedDelay = 220000 - (snake.length - 5) * 10000;
        speedDelay = speedDelay < 50000 ? 50000 : speedDelay;
    }

    // Free dynamically allocated memory
    free(snake.body);

    endwin();   // End ncurses
}