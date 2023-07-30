#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define MAX_BUFFER_SIZE 5000

void openFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printw("Failed to open the file. Check if the file exists and you have read permissions.\n");
        return;
    }

    char c;
    while ((c = fgetc(file)) != EOF) {
        addch(c);
    }

    fclose(file);
}

void saveFile(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printw("Failed to open the file for writing. Check if you have write permissions.\n");
        return;
    }

    fputs(content, file);

    fclose(file);
    printw("File saved successfully.\n");
}

int main() {
    initscr();              // Start curses mode
    raw();                  // Line buffering disabled
    keypad(stdscr, TRUE);   // We get F1, F2, etc.
    noecho();               // Don't echo() while we do getch

     char filename[100];
    printw("Enter the filename to open (include the extension, e.g., 'file.txt'): ");
    echo();  // Enable echoing of typed characters
    getstr(filename);
    noecho();  // Disable echoing of typed characters

    openFile(filename);

    printw("\nStart typing. Enter ':wq' on a new line when you're done:\n");


    char input[MAX_BUFFER_SIZE] = "";
    int input_len = 0;
    int ch;

    while ((ch = getch()) != EOF) {
    if (ch == '\n') {
        addch(ch);
        input[input_len++] = ch;

        // Check if the last line of the input is ":wq"
        if (input_len >= 4 && strcmp(input + input_len - 4, ":wq\n") == 0) {
            input[input_len - 4] = '\0';  // Remove the ':wq\n' from the input
            break;
        }
    } else if (ch == erasechar()) {  // Use erasechar() to get the erase character
        if (input_len > 0) {
            input[--input_len] = '\0';
            int y, x;
            getyx(stdscr, y, x);
            move(y, x-1);
            delch();
        }
    } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {  // Check for backspace key
        if (input_len > 0) {
            input[--input_len] = '\0';
            int y, x;
            getyx(stdscr, y, x);
            move(y, x-1);
            delch();
        }
    } else {
        if (input_len < MAX_BUFFER_SIZE - 1) {
            addch(ch);
            input[input_len++] = ch;
        }
    }
}


    endwin();  // End curses mode

    saveFile(filename, input);

    return 0;
}