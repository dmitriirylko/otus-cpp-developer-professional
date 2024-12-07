#include <iostream>
#include <string>
#include <sstream>
#include <vector>

int main(int argc, char ** argv)
{
    std::string line;
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
                        std::stod(csvItem);
                        std::cout << csvItem << std::endl;
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