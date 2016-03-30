#include "example-client/ExampleClient.h"

ExampleClient::ExampleClient(std::shared_ptr<ocra::Model> modelPtr, const int loopPeriod)
: ocra_recipes::ControllerClient(modelPtr, loopPeriod)
{
    // poopoo
}

ExampleClient::~ExampleClient()
{
    //caca
}

bool ExampleClient::initialize()
{
    startTime = yarp::os::Time::now();
    trigger = true;

    std::vector<std::string> portNames = clientComs->getTaskPortNames();

    std::cout << "portNames[4]: " << portNames[4] << std::endl;

    ocra_recipes::TRAJECTORY_TYPE trajType = ocra_recipes::MIN_JERK;

    waypoints.resize(3,1);
    waypoints <<    0.2,
                    -0.06,
                    0.55;

    ocra_recipes::TERMINATION_STRATEGY termStrategy = ocra_recipes::WAIT_DEACTIVATE;

    leftHandTrajThread = std::make_shared<ocra_recipes::TrajectoryThread>(10, portNames[4], waypoints, trajType, termStrategy);

    // leftHandTrajThread->setDisplacement(0.2);
    leftHandTrajThread->setGoalErrorThreshold(0.03);


    bool done=false;

    p1 = true;
    p2 = true;
    p3 = true;

    std::cout << "Thread started." << std::endl;
    return true;
}

void ExampleClient::release()
{
    /* Do nothing. */
}

void ExampleClient::loop()
{
    // std::cout << "Left hand position (x,y,z):";
    // std::cout << model->getSegmentPosition(model->getSegmentIndex("l_hand")).getTranslation().transpose() << std::endl;

    // if ((yarp::os::Time::now() - startTime) > 5.0 && trigger)
    // {
        //     trigger = false;
        //     yarp::os::Bottle request, reply;
        //     request.addInt(ocra_icub::OCRA_CONTROLLER_MESSAGE::REMOVE_TASK);
        //     request.addString("rightHandCartesian");
        //     if(clientComs->queryController(request).get(0).asInt() == ocra_icub::OCRA_CONTROLLER_MESSAGE::OCRA_SUCCESS){
        //         std::cout << "Yay task was removed!" << std::endl;
        //     }else{
        //         std::cout << "Poooo it didn't work..." << std::endl;
        //     }
        // }
    // }

        if((yarp::os::Time::now() - startTime) > 5.0 && !done)
        {
            if(trigger){
                leftHandTrajThread->start();
                trigger = false;
            }

            if ((yarp::os::Time::now()-startTime)>10.0){
                if(p1){
                    p1=false;
                    std::cout << "Changing to BACK_AND_FORTH mode:" << std::endl;
                    leftHandTrajThread->setTerminationStrategy(ocra_recipes::BACK_AND_FORTH);
                }
                if ((yarp::os::Time::now()-startTime)>20.0){
                    if(p2){
                        p2=false;
                        std::cout << "Changing to STOP_THREAD mode:" << std::endl;
                        leftHandTrajThread->setTerminationStrategy(ocra_recipes::STOP_THREAD);
                    }
                    if ((yarp::os::Time::now()-startTime)>30.0){
                        if(p3){
                            p3=false;
                            std::cout << "Finished while loop!" << std::endl;
                            std::cout << "Stopping thread..." << std::endl;
                            leftHandTrajThread->stop();
                            done=true;
                        }
                    }
                }
            }
        }
    // }


}
