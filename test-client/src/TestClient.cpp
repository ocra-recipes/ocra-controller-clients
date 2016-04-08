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

    // Stiffness

    std::cout << "task->setStiffness(30)\n";
    task->setStiffness(30);
    std::cout << "\n\n";

    std::cout << "task->getStiffness()\n" << task->getStiffness() << std::endl;
    std::cout << "\n\n";

    std::cout << "task->setStiffness(Eigen::VectorXd::Ones(3)*20)\n";
    Eigen::VectorXd kVec = Eigen::VectorXd::Ones(3)*20;
    task->setStiffness(kVec);
    std::cout << "\n\n";

    std::cout << "task->getStiffnessMatrix()\n" << task->getStiffnessMatrix() << std::endl;
    std::cout << "\n\n";

    std::cout << "task->setStiffness(Eigen::MatrixXd::Identity(3,3)*40)\n";
    Eigen::MatrixXd kMat = Eigen::MatrixXd::Identity(3,3)*40;
    task->setStiffness(kMat);
    std::cout << "\n\n";

    std::cout << "task->getStiffnessMatrix()\n" << task->getStiffnessMatrix() << std::endl;
    std::cout << "\n\n";


    // Damping

    std::cout << "task->setDamping(10)\n";
    task->setDamping(10);
    std::cout << "\n\n";

    std::cout << "task->getDamping()\n" << task->getDamping() << std::endl;
    std::cout << "\n\n";

    std::cout << "task->setDamping(Eigen::VectorXd::Ones(3)*5)\n";
    Eigen::VectorXd bVec = Eigen::VectorXd::Ones(3)*5;
    task->setDamping(bVec);
    std::cout << "\n\n";

    std::cout << "task->getDampingMatrix()\n" << task->getDampingMatrix() << std::endl;
    std::cout << "\n\n";

    std::cout << "task->setDamping(Eigen::MatrixXd::Identity(3,3)*10)\n";
    Eigen::MatrixXd bMat = Eigen::MatrixXd::Identity(3,3)*10;
    task->setDamping(bMat);
    std::cout << "\n\n";

    std::cout << "task->getDampingMatrix()\n" << task->getDampingMatrix() << std::endl;
    std::cout << "\n\n";



    // Damping

    std::cout << "task->setWeight(0.5)\n";
    task->setWeight(0.5);
    std::cout << "\n\n";

    std::cout << "task->getWeight()\n" << task->getWeight() << std::endl;
    std::cout << "\n\n";

    std::cout << "task->setWeight(Eigen::VectorXd::Ones(3)*0.0001)\n";
    Eigen::VectorXd wVec = Eigen::VectorXd::Ones(3)*0.0001;
    task->setWeight(wVec);
    std::cout << "\n\n";

    std::cout << "task->getWeight()\n" << task->getWeight() << std::endl;
    std::cout << "\n\n";




}
