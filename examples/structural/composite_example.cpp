#include "../include/structural/composite.h"

int main()
{
    Group root("root");
    Circle c1, c2;
    root.objects.push_back(std::make_shared<Circle>(c1));

    Group subgroup("sub");
    subgroup.objects.push_back(std::make_shared<Circle>(c2));

    root.objects.push_back(std::make_shared<Group>(subgroup));

    root.draw();

    return 0;
}
