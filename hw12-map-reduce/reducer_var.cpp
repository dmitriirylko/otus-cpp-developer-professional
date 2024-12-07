#include <iostream>
#include <string>

int main(int argc, char ** argv)
{
    size_t cnt = 0;
    double sum = 0;
    std::string line;
    double inputValue;
    while(std::getline(std::cin, line))
    {
        try
        {
            inputValue = std::stod(line);
            sum += inputValue;
            ++cnt;
        }
        catch(...)
        {}
    }
    double variancePrice = sum / (cnt + 1);
    std::cout << variancePrice;
    return 0;
}