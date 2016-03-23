/*! \file       example-client.cpp
 *  \brief
 *  \details
 *  \author     [Ryan Lober](http://www.ryanlober.com)
 *  \author     [Antoine Hoarau](http://ahoarau.github.io)
 *  \date       Feb 2016
 *  \copyright  GNU General Public License.
 */
/*
 *  This file is part of ocra-yarp.
 *  Copyright (C) 2016 Institut des Systèmes Intelligents et de Robotique (ISIR)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <yarp/os/ResourceFinder.h>
#include <yarp/os/Network.h>
#include <yarp/os/Log.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Time.h>

#include "example-client/ExampleClient.h"
#include <ocra-icub/IcubClient.h>


int main (int argc, char * argv[])
{
    yarp::os::Log yLog;
    yarp::os::Network yarp;

    double network_timeout = 10.0;
    if (!yarp.checkNetwork(network_timeout))
    {
        yLog.fatal() << "YARP network is not available";
        return -1;
    }

    std::cout << "Making model initializer" << std::endl;
    ocra_icub::ModelInitializer modelIni = ocra_icub::ModelInitializer();

    int loopPeriod = 10;

    std::shared_ptr<ocra_icub::IcubControllerClient> ctrlClient;
    std::cout << "Making controller client" << std::endl;
    ctrlClient = std::make_shared<ExampleClient>(modelIni.getModel(), loopPeriod);

    std::shared_ptr<ocra_icub::IcubControllerClientManager> clientManager;
    std::cout << "Making client manager" << std::endl;
    clientManager = std::make_shared<ocra_icub::IcubControllerClientManager>(ctrlClient);

    std::cout << "Resource finder stuff" << std::endl;
    yarp::os::ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultConfigFile("example-client.ini"); //default config file name.
    rf.setDefaultContext("example-client"); //when no parameters are given to the module this is the default context
    rf.configure(argc,argv);

    if (rf.check("help"))
    {
        clientManager->printHelp();
        return 0;
    }

    std::cout << "Configuring" << std::endl;
    clientManager->configure(rf);

    std::cout << "Launching client" << std::endl;
    return clientManager->launchClient();

    // return clientManager->runModule(rf);
}
