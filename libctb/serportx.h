#ifndef LIBCTB_SERPORTX_H_INCLUDED_
#define LIBCTB_SERPORTX_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Name:        serportx.h
// Purpose:
// Author:      Joachim Buermann
// Copyright:   (c) 2010 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include "iobase.h"

#if defined (WIN32)
# define snprintf _snprintf
#endif

/**
   \file serportx.h
*/

/*!
  \def SERIALPORT_NAME_LEN
  defines the maximum length of the os depending serial port names
*/
#define SERIALPORT_NAME_LEN 32

namespace ctb {

    /*! specifices the first serial port */
    extern const char* COM1;
    /*! specifies the second serial port */
    extern const char* COM2;
    /*! specifies the third serial port */
    extern const char* COM3;
    /*! specifies the fourth serial port */
    extern const char* COM4;
    /*! specifies the fifth serial port */
    extern const char* COM5;
    /*! specifies the sixth serial port */
    extern const char* COM6;
    /*! specifies the seventh serial port */
    extern const char* COM7;
    /*! specifies the eighth serial port */
    extern const char* COM8;
    /*! specifies the ninth serial port */
    extern const char* COM9;
    /*! specifies the tenth serial port */
    extern const char* COM10;
    /*! specifies the eleventh serial port */
    extern const char* COM11;
    /*! specifies the twelfth serial port */
    extern const char* COM12;
    /*! specifies the thriteenth serial port */
    extern const char* COM13;
    /*! specifies the fourteenth serial port */
    extern const char* COM14;
    /*! specifies the fiveteenth serial port */
    extern const char* COM15;
    /*! specifies the sixteenth serial port */
    extern const char* COM16;
    /*! specifies the seventeenth serial port */
    extern const char* COM17;
    /*! specifies the eighteenth serial port */
    extern const char* COM18;
    /*! specifies the nineteenth serial port */
    extern const char* COM19;
    /*! specifies the twentieth serial port */
    extern const char* COM20;

    /*!
	 \enum Parity

	 \brief Defines the different modes of parity checking. Under
	 Linux, the struct termios will be set to provide the wanted
	 behaviour.
    */
    enum Parity
    {
	   /*! no parity check */
	   ParityNone,
	   /*! odd parity check */
	   ParityOdd,
	   /*! even parity check */
	   ParityEven,
	   /*! mark (not implemented yet) */
	   ParityMark,
	   /*! space (not implemented yet) */
	   ParitySpace
    };

    /*!
	 \enum SerialLineState

	 Defines the different modem control lines. The value for
	 each item are defined in /usr/include/bits/ioctl-types.h.
	 This is the linux definition. The window version translate
	 each item in it's own value.
	 modem lines defined in ioctl-types.h
	 \code
	 #define TIOCM_LE	0x001
	 #define TIOCM_DTR	0x002
	 #define TIOCM_RTS	0x004
	 #define TIOCM_ST	0x008
	 #define TIOCM_SR	0x010
	 #define TIOCM_CTS	0x020
	 #define TIOCM_CAR	0x040
	 #define TIOCM_RNG	0x080
	 #define TIOCM_DSR	0x100
	 #define TIOCM_CD	TIOCM_CAR
	 #define TIOCM_RI	TIOCM_RNG
	 \endcode
    */
    enum SerialLineState
    {
	   /*! Data Carrier Detect (read only) */
	   LinestateDcd = 0x040,
	   /*! Clear To Send (read only) */
	   LinestateCts = 0x020,
	   /*! Data Set Ready (read only) */
	   LinestateDsr = 0x100,
	   /*! Data Terminal Ready (write only) */
	   LinestateDtr = 0x002,
	   /*! Ring Detect (read only) */
	   LinestateRing = 0x080,
	   /*! Request To Send (write only) */
	   LinestateRts = 0x004,
	   /*! no active line state, use this for clear */
	   LinestateNull = 0x000
    };

