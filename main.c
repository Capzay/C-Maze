#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{

    FILE *fptr;
    Display *display;
    Window window;
    XEvent event;
    int screen;
    GC gc;
    XGCValues values;

    // 1. Open connection to X server
    display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    screen = DefaultScreen(display);

    // 2. Create the window
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 800, 600, 1,
                                 BlackPixel(display, screen), WhitePixel(display, screen));

    // 3. Select kind of events we are interested in
    XSelectInput(display, window, ExposureMask | KeyPressMask);

    // 4. Map (show) the window
    XMapWindow(display, window);

    // Create a graphics context
    gc = XCreateGC(display, window, 0, &values);

    // Ask the user to type a number
    printf("Type a number: ");
    int fileNum;
    // Get and save the number the user types
    scanf("%d", &fileNum);

    char filename[20];

    sprintf(filename, "%d.txt", fileNum);

    // Open a file in read mode
    fptr = fopen(filename, "r");
    // Check if the file opened successfully
    if (fptr == NULL)
    {
        printf("Error: Could not open file.\n");
        return 1;
    }

    // Read each character until the end of the file
    char ch;
    int posX = 20;
    int posY = 20;
    // 5. Event loop
    while (1)
    {
        XNextEvent(display, &event);

        // 6. Draw or handle events
        if (event.type == Expose)
        {
            while ((ch = fgetc(fptr)) != EOF)
            {
                if (ch == 'S')
                {
                    XSetForeground(display, gc, 0x00FF00); // Green
                    XFillRectangle(display, window, gc, posX, posY, 15, 15);
                    posX += 15;
                }
                else if (ch == 'E')
                {
                    XSetForeground(display, gc, 0xFF0000); // Red
                    XFillRectangle(display, window, gc, posX, posY, 15, 15);
                    posX += 15;
                }
                else if (ch == '#')
                {
                    XSetForeground(display, gc, 0x000000); // Black
                    XFillRectangle(display, window, gc, posX, posY, 15, 15);
                    posX += 15;
                }
                else if (ch == '\n')
                {
                    posY += 15;
                    posX = 20;
                }
                else
                {
                    posX += 15;
                }
            }

            // Close the file
            fclose(fptr);
        }
        if (event.type == KeyPress)
        {
            break;
        }
    }

    // 7. Close connection to X server
    XCloseDisplay(display);

    return 0;
}
