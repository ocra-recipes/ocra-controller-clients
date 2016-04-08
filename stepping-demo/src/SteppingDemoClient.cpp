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
    // std::string leftFootPortName = clientComs->getTaskPortName("leftFootCartesian");
    // std::string rightFootPortName = clientComs->getTaskPortName("rightFootCartesian");
    // std::string comPortName = clientComs->getTaskPortName("comTask");
    //
    // std::cout << "leftFootPortName: " << leftFootPortName << std::endl;
    // std::cout << "rightFootPortName: " << rightFootPortName << std::endl;
    // std::cout << "comPortName: " << comPortName << std::endl;

    ocra_recipes::TRAJECTORY_TYPE trajType = ocra_recipes::MIN_JERK;
    ocra_recipes::TERMINATION_STRATEGY termStrategy = ocra_recipes::WAIT;
    int trajThreadPeriod = 10;


    leftFoot_TrajThread = std::make_shared<ocra_recipes::TrajectoryThread>(trajThreadPeriod, "leftFootCartesian", trajType, termStrategy);

    rightFoot_TrajThread = std::make_shared<ocra_recipes::TrajectoryThread>(trajThreadPeriod, "rightFootCartesian", trajType, termStrategy);

    com_TrajThread = std::make_shared<ocra_recipes::TrajectoryThread>(trajThreadPeriod, "comTask", trajType, termStrategy);


    leftFoot_TrajThread->setMaxVelocity(0.02);
    rightFoot_TrajThread->setMaxVelocity(0.02);
    com_TrajThread->setMaxVelocity(0.01);
    com_TrajThread->setGoalErrorThreshold(0.01);

    currentPhase = MOVE_TO_LEFT_SUPPORT;
    isMovingCoM = false;
    com_TrajThread->start();
    leftFoot_TrajThread->start();
    rightFoot_TrajThread->start();
    std::cout << "Thread started." << std::endl;

    getInitialValues = true;
    startTime = yarp::os::Time::now();

    leftFootContacts = std::make_shared<ocra_recipes::TaskConnection>("contactSetLeftFoot");
    rightFootContacts = std::make_shared<ocra_recipes::TaskConnection>("contactSetRightFoot");

    isInLeftSupportMode = true;
    isInRightSupportMode = true;

    footTrajectoryStarted = false;

    return true;
}

void SteppingDemoClient::release()
{
    com_TrajThread->stop();
    rightFoot_TrajThread->stop();
    leftFoot_TrajThread->stop();
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

            leftFootTarget = leftFootHome + Eigen::Vector3d(0.05, -0.01, 0.05);
            rightFootTarget = rightFootHome + Eigen::Vector3d(0.05, 0.01, 0.05);

            leftFootHome += Eigen::Vector3d(0.0, 0.0, -0.005);
            rightFootHome += Eigen::Vector3d(0.0, 0.0, -0.005);

            std::cout << " leftFootHome: " << leftFootHome.transpose() << std::endl;
            std::cout << " rightFootHome: " << rightFootHome.transpose() << std::endl;
            std::cout << " comHome: " << comHome.transpose() << std::endl;
            std::cout << " leftFootTarget: " << leftFootTarget.transpose() << std::endl;
            std::cout << " rightFootTarget: " << rightFootTarget.transpose() << std::endl;

            getInitialValues = false;
        }


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
                    else if (liftFoot(RIGHT_FOOT)){
                        pauseFor(5.0);
                        isInLeftSupportMode = true;
                        isInRightSupportMode = false;
                        currentPhase = MOVE_TO_DOUBLE_SUPPORT;
                        isMovingCoM = false;
                    }

                }break;

                case MOVE_TO_RIGHT_SUPPORT:
                {
                    if(!isMovingCoM) {
                        std::cout << "Moving CoM over right foot." << std::endl;
                        positionCoMOver(RIGHT_FOOT_XY);
                        isMovingCoM = true;
                        pauseFor(5.0);
                    }
                    else if (liftFoot(LEFT_FOOT)){
                        pauseFor(5.0);
                        isInLeftSupportMode = false;
                        isInRightSupportMode = true;
                        currentPhase = MOVE_TO_DOUBLE_SUPPORT;
                        isMovingCoM = false;
                    }
                }break;

                case MOVE_TO_DOUBLE_SUPPORT:
                {
                    if(isInLeftSupportMode && !isInRightSupportMode) {
                        if (setDownFoot(RIGHT_FOOT)) {
                            isInLeftSupportMode = false;
                            nextPhase = MOVE_TO_RIGHT_SUPPORT;
                            pauseFor(5.0);
                        }
                    }
                    else if(isInRightSupportMode && !isInLeftSupportMode) {
                        if (setDownFoot(LEFT_FOOT)) {
                            isInRightSupportMode = false;
                            nextPhase = MOVE_TO_LEFT_SUPPORT;
                            pauseFor(5.0);
                        }
                    }
                    else
                    {
                        if(!isMovingCoM) {
                            std::cout << "Moving CoM over between feet." << std::endl;
                            positionCoMOver(CENTERED_BETWEEN_FEET_XY);
                            isMovingCoM = true;
                            pauseFor(5.0);
                        }
                        else
                        {
                            pauseFor(5.0);
                            isInLeftSupportMode = true;
                            isInRightSupportMode = true;
                            currentPhase = nextPhase;
                            isMovingCoM = false;
                        }
                    }
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
    switch (foot) {
        case LEFT_FOOT:
        {
            if(leftFootContacts->deactivate() ) {
                std::cout << "Deactivated left foot contacts." << std::endl;
            }
        }break;

        case RIGHT_FOOT:
        {
            if (rightFootContacts->deactivate()) {
                std::cout << "Deactivated right foot contacts." << std::endl;
            }
        }break;

        default:
        break;
    }
}

