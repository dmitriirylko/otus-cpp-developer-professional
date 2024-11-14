#pragma once

#include <vector>
#include <iostream>
#include <stdexcept>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

enum class HashAlgorithm
{
    MD5,
    CRC32
};

class Config
{
public:
    static Config& instance()
    {
        static Config instance;
        return instance;
    }

    const std::vector<std::string>& getIncludedFolderPaths();

    const std::vector<std::string>& getExcludedFolderPaths();

    size_t getMinFileSize();

    size_t getBlockSize();

    const std::vector<std::string>& getMasks();

    uint8_t getScanningLevel();

    HashAlgorithm getHashAlgorithm();

    void setData(int argc, char* argv[]);

private:
    Config();
    Config(const Config& rhs) = delete;
    Config& operator=(const Config& rhs) = delete;
    Config(Config&& rhs) = delete;
    Config& operator=(Config&& rhs) = delete;

    /**
     * @brief Collection of folder paths that contain files which will be
     *          compared.
     */
    std::vector<std::string> m_includedFolderPaths;
    
    /**
     * @brief Collection of folder paths that are excluded from comparison.
     */
    std::vector<std::string> m_excludedFolderPaths;
    
    /**
     * @brief Minimal size of file to compare in bytes.
     */
    size_t m_minFileSize;

    /**
     * @brief Size of chunks on which file will be divided. Files will be
     *          compared by this chunks (hash of these chunks).
     */
    size_t m_blockSize;

    /**
     * @brief Collection of masks in form of regular expressions. Only files
     *          that match these masks will be added fir comparison.
     */
    std::vector<std::string> m_masks;
    
    /**
     * @brief Scanning level of directory:
     *          0 - only specified directory will be scanned;
     *          1 - nested directories will be scanned as well.
     */
    int m_scanningLevel;

    /**
     * @brief Hashing algorithm that will be applied to data chunks to
     *          compare files (default: CRC32).
     */
    HashAlgorithm m_hashAlgorithm;

    boost::program_options::options_description m_desc;

    boost::program_options::variables_map m_vm;
};