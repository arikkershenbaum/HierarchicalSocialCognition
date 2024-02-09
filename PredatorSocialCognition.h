#pragma once
#include "PredatorCognition.h"

#include <vector>

class PredatorSocialCognition :
    public PredatorCognition
{
public:
    PredatorSocialCognition(Predator* p) : PredatorCognition(p) {};
protected:
    std::vector<std::vector<float>> socialNet;

    float dist_recruit;
    float interest_slope;
    float weight_recruit;
    float teamweight;

};

