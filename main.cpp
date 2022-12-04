#include "check.h"


int main()
{
    std::vector<section> _cached = get_sections();

    for (;;)
    {
        check_integrity(_cached);
        Sleep(50);
    }
    getchar();
}