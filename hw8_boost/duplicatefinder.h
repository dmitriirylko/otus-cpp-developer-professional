#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>

#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <boost/crc.hpp>

#include "config.h"

struct FileInfo
{
    FileInfo(const boost::filesystem::path &fsPath);
    boost::filesystem::path path;
    std::vector<uint32_t> hash;
    size_t size;
    bool isDupFinded = false;
};

class DuplicateFinder
{
public:
    DuplicateFinder();
    ~DuplicateFinder() = default;
    void createFilePool();
    void findDuplicates();

private:
    template<typename T>
    void iterateFolder(const std::string& path);

    /**
     * @brief Checks if query path is subpath of directory. Used for detecting
     *        paths that are must be excluded.
     * @param queryPath Path to check.
     * @param directoryPath Directory path from where subpath started.
     * @return True - query path is subpath, false - is not subpath.
     */
    bool isSubpath(const boost::filesystem::path &queryPath,
                   const boost::filesystem::path &directoryPath);
    
    /**
     * @brief Inserts hash of file block to collection of hashes.
     * @param file File to calculate hash.
     * @param k Index of block.
     */
    void addBlockHash(FileInfo& file, size_t k);

    /**
     * @brief Compares hashes of two files. If hash of block doesn't
     *        exist in collection it added beforehand. Assumed that
     *        files have the same sizes.
     * @param file1 Handler of first file.
     * @param file2 Handler of second file.
     * @return True - hases are the same, false - hashes are different.
     */
    bool compareHashes(FileInfo& file1, FileInfo& file2);

    /**
     * @brief Prints to standard output stream current collection of
     *        duplicates (m_dupPaths).
     */
    void printDups();

    /**
     * @brief Collection of files that are allowed for comparison.
     */
    std::vector<FileInfo> m_filePool;
    
    /**
     * @brief Buffer where chunks of files will be copied.
     */
    std::vector<char> m_buf;

    /**
     * @brief Vector of duplicated files. Should store only same
     *        files for further printing.
     */
    std::vector<boost::filesystem::path> m_dupPaths;
};