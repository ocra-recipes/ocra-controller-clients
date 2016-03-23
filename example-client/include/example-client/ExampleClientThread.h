#ifndef EXAMPLE_CLIENT_H
#define EXAMPLE_CLIENT_H

#include <ocra-icub/IcubClient>

class ExampleClient : public ocra_icub::IcubControllerClient
{
DEFINE_CLASS_POINTER_TYPEDEFS(ExampleClient)

public:
    ExampleClient (std::shared_ptr<ocra_icub::OcraWbiModel> modelPtr, const int loopPeriod);
    virtual ~ExampleClient ();

protected:
    virtual bool initialize();
    virtual void release();
    virtual void loop();

private:

    double startTime;
    bool trigger;

    Eigen::MatrixXd waypoints;

    std::shared_ptr<ocra_yarp::TrajectoryThread> leftHandTrajThread;

    bool done;
    bool p1, p2, p3;
};


#endif // EXAMPLE_CLIENT_H
