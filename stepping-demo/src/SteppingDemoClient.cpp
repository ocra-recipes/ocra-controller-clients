#include "stepping-demo/SteppingDemoClient.h"

SteppingDemoClient::SteppingDemoClient(std::shared_ptr<ocra::Model> modelPtr, const int loopPeriod)
: ocra_recipes::ControllerClient(modelPtr, loopPeriod)
{
    // poopoo
}

SteppingDemoClient::~SteppingDemoClient()
{
    //caca
}

bool SteppingDemoClient::initialize()
{
    std::string leftFootPortName = clientComs->getTaskPortName("leftFootCartesian");
    std::string rightFootPortName = clientComs->getTaskPortName("rightFootCartesian");
    std::string comPortName = clientComs->getTaskPortName("comTask");

    std::cout << "leftFootPortName: " << leftFootPortName << std::endl;
    std::cout << "rightFootPortName: " << rightFootPortName << std::endl;
    std::cout << "comPortName: " << comPortName << std::endl;

    ocra_icub::TRAJECTORY_TYPE trajType = ocra_icub::MIN_JERK;
    ocra_icub::TERMINATION_STRATEGY termStrategy = ocra_icub::WAIT;
    int trajThreadPeriod = 10;


    leftFoot_TrajThread = std::make_shared<ocra_icub::TrajectoryThread>(trajThreadPeriod, leftFootPortName, trajType, termStrategy);

    rightFoot_TrajThread = std::make_shared<ocra_icub::TrajectoryThread>(trajThreadPeriod, rightFootPortName, trajType, termStrategy);

    com_TrajThread = std::make_shared<ocra_icub::TrajectoryThread>(trajThreadPeriod, comPortName, trajType, termStrategy);


    com_TrajThread->setMaxVelocity(0.01);
    com_TrajThread->setGoalErrorThreshold(0.01);

    currentPhase = MOVE_TO_LEFT_SUPPORT;
    isMovingCoM = false;
    com_TrajThread->start();
    // leftFoot_TrajThread->start();
    rightFoot_TrajThread->start();
    std::cout << "Thread started." << std::endl;

    getInitialValues = true;
    startTime = yarp::os::Time::now();

    leftFootRpcClient = clientComs->getTaskClient("contactSetLeftFoot");
    rightFootRpcClient = clientComs->getTaskClient("contactSetRightFoot");

    isInLeftSupportMode = true;
    isInRightSupportMode = true;

    return true;
}

void SteppingDemoClient::release()
{
    com_TrajThread->stop();
    rightFoot_TrajThread->stop();
}

void SteppingDemoClient::loop()
{

    currentTime = yarp::os::Time::now();
    // Wait for 2 seconds then start moving. Has something to do with the model states not being correctly updated during the init phase.
    if( (currentTime - startTime) > 2.0 )
    {

        // set intial constants
        if(getInitialValues)
        {
            leftFootHome = getLeftFootPosition();
            rightFootHome = getRightFootPosition();
            comHome = getCoMPosition();

            leftFootTarget = leftFootHome + Eigen::Vector3d(0.1, 0.1, 0.1);
            rightFootTarget = rightFootHome + Eigen::Vector3d(0.1, 0.01, 0.05);

            std::cout << " leftFootHome: " << leftFootHome.transpose() << std::endl;
            std::cout << " rightFootHome: " << rightFootHome.transpose() << std::endl;
            std::cout << " comHome: " << comHome.transpose() << std::endl;
            std::cout << " leftFootTarget: " << leftFootTarget.transpose() << std::endl;
            std::cout << " rightFootTarget: " << rightFootTarget.transpose() << std::endl;

            getInitialValues = false;
        }

        // checkFeetContacts();

        if(isBalanced() && pauseFinished())
        {
            // std::cout << "Hey I am balanced!" << std::endl;
            switch (currentPhase)
            {
                case MOVE_TO_LEFT_SUPPORT:
                {
                    if(!isMovingCoM) {
                        std::cout << "Moving CoM over left foot." << std::endl;
                        positionCoMOver(LEFT_FOOT_XY);
                        isMovingCoM = true;
                        pauseFor(5.0);
                    }
                    else {
                        std::cout << "Setting right foot trajectory." << std::endl;
                        rightFoot_TrajThread->setTrajectoryWaypoints(rightFootTarget);
                        yarp::os::Time::delay(0.5);
                        deactivateFootContacts(RIGHT_FOOT);
                        pauseFor(5.0);
                        isInLeftSupportMode = true;
                        isInRightSupportMode = false;
                        currentPhase = MOVE_TO_DOUBLE_SUPPORT;
                        isMovingCoM = false;
                    }

                }break;

                case MOVE_TO_RIGHT_SUPPORT:
                {

                }break;

                case MOVE_TO_DOUBLE_SUPPORT:
                {
                    if(isInLeftSupportMode) {
                        rightFoot_TrajThread->setTrajectoryWaypoints(rightFootHome);
                        isInLeftSupportMode = false;
                        pauseFor(5.0);
                    }

                    if(isInRightSupportMode) {
                        leftFoot_TrajThread->setTrajectoryWaypoints(leftFootHome);
                        isInRightSupportMode = false;
                        pauseFor(5.0);
                    }
                    // if(!isMovingCoM) {
                    //     std::cout << "Moving CoM between the feet." << std::endl;
                    //     positionCoMOver(CENTERED_BETWEEN_FEET_XY);
                    //     isMovingCoM = true;
                    //     yarp::os::Time::delay(0.2);
                    // }
                    // else{
                    //     // rightFoot_TrajThread->setTrajectoryWaypoints(rightFootTarget)
                    //     currentPhase = MOVE_TO_LEFT_SUPPORT;
                    //     yarp::os::Time::delay(0.2);
                    //     isMovingCoM = false;
                    // }
                }break;
            }
        }
    }
}

