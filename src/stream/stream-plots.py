#!/usr/bin/env python
#
# author : Anthony Lichnewsky
# description : basic matplotlib to make plots out of stream-triad benchmark results

import sys;
import string;
import re;

import numpy as np;
import matplotlib.pyplot as plt;

prefix = 'results/README.stream.triad.'
filenames =  sys.argv[1:];
machine_types = []
p = re.compile(r'Triad :.*');

colors=plt.cm.rainbow(np.linspace(0,1,len(filenames)))
i = 0
for f in filenames :
    machine_type = string.replace(f,prefix,'');
    machine_types.append(machine_type);
    with open(f) as file:
        threads = [];
        avgB = [];
        minB = [];
        maxB = [];
        for line in file:
            m = p.match(line);
            if m :
                (s1, s2, nth, s3, s4, avgBW, minBW, maxBW, agvTime, minTime, maxTime) = \
                    [t(s) for (t,s) in zip((str, str, int, str, str, \
                                          float, float, float, float, float, float), line.split()) ]
                threads.append(nth);
                avgB.append(avgBW);
                minB.append(minBW);
                maxB.append(maxBW);
        c = colors[i];
        plt.plot(threads, avgB, linestyle='-' , color=c, label=machine_type);
        plt.plot(threads, minB, linestyle='-.', color=c);
        plt.plot(threads, maxB, linestyle='-.', color=c);
        i = i+1
        

plt.xlabel('OpenMP threads used');
plt.ylabel('Stream triad memory bandwidth in GB/s');
plt.title('Stream bandwidth measurement with various machines');


#plt.legend();
# Put a legend to the right of the current axis
lgd = plt.legend(loc='center left', bbox_to_anchor=(1, 0.5))

#plt.show( bbox_extra_artists=(lgd,), bbox_inches='tight');
plt.savefig("bandwidths.png",  bbox_extra_artists=(lgd,), bbox_inches='tight');
