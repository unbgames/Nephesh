//
// Created by bruno on 07/07/18.
//

#ifndef NEPHESH_UTILS_H
#define NEPHESH_UTILS_H

#include <vector>

int WeightedDraft(vector<int> probabilityWeight){
    int totalWeights = 0;

    for(int i = 0; i < probabilityWeight.size(); i++){
        totalWeights += probabilityWeight[i];
    }

    int draftedNumber = rand() % totalWeights;

    for(int i = 0; i < probabilityWeight.size(); i++){
        if(draftedNumber < probabilityWeight[i]){
            return i;
        }

        draftedNumber -= probabilityWeight[i];
    }
}

#endif //NEPHESH_UTILS_H
