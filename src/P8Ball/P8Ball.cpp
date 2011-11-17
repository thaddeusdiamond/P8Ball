/**
 * @file
 * @author		Thaddeus Diamond
 * @version		0.1
 *
 * @section DESCRIPTION
 * This is the implementation for our custom 3D game renderer
 */

#include "pch.h"
#include "P8Ball.h"

void P8Ball::Initialize() {
	// Device sensor platform initialization
	try {
		accelerometer_		= Windows::Devices::Sensors::Accelerometer::GetDefault();
		gyrometer_			= Windows::Devices::Sensors::Gyrometer::GetDefault();
	} catch (...) {
		DIE(1, "There was an error initializing the system devices");
	}
	
	// Device webcam initialization
	try {
		device_camera_		= ref new MediaCapture();
		device_camera_ready_to_record_ = false;
		device_camera_stream_ = ref new InMemoryRandomAccessStream();
	} catch (...) {
		DIE(1, "There was an error initializing the camera");
	}

	device_camera_initialization_ = device_camera_->InitializeAsync();
	device_camera_initialization_->Completed =
		// When the camera finishes loading we call an action handler (wierd async C++)
		ref new AsyncActionCompletedHandler([=](IAsyncAction^ a) {
			if (FAILED(a->ErrorCode.Value))
				DIE(a->ErrorCode.Value, "Failed to initialize the front-facing camera");
			
			// Start the actual video recording
			StartRecordOperation^ begin_recording = 
				device_camera_->StartRecordToStreamAsync(
					MediaEncodingProfile::CreateMp4(VideoEncodingQuality::HD720p),
					device_camera_stream_);
			begin_recording->Completed =
				ref new AsyncActionCompletedHandler([=](IAsyncAction^ a) {
					if (FAILED(a->ErrorCode.Value))
						DIE(a->ErrorCode.Value, "Failed to record video");
					device_camera_ready_to_record_ = true;
					begin_recording->Close();
				});
			begin_recording->Start();

			// Done with the initialization handle
			device_camera_initialization_->Close();
		});
	device_camera_initialization_->Start();

	// Initial rotation for the camera (front position)
	camera_rotation_[X_AXIS] = camera_rotation_[Y_AXIS] = camera_rotation_[Z_AXIS] = 0;
	number_of_updates_ = 0;

 	D3DRenderer::Initialize();
}

void P8Ball::Render() {
	Clear();

#pragma region SensorResponse
	if (accelerometer_ != nullptr && gyrometer_ != nullptr) {
		// Get information to determine our location in 3D
		Windows::Devices::Sensors::AccelerometerReading^ accel_reading =
			accelerometer_->GetCurrentReading();
		/** @todo Incorporate portrait versus landscape **/
		camera_rotation_[Z_AXIS] = ((accel_reading->AccelerationY > 0 ?
									  2 - accel_reading->AccelerationX :
									  accel_reading->AccelerationX) + 1) * 90.f;;
		camera_rotation_[X_AXIS] = ((accel_reading->AccelerationZ > 0 ?
									  2 - accel_reading->AccelerationY :
									  accel_reading->AccelerationY) + 1) * 90.f;

		// Inclinometer has an unreliable z-reading, let's use the gyroscope
		Windows::Devices::Sensors::GyrometerReading^ gyro_reading =
			gyrometer_->GetCurrentReading();
		
		// Alright, so the inclinometer's reading for yaw SUCKS and the accelerometer is not very helpful, so we
		// have to keep an up to date current rotation about the z-axis
		bool* gravitational_truths = GravitationalTruths(accel_reading);
		if (gravitational_truths[X_AXIS] && ABS(gyro_reading->AngularVelocityX) > GYROMETER_NOISE)
			camera_rotation_[Y_AXIS] += gyro_reading->AngularVelocityX * ((GetTickCount() - last_gyro_update_) / 1000.f) + 360.f;
		else if (gravitational_truths[Y_AXIS] && ABS(gyro_reading->AngularVelocityY) > GYROMETER_NOISE)
			camera_rotation_[Y_AXIS] += gyro_reading->AngularVelocityY * ((GetTickCount() - last_gyro_update_) / 1000.f) + 360.f;
		else if (gravitational_truths[Z_AXIS] && ABS(gyro_reading->AngularVelocityZ) > GYROMETER_NOISE)
			camera_rotation_[Y_AXIS] += gyro_reading->AngularVelocityZ * ((GetTickCount() - last_gyro_update_) / 1000.f) + 360.f;
		
		// Update where we are in the rotation and set the clock again
		camera_rotation_[Y_AXIS] -= static_cast<int>(camera_rotation_[Y_AXIS] / 360.f) * 360;
		last_gyro_update_ =  GetTickCount();
#pragma endregion

#pragma region EyeTracking
	if (device_camera_ready_to_record_) {
		char buffer[1000];
		WCHAR printable[1000];
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%d\n", device_camera_stream_->GetOutputStreamAt(0));
		MultiByteToWideChar(0, 0, buffer, 1000, printable, 1000);
		OutputDebugString(printable);
	}
#pragma endregion

#pragma region CameraPositioning
		// Make the app change color based on where along the circumference you are
		if (number_of_updates_++ % REFRESH_INTERVAL == 0) {
			double x_position = MAX(0, MIN(1, camera_rotation_[X_AXIS] / 360.f));
			double y_position = MAX(0, MIN(1, camera_rotation_[Y_AXIS] / 360.f));
			double z_position = MAX(0, MIN(1, camera_rotation_[Z_AXIS] / 360.f));

			/** @todo Move the camera here **/
			background_color_[0] = (ABS(x_position - background_color_[0]) < STABLE ?
									background_color_[0] :
									(x_position < background_color_[0] ?
										MAX(background_color_[0] - JARRING, ABS(x_position)) :
										MIN(background_color_[0] + JARRING, ABS(x_position))));
			background_color_[1] = (ABS(y_position - background_color_[1]) < STABLE ?
									background_color_[1] :
									(y_position < background_color_[1] ?
										MAX(background_color_[1] - JARRING, ABS(y_position)) :
										MIN(background_color_[1] + JARRING, ABS(y_position))));
			background_color_[2] = (ABS(z_position - background_color_[2]) < JARRING ?
									background_color_[2] :
									(z_position < background_color_[2] ?
										MAX(background_color_[2] - JARRING, ABS(z_position)) :
										MIN(background_color_[2] + JARRING, ABS(z_position))));
		}
#pragma endregion

		/** HOW TO DEBUG **		
		char buffer[1000];
		WCHAR printable[1000];
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%f, %f, %f\n", x_position, y_position, z_position);
		MultiByteToWideChar(0, 0, buffer, 1000, printable, 1000);
		OutputDebugString(printable); */
	}

	Present();
}