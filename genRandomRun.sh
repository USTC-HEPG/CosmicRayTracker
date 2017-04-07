xcnpnpnxc/////# generates 8 plots at random (linear) energies ranging from 0 to 1 GeV 

#!/bin/bash

# rm -r plots/*

for i in {1..8}
do
    RAND=$(python -c "import random;print(random.uniform(0.0, 1.0))")
    ./exampleB4a -n $RAND -m run2.mac
done

