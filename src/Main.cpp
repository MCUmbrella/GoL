#include "GoL.h"
#include "CommonUtil.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cout << "Usage: GoL <initFilePath> [--targetGeneration={}] [--sleepMs={}]" << endl
             << "Parameters:" << endl
             << " initFilePath:     The path of the text file used for cell board initialization." << endl
             << " targetGeneration: Maximum number of generation, default is flInfiniteGenerations." << endl
             << " sleepMs:          Milliseconds to wait between iterations, default is 500." << endl;
        return 0;
    }

    bool flInfiniteGenerations = true;
    unsigned int sleepMs = 500;
    unsigned long targetGeneration;

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
                // use default: flInfiniteGenerations
            }
        }
        else if (args[i].rfind("--sleepMs=", 0) == 0)
            try
            {
                sleepMs = stoi(args[i].substr(10));
            }
            catch (...)
            {
                // use default: 500
            }

    GoL& app = GoL::getInstance();
    app.init(args[1]);
    cout << string(app.getRows() * 2, '=') << endl;
    app.display();
    cout << string(app.getRows() * 2, '=') << endl
         << "Ready" << endl;
    CommonUtil::freeze(1000);

    for (unsigned long i = 0LU; flInfiniteGenerations || i != targetGeneration; ++i)
    {
        CommonUtil::clearScreen();
        app.run().display();
        cout << "Iterations: " << app.getCurrentGeneration() << ", cell board: " << app.getLines() << " lines, " << app.getRows() << " rows" << endl;
        flush(cout);
        CommonUtil::freeze(sleepMs);
    }

    return 0;
}
