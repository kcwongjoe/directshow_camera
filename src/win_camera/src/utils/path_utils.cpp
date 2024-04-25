/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "utils/path_utils.h"
#include "utils/string_utils.h"

namespace Utils
{
    std::string Utils::PathUtils::getExtension(std::filesystem::path path)
    {
        // Get file extension
        std::string fileExtension = path.extension().string();

        // Trim
        StringUtils::Trim(fileExtension);

        // Remove the first dot
        if (fileExtension.at(0) == '.')
        {
            fileExtension.erase(0, 1);
        }

        // To lower case
        StringUtils::ToLowercase(fileExtension);

        return fileExtension;
    }
}