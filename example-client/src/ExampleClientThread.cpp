#include "example-client/ExampleClient.h"

ExampleClient::ExampleClient(std::shared_ptr<ocra_icub::OcraWbiModel> modelPtr, const int loopPeriod)
: ocra_icub::IcubControllerClient(modelPtr, loopPeriod)
{
    // poopoo
}

ExampleClient::~ExampleClient()
{
    //caca
}

bool ExampleClient::initialize()
{
    return true;
}

void ExampleClient::release()
{
    /* Do nothing. */
}

void ExampleClient::loop()
{
    std::cout << "I'm the ExampleClient!" << std::endl;
}
//
// bool ExampleClient::client_threadInit()
// {
//     // return startModelThread();
//     startTime = yarp::os::Time::now();
//     trigger = true;
//
//     std::vector<std::string> portNames = ctrlCon->getTaskPortNames();
//
//     std::cout << "portNames[4]: " << portNames[4] << std::endl;
//
//     ocra_yarp::TRAJECTORY_TYPE trajType = ocra_yarp::MIN_JERK;
//
//     waypoints.resize(3,1);
//     waypoints <<    0.25,
//                     -0.1,
//                     0.5;
//
//     ocra_yarp::TERMINATION_STRATEGY termStrategy = ocra_yarp::WAIT_DEACTIVATE;
//
//     leftHandTrajThread = std::make_shared<ocra_yarp::TrajectoryThread>(10, portNames[4], waypoints, trajType, termStrategy);
//
//     // leftHandTrajThread->setDisplacement(0.2);
//     leftHandTrajThread->setGoalErrorThreshold(0.045);
//
//
//     bool done=false;
//
//     p1 = true;
//     p2 = true;
//     p3 = true;
//
//     std::cout << "Thread started." << std::endl;
//
//     return true;
// }
// void ExampleClient::client_threadRelease()
// {
//     // stopModelThread();
// }
// void ExampleClient::client_run()
// {
//     // if ((yarp::os::Time::now() - startTime) > 5.0 && trigger) {
//     //     trigger = false;
//     //     yarp::os::Bottle request, reply;
//     //     request.addInt(ocra_yarp::OCRA_CONTROLLER_MESSAGE::REMOVE_TASK);
//     //     request.addString("rightHandCartesian");
//     //     if(ctrlCon->queryController(request).get(0).asInt() == ocra_yarp::OCRA_CONTROLLER_MESSAGE::OCRA_SUCCESS){
//     //         std::cout << "Yay task was removed!" << std::endl;
//     //     }else{
//     //         std::cout << "Poooo it didn't work..." << std::endl;
//     //     }
//     // }
//
//
//         while((yarp::os::Time::now() - startTime) > 5.0 && !done)
//         {
//             if(trigger){
//                 leftHandTrajThread->start();
//                 trigger = false;
//             }
//
//             if ((yarp::os::Time::now()-startTime)>10.0){
//                 if(p1){
//                     p1=false;
//                     std::cout << "Changing to BACK_AND_FORTH mode:" << std::endl;
//                     leftHandTrajThread->setTerminationStrategy(ocra_yarp::BACK_AND_FORTH);
//                 }
//                 if ((yarp::os::Time::now()-startTime)>20.0){
//                     if(p2){
//                         p2=false;
//                         std::cout << "Changing to STOP_THREAD mode:" << std::endl;
//                         leftHandTrajThread->setTerminationStrategy(ocra_yarp::STOP_THREAD);
//                     }
//                     if ((yarp::os::Time::now()-startTime)>30.0){
//                         if(p3){
//                             p3=false;
//                             std::cout << "Finished while loop!" << std::endl;
//                             std::cout << "Stopping thread..." << std::endl;
//                             leftHandTrajThread->stop();
//                             done=true;
//                         }
//                     }
//                 }
//             }
//         }
//
//
//
//  // Do nothing.
// }
