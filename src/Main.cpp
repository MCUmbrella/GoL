#include <csignal>
#include "GoL.h"
#include "CommonUtil.h"

using namespace std;

static bool flInfiniteGenerations = true, flPause = false;
static unsigned int sleepMs = 500;
static unsigned long targetGeneration;

/**
 * Handles the Ctrl+C event while the simulation is running.
 * Ctrl+C will pause the simulation and ask the user whether to resume.
 * @param signal currently no use.
 */
void handleSignal(int signal);

/**
 * Performs the simulation.
 */
void mainLoop();

int main(int argc, char** argv)
{
    if (argc < 2) // no input file specified. print help message
    {
        cout << "Usage: GoL <initFilePath> [--targetGeneration={}] [--sleepMs={}]" << endl
             << "Parameters:" << endl
             << " initFilePath:     The path of the text file used for cell board initialization." << endl
             << " targetGeneration: Maximum number of generation, default is flInfiniteGenerations." << endl
             << " sleepMs:          Milliseconds to wait between iterations, default is 500." << endl;
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

    GoL& app = GoL::getInstance();
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
        if(flPause) //FIXME Ctrl+C only handled once on windows
        {
            CommonUtil::freeze(500);
            --i;
            continue;
        }
        CommonUtil::clearScreen();
        app.run().display(); // iterate once and display the new state
        cout << "Iterations: " << app.getCurrentGeneration()
             << ". Board size: " << app.getRows() << "*" << app.getLines() << endl;
        flush(cout);
        CommonUtil::freeze(sleepMs); // wait a few moment to avoid the program from running too fast
    }
}

void handleSignal(int signal)
{
    flPause = true;
    // paused. display current state
    GoL& app = GoL::getInstance();
    CommonUtil::clearScreen();
    app.display();
    cout << "Current generation: " << app.getCurrentGeneration()
         << ". Board size: " << app.getRows() << "*" << app.getLines() << endl;

    // ask for resume
    cout << "Resume? [y/N] ";
    flush(cout);
    string s;
    cin >> s;
    if (s != "y" && s != "Y") exit(0);
    flPause = false;
}
