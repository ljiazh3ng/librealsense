// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2015 Intel Corporation. All Rights Reserved.

#pragma once

#include "ds5-device.h"

namespace librealsense
{
    class ds5_motion : public virtual ds5_device
    {
    public:
        std::shared_ptr<hid_sensor> create_hid_device(const uvc::backend& backend,
                                                        const std::vector<uvc::hid_device_info>& all_hid_infos,
                                                        const firmware_version& camera_fw_version);

        ds5_motion(const uvc::backend& backend,
            const std::vector<uvc::uvc_device_info>& dev_info,
            const std::vector<uvc::usb_device_info>& hwm_device,
            const std::vector<uvc::hid_device_info>& hid_info);

        rs2_motion_device_intrinsic get_motion_intrinsics(rs2_stream) const;

        std::shared_ptr<auto_exposure_mechanism> register_auto_exposure_options(uvc_sensor* uvc_ep, const uvc::extension_unit* fisheye_xu);

    private:
        friend class ds5_fisheye_sensor;

        uint8_t _fisheye_device_idx = -1;
        uint8_t _motion_module_device_idx = -1;

        lazy<std::vector<uint8_t>> _fisheye_intrinsics_raw;
        lazy<std::vector<uint8_t>> _fisheye_extrinsics_raw;
        lazy<ds::extrinsics_table> _motion_module_extrinsics_raw;
        lazy<ds::imu_intrinsics> _accel_intrinsics;
        lazy<ds::imu_intrinsics> _gyro_intrinsics;

        std::vector<uint8_t> get_raw_fisheye_intrinsics_table() const;
        std::vector<uint8_t> get_raw_fisheye_extrinsics_table() const;
        ds::imu_calibration_table get_motion_module_calibration_table() const;

        pose get_device_position(unsigned int subdevice) const;

        // Bandwidth parameters from BOSCH BMI 055 spec'
        std::vector<std::pair<std::string, stream_profile>> sensor_name_and_hid_profiles =
            {{"gyro_3d",  {RS2_STREAM_GYRO,  1, 1, 200,  RS2_FORMAT_MOTION_RAW}},
             {"gyro_3d",  {RS2_STREAM_GYRO,  1, 1, 400,  RS2_FORMAT_MOTION_RAW}},
             {"gyro_3d",  {RS2_STREAM_GYRO,  1, 1, 1000, RS2_FORMAT_MOTION_RAW}},
             {"gyro_3d",  {RS2_STREAM_GYRO,  1, 1, 200,  RS2_FORMAT_MOTION_XYZ32F}},
             {"gyro_3d",  {RS2_STREAM_GYRO,  1, 1, 400,  RS2_FORMAT_MOTION_XYZ32F}},
             {"gyro_3d",  {RS2_STREAM_GYRO,  1, 1, 1000, RS2_FORMAT_MOTION_XYZ32F}},
             {"accel_3d", {RS2_STREAM_ACCEL, 1, 1, 125,  RS2_FORMAT_MOTION_RAW}},
             {"accel_3d", {RS2_STREAM_ACCEL, 1, 1, 250,  RS2_FORMAT_MOTION_RAW}},
             {"accel_3d", {RS2_STREAM_ACCEL, 1, 1, 500,  RS2_FORMAT_MOTION_RAW}},
             {"accel_3d", {RS2_STREAM_ACCEL, 1, 1, 1000, RS2_FORMAT_MOTION_RAW}},
             {"accel_3d", {RS2_STREAM_ACCEL, 1, 1, 125,  RS2_FORMAT_MOTION_XYZ32F}},
             {"accel_3d", {RS2_STREAM_ACCEL, 1, 1, 250,  RS2_FORMAT_MOTION_XYZ32F}},
             {"accel_3d", {RS2_STREAM_ACCEL, 1, 1, 500,  RS2_FORMAT_MOTION_XYZ32F}},
             {"accel_3d", {RS2_STREAM_ACCEL, 1, 1, 1000, RS2_FORMAT_MOTION_XYZ32F}},
             {"HID Sensor Class Device: Gyroscope",     {RS2_STREAM_GYRO,  1, 1, 1000, RS2_FORMAT_MOTION_XYZ32F}} ,
             {"HID Sensor Class Device: Accelerometer", {RS2_STREAM_ACCEL, 1, 1, 1000, RS2_FORMAT_MOTION_XYZ32F}},
             {"HID Sensor Class Device: Custom",        {RS2_STREAM_ACCEL, 1, 1, 1000, RS2_FORMAT_MOTION_XYZ32F}}};

        std::map<rs2_stream, std::map<unsigned, unsigned>> fps_and_sampling_frequency_per_rs2_stream =
                                                         {{RS2_STREAM_ACCEL, {{125,  1},
                                                                              {250,  2},
                                                                              {500,  5},
                                                                              {1000, 10}}},
                                                          {RS2_STREAM_GYRO,  {{200,  1},
                                                                              {400,  4},
                                                                              {1000, 10}}}};
    };
}
