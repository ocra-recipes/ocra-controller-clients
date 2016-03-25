#ifndef STEPPING_DEMO_CLIENT_H
#define STEPPING_DEMO_CLIENT_H

#include <ocra-icub/IcubClient.h>
#include <ocra-icub/TrajectoryThread.h>
#include <ocra-recipes/ControllerClient.h>
// #include <ocra/control/Model.h>

enum COM_SUPPORT_POSITION
{
    LEFT_FOOT_XY,
    RIGHT_FOOT_XY,
    CENTERED_BETWEEN_FEET_XY
};

enum CONTROL_PHASE
{
    MOVE_TO_LEFT_SUPPORT,
    MOVE_TO_RIGHT_SUPPORT,
    MOVE_TO_DOUBLE_SUPPORT
};

enum FOOT_CONTACTS
{
    LEFT_FOOT,
    RIGHT_FOOT
};


class SteppingDemoClient : public ocra_recipes::ControllerClient
{
DEFINE_CLASS_POINTER_TYPEDEFS(SteppingDemoClient)

public:
    SteppingDemoClient (std::shared_ptr<ocra::Model> modelPtr, const int loopPeriod);
    virtual ~SteppingDemoClient ();

protected:
    virtual bool initialize();
    virtual void release();
    virtual void loop();

private:

    std::shared_ptr<yarp::os::RpcClient> leftFootRpcClient;
    std::shared_ptr<yarp::os::RpcClient> rightFootRpcClient;

    Eigen::Vector3d getLeftFootPosition();
    Eigen::Vector3d getRightFootPosition();
    Eigen::Vector3d getCoMPosition();
    void positionCoMOver(COM_SUPPORT_POSITION newSupportPos);

    bool isBalanced();
    void deactivateFootContacts(FOOT_CONTACTS foot);
    void checkFeetContacts();

    void pauseFor(double _pauseDuration);
    bool pauseFinished();

    double pauseTriggerTime;
    double pauseDuration;
    double currentTime;
    bool isPausing;

    bool isInLeftSupportMode;
    bool isInRightSupportMode;

    std::shared_ptr<ocra_icub::TrajectoryThread> leftFoot_TrajThread;
    std::shared_ptr<ocra_icub::TrajectoryThread> rightFoot_TrajThread;
    std::shared_ptr<ocra_icub::TrajectoryThread> com_TrajThread;


    Eigen::Vector3d leftFootHome;
    Eigen::Vector3d rightFootHome;
    Eigen::Vector3d comHome;
    Eigen::Vector3d leftFootTarget;
    Eigen::Vector3d rightFootTarget;

    Eigen::Vector3d newCoMGoalPosition;


    double startTime;
    bool getInitialValues;

    CONTROL_PHASE currentPhase;
    bool isMovingCoM;

};
#endif // STEPPING_DEMO_CLIENT_H
