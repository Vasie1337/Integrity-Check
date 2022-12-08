# .text Integrity Check
> Small and simple C++ integrity check / memory checksum.
> Just include the header 


### Example : 

```
int main()  {
    _integrity_check check;

    for (;;)    {
        std::cout << std::boolalpha << check.check_integrity() << std::endl;
    }
}
```
