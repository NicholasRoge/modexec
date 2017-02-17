#ifndef DLL_H
#define DLL_H 


#include <map>
#include <string>


class Dll
{
    public:
        static bool isDll(const std::string& filename);

        explicit Dll(const std::string& filename);

        ~Dll();

        bool hasSymbol(const std::string& name);

        void* resolveSymbol(const std::string& name);

        template<typename SymbolType>
        SymbolType read(const std::string& name);

        template<typename SymbolType>
        void write(const std::string& name, SymbolType value);

        template<typename SymbolType, typename ...Args>
        auto call(const std::string& name, Args&&... args);

        void reload();

    protected:
        void load();

        void unload();

    private:
        void* handle;

        std::map<std::string, void*> symbolHandles;

        std::string path;
};

template<typename SymbolType>
SymbolType Dll::read(const std::string& name)
{
    auto handle = this->resolveSymbol(name);
    auto typedHandle = reinterpret_cast<SymbolType*>(handle);
    return *typedHandle;
}

template<typename SymbolType>
void Dll::write(const std::string& name, SymbolType value)
{
    auto handle = this->resolveSymbol(name);
    auto typedHandle = reinterpret_cast<SymbolType*>(handle);
    *typedHandle = value;
}

template<typename Signature, typename ...Args>
auto Dll::call(const std::string& name, Args&&... args)
{
    auto handle = this->resolveSymbol(name); 
    auto typedHandle = reinterpret_cast<Signature*>(handle);
    return typedHandle(std::forward<Args>(args)...);
}

#endif
