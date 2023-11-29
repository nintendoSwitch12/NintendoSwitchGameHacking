#ifndef __STORAGE_HPP_
#define __STORAGE_HPP_

#include <string>
#include <filesystem>
#include <queue>
#include <cstdint>

#include <libmtp.h>

#include "pathutils.hpp"
#include "iterator.hpp"
#include "folder.hpp"
#include "file.hpp"

namespace mtp_utils
{
	class Storage
	{
	private:
		LIBMTP_mtpdevice_t* device_;
		uint32_t storage_id_;
		std::string storage_description_;
		std::string volum_identifier_;
		Folder root_;


		inline bool ReceiveFileByID(uint32_t file_id, const std::filesystem::path& file_path)
		{
			const int& result = LIBMTP_Get_File_To_File(device_, file_id, file_path.native().c_str(), nullptr, nullptr);
			return (!result);
		}

		LIBMTP_file_t* SendFileToFolderID(const std::filesystem::path& file_path, uint32_t to_folder_id,
			LIBMTP_filetype_t file_type = LIBMTP_filetype_t::LIBMTP_FILETYPE_UNKNOWN)
		{
			LIBMTP_file_t* file = LIBMTP_new_file_t();
			file->filename = strdup(file_path.filename().c_str());
			file->filesize = std::filesystem::file_size(file_path);
			if (file_type != LIBMTP_filetype_t::LIBMTP_FILETYPE_UNKNOWN)
				file->filetype = file_type;
			else
				file->filetype = GuessFiletypeByExtension(file_path);
			file->parent_id = to_folder_id;
			file->storage_id = storage_id_;

			LIBMTP_Send_File_From_File(device_, file_path.native().c_str(), file, nullptr, nullptr);
			return file;
		}

		inline bool DeleteObjectByID(uint32_t object_id)
		{
			const int& result = LIBMTP_Delete_Object(device_, object_id);
			return !result;
		}

		std::string SetFileNameByID(uint32_t file_id, const std::string& new_name)
		{
			LIBMTP_file_t* file = LIBMTP_Get_Filemetadata(device_, file_id);
			if (!file)
				return std::string{};
			const int& result = LIBMTP_Set_File_Name(device_, file, new_name.c_str());
			const std::string& name{ file->filename };
			LIBMTP_destroy_file_t(file);
			return name;
		}

		inline bool MoveFileByIDs(uint32_t target_file_id, uint32_t to_folder_id)
		{
			const int& result = LIBMTP_Move_Object(device_, target_file_id, storage_id_, to_folder_id);
			return !result;
		}


		Folder* MakeEmptyFolderToFolderID(const std::string& folder_name, Folder* to_folder)
		{
			char* name = strdup(folder_name.c_str());
			const uint32_t& id = LIBMTP_Create_Folder(device_, name, to_folder->folder_id_, storage_id_);
			if (!id)
				return nullptr;
			Folder* new_folder = new Folder( device_, storage_id_, to_folder, id, name );
			delete[] name;
			return new_folder;
		}

		std::string SetFolderNameByID(uint32_t folder_id, const std::string& new_name)
		{
			LIBMTP_folder_t* folders = LIBMTP_Get_Folder_List_For_Storage(device_, storage_id_);
			std::string name;
			for (LIBMTP_folder_t* folder = folders; folder; folder = folder->child)
			{
				if (folder->folder_id != folder_id)
					continue;
				LIBMTP_Set_Folder_Name(device_, folder, new_name.c_str());
				name = folder->name;
			}
			LIBMTP_destroy_folder_t(folders);
			return name;
		}

	public:
		Storage() = delete;

		inline Storage(LIBMTP_mtpdevice_t* device, LIBMTP_devicestorage_t* storage)
			: device_(device), storage_id_(storage->id), root_(Folder(device_, storage_id_, nullptr, LIBMTP_FILES_AND_FOLDERS_ROOT, "root")),
			storage_description_(storage->StorageDescription), volum_identifier_(storage->VolumeIdentifier) {}

		inline Storage(const Storage& storage) = delete;

		Storage(Storage&& storage)
			: device_(storage.device_), storage_id_(storage.storage_id_), root_(std::move(storage.root_)),
			storage_description_(std::move(storage.storage_description_)), volum_identifier_(std::move(storage.volum_identifier_))
		{
			storage.device_ = nullptr;
		}

		~Storage() {}

		inline Storage& operator=(const Storage& rhs) = delete;

		inline uint32_t GetStorageID() const
		{
			return storage_id_;
		}

		inline std::string GetStorageDescription() const
		{
			return storage_description_;
		}

		inline std::string GetVolumIdentifier() const
		{
			return volum_identifier_;
		}

		inline Folder* GetRootFolder()
		{
			return &root_;
		}


		uint32_t FindFileIDByName(const std::string& file_name)
		{
			auto file = root_.FindChildFileByName(file_name);
			return file ? file->GetFileID() : 0;
		}

		inline uint32_t FindFolderIDByName(const std::string& folder_name)
		{
			auto folder = root_.FindChildFolderByName(folder_name);
			return folder ? folder->GetFolderID() : 0;
		}

		uint32_t FindFileIDByExactName(const std::string& exact_file_name)
		{
			auto file = root_.FindChildFileByExactName(exact_file_name);
			return file ? file->GetFileID() : 0;
		}

		inline uint32_t FindFolderIDByExactName(const std::string& exact_folder_name)
		{
			auto folder = root_.FindChildFolderByExactName(exact_folder_name);
			return folder ? folder->GetFolderID() : 0;
		}

		inline File* FindFileByName(const std::string& file_name)
		{
			return root_.FindChildFileByName(file_name);
		}

		inline Folder* FindFolderByName(const std::string& folder_name)
		{
			return root_.FindChildFolderByName(folder_name);
		}

