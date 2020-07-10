#include <unordered_map>
#include <iostream>
#include <string>
#ifdef _WIN32
   #include <io.h> 
   #define access    _access_s
#else
   #include <unistd.h>
#endif
#include <brim.h>

using namespace brim;
using namespace std;

void print_usage(const char *exec) {
    cout << "Usage: " << exec << " <file>\n";
    cout << "Execute a file with the brim vm\n\n";
    cout << "These arguments are also available:\n";
    cout << "  -h, --help    Print this message\n\n";
}

int main(int argc, char *argv[]) {


    for (u32 arg = 1; arg < argc; arg++) {
        if (strcmp("-h", argv[arg]) == 0 || strcmp("--help", argv[arg]) == 0) {
            print_usage(argv[0]);
            return 0;
        }
    }

    if (argc <= 1) {
        print_usage(argv[0]);
        return 0;
    }

    const char* file = argv[1];
    if (access( file, 0 ) != 0) {
        cout << "Could not find file " << file << "\nUse --help to show usage\n\n";
        return 1;
    }

    VM vm;
    vm.interpret_file(file);
    
    return 0;
}