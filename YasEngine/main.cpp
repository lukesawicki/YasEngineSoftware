#include "yas_application.hpp"

int main(int argc, char* argv[]) {

    YasApplication* application = YasApplication::GetInstance();
    application->Initialize();
    application->Run();
    return 0;
}
