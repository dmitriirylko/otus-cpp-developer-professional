#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

int main(int argc, char ** argv)
{
    /* Check input args -- first one should represent mean of dataset. */
    double mean;
    if(argc > 1)
    {
        try
        {
            mean = std::stod(argv[1]);
            std::cout << mean << std::endl;
        }
        catch(...)
        {
            std::cerr << "Invalid mean value" << std::endl;
            return 1;
        }
    }
    else
    {
        std::cerr << "Mean value of dataset is not passed" << std::endl;
        return 1;
    }

    std::string line;
    double inputValue;
    double outputValue;
    while(std::getline(std::cin, line))
    {
        try
        {
            size_t cnt = 0;
            std::string csvItem;
            for(size_t i = 0; i < line.size(); ++i)
            {
                if(line[i] == ',' && line[i + 1] != ' ' && i != line.size())
                {
                    if(cnt == 9)
                    {
                        inputValue = std::stod(csvItem);
                        outputValue = pow(inputValue - mean, 2);
                        std::cout << std::setprecision(15) << outputValue << std::endl;
                    }
                    csvItem.clear();
                    ++cnt;
                }
                else
                {
                    csvItem.push_back(line[i]);
                }
            }
        }
        catch(...)
        {}
    }

    return 0;
}