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
 * @param signal currently no use.
 */
void handleSignal(int sig);

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
             << " noBorder:         Disable the border of the cell board." << endl;
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
    cout << string(app.getRows() * 2, '=') << endl;
    app.display();
    cout << string(app.getRows() * 2, '=') << endl
         << "Ready" << endl;
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
        app.run().display(); // iterate once and display the new state
        cout << "Current generation: " << app.getCurrentGeneration()
             << ". Board size: " << app.getRows() << "*" << app.getLines() << endl
             << "[Ctrl+C]Pause" << endl;
        flush(cout);
        CommonUtil::freeze(sleepMs); // wait a few moment to avoid the program from running too fast
    }
    cout << "Target generation reached" << endl;
}

/**
 * Shows the pause screen when the program received SIGINT
 * (usually triggered by pressing Ctrl+C).
 * @param sig currently no use.
 */
void handleSignal(int sig)
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
        app.display();
        cout << "Current generation: " << app.getCurrentGeneration()
             << ". Board size: " << app.getRows() << "*" << app.getLines() << endl;

        // ask for option
        cout << "[Q]Exit [W]Resume [E]Edit [R]Revert [T]Goto [Y]Export" << endl << "? ";
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
            int x, y, state;
            if (cin >> x >> y >> state && x > 0 && x <= app.getRows() && y > 0 && y <= app.getLines())
                app.setStateOf(y, x, CommonUtil::parseCellState((char) ('0' + state)));
            else // invalid input, clear stdin and ask again
            {
                cin.clear();
                fflush(stdin);
            }
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
            else
            {
                cin.clear();
                fflush(stdin);
            }
        }
        else if (s == "y" || s == "Y") // export
        {
            //TODO
        }
        else continue; // invalid option, ask again
    }

    // resume
    signal(SIGINT, handleSignal); // why re-register? cuz windows sucks
    flPause = false;
}
