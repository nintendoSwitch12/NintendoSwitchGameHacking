#include <vector>
#include <string>
#include <cstdint>
#include <cassert>

#include <libmtp.h>

#include "storage.hpp"

namespace mtp_utils
{
	class Device
	{
	private:
		LIBMTP_raw_device_t* raw_device_;
		int device_number_;
		LIBMTP_mtpdevice_t* device_;
		std::vector<Storage> storages_;
		std::string friendly_name_;
		std::string product_name_;


		inline void GetFriendlyNameFromDevice()
		{
			char* friendly_name = LIBMTP_Get_Friendlyname(device_);
			if (friendly_name)
			{
				friendly_name_ = friendly_name;
				LIBMTP_FreeMemory(friendly_name);
			}
			else
				friendly_name_ = "(null)";
		}

		inline void GetStoragesFromDevice()
		{
			assert(!LIBMTP_Get_Storage(device_, LIBMTP_STORAGE_SORTBY_NOTSORTED));
			for (LIBMTP_devicestorage_t* storage = device_->storage; storage; storage = storage->next)
				storages_.push_back(Storage(device_, storage));
		}

		inline bool OpenDevice()
		{
			device_ = LIBMTP_Open_Raw_Device_Uncached(raw_device_);
			return IsOpened();
		}

		inline bool IsOpened() const
		{
			return (device_ != nullptr);
		}

	public:
		Device() = delete;

		Device(LIBMTP_raw_device_t* raw_device)
			: raw_device_(raw_device), device_number_(raw_device->devnum), product_name_(raw_device->device_entry.product)
		{
			assert((raw_device != nullptr));
			assert(OpenDevice());
			GetFriendlyNameFromDevice();
			GetStoragesFromDevice();
		}

		Device(Device&& device)
			: raw_device_(device.raw_device_), device_(device.device_),
			device_number_(device.device_number_), product_name_(device.product_name_),
			friendly_name_(std::move(device.friendly_name_)), storages_(std::move(device.storages_))
		{
			device.raw_device_ = nullptr;
			device.device_ = nullptr;
		}

		inline Device(const Device& device) = delete;


		inline ~Device()
		{
			LIBMTP_Release_Device(device_);
		}


		Device& operator=(const Device& rhs) = delete;


		inline int GetDeviceNumber() const
		{
			return device_number_;
		}

		inline uint32_t GetBusLocation() const
		{
			return raw_device_->bus_location;
		}

		inline std::string GetFriendlyName() const
		{
			return friendly_name_;
		}

		inline std::string GetVendorName() const
		{
			return std::string{ raw_device_->device_entry.vendor };
		}

		inline uint16_t GetVendorID() const
		{
			return raw_device_->device_entry.vendor_id;
		}

		inline std::string GetProductName() const
		{
			return product_name_;
		}

		inline uint16_t GetProductID() const
		{
			return raw_device_->device_entry.product_id;
		}

		inline std::vector<Storage>& GetStorages()
		{
			return storages_;
		}


		Storage* FindStorageByID(uint32_t storage_id)
		{
			if (storage_id < 0)
				return nullptr;
			for (int i = 0; i < storages_.size(); ++i)
			{
				if (storages_[i].GetStorageID() == storage_id)
					return &storages_[i];
			}
			return nullptr;
		}

		Storage* FindStorageByStorageDescription(const std::string& storage_description)
		{
			for (int i = 0; i < storages_.size(); ++i)
			{
				if (storages_[i].GetStorageDescription().find(storage_description) != std::string::npos)
					return &storages_[i];
			}
			return nullptr;
		}

		Storage* FindStorageByExactStorageDescription(const std::string& storage_description)
		{
			for (int i = 0; i < storages_.size(); ++i)
			{
				if (storages_[i].GetStorageDescription() == storage_description)
					return &storages_[i];
			}
			return nullptr;
		}

		Storage* FindStorageByVolumIdentifier(const std::string& volum_identifier)
		{
			for (int i = 0; i < storages_.size(); ++i)
			{
				if (storages_[i].GetVolumIdentifier().find(volum_identifier) != std::string::npos)
					return &storages_[i];
			}
			return nullptr;
		}

		Storage* FindStorageByExactVolumIdentifier(const std::string& volum_identifier)
		{
			for (int i = 0; i < storages_.size(); ++i)
			{
				if (storages_[i].GetVolumIdentifier() == volum_identifier)
					return &storages_[i];
			}
			return nullptr;
		}

	};
}
