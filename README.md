# integrity-check
Small and simple C++ integrity check / memory checksum.

Example:

int main()  {
    _integrity_check check;

    for (;;)    {
        std::cout << std::boolalpha << check.check_integrity() << std::endl;
    }
}
