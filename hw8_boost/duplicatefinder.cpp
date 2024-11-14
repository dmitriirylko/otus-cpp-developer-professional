#include "duplicatefinder.h"

FileInfo::FileInfo(const boost::filesystem::path &fsPath) :
    path{fsPath}
{
    size = boost::filesystem::file_size(fsPath);
}

DuplicateFinder::DuplicateFinder()
{
    m_buf.resize(Config::instance().getBlockSize());
}

void DuplicateFinder::createFilePool()
{
    m_filePool.clear();
    for(const auto path : Config::instance().getIncludedFolderPaths())
    {
        if(Config::instance().getScanningLevel() == 1)
        {
            iterateFolder<boost::filesystem::recursive_directory_iterator>(path);
        }
        else if(Config::instance().getScanningLevel() == 0)
        {
            iterateFolder<boost::filesystem::directory_iterator>(path);
        }
        else
        {
            std::invalid_argument("There is no such type of scanning level.");
        }
    }
}

template<typename T>
void DuplicateFinder::iterateFolder(const std::string& path)
{
    /* Iterate through folder (recursively or not). */
    for(T iter{path}; iter != T{}; ++iter)
    {
        /* Check if this path corresponds to regular file. */
        if(boost::filesystem::status(*iter).type() != boost::filesystem::regular_file)
        {
            continue;
        }
        /* Check if this path is not contains in excluded folders. */
        bool isIncluded = true;
        for(const auto &excludedPath : Config::instance().getExcludedFolderPaths())
        {
            if(isSubpath(iter->path(), excludedPath))
            {
                isIncluded = false;
                break;
            }
        }
        if(!isIncluded) continue;
        /* Check file size requirements. */
        if(boost::filesystem::file_size(iter->path()) < Config::instance().getMinFileSize()) continue;
        /* Check masks. */
        isIncluded = true;
        if(!Config::instance().getMasks().empty())
        {
            isIncluded = false;
            for(const auto &mask : Config::instance().getMasks())
            {
                boost::regex regExpr{mask};
                if(boost::regex_match(iter->path().filename().string(), regExpr))
                {
                    isIncluded = true;
                }
            }
        }
        if(!isIncluded) continue;
        m_filePool.emplace_back(iter->path());
    }
}

bool DuplicateFinder::isSubpath(const boost::filesystem::path &queryPath,
                                const boost::filesystem::path &directoryPath)
{
    auto relativePath = boost::filesystem::relative(queryPath, directoryPath);
    return !relativePath.empty() && relativePath.native()[0] != '.';
}

void DuplicateFinder::findDuplicates()
{
    for(size_t i = 0; i < m_filePool.size(); ++i)
    {
        if(m_filePool[i].isDupFinded) continue;
        m_dupPaths.clear();
        for(size_t j = i + 1; j < m_filePool.size(); ++j)
        {
            /* First fast check: files can't be duplicates if they have different sizes. */
            if(m_filePool[i].size != m_filePool[j].size)
            {
                continue;
            }
            /* Secons check: compare hashes of all file blocks. */
            if(compareHashes(m_filePool[i], m_filePool[j]))
            {
                m_dupPaths.push_back(boost::filesystem::canonical(m_filePool[j].path));
            }
        }
        if(m_dupPaths.size())
        {
            m_dupPaths.push_back(boost::filesystem::canonical(m_filePool[i].path));
            printDups();
        }
    }
}

void DuplicateFinder::addBlockHash(FileInfo& file, size_t k)
{
    memset(m_buf.data(), 0, m_buf.size());
    std::ifstream fd(file.path, std::ifstream::binary);
    fd.seekg(k * Config::instance().getBlockSize());
    fd.read(m_buf.data(), Config::instance().getBlockSize());
    boost::crc_32_type hash;
    hash.process_bytes(m_buf.data(), Config::instance().getBlockSize());
    file.hash.push_back(hash.checksum());
}

bool DuplicateFinder::compareHashes(FileInfo& file1, FileInfo& file2)
{
    /* Find number of blocks of files to compare. */
    size_t blocksNum = file1.size / Config::instance().getBlockSize();
    if(file1.size % Config::instance().getBlockSize()) blocksNum++;
    for(size_t k = 0; k < blocksNum; ++k)
    {
        /* Calculate hash of file block if it is missing. */
        if(k == file1.hash.size())
        {
            addBlockHash(file1, k);
        }
        if(k == file2.hash.size())
        {
            addBlockHash(file2, k);
        }
        /* Return from the loop if at least one hash is not the same. */
        if(file1.hash[k] != file2.hash[k])
        {
            return false;
        }
    }
    file1.isDupFinded = true;
    file2.isDupFinded = true;
    return true;
}

void DuplicateFinder::printDups()
{
    for(const auto &path : m_dupPaths)
    {
        std::cout << path << std::endl;
    }
    std::cout << std::endl;
}