#include "Dll.h"

#include <dlfcn.h>
#include <fstream>
#include <iostream>


using std::string;
using std::ifstream;
using std::runtime_error;


uint32_t ReadMagic(const string& filename);


uint32_t ReadMagic(const string& filename)
{
    ifstream s;
    char     data[4];
    uint32_t magic;

    s.open(filename, std::ios_base::binary);
    s.read(data, 4);
    
    // Endianness check
    magic = 0x01;
    if (*((char*)&magic) == 0x01) {
        // System is little-endian so we need to swap the order of the bytes
        // in the read data.
        magic = *((uint32_t*)data);
        return (magic >> 24) | ((magic >> 8) & 0xFF00) | ((magic << 8) & 0xFF0000) | ((magic << 24) & 0xFF000000);
    } else {
        return *((uint32_t*)data);
    }
}

bool Dll::isDll(const string& filename)
{
    auto magic = ReadMagic(filename);

#ifdef __APPLE__
    // Looking for Mach-o header
    return magic == 0xCFFAEDFE;
#else
    // Looking for ELF header 
    return magic == 0x7F454C46;
    //                 . E L F
#endif
}

Dll::Dll(const string& path)
{
    this->path = path;
    this->load();
}

Dll::~Dll()
{
    this->unload();
}

bool Dll::hasSymbol(const string& name)
{
    return this->symbolHandles[name] || dlsym(this->handle, name.c_str());
}

void* Dll::resolveSymbol(const string& name)
{
    auto handle = this->symbolHandles[name];
    if (handle)
    {
        return handle;
    }

    handle = dlsym(this->handle, name.c_str());
    if (!handle)
    {
        string message = "";
        message += "Symbol '";
        message += name;
        message += "' not found.";
        throw runtime_error(message);
    }

    this->symbolHandles[name] = handle;

    return handle;
}

void Dll::load()
{
    if (this->handle) {
        return;
    }

    if (!Dll::isDll(this->path))
    {
        string message = "";
        message += "Not a module file:  ";
        message += this->path;
        throw runtime_error(message);
    }

    this->handle = dlopen(this->path.c_str(), RTLD_NOW);
    if (!this->handle) 
    {
        throw runtime_error("Call to dlopen failed.");
    }
}

void Dll::unload()
{
    if (!this->handle)
    {
        return;
    }

    dlclose(this->handle);
    this->handle = nullptr;
    this->symbolHandles.clear();
}

void Dll::reload()
{
    this->unload();
    this->load();
}
