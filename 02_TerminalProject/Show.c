#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

void PrintBox(WINDOW *win, char *file_name) {
    box(win, 0, 0);
    wmove(win, 0, 1);
    wprintw(win, file_name);
}

int Init(WINDOW **win, char *file_name) {
    if (!initscr()) {
        return 1;
    }
    refresh();
    *win = newwin(LINES, COLS, 0, 0);
    return 0;
}

void Finalize(WINDOW *win) {
    delwin(win);
    endwin();
}

typedef struct {
    size_t size;
    char *buffer;
} File;

typedef struct {
    size_t lines_number;
    char **lines;
} FileByLines;

File ReadAllFile(char *file_name) {
    FILE *fptr = fopen(file_name, "r");
    if (fptr == NULL) {
        fprintf(stderr, "File doesn't exist");
        exit(1);
    }
    File file;
    fseek(fptr, 0, SEEK_END);
    file.size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    file.buffer = malloc((file.size + 1) * sizeof(char));
    size_t read_size = fread(file.buffer, 1, file.size, fptr);
    file.buffer[read_size] = '\0';
    fclose(fptr);
    return file;
}

FileByLines Parse(File original_file) {
    size_t capacity = 1;
    FileByLines file;
    file.lines_number = 0;
    file.lines = (char **) malloc(capacity * sizeof(char *));

    size_t start_of_line = 0;
    for(size_t i = 0; i <= original_file.size; ++i) {
        if (original_file.buffer[i] == '\n' || original_file.buffer[i] == '\0') {
            if (file.lines_number >= capacity) {
                capacity *= 2;
                file.lines = realloc(file.lines, capacity * sizeof(char *));
            }

            file.lines[file.lines_number++] = original_file.buffer + start_of_line;
            original_file.buffer[i] = '\0';
            start_of_line = i + 1;
        }
    }

    return file;
}

FileByLines ReadFile(char *file_name) {
    return Parse(ReadAllFile(file_name));
}

void DoLogic(size_t x, size_t y, WINDOW *win, FileByLines *file, char * file_name) {
    werase(win);
    PrintBox(win, file_name);
    for (size_t i = y, k = 1; i < file->lines_number && k < LINES - 1; ++i, ++k) {
        if (strlen(file->lines[i]) <= x) {
            continue;
        }
        mvwaddnstr(win, k, 1, file->lines[i] + x, COLS-2);
    }
    wrefresh(win);
}

int main(int argc, char *argv[]) {
    WINDOW *win = NULL;

    if (argc != 2) {
        fprintf(stderr, "Incurrect arguments number\n");
        return 1;
    }

    if (Init(&win, argv[1])) {
        fprintf(stderr, "Error initialising ncurses.\n");
        return 1;
    }
    keypad(win, TRUE);
    FileByLines file = ReadFile(argv[1]);

    unsigned y = 0, x = 0;
    DoLogic(x, y, win, &file, argv[1]);

    int c = wgetch(win);
    const unsigned last_line = file.lines_number - LINES + 2;
    while (true) {
        if (c == 27) {
            Finalize(win);
            return 0;
        }
        if (c == ' ' || c == KEY_DOWN) {
            y = (y == last_line) ? last_line : y + 1;
        } else if (c == KEY_UP) {
            y = (y == 0) ? 0 : y - 1;
        } else if (c == KEY_LEFT) {
            x = (x == 0) ? 0 : x - 1;
        } else if (c == KEY_RIGHT) {
            x++;
        }
        DoLogic(x, y, win, &file, argv[1]);
        c = wgetch(win);
    }
}
//LASTLINE