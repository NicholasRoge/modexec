#include <dirent.h>
#include <functional>
#include <iostream>
#include <list>
#include <map>

#include "Dll.h"


using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::list;
using std::map;
using std::string;


void DisplayUsage(const string& programName);

void LoadDlls();

void ExecDll(const string& name);


const char* const DLL_PATH = "./modules";

list<Dll> loadedDlls;


int main(int argc, char** argv) {
    if (argc == 1) {
        DisplayUsage(argv[0]);
        
        return 1;
    }

    LoadDlls();
    for (int arg = 1; arg < argc; ++arg) {
        ExecDll(argv[arg]);
    }

    return 0;
}


void DisplayUsage(const string& programName) {
    cerr << "usage: " << programName << " name [name...]" << endl;
}

void LoadDlls()
{
    auto dirHandle = opendir(DLL_PATH);
    if (!dirHandle)
    {
        cerr << "\x1B[31m";
        cerr << endl;
        cerr << "  Failed to open DLL directory (" << DLL_PATH << ")." << endl;
        cerr << endl;
        cerr << "\x1B[0m";

        return;
    }

    while (auto entity = readdir(dirHandle))
    {
        if (entity->d_type == DT_DIR) 
        {
            continue;
        }

        string filename = DLL_PATH;
        filename.append("/");
        filename.append(entity->d_name);
        if (!Dll::isDll(filename)) 
        {
            continue;
        }

        try
        {
            loadedDlls.emplace_front(filename);
        }
        catch (const exception& e)
        {
            cerr << "\x1B[41;37m";
            cerr << endl;
            cerr << "  Caught an exception!  Message:" << endl;
            cerr << "    " << e.what() << endl;
            cerr << endl;
            cerr << "\x1B[0m";
        }
    }

    closedir(dirHandle);
}

void ExecDll(const string& name) {
    for (auto& dll : loadedDlls) {
        if (!dll.hasSymbol("name")) {
            continue;
        }

        string loadedDllName = dll.read<const char*>("name");
        if (loadedDllName != name) {
            continue;
        }


        if (dll.hasSymbol("exec")) {
            dll.call<void()>("exec");
        } else {
            cerr << "\x1B[41;37m";
            cerr << endl;
            cerr << "  A dll with name '" << name << "' was found, however it"
                 << " did not export an 'exec' symbol.";
            cerr << endl;
            cerr << "\x1B[0m";
        }

        return;
    }

    cerr << "\x1B[41;37m";
    cerr << endl;
    cerr << "  No dlls exported symbol 'name' with value '" << name << "'." << endl;
    cerr << endl;
    cerr << "\x1B[0m";
}
