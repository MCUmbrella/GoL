#include <csignal>
#include "GoL.h"
#include "CommonUtil.h"

using namespace std;

static bool flInfiniteGenerations = true, flPause = false, flNoBorder = false, flShowBorder = false;
static unsigned int sleepMs = 500;
static unsigned long targetGeneration;

/**
 * Shows the context menu, and the user can do do some
 * operations (e.g. revert, edit and export).
 * <br>
 * The menu will be shown at the beginning of an infinite
 * simulation and every time the user presses Ctrl+C.
 */
void showMenu(int);

/**
 * Performs the automated simulation.
 */
void mainLoop();

/**
 * Resets the standard input.
 */
void resetStdin()
{
    // clear the buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    fflush(stdin);
    // reset the error state
    clearerr(stdin);
    cin.clear();
}

int main(int argc, char** argv)
{
    if (argc < 2) // no input file specified. print help message
    {
        cout << "Usage: GoL <--new / initFilePath> [--targetGeneration={}] [--sleepMs={}] [--noBorder] [--showBorder]" << endl
             << "Parameters:" << endl
             << " new:              Create a new empty cell board." << endl
             << " initFilePath:     The path of the text file used for cell board initialization." << endl
             << " targetGeneration: Maximum number of generation, default is infinite." << endl
             << " sleepMs:          Milliseconds to wait between iterations, default is 500." << endl
             << " noBorder:         Turn on the transparent border feature." << endl
             << " showBorder:       Also print the border when displaying." << endl;
        return 0;
    }

    // parse command arguments
    vector<string> args;
    for (int i = 1; i != argc; ++i)
        args.emplace_back(argv[i]);

    for (string& arg : args)
        if (arg.rfind("--targetGeneration=", 0) == 0)
        {
            try
            {
                targetGeneration = stoul(arg.substr(19));
                flInfiniteGenerations = false;
            }
            catch (...)
            {
                // use default: flInfiniteGenerations = true
            }
        }
        else if (arg.rfind("--sleepMs=", 0) == 0)
            try
            {
                sleepMs = stoi(arg.substr(10));
            }
            catch (...)
            {
                // use default: sleepMs = 500
            }
        else if (arg == "--noBorder")
            flNoBorder = true;
        else if (arg == "--showBorder")
            flShowBorder = true;

    // initialize the engine
    GoL& app = GoL::getInstance();
    app.toggleBorder(flNoBorder);
    if (args[0] == "--new") // create new board
    {
        int lines = 0, rows = 0;
        cout << "Enter: Size (X Y)" << endl << "? ";
        cout.flush();
        cin >> rows >> lines;
        app.init(lines, rows);
        showMenu(0);
    }
    else // load the board from local file
    {
        // pass the file path to the GoL simulator
        app.init(args[0]);
        // display initial state if target generation is not specified
        if (flInfiniteGenerations) showMenu(0);
    }

    signal(SIGINT, showMenu); // register for Ctrl+C event
    mainLoop();

    return 0;
}

void mainLoop()
{
    GoL& app = GoL::getInstance();
    while (flInfiniteGenerations || app.getCurrentGeneration() != targetGeneration)
    {
        if (flPause)
        {
            CommonUtil::freeze(500);
            continue;
        }
        CommonUtil::clearScreen();
        app.run().display(flShowBorder); // iterate once and display the new state
        cout << "Current generation: " << app.getCurrentGeneration()
             << ". Board size: " << app.getRows() << "*" << app.getLines() << endl
             << "[Ctrl+C]Pause" << endl;
        flush(cout);
        CommonUtil::freeze(sleepMs); // wait a few moment to avoid the program from running too fast
    }
    cout << "Target generation reached" << endl;
}

void showMenu(int)
{
    flPause = true;
    GoL& app = GoL::getInstance();

    for (;;)
    {
        // display current state
        CommonUtil::clearScreen();
        app.display(flShowBorder);
        cout << "Current generation: " << app.getCurrentGeneration()
             << ". Board size: " << app.getRows() << "*" << app.getLines() << endl;

        // ask for option
        cout << "[Q]Exit [W]Start/Resume [E]Edit [R]Revert [T]Goto [Y]Export" << endl << "? ";
        flush(cout);
        string s;
        cin >> s;
        // check option
        if (s == "q" || s == "Q") // exit
        {
            cout << "Exiting" << endl;
            exit(0);
        }
        else if (s == "w" || s == "W") // resume
        {
            break;
        }
        else if (s == "e" || s == "E")
        {
            cout << "Enter: X Y State(1=alive, 0=dead)" << endl << "? ";
            flush(cout);
            int x, y;
            char state;
            if (cin >> x >> y >> state && x > 0 && x <= app.getRows() && y > 0 && y <= app.getLines())
                app.setStateOf(y, x, CommonUtil::parseCellState(state));
        }
        else if (s == "r" || s == "R") // revert
        {
            app.revert(1);
        }
        else if (s == "t" || s == "T") // goto
        {
            cout << "Enter: Target generation" << endl << "? ";
            flush(cout);
            int g;
            if (cin >> g)
            {
                if (g == app.getCurrentGeneration()) continue;
                if (g > app.getCurrentGeneration())
                {
                    cout << "Please wait";
                    flush(cout);
                    app.forward(g - app.getCurrentGeneration());
                }
                else
                {
                    cout << "Please wait";
                    flush(cout);
                    app.revert(app.getCurrentGeneration() - g);
                }
            }
        }
        else if (s == "y" || s == "Y") // export
        {
            cout << "Enter: File path" << endl << "? ";
            flush(cout);
            string path;
            cin >> path;
            if (!path.empty()) app.save(path);
        }
        resetStdin(); // invalid input, ask again
    }
    resetStdin();

    // resume
    signal(SIGINT, showMenu); // why re-register? cuz windows sucks
    flPause = false;
}
