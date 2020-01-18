#include "song_processor.hpp"

#include <ghc/filesystem.hpp>
#include <iostream>

#include "../liblsdj/error.h"
#include "../liblsdj/sav.h"
#include "common.hpp"

namespace lsdj
{
    bool SongProcessor::process(const ghc::filesystem::path& path)
    {        
        if (isHiddenFile(path.filename().string()))
            return true;
        
        if (ghc::filesystem::is_directory(path))
        {
            if (processDirectory(path) != 0)
                return false;
        }
        else if (path.extension() == ".sav")
        {
            if (processSav(path) != 0)
                return false;
        }
        else if (path.extension() == ".lsdsng")
        {
            if (processLsdsng(path) != 0)
                return false;
        }
        
        return true;
    }

    bool SongProcessor::processDirectory(const ghc::filesystem::path& path)
    {
        if (verbose)
            std::cout << "Processing folder '" << path.string() << "'" << std::endl;
        
        for (auto it = ghc::filesystem::directory_iterator(path); it != ghc::filesystem::directory_iterator(); ++it)
        {
            if (process(it->path()) != 0)
                return false;
        }
        
        return true;
    }

    bool SongProcessor::processSav(const ghc::filesystem::path& path)
    {
        if (!shouldProcessSav(path))
            return true;
        
        lsdj_error_t* error = nullptr;
        lsdj_sav_t* sav = lsdj_sav_read_from_file(path.string().c_str(), &error);
        if (error != nullptr)
        {
            lsdj_sav_free(sav);
            return false;
        }
        
        if (verbose)
            std::cout << "Processing sav '" + path.string() + "'" << std::endl;
        
        processSong(*lsdj_sav_get_working_memory_song(sav));
        
        for (int i = 0; i < lsdj_sav_get_project_count(sav); ++i)
        {
            lsdj_project_t* project = lsdj_sav_get_project(sav, i);
            if (project == nullptr)
                continue;
            
            lsdj_song_t* song = lsdj_project_get_song(project);
            if (song == nullptr)
                continue;
            
            if (!processSong(*song))
            {
                lsdj_sav_free(sav);
                return false;
            }
        }
        
        lsdj_sav_write_to_file(sav, constructSavDestinationPath(path).string().c_str(), &error);
        if (error != nullptr)
        {
            lsdj_sav_free(sav);
            return false;
        }
        
        lsdj_sav_free(sav);
        
        return true;
    }

    bool SongProcessor::processLsdsng(const ghc::filesystem::path& path)
    {
        if (!shouldProcessLsdsng(path))
            return true;
        
        lsdj_error_t* error = nullptr;
        lsdj_project_t* project = lsdj_project_read_lsdsng_from_file(path.string().c_str(), &error);
        if (error != nullptr)
        {
            lsdj_project_free(project);
            return false;
        }
        
        if (verbose)
            std::cout << "Processing lsdsng '" + path.string() + "'" << std::endl;
        
        lsdj_song_t* song = lsdj_project_get_song(project);
        if (song == nullptr)
            return true;
        
        if (!processSong(*song))
        {
            lsdj_project_free(project);
            return false;
        }
        
        lsdj_project_write_lsdsng_to_file(project, constructLsdsngDestinationPath(path).string().c_str(), &error);
        if (error != nullptr)
        {
            lsdj_project_free(project);
            return false;
        }
        
        lsdj_project_free(project);
        
        return true;
    }
}
