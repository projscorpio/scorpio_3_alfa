#ifndef __JOYSTICK_HH__
#define __JOYSTICK_HH__

#include <string>
#include <iostream>

#define JS_EVENT_BUTTON 0x01 // button pressed/released
#define JS_EVENT_AXIS   0x02 // joystick moved
#define JS_EVENT_INIT   0x80 // initial state of device

//Communication module - sending protocol
class CommunicationModule
{
private:
  bool BUTTON[8];
  short AXIS[4];

public:
  CommunicationModule()
	{
	for(int i = 0; i < 4; i++) 
		{
			BUTTON[i] = false;
			AXIS[i] = 0;
		}
	}
  ~CommunicationModule();
	void buttonSetting(int, bool);
	void axisSetting(int, int);
	std::string coding(void);
};


// Encapsulates all data relevant to a sampled joystick event.
//------------Joystick Event Class-------------
class JoystickEvent
{
//--------------Public variables---------------
public:
  // Minimum value of axes range - analog input
  static const short MIN_AXES_VALUE = -32768;

  // Maximum value of axes range - analog input
  static const short MAX_AXES_VALUE = 32767;

  // The timestamp of the event, in milliseconds.
  unsigned int time;

  //Real value of the event - button up(0) down(1)
  //For analog inputs value between MIN_AXES_VALUE and MAX_AXES_VALUE
  short value;

  //Event type
  unsigned char type;

  //The axis/button number
  unsigned char number;

  //Returns true if this event is the result of a button press
  bool isButton()
  {
    return (type & JS_EVENT_BUTTON) != 0;
  }

  //Returns true if this event is the result of an axis movement
  bool isAxis()
  {
    return (type & JS_EVENT_AXIS) != 0;
  }

  //Returns true if this event is part of the initial state obtained when
  //the joystick is first connected to
  bool isInitialState()
  {
    return (type & JS_EVENT_INIT) != 0;
  }

  //The ostream inserter needs to be a friend so it can access the
  //internal data structures.
  friend std::ostream& operator<<(std::ostream& os, const JoystickEvent& e);
};

//Stream insertion
std::ostream& operator<<(std::ostream& os, const JoystickEvent& e);

//Represents a joystick device. Allows data to be sampled from it
class Joystick
{
private:
  void openPath(std::string devicePath, bool blocking=false);
  int _fd;

public:
  ~Joystick();

  //Initialises an instance for the first joystick: /dev/input/js0
  Joystick();

  //Initialises an instance for the joystick with the specified,
  //zero-indexed number
  Joystick(int joystickNumber);

  //Initialises an instance for the joystick device specified
  Joystick(std::string devicePath);

  //Joystick objects cannot be copied
  Joystick(Joystick const&) = delete;

  //Joystick objects can be moved
  Joystick(Joystick &&) = default;

  //Initialises an instance for the joystick device specified and provide
  //the option of blocking I/O.
  Joystick(std::string devicePath, bool blocking);

  //Returns true if the joystick was found and may be used, otherwise false
  bool isFound();

  //Attempts to populate the provided JoystickEvent instance with data
  //from the joystick. Returns true if data is available, otherwise false
  bool sample(JoystickEvent* event);
};

#endif