void SteppingDemoClient::activateFootContacts(FOOT_CONTACTS foot)
{
    switch (foot) {
        case LEFT_FOOT:
        {
            if(leftFootContacts->activate() ) {
                std::cout << "Activated left foot contacts." << std::endl;
            }
        }break;

        case RIGHT_FOOT:
        {
            if (rightFootContacts->activate()) {
                std::cout << "Activated right foot contacts." << std::endl;
            }
        }break;

        default:
        break;
    }
}


bool SteppingDemoClient::isFootInContact(FOOT_CONTACTS foot)
{
    double foot_z;
    switch (foot) {
        case LEFT_FOOT:
        {
            foot_z = getLeftFootPosition()(2);
        }break;

        case RIGHT_FOOT:
        {
            foot_z = getRightFootPosition()(2);
        }break;

        default:
        break;
    }

    std::cout << "---------------------" << std::endl;
    std::cout << "foot_z " << foot_z << std::endl;
    // std::cout << "rightFoot_z " << rightFoot_z << std::endl;

    double footContactRealeaseThreshold = 0.01;


    if (foot_z <= footContactRealeaseThreshold) {
        activateFootContacts(foot);
        return true;
    } else {
        return false;
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

bool SteppingDemoClient::liftFoot(FOOT_CONTACTS foot)
{
    double delayTime = 0.2;
    switch (foot) {
        case LEFT_FOOT:
        {
            if (!footTrajectoryStarted) {
                std::cout << "Setting left foot trajectory." << std::endl;
                leftFoot_TrajThread->setGoalErrorThreshold(0.01);
                leftFoot_TrajThread->setTrajectoryWaypoints(leftFootTarget);
                yarp::os::Time::delay(delayTime);
                deactivateFootContacts(LEFT_FOOT);
                footTrajectoryStarted = true;
                return false;
            } else {
                if(leftFoot_TrajThread->goalAttained()) {
                    footTrajectoryStarted = false;
                    return true;
                } else {
                    return false;
                }
            }
        }break;

        case RIGHT_FOOT:
        {
            if (!footTrajectoryStarted) {
                std::cout << "Setting right foot trajectory." << std::endl;
                rightFoot_TrajThread->setGoalErrorThreshold(0.01);
                rightFoot_TrajThread->setTrajectoryWaypoints(rightFootTarget);
                yarp::os::Time::delay(delayTime);
                deactivateFootContacts(RIGHT_FOOT);
                footTrajectoryStarted = true;
                return false;
            } else {
                if(rightFoot_TrajThread->goalAttained()) {
                    footTrajectoryStarted = false;
                    return true;
                } else {
                    return false;
                }
            }
        }break;

        default:
        break;
    }

}


bool SteppingDemoClient::setDownFoot(FOOT_CONTACTS foot)
{
    switch (foot) {
        case LEFT_FOOT:
        {
            if (!footTrajectoryStarted) {
                std::cout << "Setting left foot trajectory." << std::endl;
                leftFoot_TrajThread->setGoalErrorThreshold(0.008);
                leftFoot_TrajThread->setTrajectoryWaypoints(leftFootHome);
                footTrajectoryStarted = true;
                return false;
            } else {
                if(isFootInContact(LEFT_FOOT)) {
                    footTrajectoryStarted = false;
                    return true;
                } else {
                    return false;
                }
            }
        }break;

        case RIGHT_FOOT:
        {
            if (!footTrajectoryStarted) {
                std::cout << "Setting right foot trajectory." << std::endl;
                rightFoot_TrajThread->setGoalErrorThreshold(0.008);
                rightFoot_TrajThread->setTrajectoryWaypoints(rightFootHome);
                footTrajectoryStarted = true;
                return false;
            } else {
                if(isFootInContact(RIGHT_FOOT)) {
                    footTrajectoryStarted = false;
                    return true;
                } else {
                    return false;
                }
            }
        }break;

        default:
        break;
    }

}
