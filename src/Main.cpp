#include <csignal>
#include "GoL.h"
#include "CommonUtil.h"

using namespace std;

static bool flInfiniteGenerations = true, flPause = false, flNoBorder = false;
static unsigned int sleepMs = 500;
static unsigned long targetGeneration;

/**
 * Handles the Ctrl+C event while the simulation is running.
 * Ctrl+C will pause the simulation and ask the user whether to resume.
 */
void handleSignal(int);

/**
 * Performs the simulation.
 */
void mainLoop();

int main(int argc, char** argv)
{
    if (argc < 2) // no input file specified. print help message
    {
        cout << "Usage: GoL <initFilePath> [--targetGeneration={}] [--sleepMs={}] [--noBorder]" << endl
             << "Parameters:" << endl
             << " initFilePath:     The path of the text file used for cell board initialization." << endl
             << " targetGeneration: Maximum number of generation, default is infinite." << endl
             << " sleepMs:          Milliseconds to wait between iterations, default is 500." << endl
             << " noBorder:         Turn on the transparent border feature." << endl;
        return 0;
    }

    // convert argv to a more comfortable String[]
    string args[argc];
    for (int i = 0; i != argc; ++i)
        args[i] = string(argv[i]);

    for (int i = 0; i != argc; ++i)
        if (args[i].rfind("--targetGeneration=", 0) == 0)
        {
            try
            {
                targetGeneration = stoul(args[i].substr(19));
                flInfiniteGenerations = false;
            }
            catch (...)
            {
                // use default: flInfiniteGenerations = true
            }
        }
        else if (args[i].rfind("--sleepMs=", 0) == 0)
            try
            {
                sleepMs = stoi(args[i].substr(10));
            }
            catch (...)
            {
                // use default: sleepMs = 500
            }
        else if (args[i] == "--noBorder")
            flNoBorder = true;

    GoL& app = GoL::getInstance();
    app.toggleBorder(flNoBorder);
    app.init(args[1]); // pass the file path to the GoL simulator
    // display the initial state of the cell board
    app.display(false);
    cout<<"Ready";
    cout.flush();
    CommonUtil::freeze(1000);

    signal(SIGINT, handleSignal); // register for Ctrl+C event
    mainLoop();

    return 0;
}

void mainLoop()
{
    GoL& app = GoL::getInstance();
    for (unsigned long i = 0LU; flInfiniteGenerations || i != targetGeneration; ++i)
    {
        if (flPause)
        {
            CommonUtil::freeze(500);
            --i;
            continue;
        }
        CommonUtil::clearScreen();
        app.run().display(false); // iterate once and display the new state
        cout << "Current generation: " << app.getCurrentGeneration()
             << ". Board size: " << app.getRows() << "*" << app.getLines() << endl
             << "[Ctrl+C]Pause" << endl;
        cout.flush();
        CommonUtil::freeze(sleepMs); // wait a few moment to avoid the program from running too fast
    }
    cout << "Target generation reached" << endl;
}

/**
 * Shows the pause screen when the program received SIGINT
 * (usually triggered by pressing Ctrl+C).
 */
void handleSignal(int)
{
    flPause = true;
    GoL& app = GoL::getInstance();

    for (;;)
    {
        if (!cin) // someone pressed Ctrl+D, exit to avoid dead-loop
        {
            cout << "Exiting" << endl;
            exit(0);
        }
        // display current state
        CommonUtil::clearScreen();
        app.display(false);
        cout << "Current generation: " << app.getCurrentGeneration()
             << ". Board size: " << app.getRows() << "*" << app.getLines() << endl;

        // ask for option
        cout << "[Q]Exit [W]Resume [E]Edit [R]Revert [T]Goto [Y]Export" << endl << "? ";
        cout.flush();
        cin.clear();
        fflush(stdin);
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
            cout << "Enter: X Y State(1=alive, 0=dead, other=border)" << endl << "? ";
            cout.flush();
            int x, y;
            char state;
            if (cin >> x >> y >> state && x > 0 && x <= app.getRows() && y > 0 && y <= app.getLines())
                app.setStateOf(y, x, CommonUtil::parseCellState((char) (state)));
        }
        else if (s == "r" || s == "R") // revert
        {
            app.revert(1);
        }
        else if (s == "t" || s == "T") // goto
        {
            cout << "Enter: Target generation" << endl << "? ";
            cout.flush();
            int g;
            if (cin >> g)
            {
                if (g == app.getCurrentGeneration()) continue;
                if (g > app.getCurrentGeneration())
                {
                    cout << "Please wait";
                    cout.flush();
                    app.forward(g - app.getCurrentGeneration());
                }
                else
                {
                    cout << "Please wait";
                    cout.flush();
                    app.revert(app.getCurrentGeneration() - g);
                }
            }
        }
        else if (s == "y" || s == "Y") // export
        {
            cout << "Enter: File path" << endl << "? ";
            cout.flush();
            string path;
            cin >> path;
            app.save(path);
        }
        else continue; // invalid option, ask again
    }

    // resume
    signal(SIGINT, handleSignal); // why re-register? cuz windows sucks
    flPause = false;
}