Eigen::Vector3d SteppingDemoClient::getLeftFootPosition()
{
    return model->getSegmentPosition(model->getSegmentIndex("l_sole")).getTranslation();
}

Eigen::Vector3d SteppingDemoClient::getRightFootPosition()
{
    return model->getSegmentPosition(model->getSegmentIndex("r_sole")).getTranslation();
}

Eigen::Vector3d SteppingDemoClient::getCoMPosition()
{
    return model->getCoMPosition();
}

void SteppingDemoClient::positionCoMOver(COM_SUPPORT_POSITION newSupportPos)
{
    switch (newSupportPos)
    {
        case LEFT_FOOT_XY:
        {
            newCoMGoalPosition = getLeftFootPosition();
            std::cout << "newCoMGoalPosition: " << newCoMGoalPosition.transpose() << std::endl;

        }break;
        case RIGHT_FOOT_XY:
        {
            newCoMGoalPosition = getRightFootPosition();
        }break;
        case CENTERED_BETWEEN_FEET_XY:
        {
            Eigen::Vector3d leftPos = getLeftFootPosition();
            Eigen::Vector3d rightPos = getRightFootPosition();

            newCoMGoalPosition = (leftPos + rightPos) / 2.0;
        }break;

        default:
        {
            newCoMGoalPosition = getCoMPosition();
            std::cout << "Error: doing nothing because you did not provide a correct CoM position." << std::endl;
        }break;
    }


    newCoMGoalPosition(2) = getCoMPosition()(2);

    std::cout << "newCoMGoalPosition: " << newCoMGoalPosition.transpose() << std::endl;

    com_TrajThread->setTrajectoryWaypoints(newCoMGoalPosition);
}


bool SteppingDemoClient::isBalanced()
{

    double comVelNorm = model->getCoMVelocity().norm();
    double jointVelNorm = model->getJointVelocities().norm();

    // std::cout << "--------------------" << std::endl;
    // std::cout << "comVelNorm: " << comVelNorm << std::endl;
    // std::cout << "jointVelNorm: " << jointVelNorm << std::endl;

    double comVelThreshold = 0.01;
    double jointThreshold = 0.07;

    return ( (comVelNorm <= comVelThreshold) && (jointVelNorm <= jointThreshold) );

}


void SteppingDemoClient::deactivateFootContacts(FOOT_CONTACTS foot)
{
    yarp::os::Bottle message, reply;
    message.addString("deactivate");

    switch (foot) {
        case LEFT_FOOT:
        {
            leftFootRpcClient->write(message, reply);
        }break;

        case RIGHT_FOOT:
        {
            rightFootRpcClient->write(message, reply);
        }break;

        default:
        break;
    }
    if (reply.get(0).asString()=="deactivated")
    {
        std::cout << "Deactivated " << foot << " contacts." << std::endl;
    }
}

void SteppingDemoClient::activateFootContacts(FOOT_CONTACTS foot)
{
    yarp::os::Bottle message, reply;
    message.addString("activate");

    switch (foot) {
        case LEFT_FOOT:
        {
            leftFootRpcClient->write(message, reply);
        }break;

        case RIGHT_FOOT:
        {
            rightFootRpcClient->write(message, reply);
        }break;

        default:
        break;
    }
    if (reply.get(0).asString()=="activated")
    {
        std::cout << "Activated " << foot << " contacts." << std::endl;
    }
}


void SteppingDemoClient::checkFeetContacts()
{
    double leftFoot_z = getLeftFootPosition()(2);
    double rightFoot_z = getRightFootPosition()(2);

    // std::cout << "---------------------" << std::endl;
    // std::cout << "leftFoot_z " << leftFoot_z << std::endl;
    // std::cout << "rightFoot_z " << rightFoot_z << std::endl;

    double footContactRealeaseThreshold = 0.005;

    if (leftFoot_z >= footContactRealeaseThreshold) {
        deactivateFootContacts(LEFT_FOOT);
    }

    if (rightFoot_z >= footContactRealeaseThreshold) {

        std::cout << "Broke contact" << std::endl;
        // deactivateFootContacts(RIGHT_FOOT);
    }

}

void SteppingDemoClient::pauseFor(double _pauseDuration)
{
    pauseTriggerTime = yarp::os::Time::now();
    isPausing = true;
    pauseDuration = _pauseDuration;
}

bool SteppingDemoClient::pauseFinished()
{
    if(isPausing)
    {
        if ((currentTime - pauseTriggerTime) >= pauseDuration) {
            isPausing = false;
            return true;
        } else {
            return false;
        }
    }
    else{return true;}
}