    /*!
	 \struct SerialPort_DCS

	 The device control struct for the serial communication class.
	 This struct should be used, if you refer advanced parameter.
    */
    struct SerialPort_DCS
    {
	   /*! the baudrate */
	   int baud;
	   /*! the parity */
	   Parity parity;
	   /*! the wordlen */
	   unsigned char wordlen;
	   /*! count of stopbits */
	   unsigned char stopbits;
	   /*! rtscts flow control */
	   bool rtscts;
	   /*! XON/XOFF flow control */
	   bool xonxoff;
	   /*! buffer for internal use */
	   char buf[16];
	   SerialPort_DCS() {
		  baud = 38400;
		  parity = ParityNone;
		  wordlen = 8;
		  stopbits = 1;
		  rtscts = false;
		  xonxoff = false;
	   };
	   // to avoid memory leak warnings generated by swig 
	   ~SerialPort_DCS() {};
	   /*!
		\brief returns the internal settings of the DCS as a human
		readable string like '8N1 115200'.
		\return the internal settings as null terminated string
	   */
	   char* GetSettings() {
		  const char ac[5] = {'N','O','E','M','S'};
		  memset(buf,0,sizeof(buf));
		  snprintf(buf,sizeof(buf)-1,"%i%c%i %i",
				 wordlen,
				 ac[parity],
				 stopbits,
				 baud);
		  return buf;
	   };
    }; 

/*!
  \struct SerialPort_EINFO

  The internal communication error struct. It contains the number
  of each error (break, framing, overrun and parity) since opening
  the serial port. Each error number will be cleared if the open
  methode was called.
*/
    struct SerialPort_EINFO
    {
	   /*! number of breaks */
	   int brk;
	   /*! number of framing errors */
	   int frame;
	   /*! number of overrun errors */
	   int overrun;
	   /*! number of parity errors */
	   int parity;
	   SerialPort_EINFO() {
		  brk = frame = overrun = parity = 0;
	   };
    };

    /*!
	 \enum SerialPortIoctls

	 The following Ioctl calls are only valid for the SerialPort
	 class.
    */
    enum SerialPortIoctls {
	   /*!
		Get all numbers of occured communication errors (breaks
		framing, overrun and parity), so the args parameter of
		the Ioctl call must pointed to a SerialPort_EINFO
		struct.
	   */
	   CTB_SER_GETEINFO = CTB_SERIAL,
	   /*!
		Get integer 1, if a break occured since the last call
		so the args parameter of the Ioctl methode must pointed
		to an integer value. If there was no break, the result
		is integer 0.
	   */
	   CTB_SER_GETBRK,
	   /*!
		Get integer 1, if a framing occured since the last call
		so the args parameter of the Ioctl methode must pointed
		to an integer value. If there was no break, the result
		is integer 0.
	   */
	   CTB_SER_GETFRM,
	   /*!
		Get integer 1, if a overrun occured since the last call
		so the args parameter of the Ioctl methode must pointed
		to an integer value. If there was no break, the result
		is integer 0.
	   */
	   CTB_SER_GETOVR,
	   /*!
		Get integer 1, if a parity occured since the last call
		so the args parameter of the Ioctl methode must pointed
		to an integer value. If there was no break, the result
		is integer 0.
	   */
	   CTB_SER_GETPAR,
	   /*!
		Get the number of bytes received by the serial port driver
		but not yet read by a Read or Readv Operation.
	   */
	   CTB_SER_GETINQUE,
	   /*!
		Set the parity bit on or off to use it as a ninth bit.
	   */
	   CTB_SER_SETPAR
    };

/*!
  \class SerialPort_x
  SerialPort_x is the basic class for serial communication via
  the serial comports. It is also an abstract class and defines
  all necessary methods, which the derivated plattform depended
  classes must be invoke.
*/
    class SerialPort_x : public IOBase
    {
    protected:
	   /*!
		\brief contains the internal settings of the serial port like
		baudrate, protocol, wordlen and so on.
	   */
	   SerialPort_DCS m_dcs;
	   /*!
		\brief contains the internal (os specific) name of the serial
		device.
	   */
	   char m_devname[SERIALPORT_NAME_LEN];
    public:

	   /*!
		\enum FlowControl
		\brief Specifies the flow control.

		
	   */
	   enum FlowControl
	   {
		  NoFlowControl, /*!< No flow control at all */

		  RtsCtsFlowControl, /*!< Enable RTS/CTS hardware flow control */

		  XonXoffFlowControl /*!< Enable XON/XOFF protocol */
	   };

	   SerialPort_x() {m_devname[0] = '\0';};
	   virtual ~SerialPort_x() {};
	   /*!
		\brief returns the name of the class instance. You find this useful,
		if you handle different devices like a serial port or a gpib device
		via a IOBase pointer.
		\return name of the class.
	   */
	   const char* ClassName() {return "ctb::SerialPort";};
    
	   /*!
		\brief change the linestates according to which bits
		are set/unset in flags.
		\param flags valid line flags are SERIAL_LINESTATE_DSR and/or
		SERIAL_LINESTATE_RTS
		\return zero on success, -1 if an error occurs
	   */
	   virtual int ChangeLineState( SerialLineState flags ) = 0;

	   /*! 
		\brief turn off status lines depending upon which bits (DSR and/or RTS)
		are set in flags.
		\param flags valid line flags are SERIAL_LINESTATE_DSR and/or
		SERIAL_LINESTATE_RTS
		\return zero on success, -1 if an error occurs
	   */
	   virtual int ClrLineState( SerialLineState flags ) = 0;

