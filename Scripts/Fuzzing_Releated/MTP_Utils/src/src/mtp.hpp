#ifndef __MTP_HPP_
#define __MTP_HPP_

#include <vector>

#include <libmtp.h>

#include "device.hpp"

namespace mtp_utils
{
	class MTP
	{
	private:
		static bool is_libmtp_initialized;
		LIBMTP_raw_device_t* raw_device_;
		int number_of_devices_;
		std::vector<Device> devices_;
		LIBMTP_error_number_t error_;

	public:
		inline MTP()
			: raw_device_(nullptr), number_of_devices_(0), error_(LIBMTP_error_number_t::LIBMTP_ERROR_NONE), devices_()
		{
			if (!is_libmtp_initialized)
			{
				is_libmtp_initialized = true;
				LIBMTP_Init();
			}

			DetectDevice();
		}

		MTP(const MTP& mtp) = delete;

		MTP(MTP&& mtp)
			: raw_device_(mtp.raw_device_), number_of_devices_(mtp.number_of_devices_),
			devices_(std::move(mtp.devices_)), error_(mtp.error_) 
		{
			mtp.raw_device_ = nullptr;
		}

		MTP& operator=(const MTP& rhs) = delete;


		inline ~MTP()
		{
			LIBMTP_FreeMemory(raw_device_);
		}


		inline LIBMTP_error_number_t GetError() const
		{
			return error_;
		}

		inline int GetNumberOfDevices() const
		{
			return number_of_devices_;
		}

		inline std::vector<Device>& GetDevices()
		{
			return devices_;
		}


		bool DetectDevice()
		{
			if (raw_device_)
				LIBMTP_FreeMemory(raw_device_);

			number_of_devices_ = 0;
			devices_.clear();

			error_ = LIBMTP_Detect_Raw_Devices(&raw_device_, &number_of_devices_);
			if (!IsSuccess())
				return false;

			for (int i = 0; i < number_of_devices_; ++i)
				devices_.emplace_back(raw_device_ + i);

			return true;
		}


		inline bool IsSuccess() const
		{
			return (error_ == LIBMTP_error_number_t::LIBMTP_ERROR_NONE);
		}


		inline Device* GetDeviceByDeviceNumber(const int device_number)
		{
			if (device_number < 0 || number_of_devices_ < device_number)
				return nullptr;
			return &devices_[device_number];
		}

		inline Device* FindDeviceByFriendlyName(const std::string& name)
		{
			for (int i = 0; i < number_of_devices_; ++i)
			{
				if (devices_[i].GetFriendlyName().find(name) != std::string::npos)
					return &devices_[i];
			}
			return nullptr;
		}

		inline Device* FindDeviceByExactFriendlyName(const std::string& exact_name)
		{
			for (int i = 0; i < number_of_devices_; ++i)
			{
				if (devices_[i].GetFriendlyName() == exact_name)
					return &devices_[i];
			}
			return nullptr;
		}

		inline Device* FindDeviceByProductName(const std::string& name)
		{
			for (int i = 0; i < number_of_devices_; ++i)
			{
				if (devices_[i].GetProductName().find(name) != std::string::npos)
					return &devices_[i];
			}
			return nullptr;
		}

		inline Device* FindDeviceByExactProductName(const std::string& exact_name)
		{
			for (int i = 0; i < number_of_devices_; ++i)
			{
				if (devices_[i].GetProductName() == exact_name)
					return &devices_[i];
			}
			return nullptr;
		}

		inline Device* FindDeviceByVendorName(const std::string& name)
		{
			for (int i = 0; i < number_of_devices_; ++i)
			{
				if (devices_[i].GetVendorName().find(name) != std::string::npos)
					return &devices_[i];
			}
			return nullptr;
		}

		inline Device* FindDeviceByExactVendorName(const std::string& exact_name)
		{
			for (int i = 0; i < number_of_devices_; ++i)
			{
				if (devices_[i].GetVendorName() == exact_name)
					return &devices_[i];
			}
			return nullptr;
		}


		static inline void SetDebugLevel(int debug_level)
		{
			LIBMTP_Set_Debug(debug_level);
		}
	};

	bool MTP::is_libmtp_initialized = false;
}

#endif