#ifndef EXAMPLE_CLIENT_THREAD
#define EXAMPLE_CLIENT_THREAD

// #include "ocra-yarp/OcraControllerClientThread.h"

class ExampleClientThread : public ocra_yarp::OcraControllerClientThread
{
DEFINE_CLASS_POINTER_TYPEDEFS(ExampleClientThread)

public:
    ExampleClientThread (const int period);
    virtual ~ExampleClientThread ();

protected:
    virtual bool client_threadInit();
    virtual void client_threadRelease();
    virtual void client_run();

private:

    double startTime;
    bool trigger;




        Eigen::MatrixXd waypoints;

        std::shared_ptr<ocra_yarp::TrajectoryThread> leftHandTrajThread;

        bool done;
        bool p1, p2, p3;
};


#endif // EXAMPLE_CLIENT_THREAD
