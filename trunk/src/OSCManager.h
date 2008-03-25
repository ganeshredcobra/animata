/*
 Animata

 Copyright (C) 2007 Peter Nemeth, Gabor Papp, Bence Samu
 Kitchen Budapest, <http://animata.kibu.hu/>

 This file is part of Animata.

 Animata is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Animata is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Animata. If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef __OSCMANAGER_H__
#define __OSCMANAGER_H__

#include <pthread.h>
#include <vector>

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"

#include "Layer.h"

#define OSC_RECEIVE_PORT 7110

/// Handles OSC messages.
class OSCListener : public osc::OscPacketListener
{
	protected:

		/// Processes OSC messages.
		virtual void ProcessMessage(const osc::ReceivedMessage& m,
				const IpEndpointName& remoteEndpoint);
	private:

		/// Helper function to call class method threadTask() from a thread.
		static void *threadFunc(void *p);

		/// Threads function running an OSC listener forever.
		void threadTask(void);

		pthread_t thread;
		pthread_mutex_t mutex;
		UdpListeningReceiveSocket *ulrs;

		Layer *rootLayer;	///< root of the layers

		int patternMatch(const char *str, const char *p);

	public:

		OSCListener();
		~OSCListener();

		/// Starts OSC listener in a new thread.
		void start(void);
		/// Stops OSC listener.
		void stop(void);

		/// Protects access to a shared data resources.
		void lock(void);
		/// Unlocks mutex, allowing access to shared data.
		void unlock(void);

		/// Sets root layers to be able to control the behaviour of layer data.
		void setRootLayer(Layer *root);

};
#endif

