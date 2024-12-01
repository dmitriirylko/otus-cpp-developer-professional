#pragma once

#include <iostream>

#include <boost/program_options.hpp>

class Options
{
public:
    Options(int argc, char** argv)
    {
        boost::program_options::options_description desc;
        desc.add_options()
        ("unnamed", boost::program_options::value<std::vector<size_t>>(), "unnamed");

        boost::program_options::positional_options_description pos_desc;
        pos_desc.add("unnamed", 2);

        boost::program_options::variables_map vm;

        if(argc == 1) {
            throw std::invalid_argument("Incorrect number of input parameters");
        }

        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).
                                                              options(desc).
                                                              positional(pos_desc).
                                                              run(), vm);
        boost::program_options::notify(vm);
        if(vm["unnamed"].as<std::vector<size_t>>().size() != 2) {
            throw std::invalid_argument("Incorrect number of input parameters");
        }

    m_host = vm["unnamed"].as<std::vector<size_t>>()[0];
    m_bulkSize = vm["unnamed"].as<std::vector<size_t>>()[1];
    }

    size_t getHost()
    {
        return m_host;
    }

    size_t getBulkSize()
    {
        return m_bulkSize;
    }

private:
    size_t m_host = 0;
    size_t m_bulkSize = 0;
};