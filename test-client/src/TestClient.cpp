#include "test-client/TestClient.h"

TestClient::TestClient(std::shared_ptr<ocra::Model> modelPtr, const int loopPeriod)
: ocra_recipes::ControllerClient(modelPtr, loopPeriod)
{
    // poopoo
}

TestClient::~TestClient()
{
    //caca
}

bool TestClient::initialize()
{
    task = std::make_shared<ocra_recipes::TaskConnection>("leftHandCartesian");
    std::cout << "\n\n\n\n" << std::endl;



    // task->activate();
    // std::cout << "\n\n";


    return true;
}

void TestClient::release()
{
    /* Do nothing. */
}

void TestClient::loop()
{
    yarp::os::Time::delay(2.0);
    std::cout << "task->deactivate()" << std::endl;
    task->deactivate();
    std::cout << "\n\n";

    yarp::os::Time::delay(5.0);
    std::cout << "task->activate()" << std::endl;
    task->activate();
    std::cout << "\n\n";


    std::cout << "task->getPortName()\n" << task->getPortName() << std::endl;
    std::cout << "\n\n";

    std::cout << "task->isActivated()\n" << task->isActivated() << std::endl;
    std::cout << "\n\n";

    std::cout << "task->getTaskError()\n" << task->getTaskError() << std::endl;
    std::cout << "\n\n";

    std::cout << "task->getTaskErrorNorm()\n" << task->getTaskErrorNorm() << std::endl;
    std::cout << "\n\n";

    std::cout << "task->getStiffness()\n" << task->getStiffness() << std::endl;
    std::cout << "\n\n";

    std::cout << "task->getStiffnessMatrix()\n" << task->getStiffnessMatrix() << std::endl;
    std::cout << "\n\n";


}
