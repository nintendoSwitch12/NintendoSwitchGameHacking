#ifndef __FOLDER_HPP_
#define __FOLDER_HPP_

#include <string>
#include <map>
#include <stack>
#include <cstring>
#include <cassert>

#include <libmtp.h>

#include "file.hpp"
#include "iterator.hpp"

namespace mtp_utils
{
	class Folder
	{
		friend class Storage;
	private:
		LIBMTP_mtpdevice_t* device_;
		uint32_t folder_id_;
		uint32_t storage_id_;
		Folder* parent_;
		std::string folder_name_;
		std::map<uint32_t, Folder*> child_folders_;
		std::map<uint32_t, File*> child_files_;


		inline void SetFolderID(uint32_t new_folder_id)
		{
			folder_id_ = new_folder_id;
		}

		inline void SetStorageID(uint32_t new_storage_id)
		{
			storage_id_ = new_storage_id;
		}

		inline void SetFolderName(const std::string& new_name)
		{
			folder_name_ = new_name;
		}

		inline void SetFolderName(std::string&& new_name)
		{
			folder_name_ = std::move(new_name);
		}

		inline void SetParentFolder(Folder* new_parent_folder)
		{
			parent_ = new_parent_folder;
		}


		void GetFilesAndFoldersFromStorage()
		{
			LIBMTP_file_t* files = LIBMTP_Get_Files_And_Folders(device_, storage_id_, folder_id_);
			for (LIBMTP_file_t* file = files; file; file = file->next)
			{
				if (file->filetype == LIBMTP_filetype_t::LIBMTP_FILETYPE_FOLDER)
				{
					child_folders_.insert(std::make_pair(file->item_id, new Folder(device_, storage_id_, this, file->item_id, file->filename)));
				}
				else
				{
					child_files_.insert(std::make_pair(file->item_id, new File(device_, this, file)));
				}
			}
			LIBMTP_destroy_file_t(files);
		}

		inline void AddChildFile(LIBMTP_file_t* file)
		{
			AddChildFile(new File{ device_, this, file });
		}

		inline void AddChildFile(File* file)
		{
			child_files_.insert(std::make_pair(file->GetFileID(), file));
		}

		inline void RemoveFile(File* file)
		{
			if (file)
			{
				child_files_.erase(file->GetFileID());
				delete file;
			}
		}

		inline void AddChildFolder(LIBMTP_file_t* folder)
		{
			AddChildFolder(new Folder{ device_, this, folder });
		}

		inline void AddChildFolder(LIBMTP_folder_t* folder)
		{
			AddChildFolder(new Folder{ device_, this, folder });
		}

		inline void AddChildFolder(Folder* folder)
		{
			child_folders_.insert(std::make_pair(folder->folder_id_, folder));
		}

		inline void RemoveFolder(Folder* folder)
		{
			if (folder)
			{
				child_folders_.erase(folder->folder_id_);
				delete folder;
			}
		}

	public:
		class FolderIterator
		{
			using self_type = FolderIterator;
			using map_type = std::map<uint32_t, Folder*>;
			using iterator_type = SimpleMapIterator<Folder*>;
		private:
			std::stack<iterator_type> iterator_stack_;
			iterator_type original_itr_;
			iterator_type itr_;
			int depth_;

		public:
			FolderIterator() 
				: iterator_stack_(), original_itr_(), itr_() {}

			FolderIterator(Folder& folder)
				: iterator_stack_(), original_itr_(folder.GetChildFoldersOnlyOnThis()), itr_(original_itr_), depth_(0) {}

			FolderIterator(map_type& map)
				: iterator_stack_(), original_itr_(iterator_type(map)), itr_(iterator_type(map)), depth_(0){}

			FolderIterator(iterator_type itr)
				: iterator_stack_(), original_itr_(itr), itr_(itr), depth_(0) {}

			FolderIterator(const self_type& itr)
				: iterator_stack_(itr.iterator_stack_), original_itr_(itr.original_itr_), itr_(itr.itr_), depth_(itr.depth_) {}

			FolderIterator(self_type&& itr)
				: iterator_stack_(std::move(itr.iterator_stack_)), original_itr_(std::move(itr.original_itr_)), 
				itr_(std::move(itr.itr_)), depth_(itr.depth_) {}


			inline int GetDepth() const
			{
				return depth_;
			}


			inline self_type begin()
			{
				return self_type{ original_itr_.begin() };
			}

			inline self_type end()
			{
				return self_type{ original_itr_.end() };
			}


			inline self_type operator++()
			{
				if ((*itr_)->GetNumberOfChildFoldersOnThis())
				{
					iterator_stack_.push(itr_);
					itr_ = (*itr_)->GetChildFoldersOnlyOnThis();
					++depth_;
				}
				else
				{
					++itr_;
					while (itr_ == itr_.end())
					{
						if (itr_ == original_itr_.end())
							return *this;
						itr_ = ++iterator_stack_.top();
						iterator_stack_.pop();
						--depth_;
					}
				}
				return *this;
			}

			inline self_type operator++(int junk)
			{
				self_type now = *this;
				if ((*itr_)->GetNumberOfChildFoldersOnThis())
				{
					iterator_stack_.push(itr_);
					itr_ = (*itr_)->GetChildFoldersOnlyOnThis();
					++depth_;
				}
				else
				{
					++itr_;
					while (itr_ == itr_.end())
					{
						if (itr_ == original_itr_.end())
							return *this;
						itr_ = ++iterator_stack_.top();
						iterator_stack_.pop();
						--depth_;
					}
				}
				return now;
			}


