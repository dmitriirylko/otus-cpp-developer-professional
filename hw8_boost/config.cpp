#include "config.h"

Config::Config() :
    m_minFileSize{1},
    m_blockSize{32},
    m_scanningLevel{1},
    m_hashAlgorithm{HashAlgorithm::CRC32},
    m_desc{"Options"}
{
    m_desc.add_options()
    ("include",
    boost::program_options::value<std::vector<std::string>>()->multitoken(),
    "Set folder paths to scan")
    ("exclude",
    boost::program_options::value<std::vector<std::string>>()->multitoken(),
    "Exclude folder paths from scanning")
    ("filesize",
    boost::program_options::value<size_t>(),
    "Minimum size of query file")
    ("blocksize",
    boost::program_options::value<size_t>(),
    "Block size of one file chunk on which file will be splitted")
    ("masks",
    boost::program_options::value<std::vector<std::string>>()->multitoken(),
    "Masks of filenames to scan")
    ("level",
    boost::program_options::value<int>(),
    "Scanning level: 0 - only current directory, 1 - current directory with all subfolders")
    ("hash",
    boost::program_options::value<std::string>(),
    "Hashing algorithm");
}

const std::vector<std::string>& Config::getIncludedFolderPaths()
{
    return m_includedFolderPaths;
}

const std::vector<std::string>& Config::getExcludedFolderPaths()
{
    return m_excludedFolderPaths;
}

size_t Config::getMinFileSize()
{
    return m_minFileSize;
}

size_t Config::getBlockSize()
{
    return m_blockSize;
}

const std::vector<std::string>& Config::getMasks()
{
    return m_masks;
}

uint8_t Config::getScanningLevel()
{
    return m_scanningLevel;
}

HashAlgorithm Config::getHashAlgorithm()
{
    return m_hashAlgorithm;
}

void Config::setData(int argc, char* argv[])
{
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, m_desc), m_vm);
    boost::program_options::notify(m_vm);
    
    if(m_vm.count("include"))
    {
        m_includedFolderPaths = m_vm["include"].as<std::vector<std::string>>();
        std::cout << m_includedFolderPaths[0] << std::endl;
    }
    else
    {
        throw std::invalid_argument("--include arg is required.");
    }
    
    if(m_vm.count("exclude"))
    {
        m_excludedFolderPaths = m_vm["exclude"].as<std::vector<std::string>>();
    }
    
    if(m_vm.count("filesize"))
    {
        m_minFileSize = m_vm["filesize"].as<size_t>();
    }
    
    if(m_vm.count("blocksize"))
    {
        m_blockSize = m_vm["blocksize"].as<size_t>();
    }
    
    if(m_vm.count("masks"))
    {
        m_masks = m_vm["masks"].as<std::vector<std::string>>();
    }
    
    if(m_vm.count("level"))
    {
        if(!(m_vm["level"].as<int>() == 0 || m_vm["level"].as<int>() == 1))
        {
            throw std::invalid_argument("Scanning level must be 0 or 1.");
        }
        m_scanningLevel = m_vm["level"].as<int>();
    }
    
    if(m_vm.count("hash"))
    {
        std::string hashAlg = m_vm["hash"].as<std::string>();
        boost::algorithm::to_lower(hashAlg);
        if(hashAlg == "md5")
        {
            m_hashAlgorithm = HashAlgorithm::MD5;
        }
        else if(hashAlg == "crc32")
        {
            m_hashAlgorithm == HashAlgorithm::CRC32;
        }
        else
        {
            throw std::invalid_argument("Invalid type of crc algorithm.");
        }
    }
}