	   /*!
		\brief Read the line states of DCD, CTS, DSR and RING
		\return returns the appropriate bits on sucess, otherwise -1
	   */
	   virtual int GetLineState() = 0;

	   /*!
		\brief request the current settings of the connected serial port
		as a null terminated string.
		\return the settings as a string like '8N1 115200'
	   */
	   virtual char* GetSettingsAsString() {
		  return m_dcs.GetSettings();
	   };

	   /*!
		\brief Many operating characteristics are only possible for
		special devices. To avoid the need of a lot of different functions
		and to give the user a uniform interface, all this special
		operating instructions will covered by one Ioctl methode (like
		the linux ioctl call).
		The Ioctl command (cmd) has encoded in it whether the argument 
		is an in parameter or out parameter, and the size of the
		argument args in bytes. Macros and defines used in specifying an
		ioctl request are located in iobase.h and the header file for
		the derivated device (for example in serportx.h).
		\param cmd one of SerialPortIoctls specify the ioctl request.
		\param args is a typeless pointer to a memory location, where
		Ioctl reads the request arguments or write the results.
		Please note, that an invalid memory location or size involving
		a buffer overflow or segmention fault!
	   */
	   virtual int Ioctl(int cmd,void* args) {return -1;};

	   /*!
		\brief Opens a serial port in a user likely way. Insteed of
		using the Device Control Struct just input your parameter in a
		more intuitive manner.
		\param portname the name of the serial port
		\param baudrate any baudrate, also an unusual one, if your
		serial device support them
		\param protocol a string with the number of databits (5...8),
		the parity setting (N=None,O=Odd,E=Even,M=Mark,S=Space), also
		in lower case, and the count of stopbits (1...2)
		\param flowControl one of NoFlowControl, RtsCtsFlowControl or
		XonXoffFlowControl.
		\return the new file descriptor, or -1 if an error occurred
	    */
	   int Open( const char* portname, int baudrate,
			   const char* protocol = "8N1",
			   FlowControl flowControl = NoFlowControl );

	   /*!
		\brief Opens the serial port with the given number.
		\note The port numbering starts with 1 (COM1 for windows and
		/dev/ttyS0 for Linux. Please note, that USB to RS232 converter
		in Linux are named as /dev/ttyUSBx and from there have to
		opened with their device name!
		\param number of the serial port count from 1
		\param baudrate any baudrate, also an unusual one, if your
		serial device support them
		\param protocol a string with the number of databits (5...8),
		the parity setting (N=None,O=Odd,E=Even,M=Mark,S=Space), also
		in lower case, and the count of stopbits (1...2)
		\param flowControl one of NoFlowControl, RtsCtsFlowControl or
		XonXoffFlowControl.
		\return the new file descriptor, or -1 if an error occurred
	   */
	   int Open( const int portnumber, int baudrate,
			   const char* protocol = "8N1",
			   FlowControl flowControl = NoFlowControl );

	   // Open overrides and hides the IOBase::Open( const char*
	   // devname, void* dcs ) of the base class! So bring it into scope 
	   // again!
	   using IOBase::Open;

	   /*!
		\brief Sendbreak transmits a continuous stream of zero-valued
		bits for a specific duration.
		\param duration If duration is zero, it transmits 
		zero-valued bits for at least 0.25 seconds, and  not more that 
		0.5 seconds. If duration is not zero, it sends zero-valued bits
		for  duration*N  seconds, where N is at least 0.25, and not more
		than 0.5.
		\return zero on success, -1 if an error occurs.
	   */
	   virtual int SendBreak(int duration) = 0;

	   /*!
		\brief Set the baudrate (also non-standard)
		Please note: Non-standard baudrates like 70000 are not supported
		by each UART and depends on the RS232 chipset you apply.
		\param baudrate the new baudrate
		\return zero on success, -1 if an error occurs
	   */
	   virtual int SetBaudrate( int baudrate ) = 0;

	   /*! 
		\brief turn on status lines depending upon which bits (DSR and/or RTS)
		are set in flags.
		\param flags valid line flags are SERIAL_LINESTATE_DSR and/or
		SERIAL_LINESTATE_RTS
		\return zero on success, -1 if an error occurs
	   */
	   virtual int SetLineState( SerialLineState flags ) = 0;

	   /*!
		\brief Set the parity bit to a firm state, for instance to use
		the parity bit as the ninth bit in a 9 bit dataword
		communication.
		\return zero on succes, a negative value if an error occurs
	    */
	   virtual int SetParityBit( bool parity ) = 0;

	   /*!
		\brief check the given baudrate against a list of standard rates.
		\ return true, if the baudrate is a standard value, false
		otherwise
	    */
	   static bool IsStandardRate( int rate );

    };

} // namespace ctb

#endif
