#include "source.h"

std::string StdInputSource::getCmd()
{
    std::string str;
    std::cin >> str;
    return str;
}

Source::Source(std::unique_ptr<ISource> sourcePtr) :
    m_pSource{std::move(sourcePtr)}
{}

std::string Source::getCmd()
{
    return m_pSource->getCmd();
}