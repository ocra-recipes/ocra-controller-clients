#ifndef TEST_CLIENT_H
#define TEST_CLIENT_H

#include <ocra-icub/IcubClient.h>
#include <ocra-recipes/TrajectoryThread.h>
#include <ocra-recipes/ControllerClient.h>
// #include <ocra/control/Model.h>

class TestClient : public ocra_recipes::ControllerClient
{
DEFINE_CLASS_POINTER_TYPEDEFS(TestClient)

public:
    TestClient (std::shared_ptr<ocra::Model> modelPtr, const int loopPeriod);
    virtual ~TestClient ();

protected:
    virtual bool initialize();
    virtual void release();
    virtual void loop();

private:
    std::shared_ptr<ocra_recipes::TaskConnection> task;
};


#endif // TEST_CLIENT_H
