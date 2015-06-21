#!/usr/bin/env python
#
# author : Anthony Lichnewsky
# description : basic matplotlib to make plots out of stream-triad benchmark results

import sys;
import string;
import re;

import numpy as np;
import matplotlib.pyplot as plt;

prefix = 'results/README.sgemm.'
filenames =  sys.argv[1:];
machine_types = []
p = re.compile(r'.*GFlops');

colors=plt.cm.rainbow(np.linspace(0,1,len(filenames)))
i = 0
for f in filenames :
    machine_type = string.replace(f,prefix,'');
    machine_types.append(machine_type);
    with open(f) as file:
        threads = [];
        flops = [];
#        minB = [];
#        maxB = [];
        for line in file:
            #print zip((int, str, str, float, str, float, str, float, str, float, str), line.split())
            m = p.match(line);
            if m :
                (nth, s1, s2, agvTime, s3, minTime, s4, maxTime, s5, gflops, s4) = \
                    [t(s) for (t,s) in zip((int, str, str, float, str, float, str, float, str, float, str), line.split()) ]
                threads.append(nth);
                flops.append(gflops);
        c = colors[i];
        plt.plot(threads, flops, linestyle='-' , color=c, label=machine_type);
#        plt.plot(threads, minB, linestyle='-.', color=c);
#        plt.plot(threads, maxB, linestyle='-.', color=c);
        i = i+1
        

plt.xlabel('OpenMP threads used');
plt.ylabel('SGEMM GFlops');
plt.title('SGEMM GFlops');


#plt.legend();
# Put a legend to the right of the current axis
lgd = plt.legend(loc='center left', bbox_to_anchor=(1, 0.5))

#plt.show( bbox_extra_artists=(lgd,), bbox_inches='tight');
plt.savefig("flops.png",  bbox_extra_artists=(lgd,), bbox_inches='tight');