			inline bool operator==(const self_type& rhs)
			{
				return (itr_ == rhs.itr_);
			}

			inline bool operator!=(const self_type& rhs)
			{
				return (itr_ != rhs.itr_);
			}

			inline Folder& operator*()
			{
				return **itr_;
			}

			inline Folder* operator->()
			{
				return *itr_;
			}
		};
		
		Folder() = default;

		inline Folder(LIBMTP_mtpdevice_t* device, Folder* parent, LIBMTP_file_t* this_folder)
			: device_(device), storage_id_(this_folder->storage_id), parent_(parent),
			folder_id_(this_folder->item_id), folder_name_(this_folder->filename)
		{
			GetFilesAndFoldersFromStorage();
		}

		inline Folder(LIBMTP_mtpdevice_t* device, Folder* parent, LIBMTP_folder_t* this_folder)
			: device_(device), storage_id_(this_folder->storage_id), parent_(parent),
			folder_id_(this_folder->folder_id), folder_name_(this_folder->name)
		{
			GetFilesAndFoldersFromStorage();
		}

		inline Folder(LIBMTP_mtpdevice_t* device, uint32_t storage_id, Folder* parent, uint32_t folder_id, const std::string& folder_name)
			: device_(device), storage_id_(storage_id), parent_(parent), folder_id_(folder_id), folder_name_(folder_name)
		{
			GetFilesAndFoldersFromStorage();
		}

		inline Folder(const Folder& folder) = delete;

		inline Folder(Folder&& folder)
			: device_(folder.device_), storage_id_(folder.storage_id_), parent_(folder.parent_),
			folder_id_(folder.folder_id_), folder_name_(std::move(folder.folder_name_)),
			child_folders_(std::move(folder.child_folders_)), child_files_(std::move(folder.child_files_)) {}


		inline ~Folder() 
		{
			for (auto& folder : child_folders_)
				delete folder.second;
			child_folders_.clear();
			for (auto& file : child_files_)
				delete file.second;
			child_folders_.clear();
		}


		inline uint32_t GetFolderID() const
		{
			return folder_id_;
		}
		
		inline std::string GetFolderName() const
		{
			return folder_name_;
		}

		inline Folder* GetParentFolder() const
		{
			return parent_;
		}

		inline size_t GetNumberOfChildFoldersOnThis() const
		{
			return child_folders_.size();
		}

		inline size_t GetNumberOfChildFiles() const
		{
			return child_files_.size();
		}


		inline bool IsRootFolder() const
		{
			return (parent_ == nullptr);
		}


		File* FindChildFileByID(uint32_t file_id)
		{
			if (child_files_.empty())
				return nullptr;
			auto itr = child_files_.find(file_id);
			if (itr != child_files_.end())
				return itr->second;
			for (auto& child : child_folders_)
			{
				auto* target = child.second->FindChildFileByID(file_id);
				if (target)
					return target;
			}
			return nullptr;
		}

		Folder* FindChildFolderByID(uint32_t folder_id)
		{
			if (child_folders_.empty())
				return nullptr;
			auto itr = child_folders_.find(folder_id);
			if (itr != child_folders_.end())
				return itr->second;
			for (auto& child : child_folders_)
			{
				auto* target = child.second->FindChildFolderByID(folder_id);
				if (target)
					return target;
			}
			return nullptr;
		}

		File* FindChildFileByName(const std::string& file_name)
		{
			if (child_files_.empty())
				return nullptr;
			for (auto& child : child_files_)
			{
				if (child.second->GetFileName().find(file_name) != std::string::npos)
					return child.second;
			}
			for (auto& child : child_folders_)
			{
				auto* target = child.second->FindChildFileByName(file_name);
				if (target)
					return target;
			}
			return nullptr;
		}

		Folder* FindChildFolderByName(const std::string& folder_name)
		{
			if (child_folders_.empty())
				return nullptr;
			for (auto& child : child_folders_)
			{
				if (child.second->GetFolderName().find(folder_name) != std::string::npos)
					return child.second;
			}
			for (auto& child : child_folders_)
			{
				auto* target = child.second->FindChildFolderByName(folder_name);
				if (target)
					return target;
			}
			return nullptr;
		}

		File* FindChildFileByExactName(const std::string& exact_file_name)
		{
			if (child_files_.empty())
				return nullptr;
			for (auto& child : child_files_)
			{
				if (child.second->GetFileName() == exact_file_name)
					return child.second;
			}
			for (auto& child : child_folders_)
			{
				auto* target = child.second->FindChildFileByExactName(exact_file_name);
				if (target)
					return target;
			}
			return nullptr;
		}

		Folder* FindChildFolderByExactName(const std::string& exact_folder_name)
		{
			if (child_folders_.empty())
				return nullptr;
			for (auto& child : child_folders_)
			{
				if (child.second->GetFolderName() == exact_folder_name)
					return child.second;
			}
			for (auto& child : child_folders_)
			{
				auto* target = child.second->FindChildFolderByExactName(exact_folder_name);
				if (target)
					return target;
			}
			return nullptr;
		}


		inline SimpleMapIterator<Folder*> GetChildFoldersOnlyOnThis()
		{
			return SimpleMapIterator<Folder*>(child_folders_);
		}

		inline SimpleMapIterator<File*> GetChildFilesOnlyOnThis()
		{
			return SimpleMapIterator<File*>(child_files_);
		}
		
		inline FolderIterator GetAllChildFolders()
		{
			return FolderIterator(child_folders_);
		}

	};

}

#endif