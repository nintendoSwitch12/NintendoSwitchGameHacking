#include <iostream>
#include <queue>

#include <libmtp.h>

#include "mtp.hpp"

using namespace mtp_utils;

void ReplaceSaveFolder(MTP& mtp, const std::filesystem::path& save_folder_location_on_pc)
{
    Device* device = mtp.FindDeviceByExactFriendlyName("DBI MTP Responder");
    if (!device)
        return;
    std::cout << device->GetFriendlyName() << " is found!" << std::endl;

    Storage* storage = device->FindStorageByExactStorageDescription("7: Saves");
    if (!storage)
        return;
    std::cout << storage->GetStorageDescription() << " is found!" << std::endl;

    Folder* nh = storage->FindFolderByExactName("모여봐요 동물의 숲");
    if (!nh)
        return;
    std::cout << nh->GetFolderName() << " is found!" << std::endl;

    Folder* savefolder = nh->FindChildFolderByExactName("Device");
    if (savefolder)
    {
        std::cout << savefolder->GetFolderName() << " is found!" << std::endl;
        
        std::queue<File*> files;
        for (auto& file : savefolder->GetChildFilesOnlyOnThis())
            files.push(file);
        for (auto& file : savefolder->FindChildFolderByExactName("Villager0")->GetChildFilesOnlyOnThis())
            files.push(file);
        
        while (!files.empty())
        {
            File* f = files.front();
            files.pop();
            Folder* p = f->GetParent();
            const std::string file_name = f->GetFileName();
            storage->DeleteFile(*f);
            if(p->GetFolderName() == "Device")
                storage->SendFile(save_folder_location_on_pc / file_name, *p);
            else
                storage->SendFile(save_folder_location_on_pc / p->GetFolderName() / file_name, *p);
        }
    }
    else
    {
        std::cout << "Device is not found. Create folder." << std::endl;
        storage->SendFolder(save_folder_location_on_pc, *nh);
    }
}

void DumpNHSaveFolder(MTP& mtp, const std::filesystem::path& location)
{
    Device* device = mtp.FindDeviceByExactFriendlyName("DBI MTP Responder");
    if (!device)
        return;
    std::cout << device->GetFriendlyName() << " is found!" << std::endl;

    Storage* storage = device->FindStorageByExactStorageDescription("7: Saves");
    if (!storage)
        return;
    std::cout << storage->GetStorageDescription() << " is found!" << std::endl;

    Folder* nh = storage->FindFolderByExactName("모여봐요 동물의 숲");
    if (!nh)
        return;
    std::cout << nh->GetFolderName() << " is found!" << std::endl;

    Folder* savefolder = nh->FindChildFolderByExactName("Device");
    if (!savefolder)
        return;
    std::cout << savefolder->GetFolderName() << " is found!" << std::endl;

    std::cout << "Dump Device to " << location.string() << std::endl;
    storage->ReceiveFolder(*savefolder, location);
    std::cout << "Done" << std::endl;;
}

void ShowDirectoryTree(MTP& mtp)
{
    Device* device = mtp.FindDeviceByExactFriendlyName("DBI MTP Responder");
    if (!device)
        return;
    std::cout << device->GetFriendlyName() << " is found!" << std::endl;

    for (auto& storage : device->GetStorages())
    {
        std::cout << storage.GetStorageDescription() << std::endl;

        auto folder_iterator = storage.GetAllChildFolders();
        for (auto itr = folder_iterator.begin(); itr != itr.end(); ++itr)
        {
            const std::string& indent = std::string(4 * itr.GetDepth() + 4, ' ');
            std::cout << indent + itr->GetFolderName() << std::endl;
            for (auto& file : itr->GetChildFilesOnlyOnThis())
                std::cout << indent + "  " + file->GetFileName() << std::endl;
        }

        for (auto& file : storage.GetFilesOnRoot())
            std::cout << "   " + file->GetFileName() << std::endl;
    }
}

void Usage(const char* file_name)
{
    std::cout << "Usage: " << file_name << " fuzz [path_of_savefolder_on_pc]" << std::endl;
    std::cout << "                    or" << std::endl;
    std::cout << "       " << file_name << " dump [path_to_dump]" << std::endl;
    std::cout << "                    or" << std::endl;
    std::cout << "       " << file_name << " tree" << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        Usage(argv[0]);
        return -1;
    }
    MTP mtp;
    if (mtp.IsSuccess())
    {
        const std::string& command = argv[1];
        if (command == "fuzz" && argc >= 3)
        {
            const std::filesystem::path& path = argv[2];
            if (path.filename() != "Device")
            {
                std::cout << "Folder name must be \"Device\"." << std::endl;
                return 0;
            }
            ReplaceSaveFolder(mtp, argv[2]);
            return 0;
        }
        else if (command == "dump" && argc >= 3)
        {
            DumpNHSaveFolder(mtp, argv[2]);
            return 0;
        }
        else if (command == "tree")
        {
            ShowDirectoryTree(mtp);
            return 0;
        }
        Usage(argv[0]);
        return 0;
    }
    LIBMTP_error_number_t err = mtp.GetError();
    switch (err) {
    case LIBMTP_ERROR_NO_DEVICE_ATTACHED:
        std::cout << "   No raw devices found." << std::endl;
        return 0;
    case LIBMTP_ERROR_CONNECTING:
        std::cerr << "Detect: There has been an error connecting. Exiting" << std::endl;
        return 1;
    case LIBMTP_ERROR_MEMORY_ALLOCATION:
        std::cerr << "Detect: Encountered a Memory Allocation Error. Exiting" << std::endl;
        return 1;
    case LIBMTP_ERROR_NONE:
        break;
    case LIBMTP_ERROR_GENERAL:
    default:
        std::cerr << "Unknown connection error." << std::endl;
        return 1;
    }
}