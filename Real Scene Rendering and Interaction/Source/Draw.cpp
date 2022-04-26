#include "Draw.h"

using namespace std;

void DrawArraysInstancedARB(mode, first, count, primcount);
void DrawElementsInstancedARB(enum mode, sizei count, enum type, const void *indices, sizei primcount);

if (mode || count is invalid)
{        
        else {
            for (i = 0; i < primcount; i++) {
                instanceID = i;
                DrawArrays(mode, first, count, i);
            }
            instanceID = 0;
        }
}
