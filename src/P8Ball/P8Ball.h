/**
 * @file
 * @author		Thaddeus Diamond
 * @version		0.1
 *
 * @section DESCRIPTION
 * This is a custom renderer for our 3D landscape to do cool things with devices
 */

#include "D3DRenderer.h"

#ifndef _P8BALL_P8BALL_P8BALL_H_
#define _P8BALL_P8BALL_P8BALL_H_

using Windows::Media::Capture::MediaEncodingProfile;
using Windows::Media::Capture::MediaCapture;
using Windows::Media::Capture::InitializeOperation;
using Windows::Media::Capture::VideoEncodingQuality;
using Windows::Media::Capture::StartRecordOperation;
using Windows::Foundation::AsyncActionCompletedHandler;
using Windows::Foundation::IAsyncAction;
using Windows::Storage::Streams::InMemoryRandomAccessStream;

/**
 * We enumerate the game states for fast switching
 */
enum GameState {
	InitialScreen,

	ActiveInGame,
};

/**
 * The P8Ball class is the main class we use for interpreting game events and rendering
 * the screen
 *
 * @class P8Ball
 */
ref class P8Ball : public D3DRenderer {
public:
	/**
	 * The constructor for our application is only responsible for initializing the game
	 * state to the initial state.
	 */
	P8Ball(Windows::UI::Core::CoreWindow^ window) :
	  D3DRenderer(window), game_state_(InitialScreen) {}
	~P8Ball() {}

	/**
	 * The main portion of the initialization function that we care about is the device
	 * initialization.  This allows us to get things like accelerometer, gyrometer, etc.
	 * from the system
	 */
	virtual void Initialize();

	/**
	 * The render function does exactly three things.  First, it clears the screen's current
	 * buffer.  Then, it draws what it needs to.  Finally, it presents what it drew,
	 * with a back buffer, to the screen
	 */
	virtual void Render();

protected:
	/** We create an enumerated representation for our current game state **/
	GameState game_state_;

	/** Keep track of when we update the frame for fine-grained control over our UI **/
	unsigned int number_of_updates_;
	unsigned int last_gyro_update_;
	double camera_rotation_[3];

	/** We represent all the devices we use from the system as member vars **/
	Windows::Devices::Sensors::Accelerometer^		accelerometer_;
	Windows::Devices::Sensors::Gyrometer^			gyrometer_;
	Windows::Devices::Sensors::Compass^				compass_;

	/** Capture the video from the built-in camera for face tracking **/
	MediaCapture^				device_camera_;
	InitializeOperation^		device_camera_initialization_;
	InMemoryRandomAccessStream^	device_camera_stream_;
	bool						device_camera_ready_to_record_;
};

#endif  // _P8BALL_P8BALL_P8BALL_H_