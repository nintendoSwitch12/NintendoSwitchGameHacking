#ifndef __FILE_HPP_
#define __FILE_HPP_

#include <string>
#include <filesystem>

#include <libmtp.h>

namespace mtp_utils
{
	class Folder;

	class File
	{
		friend class Storage;

	private:
		LIBMTP_mtpdevice_t* device_;
		uint32_t storage_id_;
		uint32_t file_id_;
		Folder* parent_;
		std::string file_name_;
		LIBMTP_filetype_t file_type_;


		inline void SetFolderID(uint32_t new_file_id)
		{
			file_id_ = new_file_id;
		}

		inline void SetStorageID(uint32_t new_storage_id)
		{
			storage_id_ = new_storage_id;
		}

		inline void SetFileName(const std::string& new_name)
		{
			file_name_ = new_name;
		}

		inline void SetFileName(std::string&& new_name)
		{
			file_name_ = std::move(new_name);
		}

		inline void SetParentFolder(Folder* new_parent_folder)
		{
			parent_ = new_parent_folder;
		}

		inline void SetFileType(LIBMTP_filetype_t new_file_type)
		{
			file_type_ = new_file_type;
		}

	public:
		File() = delete;

		inline File(LIBMTP_mtpdevice_t* device, Folder* parent, LIBMTP_file_t* file)
			: device_(device), storage_id_(file->storage_id), file_id_(file->item_id),
			parent_(parent), file_name_(file->filename), file_type_(file->filetype) {}

		inline File(LIBMTP_mtpdevice_t* device, uint32_t storage_id, uint32_t file_id, 
			Folder* parent, const std::string& file_name, LIBMTP_filetype_t file_type)
			: device_(device), storage_id_(storage_id), file_id_(file_id), 
			parent_(parent), file_name_(file_name), file_type_(file_type) {}
		
		inline File(File&& file)
			: device_(file.device_), storage_id_(file.storage_id_), file_id_(file.file_id_),
			parent_(file.parent_), file_name_(file.file_name_), file_type_(file.file_type_) {}

		File(const File& file) = delete;


		~File() {}


		File& operator=(const File& rhs) = delete;


		inline uint32_t GetStorageID() const
		{
			return storage_id_;
		}

		inline uint32_t GetFileID() const
		{
			return file_id_;
		}

		inline Folder* GetParent()
		{
			return parent_;
		}

		inline std::string GetFileName() const
		{
			return file_name_;
		}

		inline LIBMTP_filetype_t GetFileType() const
		{
			return file_type_;
		}


		inline bool IsOnRoot() const
		{
			return (parent_ == nullptr);
		}

	};
}

#endif