		inline File* FindFileByExactName(const std::string& exact_file_name)
		{
			return root_.FindChildFileByExactName(exact_file_name);
		}

		inline Folder* FindFolderByExactName(const std::string& exact_folder_name)
		{
			return root_.FindChildFolderByExactName(exact_folder_name);
		}

		inline File* FindFileByID(uint32_t file_id)
		{
			return root_.FindChildFileByID(file_id);
		}

		inline Folder* FindFolderByID(uint32_t folder_id)
		{
			return root_.FindChildFolderByID(folder_id);
		}


		inline bool IsFileExistByID(uint32_t file_id)
		{
			return (FindFileByID(file_id) != nullptr);
		}

		inline bool IsFolderExistByID(uint32_t folder_id)
		{
			return (FindFolderByID(folder_id) != nullptr);
		}


		inline bool ReceiveFile(File& file, const std::filesystem::path& file_path)
		{
			if (!std::filesystem::is_directory(file_path))
				return ReceiveFileByID(file.file_id_, file_path);
			else
				return ReceiveFileByID(file.file_id_, file_path / file.GetFileName());
		}

		bool SendFile(const std::filesystem::path& file_path, Folder& folder)
		{
			if (!file_path.has_filename())
				return false;
			LIBMTP_file_t* file = SendFileToFolderID(file_path, folder.GetFolderID());
			if (file->item_id == 0)
			{
				LIBMTP_destroy_file_t(file);
				return false;
			}
			folder.AddChildFile(new File(device_, &folder, file));
			LIBMTP_destroy_file_t(file);
			return true;
		}

		inline bool DeleteFile(File& file)
		{
			if (!DeleteObjectByID(file.file_id_))
				return false;
			Folder* parent = file.GetParent();
			const uint32_t& file_id = file.GetFileID();
			parent->RemoveFile(&file);
			return true;
		}

		inline std::string SetFileName(File& file, const std::string& new_name)
		{
			const std::string& name = SetFileNameByID(file.GetFileID(), new_name);
			file.SetFileName(name);
			return name;
		}

		inline bool MoveFile(File& file, Folder& to)
		{
			const bool& result = MoveFileByIDs(file.file_id_, to.folder_id_);
			file.GetParent()->RemoveFile(&file);
			to.AddChildFile(&file);
			return result;
		}


		bool ReceiveFolder(Folder& folder, const std::filesystem::path& file_path)
		{
			if (!std::filesystem::exists(file_path))
				return false;

			bool result = true;
			const std::filesystem::path folder_location{ file_path / folder.GetFolderName() };
			std::filesystem::create_directories(folder_location);

			for (auto& file : folder.GetChildFilesOnlyOnThis())
				result &= ReceiveFile(*file, folder_location);

			for (auto& folder : folder.GetChildFoldersOnlyOnThis())
				result &= ReceiveFolder(*folder, folder_location);

			return result;
		}

		inline Folder* MakeEmptyFolderToFolder(const std::string& folder_name, Folder& to_folder)
		{
			Folder* folder = MakeEmptyFolderToFolderID(folder_name, &to_folder);
			to_folder.AddChildFolder(folder);
			return folder;
		}

		bool SendFolder(const std::filesystem::path& folder_path, Folder& to_folder)
		{
			if (!std::filesystem::exists(folder_path) || !std::filesystem::is_directory(folder_path))
				return false;
			bool result = true;

			Folder* new_folder = to_folder.FindChildFolderByExactName(folder_path.filename());
			if (!new_folder)
				new_folder = MakeEmptyFolderToFolder(folder_path.filename().string(), to_folder);

			for (auto& directory_entry : std::filesystem::directory_iterator(folder_path))
			{
				if (directory_entry.is_directory())
					result &= SendFolder(directory_entry, *new_folder);
				else
					result &= SendFile(directory_entry, *new_folder);
			}

			return result;
		}

		bool DeleteFolder(Folder& folder)
		{
			bool result = true;

			std::queue<File*> files;
			for (auto& file : folder.GetChildFilesOnlyOnThis())
				files.push(file);
			while (!files.empty())
			{
				File* f = files.front();
				files.pop();
				result &= DeleteFile(*f);
			}

			std::queue<Folder*> folders;
			for (auto& folder : folder.GetChildFoldersOnlyOnThis())
				folders.push(folder);
			while (!folders.empty())
			{
				Folder* f = folders.front();
				folders.pop();
				result &= DeleteFolder(*f);
			}

			DeleteObjectByID(folder.folder_id_);
			folder.GetParentFolder()->RemoveFolder(&folder);

			return result;
		}

		inline std::string SetFolderName(Folder& folder, const std::string& new_name)
		{
			const std::string name = SetFolderNameByID(folder.folder_id_, new_name);
			folder.SetFolderName(name);
			return name;
		}

		bool MoveFolder(Folder& folder, Folder& to_folder)
		{
			bool result = true;

			Folder* target_folder = MakeEmptyFolderToFolder(folder.folder_name_, to_folder);

			for (auto& file : folder.GetChildFilesOnlyOnThis())
				result &= MoveFile(*file, *target_folder);
			for (auto& folder : folder.GetChildFoldersOnlyOnThis())
				result &= MoveFolder(*folder, *target_folder);
			result &= DeleteFolder(folder);

			return result;
		}


		inline SimpleMapIterator<Folder*> GetFoldersOnRoot()
		{
			return root_.GetChildFoldersOnlyOnThis();
		}

		inline SimpleMapIterator<File*> GetFilesOnRoot()
		{
			return root_.GetChildFilesOnlyOnThis();
		}

		inline Folder::FolderIterator GetAllChildFolders()
		{
			return root_.GetAllChildFolders();
		}
	};
}

#endif