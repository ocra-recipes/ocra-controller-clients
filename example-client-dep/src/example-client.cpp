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

#include <ocra-yarp/OcraControllerClientModule.h>
#include <ocra-yarp/OcraControllerClientThread.h>
#include "example-client/ExampleClientThread.h"


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

    int threadPeriod = 10;
    ocra_yarp::OcraControllerClientThread::shared_ptr threadPtr = std::make_shared<ExampleClientThread>(threadPeriod);
    ocra_yarp::OcraControllerClientModule module(threadPtr);


    yarp::os::ResourceFinder rf;
    module.configure(rf);
    // rf.setVerbose(true);
    // rf.setDefaultConfigFile("example-client.ini"); //default config file name.
    // rf.setDefaultContext("example-client"); //when no parameters are given to the module this is the default context
    // rf.configure(argc,argv);
    //
    // if (rf.check("help"))
    // {
    //     module.printHelp();
    //     return 0;
    // }


    return module.runModule(rf);
    // return 0;
}



//
// int main(int argc, char *argv[])
// {
//     yarp::os::Network yarp;
//
//     if (!yarp.checkNetwork())
//     {
//         std::cout << "No yarp network, quitting\n";
//         return 1;
//     }
//
//     ocra_yarp::ControllerConnection ctlCon;
//
//     std::vector<std::string> portNames = ctlCon.getTaskPortNames();
//
//     std::cout << "portNames[4]: " << portNames[4] << std::endl;
//
//     ocra_yarp::TRAJECTORY_TYPE trajType = ocra_yarp::GAUSSIAN_PROCESS;
//
//     Eigen::MatrixXd waypoints(3,1);
//     waypoints <<    0.1,
//                     0.1,
//                     0.6;
//
//     ocra_yarp::TERMINATION_STRATEGY termStrategy = ocra_yarp::WAIT_DEACTIVATE;
//
//     ocra_yarp::TrajectoryThread leftHandTrajThread(10, portNames[4], waypoints, trajType, termStrategy);
//
//     leftHandTrajThread.setGoalErrorThreshold(0.045);
//
//     std::cout << "Thread started." << std::endl;
//     leftHandTrajThread.start();
//
//     bool done=false;
//     double startTime=yarp::os::Time::now();
//
//     std::cout << "In the while loop..." << std::endl;
//
//     bool p1, p2, p3;
//     p1 = true;
//     p2 = true;
//     p3 = true;
//     while(!done)
//     {
//
//         if ((yarp::os::Time::now()-startTime)>10.0){
//             if(p1){std::cout << "Changing to BACK_AND_FORTH mode:" << std::endl; p1=false;}
//             leftHandTrajThread.setTerminationStrategy(ocra_yarp::BACK_AND_FORTH);
//             if ((yarp::os::Time::now()-startTime)>20.0){
//                 if(p2){std::cout << "Changing to STOP_THREAD mode:" << std::endl; p2=false;}
//                 leftHandTrajThread.setTerminationStrategy(ocra_yarp::STOP_THREAD);
//                 if ((yarp::os::Time::now()-startTime)>30.0){
//                     if(p3){std::cout << "Finished while loop!" << std::endl; p3=false;}
//                     done=true;
//                 }
//             }
//         }
//     }
//
//     std::cout << "Stopping thread..." << std::endl;
//     leftHandTrajThread.stop();
//
//     std::cout << "Module finished." << std::endl;
//     return 0;
//
//
// }
